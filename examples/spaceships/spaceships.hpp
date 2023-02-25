#pragma once

#include <quark/module.hpp>
using namespace quark;

#include "api.hpp"

#define api_decl spaceships_api
#define var_decl spaceships_var

#define create_system2(name) create_system(#name, name);
#define add_system2(list, name, after, position) add_system(list, #name, #after, position);

namespace quark::spaceships {
//
// Utils
//

  struct Pid {
    f32 kp;
    f32 ki;
    f32 kd;

    f32 error_sum;
    f32 last_error;
  };

  spaceships_api Pid create_pid(f32 kp, f32 ki, f32 kd);
  spaceships_api f32 tick_pid(Pid* pid, f32 error, f32 delta_time);

//
// Physics
//
  struct RawRayHitInfo {
    f32 t;
    vec3 normal;
  };

  struct RaycastInfo {
    bool hit;
    vec3 hit_position;
    vec3 hit_normal;
    f32 hit_distance;
    u32 hit_id;
  };

  declare_component(Motion,
    vec3 velocity;
    vec3 acceleration;
    vec3 impulse;

    vec3 angular_velocity;
    vec3 angular_acceleration;
    vec3 angular_impulse;
  );

  declare_component(BoxCollider,
    vec3 half_extents;
  );

  struct CastRay {
    u32 caster_id;
    vec3 origin;
    vec3 direction;
    f32 length;
  };

  struct CastRayMulti {
    u32 caster_id;
    vec3 origin;
    vec3 direction;
    f32 length;
  };

  struct CollisionEvent {
    u32 hitter;
    u32 hittee;

    vec3 contact_point;
    vec3 velocity_difference;
  };

  // declare_resource(PhysicsContext,
  // );

  spaceships_api RawRayHitInfo ray_intersects_box(vec3 ray_origin, vec3 ray_direction, Transform* transform, BoxCollider* collider);
  spaceships_api RaycastInfo cast_ray(u32 caster_id, vec3 ray_origin, vec3 ray_direction, f32 ray_length);

  spaceships_api void update_physics();

//
// Gameplay
//

  declare_component(EnemyMove);
  declare_component(EnemyAttack);
  declare_component(EnemyRetreat);

  declare_component(Enemy,
    // Pid move_target_pid;

    Pid yaw_pid;
    Pid pitch_pid;
    Pid roll_pid;
  );

  declare_component(Asteroid);

  struct Shield {
    f32 health;
    f32 broken_time; // if health goes to zero then we want to set this
    f32 last_hit_time;
  };

  constexpr vec3 SHIELD_DIRECTIONS[6] = {
    { 0,  1,  0},
    { 0, -1,  0},
    { 1,  0,  0},
    {-1,  0,  0},
    { 0,  0,  1},
    { 0,  0, -1},
  };

  struct HullInfo {
    f32 base_health;

    vec3 base_color;
  };

  struct ThrustersInfo {
    f32 max_speed;
    vec3 max_angular_speeds;

    f32 max_boost_amount;
    f32 boost_recharge_rate;
    f32 boost_drain_rate;
    f32 boost_speedup_factor;

    vec3 acceleration_amounts;
    vec3 angular_acceleration_amounts;

    vec3 base_color;
    vec3 tail_color;
  };

  struct ShieldsInfo {
    f32 unbreak_time;
    f32 recover_time;
    f32 recharge_rate;
    f32 max_health;

    vec4 base_color;
    vec4 low_hp_color;
    vec4 broken_color;
  };

  declare_enum(WeaponType, u32,
    Beam,
    Projectile,
    // Missile,
    // Rocket,
  );

  struct BeamWeaponInfo {
    f32 base_dps;
    f32 beam_length;
  };

  struct ProjectileWeaponInfo {
    f32 base_damage;
    f32 base_cooldown;
    f32 velocity;
    f32 lifetime;
  };

  declare_component(Projectile,
    f32 base_damage;
    f32 shield_modifier;
    f32 hull_modifier;
    vec3 velocity;
    f32 lifetime;
    u32 caster_id;
  );

  struct WeaponsInfo {
    WeaponType type;
    union {
      BeamWeaponInfo beam;
      ProjectileWeaponInfo projectile;
    };

    f32 shield_modifier;
    f32 hull_modifier;

    vec3 base_color;
  };

  declare_component(Spaceship,
    f32 hull_health;

    vec3 input_movement_direction;
    vec3 input_rotation_direction;

    bool boost_active;
    f32 boost_amount;

    // forward, backward, right, left, top, bottom.
    // Use SHIELD_DIRECTIONS to get vec3 directions
    Shield shields[6];

    bool fire_weapons;
    f32 weapons_cooldown;
    f32 weapons_duration;
    vec3 weapons_relative_position;

    HullInfo hull_info;
    ThrustersInfo thrusters_info;
    ShieldsInfo shields_info;
    WeaponsInfo weapons_info;
  );

  declare_component(AvoidanceRadius,
    f32 radius;
  );

  declare_component(TestingTag,
    // Pid yaw_pid;
    // Pid pitch_pid;
    // Pid roll_pid;
  );

  declare_resource(SpaceshipTypes,
    Spaceship pheonix;
    Spaceship wyvern;
    Spaceship minotaur;

    const char* names[3] = {
      "Pheonix",
      "Wyvern",
      "Minotaur",
    };
  );

  spaceships_var u32 player_id;

  spaceships_api void init_entities();

  spaceships_api void update_player_input();
  spaceships_api void update_spaceship_movement();
  spaceships_api void update_spaceship_weapons();
  spaceships_api void sync_camera_to_player();

  spaceships_api void add_entities_to_render_batches();

  spaceships_api void exit_on_esc();

  inline f32 rand_f32_range(f32 low, f32 high) {
    f32 t = (f32)rand() / (f32)RAND_MAX;

    return lerp(low, high, t);
  }

  inline vec3 rand_vec3_range(vec3 min, vec3 max) {
    vec3 result = {};
    result.x = rand_f32_range(min.x, max.x);
    result.y = rand_f32_range(min.y, max.y);
    result.z = rand_f32_range(min.z, max.z);
    return result;
  }

  inline u32 rand_u32_range(u32 min, u32 max) {
    return (rand() % (max - min)) + min;
  }
}

#undef api_decl
#undef var_decl
