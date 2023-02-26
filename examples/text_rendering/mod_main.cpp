#define TEXT_RENDERING_IMPLEMENTATION
#include "text_rendering.hpp"

mod_main() {
  using namespace text_rendering;
  
  // Update jobs
  create_system("exit_on_esc", exit_on_esc);
  create_system("push_text_to_screen", push_text_to_screen);
  
  add_system("update", "push_text_to_screen", "begin_frame", -1);
  add_system("update", "exit_on_esc", "", -1);

  // Set some engine constants
  set_window_dimensions({1920 / 2, 1080 / 2});
  PRINT_PERFORMANCE_STATISTICS = false;
}
