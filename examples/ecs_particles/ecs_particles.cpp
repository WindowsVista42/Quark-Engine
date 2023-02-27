#define ECS_PARTICLES_IMPLEMENTATION
#include "ecs_particles.hpp"

namespace quark::ecs_particles {
  // Define our components
  define_component(Particle);
  define_component(Motion);

  // Constants for particles
  static const char* PARTICLE_MESH = "cube";
  static Model PARTICLE_BASE_MODEL = {};
  static vec3 PARTICLE_BASE_POS = { 0, 10, 0 };

  // Savable parameters to particles
  // These get loaded if there was a previous run of the program
  static savable f32 PARTICLE_SCALE_BASE = 0.125f;
  static savable f32 PARTICLE_SCALE_BASE_RANGE = 0.025f;

  static savable f32 PARTICLE_SCALE_FINAL = 0.0625f / 8.0f;
  static savable f32 PARTICLE_SCALE_FINAL_RANGE = 0.025f;

  static savable f32 PARTICLE_LIFETIME_BASE = 0.75f;
  static savable f32 PARTICLE_LIFETIME_RANGE = 0.25f;

  static savable vec3 PARTICLE_COLOR_BASE = { 4.0f, 1.0f, 0.1f };
  static savable vec3 PARTICLE_COLOR_BASE_RANGE = { 0.2f, 0.1f, 0.05f };

  static savable vec3 PARTICLE_COLOR_FINAL = { 0.5f, 0.05f, 0.01f };
  static savable vec3 PARTICLE_COLOR_FINAL_RANGE = { 0.2f, 0.01f, 0.01f };

  static savable f32 PARTICLE_VEL_SCALE = 6.0f;
  static savable f32 PARTICLE_VEL_SCALE_RANGE = 2.0f;

  static savable f32 PARTICLE_ACC_SCALE = 3.0f;
  static savable f32 PARTICLE_ACC_SCALE_RANGE = 1.0f;

  static savable f32 PARTICLE_SPAWN_DELAY = 1.0f / 300.0f;

  // Helper functions for generating random values
  f32 rand_f32_jitter(f32 base, f32 range) {
    return rand_f32_range(base - range, base + range);
  }

  vec3 rand_vec3_jitter(vec3 base, vec3 range) {
    return rand_vec3_range(base - range, base + range);
  }

  void init_ecs_particles() {
    // Init our components
    init_component(Particle);
    init_component(Motion);
  
    // Define some actions
    add_action("increase_value", KeyCode::U);
    add_action("decrease_value", KeyCode::J);

    add_action("goto_prev", KeyCode::I);
    add_action("goto_next", KeyCode::K);

    add_action("save", KeyCode::Z);
    add_action("load", KeyCode::X);

    // Set constants
    PARTICLE_BASE_MODEL = create_model(PARTICLE_MESH, VEC3_ONE);
    set_mouse_mode(MouseMode::Captured);

    // If there was a previous run of the program load the snapshot
    if(file_exists("quark/saves/ecs_particles.snapshot")) {
      load_snapshot("quark/saves/ecs_particles.snapshot");
    }
  }

  // Function to spawn a new particle with some random parameters
  void spawn_new_particle() {
    Transform transform = {};
    transform.position = PARTICLE_BASE_POS;
    transform.rotation =
      quat_from_axis_angle(VEC3_UNIT_Z, rand_f32_range(0, F32_2PI)) *
      quat_from_axis_angle(VEC3_UNIT_Y, rand_f32_range(0, F32_2PI)) *
      quat_from_axis_angle(VEC3_UNIT_X, rand_f32_range(0, F32_2PI));
  
    // We can leave this as default for now because we update it later
    Model model = PARTICLE_BASE_MODEL;
  
    // We can leave this as default for now because we update it later
    ColorMaterial material = {};
  
    // Fill out motion parameters
    Motion motion = {};
  
    vec3 vel_forward = normalize_or_value(rand_vec3_range(-vec3 {1.0f, 1.0f, 1.0f}, VEC3_ONE), VEC3_UNIT_Z);
    vec3 acc_forward = normalize_or_value(rand_vec3_range(-vec3 {1.0f, 1.0f, 1.0f}, VEC3_ONE), VEC3_UNIT_Z);
  
    motion.velocity = vel_forward * max(rand_f32_jitter(PARTICLE_VEL_SCALE, PARTICLE_VEL_SCALE_RANGE), 0.0f);
    motion.acceleration = acc_forward * max(rand_f32_jitter(PARTICLE_ACC_SCALE, PARTICLE_ACC_SCALE_RANGE), 0.0f);

    motion.angular_velocity = rand_vec3_range(-VEC3_ONE, VEC3_ONE);
    motion.angular_acceleration = rand_vec3_range(-VEC3_ONE, VEC3_ONE) / 4.0f;
  
    // Fill out particle parameters
    Particle particle = {};
  
    particle.alive_time = 0.0f;
    particle.lifetime = max(rand_f32_jitter(PARTICLE_LIFETIME_BASE, PARTICLE_LIFETIME_RANGE), 0.0f);
  
    vec3 base_color = rand_vec3_jitter(PARTICLE_COLOR_BASE, PARTICLE_COLOR_BASE_RANGE);
    vec3 final_color = rand_vec3_jitter(PARTICLE_COLOR_FINAL, PARTICLE_COLOR_FINAL_RANGE);
  
    particle.base_color = as_vec4(base_color, 1.0f);
    particle.final_color = as_vec4(final_color, 1.0f);

    f32 base_scale = max(rand_f32_jitter(PARTICLE_SCALE_BASE, PARTICLE_SCALE_BASE_RANGE), 0.0f);
    f32 final_scale = max(rand_f32_jitter(PARTICLE_SCALE_FINAL, PARTICLE_SCALE_FINAL_RANGE), 0.0f);

    particle.base_half_extents = VEC3_ONE * base_scale;
    particle.final_half_extents = VEC3_ONE * final_scale;

    // Add components to entities
    u32 entity_id = create_entity();
    add_components(entity_id, transform, model, material, motion, particle);
  }

  // This is the same camera code from rendering_basics
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

  // Update a parameter
  void update_param(f32* param) {
    bool updated = false;
  
    if(get_action("increase_value").down) {
      *param += 2.0f * *param * delta();
      updated = true;
    }
  
    if(get_action("decrease_value").down) {
      *param -= 2.0f * *param * delta();
      updated = true;
    }

    // If we just finished changing some values, save the current particle config
    if(get_action("increase_value").just_up || get_action("decrease_value").just_up) {
      log_message("Particle parameters updated!");
      save_snapshot("quark/saves/ecs_particles.snapshot");
    }
  }

  void update_params() {
    static u32 state = 0;

    // Select prev/next option
    if(get_action("goto_prev").just_down) {
      state += 22;
      state %= 23;
    }
  
    if(get_action("goto_next").just_down) {
      state += 1;
      state %= 23;
    }

    // Formatting
    f32 vertical_offset = 40.0f;
    char text[128];

    sprintf(text, "Fps: %.2f", 1.0f / delta());
    push_text(20, 20, 20, {3, 3, 3, 1}, text);
  
    // Helper macro to make things easier
    #define update_param_and_add_name(b, name) { \
      zero_mem(text, 128); \
      if(b) { update_param(&name); } \
      sprintf(text, "%s: %f", #name, name); \
      vec4 color = {3, 3, 3, 1}; \
      if(b) { color = {2, 0.5, 0.5, 1}; } \
      push_text(20, vertical_offset, 20, color, text); \
      vertical_offset += 20.0f; \
    }

    // Conditionally update options if they are selected
    update_param_and_add_name(state == 0,  PARTICLE_SCALE_BASE);
    update_param_and_add_name(state == 1,  PARTICLE_SCALE_BASE_RANGE);
    update_param_and_add_name(state == 2,  PARTICLE_SCALE_FINAL);
    update_param_and_add_name(state == 3,  PARTICLE_SCALE_FINAL_RANGE);
    update_param_and_add_name(state == 4,  PARTICLE_LIFETIME_BASE);
    update_param_and_add_name(state == 5,  PARTICLE_LIFETIME_RANGE);
    update_param_and_add_name(state == 6,  PARTICLE_VEL_SCALE);
    update_param_and_add_name(state == 7,  PARTICLE_VEL_SCALE_RANGE);
    update_param_and_add_name(state == 8,  PARTICLE_ACC_SCALE);
    update_param_and_add_name(state == 9,  PARTICLE_ACC_SCALE_RANGE);
    update_param_and_add_name(state == 10, PARTICLE_SPAWN_DELAY);

    update_param_and_add_name(state == 11, PARTICLE_COLOR_BASE.x);
    update_param_and_add_name(state == 12, PARTICLE_COLOR_BASE.y);
    update_param_and_add_name(state == 13, PARTICLE_COLOR_BASE.z);
    update_param_and_add_name(state == 14, PARTICLE_COLOR_BASE_RANGE.x);
    update_param_and_add_name(state == 15, PARTICLE_COLOR_BASE_RANGE.y);
    update_param_and_add_name(state == 16, PARTICLE_COLOR_BASE_RANGE.z);
  
    update_param_and_add_name(state == 17, PARTICLE_COLOR_FINAL.x);
    update_param_and_add_name(state == 18, PARTICLE_COLOR_FINAL.y);
    update_param_and_add_name(state == 19, PARTICLE_COLOR_FINAL.z);
    update_param_and_add_name(state == 20, PARTICLE_COLOR_FINAL_RANGE.x);
    update_param_and_add_name(state == 21, PARTICLE_COLOR_FINAL_RANGE.y);
    update_param_and_add_name(state == 22, PARTICLE_COLOR_FINAL_RANGE.z);

    PARTICLE_LIFETIME_BASE = clamp(PARTICLE_LIFETIME_BASE, 0.0f, 5.0f);
    PARTICLE_LIFETIME_RANGE = clamp(PARTICLE_LIFETIME_RANGE, 0.0f, 2.0f);

    PARTICLE_SPAWN_DELAY = clamp(PARTICLE_SPAWN_DELAY, 0.0005f, F32_MAX);

    push_text(20, vertical_offset + 20, 20, {3, 3, 3, 1}, "Press I/K to move selection up/down\nPress U/J to inc/dec the selected parameter");
  }

  void spawn_particles() {
    static f32 spawn_timer = 0.0f;
    spawn_timer -= delta();
    spawn_timer = clamp(spawn_timer, 0.0f, F32_MAX);

    const f32 spawn_delay = PARTICLE_SPAWN_DELAY;
  
    // Spawn as many particles as we can during this frame
    while(spawn_timer < delta()) {
      spawn_timer += spawn_delay;

      spawn_new_particle();
    }
  }

  void update_entities() {
    // Semi-implicit euler integration.
    // This code is just ripped from the spaceships example.
    // We probably don't need full physics for simple particles,
    // but I already had code that somewhat fit the role and im *lazy*.
    for_archetype(Include<Transform, Motion> {}, Exclude<> {},
    [&](u32 entity_id, Transform* transform, Motion* motion) {
      motion->velocity += motion->impulse;
      motion->impulse = VEC3_ZERO;
      motion->velocity += motion->acceleration * delta();
      transform->position += motion->velocity * delta();

      motion->angular_velocity += motion->angular_impulse;
      motion->angular_impulse = VEC3_ZERO;
      motion->angular_velocity += motion->angular_acceleration * delta();
      transform->rotation = transform->rotation * quat_from_eul3(as_eul3(motion->angular_velocity * delta()));
    });
  
    // Apply color shifting and size changing
    for_archetype(Include<Model, ColorMaterial, Particle> {}, Exclude<> {},
    [](u32 entity_id, Model* model, ColorMaterial* material, Particle* particle) {
      particle->alive_time += delta();

      // Destroy the particle if its past its lifetime
      if(particle->alive_time >= particle->lifetime) {
        destroy_entity(entity_id);
        return;
      }

      f32 t =  particle->alive_time / particle->lifetime;

      material->color = lerp(particle->base_color, particle->final_color, t);
      model->half_extents = lerp(particle->base_half_extents, particle->final_half_extents, t);
    });
  }

  void push_renderables() {
    // Add everything to the render batches
    for_archetype(Include<Transform, Model, ColorMaterial> {}, Exclude<> {},
    [](u32 entity_id, Transform* transform, Model* model, ColorMaterial* material) {
      Drawable drawable = {*transform, *model};
      push_drawable_instance(&drawable, material);
    });
  }

  void exit_on_esc() {
    if(is_key_down(KeyCode::Escape)) {
      set_window_should_close();
    }
  }
}

