#define TEXT_RENDERING_IMPLEMENTATION
#include "text_rendering.hpp"

namespace quark::text_rendering {
  void push_text_to_screen() {
    // Alphabet to use
    const char alphabet[] = 
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "0123456789"
      "!@#$%^&*(){}[]<>.,/?\\|";
  
    // Generate some random characters to show
    char random_chars[128] = {};
    for_every(i, 128) {
      random_chars[i] = alphabet[rand() % (count_of(alphabet) - 1)];
    }
  
    // Fill in where the newlines should be
    random_chars[31]  = '\n';
    random_chars[63]  = '\n';
    random_chars[95]  = '\n';

    // Append an escape char on the end
    random_chars[127] = '\0';
  
    // Format the text we want to display
    char* text;
    format(text, frame_arena(),
      "abcdefghijklmnopqrstuvwxyz\n"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
      "0123456789\n"
      "!@#$%^&*(){}[]<>.,/?\\|\n\n" +
      random_chars + "\n\n" +
      "Fps: " + (1.0f / delta())
    );

    // Create a color for the text
    vec4 color = {};
    color.x = sin(time()) + 1.0f;
    color.y = cos(time()) + 1.0f;
    color.z = sincos(time()) + 1.0f;
    color.w = 1.0f;
    
    // Display the text
    push_text(44, 30, 44, color, text);
  }

  void exit_on_esc() {
    if(is_key_down(KeyCode::Escape)) {
      set_window_should_close();
    }
  }
}

