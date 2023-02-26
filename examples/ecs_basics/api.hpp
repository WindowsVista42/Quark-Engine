#pragma once

#if defined(_WIN32) || defined(_WIN64)
  #if defined(ECS_BASICS_IMPLEMENTATION)
    #define ecs_basics_api __declspec(dllexport)
    #define ecs_basics_var extern __declspec(dllexport)
  #else
    #define ecs_basics_api __declspec(dllimport)
    #define ecs_basics_var extern __declspec(dllimport)
  #endif
#endif

#ifndef ecs_basics_api
  #define ecs_basics_api
  #define ecs_basics_var extern
#endif