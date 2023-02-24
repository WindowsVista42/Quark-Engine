#define SPACESHIPS_IMPLEMENTATION
#include "spaceships.hpp"



namespace quark::spaceships {
  define_component(EnemyMove);
  define_component(EnemyAttack);
  define_component(EnemyRetreat);
  define_component(Enemy);
  define_component(Asteroid);
  define_component(Motion);
  define_component(Projectile);
  define_component(Spaceship);
  define_component(AvoidanceRadius);
  define_component(TestingTag);
  define_component(BoxCollider);

  define_savable_resource(SpaceshipTypes, {});

  savable u32 player_id;

  savable static MeshId CUBE_ID = {};

  void init_spaceships_game() {
    log_message("SPACESHIPS INIT!\n");

    update_component(EnemyMove);
    update_component(EnemyAttack);
    update_component(EnemyRetreat);
    update_component(Enemy);
    update_component(Asteroid);
    update_component(Motion);
    update_component(Projectile);
    update_component(Spaceship);
    update_component(AvoidanceRadius);
    update_component(TestingTag);
    update_component(BoxCollider);

    log_message("Added all component types!\n");

    create_action("move_forward");
    create_action("move_backward");
    create_action("move_left");
    create_action("move_right");
    create_action("move_up");
    create_action("move_down");

    create_action("look_right", 0.0f);
    create_action("look_left",  0.0f);
    create_action("look_up",    0.0f);
    create_action("look_down",  0.0f);
    create_action("roll_left");
    create_action("roll_right");

    create_action("shoot");
    create_action("brakes");
    create_action("boost");

    create_action("ui_save");
    create_action("ui_load");
    create_action("ui_mouse");

    bind_action("move_forward",  KeyCode::W);
    bind_action("move_backward", KeyCode::S);
    bind_action("move_left",     KeyCode::A);
    bind_action("move_right",    KeyCode::D);
    bind_action("move_up",       KeyCode::Space);
    bind_action("move_down",     KeyCode::LeftControl);

    bind_action("look_right", MouseAxisCode::MoveRight, 1.0f / 256.0f);
    bind_action("look_left",  MouseAxisCode::MoveLeft,  1.0f / 256.0f);
    bind_action("look_up",    MouseAxisCode::MoveUp,    1.0f / 256.0f);
    bind_action("look_down",  MouseAxisCode::MoveDown,  1.0f / 256.0f);
    bind_action("roll_left",  KeyCode::Q);
    bind_action("roll_right", KeyCode::E);

    bind_action("shoot",  MouseButtonCode::LeftButton);
    bind_action("brakes", KeyCode::C);
    bind_action("boost",  KeyCode::LeftShift);

    bind_action("ui_save",  KeyCode::O);
    bind_action("ui_load",  KeyCode::P);
    bind_action("ui_mouse", KeyCode::Escape);

    CUBE_ID = *get_asset<MeshId>("cube");

    log_message("Added action!\n");
  }
}

mod_main() {
  using namespace spaceships;

  //
  // Global init
  //

  create_system2(init_spaceships_game);
  add_system2("quark_init", init_spaceships_game, , -1);

  //
  // Game init
  //

  create_system2(init_entities);
  add_system2("init", init_entities, , -1);

  //
  // Game update
  //

  create_system2(update_physics);

  create_system2(update_player_input);
  create_system2(update_spaceship_movement);
  create_system2(update_spaceship_weapons);
  create_system2(sync_camera_to_player);

  create_system2(add_entities_to_render_batches);

  create_system2(exit_on_esc);

  add_system("update", "exit_on_esc", "", -1);

  add_system2("update", update_player_input,            update_all_actions,  1);
  add_system2("update", update_spaceship_movement,      update_tag,         -1);
  add_system2("update", update_physics,                 update_tag,          1);
  add_system2("update", update_spaceship_weapons,       update_physics,      1);
  add_system2("update", sync_camera_to_player,          update_physics,      1);
  add_system2("update", add_entities_to_render_batches, begin_frame,        -1);

  ECS_MAX_STORAGE = 64 * 1024;
  PRINT_PERFORMANCE_STATISTICS = false;

  print_system_list("update");
}
