# Quark-Engine
Game engine written in C-style C++.  
I started working on this project in roughly January 2021.

My goal with this project has been to learn more about making game engines and create something that I enjoy using.
The aim is to make an engine that prioritizes keeping things simple while also providing higher-level features that dont get in the way.
I'm still actively developing this project, and it has been a great way to learn and improve my skills, but if you would like to see the main repository send me an email!

## Project Overview
**Core layer:**
- Linear algebra math library
- C++ helpers
- Typedefs

**Platform layer:**
- Window API
- Input API
- Custom threadpool
- General OS abstractions (shared libraries, memory allocation, precise timing)
- Arena allocator pool

**Engine layer:**
- Core component types (Transform, Model, etc...)
- Global resource system
- Custom bitset-based entity component system
- Game state serialization and deserialization
- Input agnostic actions
- Jobs system
- String Builder API
- Logging API
- Asset loader
- Asset package format
- General vulkan abstraction
- Bindless gpu-driven forward renderer
- Material and material batching system
- UI system

**Tooling:**
- GLSL shader abstraction
- CLI tool for common tasks

**Libraries being used:**
- GLFW
- Vulkan
- VulkanMemoryAllocator
- lz4
- meshoptimizer
- stb
- tinyobjloader
- ttf2mesh
- vk-boostrap
