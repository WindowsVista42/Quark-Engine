#pragma once

#include <quark/module.hpp>
using namespace quark;

#include "api.hpp"

#define api_decl performance_test_api
#define var_decl performance_test_var

namespace quark::performance_test {
//
// Perf Test Params
//

  static const u32 PERF_COUNT_X = 48;
  static const u32 PERF_COUNT_Y = 48;
  static const u32 PERF_COUNT_Z = 48;
  static const f32 PERF_OFFSET = 6.0f;
  static const char* PERF_MODELS[] = { "suzanne", "cube", "sphere" };
  static const vec3 PERF_ROOT_POS = { 0.0f, 0.0f, 0.0f };

//
// Global Init Jobs
//

  api_decl void init_performance_test();

//
// Init Jobs
//

  api_decl void init_entities();

//
// Update Jobs
//

  api_decl void update_camera();
  api_decl void update_perf_test();
  api_decl void add_entities_to_render_batches();
  api_decl void exit_on_esc();
}

#undef api_decl
#undef var_decl
