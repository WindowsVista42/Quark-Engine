#pragma once

#include <quark/module.hpp>
using namespace quark;

#include "api.hpp"

#define api_decl ecs_basics_api
#define var_decl ecs_basics_var

namespace quark::ecs_basics {
  // This is how you "declare" a component.
  // The syntax is effectively the same as declaring a struct.
  // Under the hood, this defines a couple of variables to be used internally.
  // Namely this declares an integer id that gets used to refer to which table
  // stores the data for this component.
  declare_component(CustomTimer,
    f32 remainder;
  );

  // Init the example
  api_decl void init_ecs_basics();

  // Add CustomTimers
  api_decl void add_custom_timers();

  // Update the timers, decrementing the remaining time.
  // Add new timers when all timers have finished.
  api_decl void update_custom_timers();

  api_decl void exit_on_esc();
}

#undef api_decl
#undef var_decl
