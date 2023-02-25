#pragma once

#if defined(_WIN32) || defined(_WIN64)
  #if defined(PERFORMANCE_TEST_IMPLEMENTATION)
    #define performance_test_api __declspec(dllexport)
    #define performance_test_var extern __declspec(dllexport)
  #else
    #define performance_test_api __declspec(dllimport)
    #define performance_test_var extern __declspec(dllimport)
  #endif
#endif

#ifndef performance_test_api
  #define performance_test_api
  #define performance_test_var extern
#endif