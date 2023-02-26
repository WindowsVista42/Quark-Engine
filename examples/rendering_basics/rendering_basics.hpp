#pragma once

#include <quark/module.hpp>
using namespace quark;

#include "api.hpp"

#define api_decl rendering_basics_api
#define var_decl rendering_basics_var

namespace quark::rendering_basics {
  // Init the example
  api_decl void init_rendering_basics();

  // Add drawawble entities to the scene
  api_decl void add_entities();

  // Update the colors for all of the entities
  api_decl void update_entities();

  // Read input and move the camera around the scene
  api_decl void update_camera();

  // Add the drawable to the draw batches
  api_decl void add_entities_for_rendering();

  api_decl void exit_on_esc();
}

#undef api_decl
#undef var_decl
