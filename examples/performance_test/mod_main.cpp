#define PERFORMANCE_TEST_IMPLEMENTATION
#include "performance_test.hpp"

mod_main() {
  using namespace performance_test;

  // Add init_performance_test() to global_init
  create_system("init_performance_test", init_performance_test);
  add_system("quark_init", "init_performance_test", "", -1);

  // Add init jobs to init
  create_system("init_entities", init_entities);
  add_system("init", "init_entities", "", -1);

  // Add update jobs to update
  create_system("update_camera", update_camera);
  create_system("update_perf_test", update_perf_test);
  create_system("add_entities_to_render_batches", add_entities_to_render_batches);
  create_system("exit_on_esc", exit_on_esc);
  
  add_system("update", "update_camera", "update_tag", -1);
  add_system("update", "update_perf_test", "update_tag", 1);
  add_system("update", "add_entities_to_render_batches", "begin_frame", -1);
  add_system("update", "exit_on_esc", "", -1);
  
  // Update some engine constants.
  // In the short future these will be moved to configuration resources
  ECS_MAX_STORAGE = 256 * 1024;
  PRINT_PERFORMANCE_STATISTICS = true;
}
