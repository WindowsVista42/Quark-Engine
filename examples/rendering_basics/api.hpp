#pragma once

#if defined(_WIN32) || defined(_WIN64)
  #if defined(RENDERING_BASICS_IMPLEMENTATION)
    #define rendering_basics_api __declspec(dllexport)
    #define rendering_basics_var extern __declspec(dllexport)
  #else
    #define rendering_basics_api __declspec(dllimport)
    #define rendering_basics_var extern __declspec(dllimport)
  #endif
#endif

#ifndef rendering_basics_api
  #define rendering_basics_api
  #define rendering_basics_var extern
#endif