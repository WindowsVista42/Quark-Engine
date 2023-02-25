#define SPACESHIPS_IMPLEMENTATION
#include "spaceships.hpp"

namespace quark::spaceships {
  bool vec3_all_less_than_or_equal(vec3 a, vec3 b) {
    return a.x <= b.x && a.y <= b.y && a.z <= b.z;
  }

  RawRayHitInfo ray_intersects_box(vec3 ray_origin, vec3 ray_direction, Transform* transform, BoxCollider* collider) {
    vec3 f = quat_forward(transform->rotation);
    vec3 r = quat_right(transform->rotation);
    vec3 u = quat_up(transform->rotation);

    f32 fd = dot(ray_direction, f);
    f = fd < 0 ? f : -f;

    f32 rd = dot(ray_direction, r);
    r = rd < 0 ? r : -r;

    f32 ud = dot(ray_direction, u);
    u = ud < 0 ? u : -u;

    f32 ft = abs(dot((transform->position + f * collider->half_extents.y) - ray_origin, f) / fd);
    f32 rt = abs(dot((transform->position + r * collider->half_extents.x) - ray_origin, r) / rd);
    f32 ut = abs(dot((transform->position + u * collider->half_extents.z) - ray_origin, u) / ud);

    vec3 fdiff = abs(rotate((ray_origin + ray_direction * ft) - transform->position, conjugate(transform->rotation)));
    vec3 rdiff = abs(rotate((ray_origin + ray_direction * rt) - transform->position, conjugate(transform->rotation)));
    vec3 udiff = abs(rotate((ray_origin + ray_direction * ut) - transform->position, conjugate(transform->rotation)));

    if(!vec3_all_less_than_or_equal(fdiff, collider->half_extents + 0.01f)) {
      ft = F32_MAX;
    }

    if(!vec3_all_less_than_or_equal(rdiff, collider->half_extents + 0.01f)) {
      rt = F32_MAX;
    }

    if(!vec3_all_less_than_or_equal(udiff, collider->half_extents + 0.01f)) {
      ut = F32_MAX;
    }

    RawRayHitInfo info = {};
    info.t = -1;
    info.normal = VEC3_ZERO;

    if(ft < rt && ft < ut && ft != F32_MAX) {
      info.t = ft;
      info.normal = f;
    }

    if(rt < ft && rt < ut && rt != F32_MAX) {
      info.t = rt;
      info.normal = r;
    }

    if(ut < ft && ut < rt && ut != F32_MAX) {
      info.t = ut;
      info.normal = u;
    }

    return info;
  }

  RaycastInfo cast_ray(u32 caster_id, vec3 ray_origin, vec3 ray_direction, f32 ray_length) {
    u32 hit_id = -1;

    f32 closest_distance2 = ray_length * ray_length;
    vec3 hit_position = {};
    vec3 hit_normal_unnorm = {};
    f32 hit_distance = -1;

    // @todo move this into an acceleration structure
    // probably just list entities that intersect the current voxel
    // which we can probably sparsely populate and put into a frame-local
    // arena
    for_archetype(Include<Transform, BoxCollider> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, BoxCollider* collider) {
      if(entity_id == caster_id) {
        return;
      }

      vec3 dir = transform->position - ray_origin;
      vec3 projection = (dot(ray_direction, transform->position) / dot(transform->position, transform->position)) * transform->position;
      vec3 projected_position = ray_origin + projection;

      if(distance2(projected_position, transform->position) > length2(collider->half_extents)) {
        return;
      }

      auto [t, n] = ray_intersects_box(ray_origin, ray_direction, transform, collider);
      if(t >= 0 && t * t < closest_distance2) {
        closest_distance2 = t * t;
        hit_id = entity_id;
        hit_position = ray_origin + (t * ray_direction);
        hit_normal_unnorm = n;
        hit_distance = t;
      }
    });

    RaycastInfo info = {};
    info.hit = hit_id != -1;
    info.hit_id = hit_id;
    info.hit_position = hit_position;
    info.hit_normal = normalize(hit_normal_unnorm);
    info.hit_distance = hit_distance;

    return info;
  }

  struct FatObb {
    vec3 Pos, AxisX, AxisY, AxisZ, Half_size;
  };

  // check if there's a separating plane in between the selected axes
  bool getSeparatingPlane(const vec3& RPos, const vec3& Plane, const FatObb& box1, const FatObb& box2) {
      return abs(dot(RPos, Plane)) > 
          abs(dot(box1.AxisX * box1.Half_size.x, Plane)) +
          abs(dot(box1.AxisY * box1.Half_size.y, Plane)) +
          abs(dot(box1.AxisZ * box1.Half_size.z, Plane)) +
          abs(dot(box2.AxisX * box2.Half_size.x, Plane)) + 
          abs(dot(box2.AxisY * box2.Half_size.y, Plane)) +
          abs(dot(box2.AxisZ * box2.Half_size.z, Plane));
  }

  bool is_obb_colliding(FatObb box1, FatObb box2) {
    vec3 RPos;
    RPos = box2.Pos - box1.Pos;

    return !(getSeparatingPlane(RPos, box1.AxisX, box1, box2) ||
        getSeparatingPlane(RPos, box1.AxisY, box1, box2) ||
        getSeparatingPlane(RPos, box1.AxisZ, box1, box2) ||
        getSeparatingPlane(RPos, box2.AxisX, box1, box2) ||
        getSeparatingPlane(RPos, box2.AxisY, box1, box2) ||
        getSeparatingPlane(RPos, box2.AxisZ, box1, box2) ||
        getSeparatingPlane(RPos, cross(box1.AxisX, box2.AxisX), box1, box2) ||
        getSeparatingPlane(RPos, cross(box1.AxisX, box2.AxisY), box1, box2) ||
        getSeparatingPlane(RPos, cross(box1.AxisX, box2.AxisZ), box1, box2) ||
        getSeparatingPlane(RPos, cross(box1.AxisY, box2.AxisX), box1, box2) ||
        getSeparatingPlane(RPos, cross(box1.AxisY, box2.AxisY), box1, box2) ||
        getSeparatingPlane(RPos, cross(box1.AxisY, box2.AxisZ), box1, box2) ||
        getSeparatingPlane(RPos, cross(box1.AxisZ, box2.AxisX), box1, box2) ||
        getSeparatingPlane(RPos, cross(box1.AxisZ, box2.AxisY), box1, box2) ||
        getSeparatingPlane(RPos, cross(box1.AxisZ, box2.AxisZ), box1, box2));
  }

  FatObb create_fatobb(Transform* transform, BoxCollider* collider) {
    FatObb obb = {};
    obb.Pos = transform->position;
    obb.AxisX = quat_right(transform->rotation);
    obb.AxisY = quat_forward(transform->rotation);
    obb.AxisZ = quat_up(transform->rotation);
    obb.Half_size = collider->half_extents;

    return obb;
  }

  void update_physics() {
    f32 target_tick = 1.0f / 240.0f;
    savable static f32 accum = 0.0f;
    u32 count = 0;

    accum += delta();
    while(accum > 0.0f && count < 8) {
      f32 dt = target_tick;
      accum -= target_tick;
      count += 1;

      // @info find collisions
      for_archetype(Include<Transform, Motion, BoxCollider> {}, Exclude<> {},
      [&](u32 entity_id, Transform* transform, Motion* motion, BoxCollider* collider) {
        FatObb a = create_fatobb(transform, collider);

        f32 radius_sqr = length2(collider->half_extents) * 1.5f;

        // @info static collisions
        for_archetype(Include<Transform, BoxCollider> {}, Exclude<Motion> {},
        [&](u32 entity_id2, Transform* transform2, BoxCollider* collider2) {
          if(entity_id2 == entity_id) {
            return;
          }

          f32 radius_sqr2 = length2(collider2->half_extents) * 1.5f;
          f32 diff = radius_sqr + radius_sqr2;

          f32 dist = distance2(transform->position, transform2->position);

          if(dist > diff) {
            return;
          }

          FatObb b = create_fatobb(transform2, collider2);

          if(is_obb_colliding(a, b)) {
            vec3 decollide_dir = normalize(transform->position - transform2->position);
            decollide_dir = normalize(rotate(rotate(decollide_dir, conjugate(transform->rotation)) * collider->half_extents, transform->rotation));

            vec3 vel_dir = normalize(motion->velocity);
            f32 vel_len = length(motion->velocity);
            f32 rel = dot(decollide_dir, -vel_dir);

            motion->impulse += (-vel_dir * vel_len * 0.5f) + (decollide_dir * vel_len * rel * 0.4f);
            motion->impulse += decollide_dir * 0.2f * vel_len + decollide_dir * 5.0f;

            {
              quat anti_rotate = conjugate(transform->rotation);
              vec3 rotation_axis_local = normalize(cross(rotate(decollide_dir, anti_rotate), rotate(vel_dir, anti_rotate)));
              f32 power = (1.0f - dot(decollide_dir, vel_dir)) * clamp((vel_len / 300.0f), 0.0f, 0.5f);

              f32 yaw   = dot(rotation_axis_local, VEC3_UNIT_Z);
              f32 pitch = dot(rotation_axis_local, VEC3_UNIT_X);
              f32 roll  = dot(rotation_axis_local, VEC3_UNIT_Y);

              motion->angular_impulse.x = power * yaw;
              motion->angular_impulse.y = power * pitch;
              motion->angular_impulse.z = power * roll;
            }
          }
        });

        // @info dynamic collisions
        for_archetype(Include<Transform, Motion, BoxCollider> {}, Exclude<> {},
        [&](u32 entity_id2, Transform* transform2, Motion* motion2, BoxCollider* collider2) {
          if(entity_id2 == entity_id) {
            return;
          }

          f32 radius_sqr2 = length2(collider2->half_extents) * 1.5f;
          f32 diff = radius_sqr + radius_sqr2;

          f32 dist = distance2(transform->position, transform2->position);

          if(dist > diff) {
            return;
          }

          FatObb b = create_fatobb(transform2, collider2);

          if(is_obb_colliding(a, b)) {
            vec3 decollide_dir = normalize(transform->position - transform2->position);
            decollide_dir = normalize(rotate(rotate(decollide_dir, conjugate(transform->rotation)) * collider->half_extents, transform->rotation));

            vec3 net_vel_dir = normalize_or_zero(motion->velocity - motion2->velocity);
            f32 net_vel_len = length(motion->velocity - motion2->velocity);
            f32 rel = dot(decollide_dir, -net_vel_dir);

            motion->impulse += (-net_vel_dir * net_vel_len * 0.02f) + (decollide_dir * net_vel_len * rel * 0.08f);
            motion->impulse += decollide_dir * 10.0f;

            motion2->impulse += (net_vel_dir * net_vel_len * 0.02f) + (-decollide_dir * net_vel_len * rel * 0.02f);

            {
              quat anti_rotate = conjugate(transform->rotation);
              vec3 rotation_axis_local = normalize(cross(rotate(decollide_dir, anti_rotate), rotate(net_vel_dir, anti_rotate)));
              f32 power = (1.0f - dot(decollide_dir, net_vel_dir)) * clamp((net_vel_len / 300.0f), 0.0f, 0.5f);

              f32 yaw   = dot(rotation_axis_local, VEC3_UNIT_Z);
              f32 pitch = dot(rotation_axis_local, VEC3_UNIT_X);
              f32 roll  = dot(rotation_axis_local, VEC3_UNIT_Y);

              motion->angular_impulse.x += power * yaw;
              motion->angular_impulse.y += power * pitch;
              motion->angular_impulse.z += power * roll;

              motion2->angular_impulse.x += power * yaw;
              motion2->angular_impulse.y += power * pitch;
              motion2->angular_impulse.z += power * roll;
            }
          }
        });
      });

      // @info update linear/angular positions and velocities
      // @info semi-implicit euler integration, ie update velocity then position
      for_archetype(Include<Transform, Motion> {}, Exclude<> {},
      [&](u32 entity_id, Transform* transform, Motion* motion) {
        motion->velocity += motion->impulse;
        motion->impulse = VEC3_ZERO;
        motion->velocity += motion->acceleration * dt;
        transform->position += motion->velocity * dt;

        motion->angular_velocity += motion->angular_impulse;
        motion->angular_impulse = VEC3_ZERO;
        motion->angular_velocity += motion->angular_acceleration * dt;
        transform->rotation = transform->rotation * quat_from_eul3(as_eul3(motion->angular_velocity * dt));
      });
    }
  }
};
