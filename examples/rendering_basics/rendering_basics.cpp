#define RENDERING_BASICS_IMPLEMENTATION
#include "rendering_basics.hpp"

namespace quark::rendering_basics {
  void init_rendering_basics() {
    set_mouse_mode(MouseMode::Captured);
  }

  void add_entities() {
    // Spawn entities in a cube
    const vec3 entity_root_position = {0.0f, 10.0f, 0.0f};
    for_every(x, 4) {
      for_every(y, 4) {
        for_every(z, 4) {
          // Create a "Transform" to store the 3D position and orientation
          Transform transform = {};
          transform.position = 2.0f * vec3 {(f32)x, (f32)y, (f32)z} + entity_root_position;
          transform.rotation = QUAT_IDENTITY;

          // Create a "Model" with a specific mesh and half-extent scale
          Model model = create_model("suzanne", VEC3_ONE * 0.5f);

          // Create a "ColorMaterial" and give it a color.
          // The color we use here doesn't matter since we will be
          // updating it in update_entities()
          ColorMaterial material = {};
          material.color = {0.0f, 0.0f, 0.0f, 1.0f};

          // Create an entity and add the components to it
          u32 entity_id = create_entity();
          add_components(entity_id, transform, model, material);
        }
      }
    }
  }

  void update_entities() {
    // Update the colors for all of the entities we spawned
    for_archetype(Include<Transform, ColorMaterial> {}, Exclude<> {},
    [](u32 entity_id, Transform* transform, ColorMaterial* material) {
      material->color.x = sin(transform->position.x + time()) + 1.0f;
      material->color.y = cos(transform->position.y + time()) + 1.0f;
      material->color.z = sincos(transform->position.z + time()) + 1.0f;
    });
  }

  void add_entities_for_rendering() {
    // Add all of the entities we spawned to the draw batches.
    // They will then be rendered when the material batches are drawn.
    for_archetype(Include<Transform, Model, ColorMaterial> {}, Exclude<> {},
    [](u32 entity_id, Transform* transform, Model* model, ColorMaterial* material) {
      Drawable drawable = {*transform, *model};
      push_drawable_instance(&drawable, material);
    });
  }

  // This is one of the more involved functions in this example.
  // It takes the camera and updates the position
  // and rotation using first-person-shooter-style controls.
  void update_camera() {
    // Grab the "MainCamera" resource and auto-cast it to Camera3D.
    // This is safe to do in this scenario since "MainCamera" directly
    // inherits from Camera3D without adding any new fields.
    // Internally, it's labelled as a "resource_duplicate".
    Camera3D* camera = get_resource_as(MainCamera, Camera3D);

    // Get the change in mouse position since the last frame and scale it down a lot
    vec2 mouse_delta = get_mouse_delta() / 256.0f;

    // Static variable to store the current camera angle
    static eul2 camera_angle = {};
  
    // Update the camera angles
    camera_angle.yaw += mouse_delta.x;
    camera_angle.pitch += mouse_delta.y;
  
    // Clamp the pitch so we can't look upside down
    camera_angle.pitch = clamp(camera_angle.pitch, -F32_PI_2 + 0.1f, F32_PI_2 - 0.1f);
  
    // Calculate look rotations
    quat yaw_rotation = quat_from_axis_angle(VEC3_UNIT_Z, camera_angle.yaw);
    quat pitch_rotation = quat_from_axis_angle(VEC3_UNIT_X, camera_angle.pitch);

    // Horizontal movement
    vec2 movement_direction_xy = VEC2_ZERO;
  
    // Forward-backwards
    is_key_down(KeyCode::W) ? movement_direction_xy.y += 1 : 0;
    is_key_down(KeyCode::S) ? movement_direction_xy.y -= 1 : 0;
  
    // Left-Right
    is_key_down(KeyCode::A) ? movement_direction_xy.x -= 1 : 0;
    is_key_down(KeyCode::D) ? movement_direction_xy.x += 1 : 0;

    // Clamp and rotate horizontal movement vector
    movement_direction_xy = normalize_or_zero(movement_direction_xy);
    movement_direction_xy = rotate(movement_direction_xy, camera_angle.yaw);

    // Full 3D movement
    vec3 movement_direction = as_vec3(movement_direction_xy, 0.0f);
  
    // Up-down movement
    is_key_down(KeyCode::Space)       ? movement_direction.z += 1 : 0;
    is_key_down(KeyCode::LeftControl) ? movement_direction.z -= 1 : 0;
  
    // Update camera position and rotation
    camera->position += movement_direction * delta() * 10.0f;
    camera->rotation = yaw_rotation * pitch_rotation;    
  }

  void exit_on_esc() {
    if(is_key_down(KeyCode::Escape)) {
      set_window_should_close();
    }
  }
}

