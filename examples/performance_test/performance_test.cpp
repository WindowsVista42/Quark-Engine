#define PERFORMANCE_TEST_IMPLEMENTATION
#include "performance_test.hpp"

namespace quark::performance_test {
//
// Global Init Jobs
//

  void init_performance_test() {
    set_mouse_mode(MouseMode::Captured);
  }

//
// Init Jobs
//

  void init_entities() {
    Model models[] = {
      create_model(PERF_MODELS[0], VEC3_ONE / 2.0f),
      create_model(PERF_MODELS[1], VEC3_ONE / 2.0f),
      create_model(PERF_MODELS[2], VEC3_ONE / 2.0f),
    };
  
    for_every(x, PERF_COUNT_X) {
      for_every(y, PERF_COUNT_Y) {
        for_every(z, PERF_COUNT_Z) {
          vec3 local_pos = { (f32)x, (f32)y, (f32)z };
          local_pos -= vec3 { PERF_COUNT_X, PERF_COUNT_Y, PERF_COUNT_Z } / 2.0f;
        
          Transform transform = {};
          transform.position = PERF_ROOT_POS + local_pos * PERF_OFFSET;
          transform.rotation = QUAT_IDENTITY;
        
          ColorMaterial material = {};
          material.color.x = (x % 3 + 1) / 3.0f;
          material.color.y = (y % 3 + 1) / 3.0f;

          add_components(create_entity(), transform, models[(x + y + z) % 3], material);
        }
      }
    }
  }

//
// Update Jobs
//

  void update_camera() {
    Camera3D* camera = get_resource_as(MainCamera, Camera3D);

    // Calculate look
    quat yaw_rotation = quat_from_axis_angle(VEC3_UNIT_Z, delta() / 7.0f);
    quat pitch_rotation = quat_from_axis_angle(VEC3_UNIT_X, delta() / 3.0f);
  
    // Update camera
    camera->rotation = yaw_rotation * camera->rotation * pitch_rotation;
  }

  void update_perf_test() {
    // Update perf test entities
    f32 position_offset = sin(time());
    f32 dt = delta();
    f32 t = time();

    quat rot = quat_from_axis_angle(VEC3_UNIT_Z, 2.0f);
    for_archetype(Include<Transform, ColorMaterial> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, ColorMaterial* material) {
      f32 d = distance2(PERF_ROOT_POS, transform->position);
      f32 x = ((i32)(u32)t % 5) - 2;
      
      transform->position.z += x * dt;
      transform->rotation = rot * quat_from_axis_angle(VEC3_UNIT_X, transform->position.z) * quat_from_axis_angle(VEC3_UNIT_Y, t);
      material->color.z = max(transform->position.z, 0.0f) / 8.0f;
    });
  }

  void add_entities_to_render_batches() {
    for_archetype(Include<Transform, Model, ColorMaterial> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Model* model, ColorMaterial* material) {
      Drawable drawable = { *transform, *model };
      push_drawable_instance(&drawable, material);
    });
  }

  void exit_on_esc() {
    if(is_key_down(KeyCode::Escape)) {
      set_window_should_close();
    }
  }
}

