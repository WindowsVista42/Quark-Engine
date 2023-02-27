#pragma once

#include <quark/module.hpp>
using namespace quark;

#include "api.hpp"

#define api_decl ecs_particles_api
#define var_decl ecs_particles_var

namespace quark::ecs_particles {
  // Physics "Motion" struct, pulled from the spaceships demo
  declare_component(Motion,
    vec3 velocity;
    vec3 acceleration;
    vec3 impulse;

    vec3 angular_velocity;
    vec3 angular_acceleration;
    vec3 angular_impulse;
  );

  // Particle parameters to animate over time
  declare_component(Particle,
    f32 alive_time;
    f32 lifetime;

    vec4 base_color;
    vec4 final_color;

    vec3 base_half_extents;
    vec3 final_half_extents;
  );

  // Init the plugin
  api_decl void init_ecs_particles();

  // Update the camera
  api_decl void update_camera();

  // Update particle simulation parameters
  api_decl void update_params();

  // Spawn particles when the spacebar is pressed
  api_decl void spawn_particles();

  // Update entities
  api_decl void update_entities();

  // Push renderables
  api_decl void push_renderables();

  api_decl void exit_on_esc();
}

#undef api_decl
#undef var_decl
