#pragma once

#include <quark/module.hpp>
using namespace quark;

#include "api.hpp"

#define api_decl text_rendering_api
#define var_decl text_rendering_var

namespace quark::text_rendering {
  api_decl void push_text_to_screen();

  api_decl void exit_on_esc();
}

#undef api_decl
#undef var_decl
