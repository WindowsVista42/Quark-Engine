#pragma once

#if defined(_WIN32) || defined(_WIN64)
  #if defined(ECS_PARTICLES_IMPLEMENTATION)
    #define ecs_particles_api __declspec(dllexport)
    #define ecs_particles_var extern __declspec(dllexport)
  #else
    #define ecs_particles_api __declspec(dllimport)
    #define ecs_particles_var extern __declspec(dllimport)
  #endif
#endif

#ifndef ecs_particles_api
  #define ecs_particles_api
  #define ecs_particles_var extern
#endif