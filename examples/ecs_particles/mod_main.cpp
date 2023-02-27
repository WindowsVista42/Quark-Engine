#define ECS_PARTICLES_IMPLEMENTATION
#include "ecs_particles.hpp"

mod_main() {
  using namespace ecs_particles;

  // Global init jobs
  create_system("init_ecs_particles", init_ecs_particles);
  add_system("quark_init", "init_ecs_particles", "", -1);
  
  // Update jobs
  create_system("exit_on_esc", exit_on_esc);
  create_system("update_params", update_params);
  create_system("spawn_particles", spawn_particles);
  create_system("update_entities", update_entities);
  create_system("update_camera", update_camera);
  create_system("push_renderables", push_renderables);
  
  add_system("update", "update_params", "update_tag", -1);
  add_system("update", "spawn_particles", "update_tag", -1);
  add_system("update", "update_entities", "update_tag", -1);
  add_system("update", "update_camera", "update_tag", 1);
  add_system("update", "push_renderables", "begin_frame", -1);
  add_system("update", "exit_on_esc", "", -1);

  // Set some engine constants
  set_window_dimensions({1920 / 1, 1080 / 1});
  PRINT_PERFORMANCE_STATISTICS = false;
}
