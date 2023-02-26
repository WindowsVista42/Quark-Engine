#define ECS_BASICS_IMPLEMENTATION
#include "ecs_basics.hpp"

namespace quark::ecs_basics {
  // This "defines" a component.
  // We have to do this to make sure that the variables
  // defined for components actually have somewhere to live.
  // This is required because we are using share libraries.
  define_component(CustomTimer);

  void init_ecs_basics() {
    // Finally, components need to be "initialized".
    // This process assigns a unique integer id to each
    // component and allocates storage for it in the ECS.
    init_component(CustomTimer);
  }

  void add_custom_timers() {
    // Create 10 entities with a timer component
    for_every(i, 10) {
      // Create a new timer with a delay of 0.5 to 6.0 seconds
      CustomTimer timer = {};
      timer.remainder = rand_f32_range(0.5f, 6.0f);
    
      // Create a new entity and add components to it
      u32 entity_id = create_entity();
      add_components(entity_id, timer);
    }

    log_message("Added timers!");
  }

  void update_custom_timers() {
    u32 timer_count = 0;
  
    // "for_archetype()"" iterates through the entity component system and applies
    // a function onto every entity that matches the filter.
    //
    // We filter for entities we want by specifying an "Include" parameter, with
    // components we want to include, and an "Exclude" parameter with components
    // we want to exclude. Then all we do is specify a function to apply onto all
    // of the entities that matched the specified filter.
    //
    // In this case all of this is really simple. We are just grabbing all of the
    // entities with a "CustomTimer" component and decrementing their timer.
    // If the timer goes below 0 (the timer has finished), we print out a message
    // and destroy the entity.
    for_archetype(Include<CustomTimer> {}, Exclude<> {}, [&](u32 entity_id, CustomTimer* timer) {
      timer->remainder -= delta();

      if(timer->remainder <= 0.0f) {
        print("CustomTimer finished! (Entity ID: " + entity_id + ")\n");
      
        // Destroying an entity is as simple as calling destroy_entity().
        // This is safe to call during iteration!
        destroy_entity(entity_id);
      }

      timer_count += 1;
    });

    // Add more timers if there are no more remaining
    if(timer_count == 0) {
      log_message("Timers finished!");
      add_custom_timers();
    }
  }

  void exit_on_esc() {
    if(is_key_down(KeyCode::Escape)) {
      set_window_should_close();
    }
  }
}

