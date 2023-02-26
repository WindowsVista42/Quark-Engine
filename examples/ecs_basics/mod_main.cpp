#define ECS_BASICS_IMPLEMENTATION
#include "ecs_basics.hpp"

mod_main() {
  using namespace ecs_basics;

  // Global init jobs
  create_system("init_ecs_basics", init_ecs_basics);
  add_system("quark_init", "init_ecs_basics", "", -1);

  // Init jobs
  create_system("add_custom_timers", add_custom_timers);
  add_system("init", "add_custom_timers", "", -1);
  
  // Update jobs
  create_system("exit_on_esc", exit_on_esc);
  create_system("update_custom_timers", update_custom_timers);
  
  add_system("update", "update_custom_timers", "update_tag", -1);
  add_system("update", "exit_on_esc", "", -1);

  // Set some engine constants
  set_window_dimensions({1920 / 8, 1080 / 8});
  PRINT_PERFORMANCE_STATISTICS = false;
}
