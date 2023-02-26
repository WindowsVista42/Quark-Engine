#pragma once

#if defined(_WIN32) || defined(_WIN64)
  #if defined(TEXT_RENDERING_IMPLEMENTATION)
    #define text_rendering_api __declspec(dllexport)
    #define text_rendering_var extern __declspec(dllexport)
  #else
    #define text_rendering_api __declspec(dllimport)
    #define text_rendering_var extern __declspec(dllimport)
  #endif
#endif

#ifndef text_rendering_api
  #define text_rendering_api
  #define text_rendering_var extern
#endif