#define SPACESHIPS_IMPLEMENTATION
#include "spaceships.hpp"

#define ENEMY_COUNT 10
#define ASTEROID_COUNT 2000

//

namespace quark::spaceships {
  void init_entities() {
    // Add player
    // @info create player
    set_mouse_mode(MouseMode::Captured);
  
    {
      player_id = create_entity();

      Transform transform = {};
      transform.position.x = 0;
      transform.position.y = 0;
      transform.position.z = 0;
      transform.rotation = { 0, 0, 0, 1 };

      Motion motion = {};
      motion.velocity = VEC3_ZERO;
      motion.acceleration = VEC3_ZERO;

      Spaceship spaceship = {};
      spaceship.hull_info.base_health = 100.0f;
      spaceship.hull_info.base_health *= 1.5f;
      spaceship.hull_health = spaceship.hull_info.base_health;
      spaceship.hull_info.base_color = { 1, 1, 1 };

      spaceship.input_movement_direction = {};
      spaceship.input_rotation_direction = {};

      spaceship.thrusters_info.max_speed = 300.0f;
      spaceship.thrusters_info.max_speed /= 1.5f;
      spaceship.thrusters_info.max_angular_speeds = { F32_PI / 3.0f , F32_PI, F32_PI };
      spaceship.thrusters_info.max_angular_speeds /= 2.0f;

      spaceship.boost_active = false;
      spaceship.boost_amount = 1.0f;
      spaceship.thrusters_info.max_boost_amount = 1.0f;
      spaceship.thrusters_info.boost_recharge_rate = 0.25f;
      spaceship.thrusters_info.boost_drain_rate = 1.0f;
      spaceship.thrusters_info.boost_speedup_factor = 2.0f;

      spaceship.thrusters_info.acceleration_amounts = { 33, 125, 66 };
      spaceship.thrusters_info.acceleration_amounts /= 1.5f;
      spaceship.thrusters_info.angular_acceleration_amounts = { F32_PI, F32_2PI, F32_2PI * 1.5f };
      spaceship.thrusters_info.angular_acceleration_amounts /= 1.5f;
      spaceship.thrusters_info.base_color = {};
      spaceship.thrusters_info.tail_color = {};

      spaceship.shields_info.unbreak_time = 2.0f;
      spaceship.shields_info.recover_time = 1.0f;
      spaceship.shields_info.recharge_rate = 4.0f;
      spaceship.shields_info.max_health = 40.0f;
      spaceship.shields_info.max_health *= 1.5f;
      spaceship.shields_info.base_color = {};
      spaceship.shields_info.low_hp_color = {};
      spaceship.shields_info.broken_color = {};

      // spaceship.weapons_info.type = WeaponType::Beam;
      // spaceship.weapons_info.beam.base_dps = 20.0f;
      // spaceship.weapons_info.beam.beam_length = 500.0f;

      spaceship.weapons_info.type = WeaponType::Projectile;
      spaceship.weapons_info.projectile.lifetime = 2.0f;
      spaceship.weapons_info.projectile.velocity = 2000.0f;
      spaceship.weapons_info.projectile.base_damage = 4.0f;
      spaceship.weapons_info.projectile.base_cooldown = 0.3333f;

      spaceship.weapons_info.shield_modifier = 1.0f;
      spaceship.weapons_info.hull_modifier = 1.0f;

      spaceship.weapons_info.base_color = {5.0f, 1, 1};

      spaceship.weapons_relative_position = { 0, 1, -3 };

      get_resource(SpaceshipTypes)->pheonix = spaceship;

      BoxCollider collider = {};
      collider.half_extents = { 10, 10, 5 };

      spaceship.thrusters_info.acceleration_amounts *= 4.0f;
      spaceship.thrusters_info.max_speed *= 16.0f;

      add_components(player_id, transform, motion, spaceship, collider);
    }

    f32 bias = 2.0f;
    f32 scale = 8000.0f;

    for_every(i, ASTEROID_COUNT) {
      vec3 forward = normalize(vec3 {
        rand_f32_range(-1.0f, 1.0f),
        rand_f32_range(-1.0f, 1.0f),
        rand_f32_range(-1.0f, 1.0f),
      });
      f32 x = rand_f32_range(0.05f / bias, 1.0f);
      f32 distance = scale * pow(x, 1.0f / bias);
      vec3 center = {};

      vec3 position = forward * distance + center;

      Transform transform = {};
      transform.position = position;
      // transform.position.x = rand_f32_range(-6000, 6000);
      // transform.position.y = rand_f32_range(-6000, 6000);
      // transform.position.z = rand_f32_range(-6000, 6000);
      transform.rotation =
        quat_from_axis_angle(VEC3_UNIT_Z, rand_f32_range(0, F32_2PI)) *
        quat_from_axis_angle(VEC3_UNIT_Y, rand_f32_range(0, F32_2PI)) *
        quat_from_axis_angle(VEC3_UNIT_X, rand_f32_range(0, F32_2PI));

      Model model = create_model("cube", VEC3_ONE * ((rand() % 10) * 10));

      // Motion motion = {};
      // motion.velocity = VEC3_ZERO;
      // motion.acceleration = VEC3_ZERO;

      ColorMaterial material = {};
      material.color = { rand_f32_range(0.2f, 1.0f), 0, 0, 1 };

      Asteroid asteroid = {};

      AvoidanceRadius avoid = {};
      avoid.radius = length(model.half_extents) * 5.0f;

      BoxCollider collider = {};
      collider.half_extents = model.half_extents;

      u32 entity_id = create_entity();
      add_components(entity_id, transform, model, material, Asteroid::COMPONENT_ID, avoid, collider);
    }

    bias = 64.0f;
    scale = 20000.0f;

    for_every(i, ASTEROID_COUNT) {
      vec3 forward = normalize(vec3 {
        rand_f32_range(-1.0f, 1.0f),
        rand_f32_range(-1.0f, 1.0f),
        rand_f32_range(-1.0f, 1.0f),
      });
      f32 x = rand_f32_range(0.05f / bias, 1.0f);
      f32 distance = scale * pow(x, 1.0f / bias);
      vec3 center = {};

      vec3 position = forward * distance + center;

      Transform transform = {};
      transform.position = position;
      transform.rotation =
        quat_from_axis_angle(VEC3_UNIT_Z, rand_f32_range(0, F32_2PI)) *
        quat_from_axis_angle(VEC3_UNIT_Y, rand_f32_range(0, F32_2PI)) *
        quat_from_axis_angle(VEC3_UNIT_X, rand_f32_range(0, F32_2PI));

      Model model = create_model("cube", VEC3_ONE * ((rand() % 10) * 10));

      ColorMaterial material = {};
      material.color = { rand_f32_range(0.2f, 1.0f), 0, 0, 1 };

      u32 entity_id = create_entity();
      add_components(entity_id, transform, model, material);
    }

    // add enemies
    for_every(i, ENEMY_COUNT) {
      Transform transform = {};
      transform.position.x = rand_f32_range(-6000, 6000);
      transform.position.y = rand_f32_range(-6000, 6000);
      transform.position.z = rand_f32_range(-6000, 6000);
      transform.rotation = { 0, 0, 0, 1 };

      Model model = create_model("cube", VEC3_ONE * rand_f32_range(12, 20));

      Motion motion = {};
      motion.velocity = VEC3_ZERO;
      motion.acceleration = VEC3_ZERO;

      // Spaceship spaceship = {};
      // spaceship.health = 2.0f;
      // spaceship.max_speed = 500.0f;
      // spaceship.acceleration = 200.0f;

      // spaceship.shield_unbreak_time = 2.0f;
      // spaceship.shield_recover_time = 0.5f;
      // spaceship.shield_recharge_rate = 1.0f;
      // spaceship.max_shield_health = 2.0f;

      ColorMaterial material = {};
      material.color = { 0, rand_f32_range(0.8f, 1.0f), 0, 1 };

      Enemy enemy = {};
      enemy.yaw_pid = create_pid(1.8f, 0.05f, 0.3f);
      enemy.pitch_pid = create_pid(1.8f, 0.05f, 0.3f);
      enemy.roll_pid = create_pid(1.8f, 0.05f, 0.3f);

      BoxCollider collider = {};
      collider.half_extents = model.half_extents * 1.25f;

      AvoidanceRadius avoid = {};
      avoid.radius = length(model.half_extents) * 2.0f;

      Spaceship spaceship = get_resource(SpaceshipTypes)->pheonix;
      spaceship.shields_info.max_health /= 1.0f;
      spaceship.hull_info.base_health /= 1.0f;
      spaceship.hull_health = spaceship.hull_info.base_health;
      spaceship.thrusters_info.acceleration_amounts /= 1.0f;
      spaceship.thrusters_info.max_speed *= 1.05f;
      spaceship.weapons_info.projectile.velocity /= 1.0f;

      u32 entity_id = create_entity();
      add_components(entity_id, transform, model, motion, spaceship, material, enemy, collider);
    }
  
    // Going to use this entity to test the shields system
    // @info testing
    // {
    //   Transform transform = {};
    //   transform.position.x = 0;
    //   transform.position.y = 50;
    //   transform.position.z = 0;
    //   transform.rotation = { 0, 0, 0, 1 };

    //   Model model = create_model("cube", VEC3_ONE * 10.0f);
    //   model.half_extents.z *= 2.0f;

    //   ColorMaterial material = {};
    //   material.color = { 1.0, 1.0, 1.0, 1.0 };

    //   BoxCollider collider = {};
    //   collider.half_extents = model.half_extents * 1.05f;

    //   Motion motion = {};

    //   Spaceship spaceship = get_resource(SpaceshipTypes)->pheonix;
    //   // spaceship.thrusters_info.angular_acceleration_amounts /= 2.0f;
    //   // spaceship.thrusters_info.max_angular_speeds /= 2.0f;
    //   spaceship.thrusters_info.acceleration_amounts /= 1.5f;
    //   spaceship.weapons_info.projectile.velocity /= 1.5f;

    //   TestingTag test = {};
    //   test.test = false;
    //   test.yaw_pid = create_pid(1.0f, 0.1f, 0.1f);
    //   test.pitch_pid = create_pid(1.0f, 0.1f, 0.2f);
    //   test.roll_pid = create_pid(1.0f, 0.1f, 0.2f);

    //   add_components(create_entity(), transform, model, material, collider, spaceship, motion, test);

    //   material.color = { 1.0, 1.2, 1.2, 1.0 };
    //   transform.position.x = 30.0f;
    //   add_components(create_entity(), transform, model, material, collider, spaceship, motion, test);
    // }

    save_ecs();
  }

  // @move to engine
  // void move_camera_basic(Camera3D* camera, vec3 move_inputs, vec2 rotate_dir_and_amount, f32 movement_speed) {
  //   // Rotate camera
  //   camera->rotation += as_eul3(as_vec3(rotate_dir_and_amount, 0));
  //   camera->rotation.pitch = clamp(camera->rotation.pitch, 0.01f, F32_PI - 0.01f);

  //   // Move Camera
  //   vec2 move_xy_dir = normalize_max_length(swizzle(move_inputs, 0, 1), 1.0f);
  //   move_xy_dir = rotate_point(move_xy_dir, camera->rotation.yaw);

  //   vec3 move_dir = as_vec3(move_xy_dir, move_inputs.z);
  //   camera->position += move_dir * delta() * movement_speed;
  // }

  savable vec2 yaw_pitch_accumulator = VEC2_ZERO;

  quat quat_from_look_dir2(vec3 direction, vec3 forward) {
    f32 d = dot(forward, direction);

    if (d < F32_EPSILON - 1.0f) {
      return quat { 0.0f, 0.0f, 0.0f, 1.0f };
    }

    if (d > 1.0f - F32_EPSILON) {
      return quat { 0.0f, 0.0f, 0.0f, 1.0f };
    }

    f32 angle = (float)acos(d);
    vec3 axis = cross(forward, direction);
    axis = normalize(axis);
    return quat_from_axis_angle(axis, angle);
  }

  quat quat_from_look_at2(vec3 position, vec3 target, vec3 forward) {
    vec3 direction = normalize(target - position);
    return quat_from_look_dir2(direction, forward);
  }

  quat quat_from_forward_up(vec3 forward, vec3 up) {
    vec3 f = normalize(-forward);
    vec3 s = normalize(cross(up, f));
    vec3 u = cross(s, f);

    return quat_from_orthonormal_basis(s, f, u);
  }

  void update_player_input() {
    Camera3D* main_camera = get_resource_as(MainCamera, Camera3D);
  
    Transform* player_transform = get_component(player_id, Transform);
    Motion* player_motion = get_component(player_id, Motion);
    Spaceship* player_spaceship = get_component(player_id, Spaceship);

    // @info Camera
    {
      vec2 input = get_action_vec2("look_right", "look_left", "look_up", "look_down");
      f32 roll_input = (get_action("roll_right").value - get_action("roll_left").value);

      yaw_pitch_accumulator.x += input.x * 0.25f;
      yaw_pitch_accumulator.y += input.y * 0.25f;

      yaw_pitch_accumulator.x = clamp(yaw_pitch_accumulator.x, -1, 1);
      yaw_pitch_accumulator.y = clamp(yaw_pitch_accumulator.y, -1, 1);

      f32 target_x = yaw_pitch_accumulator.x * player_spaceship->thrusters_info.max_angular_speeds.x;
      f32 target_y = yaw_pitch_accumulator.y * player_spaceship->thrusters_info.max_angular_speeds.y;
      f32 target_z = roll_input * player_spaceship->thrusters_info.max_angular_speeds.z;

      f32 x = (target_x > player_motion->angular_velocity.x) ? 1 : -1;
      f32 y = (target_y > player_motion->angular_velocity.y) ? 1 : -1;
      f32 z = (target_z > player_motion->angular_velocity.z) ? 1 : -1;

      if(abs(target_x - player_motion->angular_velocity.x) < 0.1f) {
        x *= abs(target_x - player_motion->angular_velocity.x) * 4.0f; // the 4 lets us taper from .4 --> .0
      }

      if(abs(target_y - player_motion->angular_velocity.y) < 0.1f) {
        y *= abs(target_y - player_motion->angular_velocity.y) * 4.0f;
      }

      if(abs(target_z - player_motion->angular_velocity.z) < 0.1f) {
        z *= abs(target_z - player_motion->angular_velocity.z) * 4.0f;
      }

      player_spaceship->input_rotation_direction = vec3 {
        deadzone(x, 0.05f),
        deadzone(y, 0.05f),
        deadzone(z, 0.05f),
      };
    }

    if(get_action("brakes").down) {
      vec3 movement_direction = -normalize(player_motion->velocity);
      player_spaceship->input_movement_direction = rotate(movement_direction, conjugate(player_transform->rotation));
    } else {
      vec3 input_vec3 = get_action_vec3("move_right", "move_left", "move_forward", "move_backward", "move_up", "move_down");
      player_spaceship->input_movement_direction = input_vec3;
    }

    if(get_action("boost").down) {
      player_spaceship->boost_active = true;
    } else {
      player_spaceship->boost_active = false;
    }

    if(get_action("shoot").down) {
      player_spaceship->fire_weapons = true;
    } else {
      player_spaceship->fire_weapons = false;
    }

    if(get_action("ui_save").just_down) {
      save_ecs();
    }

    if(get_action("ui_load").just_down) {
      load_ecs();
    }

    auto nlerp = [&](quat a, quat b, f32 t) {
      return as_quat(normalize(lerp(as_vec4(a), as_vec4(b), t)));
    };

    // @info prediction code for enemies
    for_archetype(Include<Transform, Motion, Spaceship, Enemy> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Motion* motion, Spaceship* spaceship, Enemy* testing) {
      f32 dist = distance(transform->position, player_transform->position);
      f32 velocity = spaceship->weapons_info.type == WeaponType::Projectile ? spaceship->weapons_info.projectile.velocity : F32_MAX;
      f32 predicted_time_to_impact = dist / velocity;
      vec3 predicted_position = player_transform->position +
        (player_motion->velocity * predicted_time_to_impact) +
        (0.5f * player_motion->acceleration * predicted_time_to_impact * predicted_time_to_impact);

      vec3 direction = normalize(predicted_position - transform->position);
      vec3 direction_local = rotate(direction, conjugate(transform->rotation));

      vec3 angles = {};
      f32 yaw = atan2(-direction_local.y, -direction_local.x);
      f32 pitch = asin(direction_local.z);
      f32 roll = motion->angular_velocity.z > 0.0f ? -1 : 1;

      angles.x = yaw + F32_PI_2;
      angles.y = pitch;
      angles.z = roll;

      angles *= 4.0f;
      angles.x = clamp(angles.x, -1.0f, 1.0f);
      angles.y = clamp(angles.y, -1.0f, 1.0f);
      angles.z = clamp(angles.z, -1.0f, 1.0f);

      spaceship->input_rotation_direction.x = tick_pid(&testing->yaw_pid, angles.x, delta());
      spaceship->input_rotation_direction.y = tick_pid(&testing->pitch_pid, angles.y, delta());
      spaceship->input_rotation_direction.z = tick_pid(&testing->roll_pid, angles.z, delta());

      spaceship->input_movement_direction = direction_local;

      if(dot(direction_local, VEC3_UNIT_Y) > 0.9f && distance(transform->position, player_transform->position) < 2000.0f) {
        spaceship->fire_weapons = true;
      } else {
        spaceship->fire_weapons = false;
      }

      // {
      //   Model base_model = create_model("cube", VEC3_ONE * 1.0f);

      //   Drawable drawable = {};
      //   drawable.model = base_model;
      //   drawable.model.half_extents.y *= 20.0f;
      //   drawable.transform.position = transform->position;
      //   drawable.transform.rotation = transform->rotation;

      //   ColorMaterial material = {};
      //   material.color = { 0, 1, 0, 1 };

      //   push_drawable_instance(&drawable, &material);
      // }
    });
  }

  Shield* select_shield(Transform* transform, Spaceship* spaceship, vec3 normal) {
    Shield* hit_shield = 0;
    f32 best_similarity = -1;

    for_every(i, 6) {
      Shield* shield = &spaceship->shields[i];

      // we want positive similarity
      f32 similarity = dot(rotate(SHIELD_DIRECTIONS[i], transform->rotation), normal);

      if(similarity > best_similarity) {
        hit_shield = shield;
        best_similarity = similarity;
      }
    }

    return hit_shield;
  }

  void damage_spaceship(u32 entity_id, vec3 hit_normal, f32 damage, f32 shield_modifier, f32 hull_modifier) {
    Transform* transform = get_component(entity_id, Transform);
    Spaceship* spaceship = get_component(entity_id, Spaceship);

    Shield* hit_shield = select_shield(transform, spaceship, hit_normal);

    hit_shield->health -= damage * shield_modifier;
    if(hit_shield->health < 0.0f) {
      f32 hull_damage_rollover = abs(hit_shield->health / shield_modifier) * hull_modifier;

      hit_shield->health = 0.0f;
      hit_shield->last_hit_time = 0.0f;
      hit_shield->broken_time = spaceship->shields_info.unbreak_time;

      spaceship->hull_health -= hull_damage_rollover;
    } else {
      hit_shield->last_hit_time = spaceship->shields_info.recover_time;
    }
  }

  void update_spaceship_weapons() {
    // @info fire weapons
    for_archetype(Include<Transform, Spaceship, Motion> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Spaceship* spaceship, Motion* motion) {
      spaceship->weapons_cooldown = clamp(spaceship->weapons_cooldown - delta(), 0.0f, FLT_MAX);

      if(!spaceship->fire_weapons) {
        return;
      }

      // @todo update this to fire as many projectiles as can fit into
      // the frame
      if(spaceship->weapons_cooldown <= 0.0f) {
        vec3 fire_direction = quat_forward(transform->rotation);
        vec3 fire_position = transform->position + rotate(spaceship->weapons_relative_position, transform->rotation);

        if(spaceship->weapons_info.type == WeaponType::Beam) {
          BeamWeaponInfo info = spaceship->weapons_info.beam;

          spaceship->weapons_cooldown = 0;

          RaycastInfo raycast_info = cast_ray(entity_id, fire_position, fire_direction, info.beam_length);

          if(raycast_info.hit && has_component(raycast_info.hit_id, Spaceship)) {
            f32 damage = info.base_dps * delta();
            damage_spaceship(raycast_info.hit_id, raycast_info.hit_normal, damage, spaceship->weapons_info.shield_modifier, spaceship->weapons_info.hull_modifier);
          }

          f32 beam_length = raycast_info.hit ? raycast_info.hit_distance : info.beam_length;
          f32 half_length = beam_length / 2.0f;

          // @info draw beam
          {
            Drawable drawable = {};
            drawable.transform.position = fire_position + half_length * fire_direction;
            drawable.transform.rotation = transform->rotation;
            drawable.model = create_model("cube", VEC3_ONE);
            drawable.model.half_extents = vec3 { 0.3f, beam_length / 2.0f, 0.3f };

            ColorMaterial material = {};
            material.color = as_vec4(spaceship->weapons_info.base_color, 1.0f);

            push_drawable_instance(&drawable, &material);
          }
        }

        else if(spaceship->weapons_info.type == WeaponType::Projectile) {
          ProjectileWeaponInfo info = spaceship->weapons_info.projectile;

          spaceship->weapons_cooldown = info.base_cooldown;

          Transform projectile_transform = {};
          projectile_transform.position = fire_position;
          projectile_transform.rotation = transform->rotation;

          Model model = {};
          model = create_model("cube", VEC3_ONE);
          model.half_extents = vec3 { 0.8f, info.velocity / 200.0f, 0.8f };

          ColorMaterial material = {};
          material.color = as_vec4(spaceship->weapons_info.base_color, 1.0f);

          Projectile projectile = {};
          projectile.base_damage = info.base_damage;
          projectile.velocity = normalize_to_max_length(motion->velocity + quat_forward(transform->rotation) * info.velocity, info.velocity + length(motion->velocity));
          projectile.caster_id = entity_id;
          projectile.lifetime = info.lifetime;
          projectile.hull_modifier = spaceship->weapons_info.hull_modifier;
          projectile.shield_modifier = spaceship->weapons_info.shield_modifier;

          u32 projectile_id = create_entity();
          add_components(projectile_id, projectile_transform, model, material, projectile);
          if(entity_id == player_id) {
            play_sound("spaceships/sound/shoot.wav", {});
          }
        }

        else {
        }
      }
    });

    // @info update projectile weapons
    for_archetype(Include<Transform, Projectile> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Projectile* projectile) {
      if(projectile->lifetime <= 0.0f) {
        destroy_entity(entity_id);
        return;
      }
      projectile->lifetime -= delta();

      f32 ray_length = length(projectile->velocity) * delta();
      RaycastInfo raycast_info = cast_ray(projectile->caster_id, transform->position, quat_forward(transform->rotation), ray_length);

      transform->position += projectile->velocity * delta();

      if(raycast_info.hit && has_component(raycast_info.hit_id, Spaceship)) {
        damage_spaceship(raycast_info.hit_id, raycast_info.hit_normal, projectile->base_damage, projectile->shield_modifier, projectile->hull_modifier);
        destroy_entity(entity_id);
        return;
      }

      if(raycast_info.hit) {
        destroy_entity(entity_id);
        return;
      }
    });

    // @info remove dead spaceships
    for_archetype(Include<Motion, Spaceship> {}, Exclude<> {},
    [&](u32 entity_id, Motion* motion, Spaceship* spaceship) {
      if(spaceship->hull_health <= 0.0f) {
        // destroy_entity(entity_id);
        remove_component_id(entity_id, Spaceship::COMPONENT_ID);
        motion->acceleration = VEC3_ZERO;
        motion->angular_acceleration = VEC3_ZERO;

        if(has_component(entity_id, ColorMaterial)) {
          ColorMaterial* material = get_component(entity_id, ColorMaterial);
          material->color *= 0.333333f;
        }
      }
    });

    if(!has_component(player_id, Spaceship)) {
      load_ecs();
    }

    // @info update spaceship regens
    for_archetype(Include<Spaceship> {}, Exclude<> {},
    [&](u32 entity_id, Spaceship* spaceship) {
      // @info update shields
      for_every(i, 6) {
        Shield* shield = &spaceship->shields[i];
        shield->broken_time -= delta();
        shield->last_hit_time -= delta();

        if(shield->broken_time <= 0 && shield->last_hit_time <= 0) {
          shield->health += spaceship->shields_info.recharge_rate * delta();
          shield->health = clamp(shield->health, 0, spaceship->shields_info.max_health);
        }
      }

      // @info update thrusters
      if(spaceship->boost_active) {
        spaceship->boost_amount -= spaceship->thrusters_info.boost_drain_rate * delta();
        spaceship->boost_amount = clamp(spaceship->boost_amount, 0.0f, F32_MAX);
      } else {
        spaceship->boost_amount += spaceship->thrusters_info.boost_recharge_rate * delta();
        spaceship->boost_amount = clamp(spaceship->boost_amount, 0.0f, spaceship->thrusters_info.max_boost_amount);
      }
    });

    // for_archetype(Include<Transform, Motion, TestingTag> {}, Exclude<> {},
    // [&](u32 entity_id, Transform* transform, Motion* motion, TestingTag*) {
    //   motion->angular_velocity.x = sinf(entity_id * F32_PI_2);
    //   motion->angular_velocity.y = cosf(entity_id * -F32_PI_2);
    //   motion->angular_velocity.z = F32_PI_2;
    // });

    for_archetype(Include<Transform, Motion, Enemy> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Motion* motion, Enemy*) {
    });
  }

  void add_entities_to_render_batches() {
    {
      char text[128];
      auto* info =  get_resource(DrawBatchContext);
      sprintf(text, "Fps: %.0f\nDraw Count: %d\nCull Count: %d", 1.0f / delta(), info->saved_total_draw_count, info->saved_total_culled_count);
      push_ui_text(20, (f32)get_window_dimensions().y - 20, 20, 20, {2, 2, 2, 1}, text);
    }
  
    for_archetype(Include<Transform, Model, ColorMaterial> {}, Exclude<TestingTag> {},
    [&](u32 entity_id, Transform* transform, Model* model, ColorMaterial* material) {
      Drawable drawable = {};
      drawable.model = *model;
      drawable.transform = *transform;

      push_drawable_instance(&drawable, material);
    });

    for_archetype(Include<Transform, Model, ColorMaterial, TestingTag> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Model* model, ColorMaterial* material, TestingTag* testing) {
      Drawable drawable = {};
      drawable.model = *model;
      drawable.transform = *transform;


      ColorMaterial material2 = *material;

      push_drawable_instance(&drawable, &material2);
    });

    for_archetype(Include<Transform, Model, Spaceship, BoxCollider> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Model* model, Spaceship* spaceship, BoxCollider* collider) {
      for_every(i, 6) {
        vec3 s = abs(SHIELD_DIRECTIONS[i]);
        vec3 s_inv = 1.0f - s;

        Shield* shield = &spaceship->shields[i];

        Drawable drawable = {};
        drawable.transform.position = transform->position + rotate(SHIELD_DIRECTIONS[i], transform->rotation) * collider->half_extents;
        drawable.transform.rotation = transform->rotation;
        drawable.model.id = model->id;
        drawable.model.half_extents = s_inv * collider->half_extents;
        drawable.model.half_extents += s * clamp(length(collider->half_extents), 0.0f, 2.0f) * 0.1f;

        vec4 shield_hit_color = { 1, 1.5, 2, 1 };
        vec4 hull_hit_color = { 4, 1, 1, 1 };

        ColorMaterial material = {};
        material.color = shield_hit_color * (0.5f + (shield->health / spaceship->shields_info.max_health));

        if(shield->last_hit_time > 0.0f) {
          material.color = shield_hit_color * (0.5f + (1.0f - (shield->last_hit_time / spaceship->shields_info.recover_time))) + (0.5f * (shield->health / spaceship->shields_info.max_health));
          push_drawable_instance(&drawable, &material);
        }
        else if (shield->broken_time > 0.0f) {
          material.color = hull_hit_color * (0.5f + (1.0f - (shield->broken_time / spaceship->shields_info.unbreak_time)));
          push_drawable_instance(&drawable, &material);
        }
      }
    });
  }

  vec3 vec3_clamp(vec3 a, f32 min, f32 max) {
    return vec3 {
      clamp(a.x, min, max),
      clamp(a.y, min, max),
      clamp(a.z, min, max),
    };
  }

  void update_spaceship_movement() {
    // @info floating point precision fix
    // Move everything so that it is centered around the player,
    // this fixes floating point precision problems when the player
    // is far away from the original world origin.
    {
      Transform* player_transform = get_component(player_id, Transform);
      for_archetype(Include<Transform> {}, Exclude<>{}, [&](u32 entity_id, Transform* transform) {
        if(entity_id == player_id) {
          return;
        }

        transform->position -= player_transform->position;
      });

      player_transform->position = VEC3_ZERO;
    }

    // @info spaceship input to physics
    for_archetype(Include<Transform, Motion, Spaceship> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Motion* motion, Spaceship* spaceship) {
      vec3 local_movement_direction = rotate(normalize(motion->velocity), conjugate(transform->rotation));
      f32 fraction_of_max_speed = length(motion->velocity) / spaceship->thrusters_info.max_speed;
      vec3 spaceship_input_dir = normalize_or_zero(spaceship->input_movement_direction);

      // @info general movement
      {
        // @fix not sure if i should normalize here
        vec3 acceleration = spaceship_input_dir * spaceship->thrusters_info.acceleration_amounts;
        motion->acceleration = rotate(acceleration, transform->rotation);
      }

      // @info slowdown movement
      {
        vec3 input_direction = rotate(-normalize(motion->velocity), conjugate(transform->rotation));
        vec3 acceleration_local = input_direction * spaceship->thrusters_info.acceleration_amounts;
        vec3 acceleration = rotate(acceleration_local, transform->rotation);

        // @info overspeed
        if (fraction_of_max_speed > 1.0f) {
          f32 fac = clamp(lerp(0.1f, 100.0f, fraction_of_max_speed - 1.0f), 0.0f, 1.0f);
          f32 extra = (clamp(dot(spaceship_input_dir, local_movement_direction), 0.0f, 1.0f) + 1.0f);

          motion->acceleration += acceleration * fac * extra;
        }
        // @info no inputs
        else if(spaceship->input_movement_direction == VEC3_ZERO) {
          motion->acceleration = acceleration * clamp(fraction_of_max_speed, 0.1f, 0.2f);
        }
      }

      // @fix technically this allows you to boost every other frame which is a little jank
      if(spaceship->boost_active && spaceship->boost_amount > 0.0f) {
        motion->acceleration *= spaceship->thrusters_info.boost_speedup_factor;
      }

      // @info angular movement
      {
        vec3 angular_acceleration = spaceship->input_rotation_direction * spaceship->thrusters_info.angular_acceleration_amounts;

        f32 x_fac = abs(motion->angular_velocity.x) / spaceship->thrusters_info.max_angular_speeds.x;
        f32 y_fac = abs(motion->angular_velocity.y) / spaceship->thrusters_info.max_angular_speeds.y;
        f32 z_fac = abs(motion->angular_velocity.z) / spaceship->thrusters_info.max_angular_speeds.z;

        if(x_fac > 1.0f) {
          f32 dir = motion->angular_velocity.x > 0.0f ? -1 : 1;
          dir *= clamp(lerp(0.0f, 100.0f, abs(x_fac)), 0.0f, 1.0f);
          angular_acceleration.x = dir * spaceship->thrusters_info.angular_acceleration_amounts.x;
        }

        if(y_fac > 1.0f) {
          f32 dir = motion->angular_velocity.y > 0.0f ? -1 : 1;
          dir *= clamp(lerp(0.0f, 100.0f, abs(y_fac)), 0.0f, 1.0f);
          angular_acceleration.y = dir * spaceship->thrusters_info.angular_acceleration_amounts.y;
        }

        if(z_fac > 1.0f) {
          f32 dir = motion->angular_velocity.z > 0.0f ? -1 : 1;
          dir *= clamp(lerp(0.0f, 100.0f, abs(z_fac)), 0.0f, 1.0f);
          angular_acceleration.z = dir * spaceship->thrusters_info.angular_acceleration_amounts.z;
        }

        motion->angular_acceleration = angular_acceleration;
      }
    });

    // @info asteroid movement
    for_archetype(Include<Transform, Motion, Asteroid> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Motion* motion, Asteroid* a) {
    });
  }

  void sync_camera_to_player() {
    Transform* player_transform = get_component(player_id, Transform);
    Camera3D* main_camera = get_resource_as(MainCamera, Camera3D);
    Motion* player_motion = get_component(player_id, Motion);
    Spaceship* player_spaceship = get_component(player_id, Spaceship);

    main_camera->position = player_transform->position;
    main_camera->rotation = player_transform->rotation;

    // @info Draw blips
    {
      vec3 movement_direction = normalize(player_motion->velocity);
      f32 movement_factor = length(player_motion->velocity) / player_spaceship->thrusters_info.max_speed;

      vec3 f = quat_forward(main_camera->rotation);
      vec3 d = -quat_up(main_camera->rotation);
      vec3 r = quat_right(main_camera->rotation);

      bool boosting = player_spaceship->boost_active && player_spaceship->boost_amount > 0.0f;

      // @info blip for center of screen
      {
        vec3 base_draw_position = main_camera->position + f * 1.0f;

        Drawable drawable = {};
        drawable.model = create_model("cube", VEC3_ONE * 0.005f);
        drawable.transform.position = base_draw_position;
        drawable.transform.rotation = main_camera->rotation;

        ColorMaterial material = {};
        material.color = { 0.5, 0.5, 0.5, 1 };
        // material.color += clamp((shoot_timer / max_shoot_time), 0, 1);

        push_drawable_instance(&drawable, &material);

        material.color = { 1.5, 1.5, 1.0f, 1.0f };
        drawable.transform.position += -r * yaw_pitch_accumulator.x * 0.1f;
        drawable.transform.position += -d * yaw_pitch_accumulator.y * 0.1f;
        drawable.model.half_extents *= 1.25f;

        push_drawable_instance(&drawable, &material);
      }

      // @info blip for current velocity
      {
        vec3 base_draw_position = main_camera->position + f * 1.0f + d * 0.5f;

        Drawable drawable = {};
        drawable.model = create_model("cube", VEC3_ONE * 0.03333f);
        drawable.transform.position = base_draw_position;
        drawable.transform.rotation = main_camera->rotation;

        ColorMaterial material = {};
        material.color = { 0.5, 0.8, 0.8, 1 };

        f32 pos_delta = 0.1f;
        f32 scl_delta = 0.666667f;

        if(get_action("brakes").down) {
          material.color.y += 1.0f;
        }

        if(boosting) {
          material.color.x += 1.0f;
        }

        push_drawable_instance(&drawable, &material);

        drawable.transform.position += movement_direction * pos_delta * movement_factor;
        drawable.model.half_extents *= scl_delta; 

        material.color *= 0.8f;
        push_drawable_instance(&drawable, &material);

        drawable.transform.position += movement_direction * pos_delta * movement_factor;
        drawable.model.half_extents *= scl_delta; 

        material.color *= 0.8f;
        push_drawable_instance(&drawable, &material);
      }

      // @info blip for boost amount
      {
        vec3 base_draw_position = main_camera->position + f * 0.75f + d * 0.5f + r * 0.5f;

        Drawable drawable = {};
        drawable.model = create_model("cube", VEC3_ONE * 0.03333f * ((player_spaceship->boost_amount / player_spaceship->thrusters_info.max_boost_amount) + 0.2f));
        drawable.transform.position = base_draw_position;
        drawable.transform.rotation = main_camera->rotation;

        ColorMaterial material = {};
        material.color = { .5, .8, .8, 1 };
        material.color.x *= boosting ? 4.0f : 1.0f;

        push_drawable_instance(&drawable, &material);
      }
    }
  }

  void exit_on_esc() {
    if(get_action("ui_mouse").just_down) {
      set_window_should_close();
      // MouseMode mouse_mode = get_mouse_mode();
  
      // if(mouse_mode == MouseMode::Hidden || mouse_mode == MouseMode::Visible) {
      //   set_mouse_mode(MouseMode::Captured);
      //   return;
      // }
  
      // if(mouse_mode == MouseMode::Captured) {
      //   set_mouse_mode(MouseMode::Visible);
      //   return;
      // }
    }
  }
};

