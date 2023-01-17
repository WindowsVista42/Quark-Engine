# Quark-Engine
**Note: This repository is only for my portfolio! Send me an email if you would like to see the main repository.**

A game engine written in C-style C++.  
I started working on this project in roughly January 2021.

My goal with this project has been to learn more about making game engines and create something that I enjoy using.
The aim is to make an engine that prioritizes keeping things simple while also providing higher-level features that dont get in the way.
I'm still actively developing this project, but if you would like to see the main repository send me an email so we can get in touch!

## Project Overview
**Core layer:**
- GLSL-style math library
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

**Loader:**
- Plugin loading

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

## Open Sourced Parts
I have open sourced some parts of the engine that are relatively feature-stable that other people might find useful.
- [WindowsVista42/ThreadPool](https://github.com/WindowsVista42/ThreadPool)  
- [WindowsVist42/QuarkPackge](https://github.com/WindowsVista42/QuarkPackage)
