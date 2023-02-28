# Spaceships Demo
Video explanation: https://www.youtube.com/watch?v=kleAUZQlFJ4

This demo is a simple game made in the engine. It's very **work in progress** right now and primarily needs: an actual gameplay loop, better audio, nicer shaders, and controller support.

The code for this demo isn't documented very well, but if you feel like looking through, go for it!

## Running
Clone this repository and run `quark_loader.exe` in the build folder.

## Controls
```
"move_forward"  - W
"move_backward" - S
"move_left"     - A
"move_right"    - D
"move_up"       - Space
"move_down"     - Left Control

"shoot"         - Left Button
"brakes"        - C
"boost"         - Left Shift

"save_snapshot" - O
"load_snapshot" - P

"exit"          - Escape
```

## Engine Features Shown
- **3D Rendering**  
  Look for `push_drawable_instance()` calls.
  
- **Materials**  
  Look for `ColorMaterial` being used.
  
- **UI Text Rendering**  
  Look for `push_ui_text()` calls.
  
- **Entity Component System**  
  Look for `create_entity()`, `add_components()`, and `for_archetype()`.
  
- **Game State Snapshots**  
  Look for `load_ecs()` and `save_ecs()`. The function name will undoubtedly change in the future.
  
- **Audio (WIP)**  
  Look for `play_sound()`. This API is WIP, so the semanics are likely to change.
  
- **Asset Manager**  
  All of the assets in the `quark/` and `spaceships/` folders get loaded.
  
- **Job Scheduler**  
  Look for `create_system2()` and `add_system2()`. At the time of writing this I was testing a new API to make it simpler.
  
- **Math Library**  
  Look for any `quat...` or `vec3...` things being used.
  
- **Input Actions**  
  Look for `create_action` and `bind_action()`.
  
- **Logging**  
  Look for `log_message()`.
  
- **Window Management**  
  The game calls `set_window_should_close()` to tell the window to close.

## Demo Features
- **3D Physics (Raycasting & Collision)**  
  Look in `physics.cpp`.
  
- **NPC AI**  
  I'm using the ECS for this, most of the code is in `gameplay.cpp` and uses `for_archetype()` to iterate through enemies.
  
- **PID Controllers**  
  Declaration is in `spaceships.hpp` and code is in `util.cpp`.
