#define RENDERING_BASICS_IMPLEMENTATION
#include "rendering_basics.hpp"

mod_main() {
  using namespace rendering_basics;

  // Global init jobs
  create_system("init_rendering_basics", init_rendering_basics);
  add_system("quark_init", "init_rendering_basics", "", -1);

  // Init jobs
  create_system("add_entities", add_entities);
  add_system("init", "add_entities", "", -1);

  // Update jobs
  create_system("exit_on_esc", exit_on_esc);
  create_system("update_entities", update_entities);
  create_system("update_camera", update_camera);
  create_system("add_entities_for_rendering", add_entities_for_rendering);

  add_system("update", "update_entities", "update_tag", -1);
  add_system("update", "add_entities_for_rendering", "begin_frame", -1);
  add_system("update", "update_camera", "add_entities_for_rendering", 1);
  add_system("update", "exit_on_esc", "", -1);

  // Set some engine constants
  PRINT_PERFORMANCE_STATISTICS = false;
}
