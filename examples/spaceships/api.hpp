#pragma once

#if defined(_WIN32) || defined(_WIN64)
  #if defined(SPACESHIPS_IMPLEMENTATION)
    #define spaceships_api __declspec(dllexport)
    #define spaceships_var extern __declspec(dllexport)
  #else
    #define spaceships_api __declspec(dllimport)
    #define spaceships_var extern __declspec(dllimport)
  #endif
#endif

#ifndef spaceships_api
  #define spaceships_api
  #define spaceships_var extern
#endif