unsigned char send_glfw_keycode(struct terminal* term, int key, int mods) {
  switch(key) {
    case GLFW_KEY_UNKNOWN:
      pseudo_write_byte(term, 0);
      break;
    case GLFW_KEY_SPACE:
      pseudo_write_byte(term, ' ');
      break;
    case GLFW_KEY_APOSTROPHE:
      pseudo_write_byte(term, '\'');
      break;
    case GLFW_KEY_COMMA:
      pseudo_write_byte(term, ',');
      break;
    case GLFW_KEY_MINUS:
      pseudo_write_byte(term, '-');
      break;
    case GLFW_KEY_PERIOD:
      pseudo_write_byte(term, '.');
      break;
    case GLFW_KEY_SLASH:
      pseudo_write_byte(term, '/');
      break;
    case GLFW_KEY_0:
      pseudo_write_byte(term, '0');
      break;
    case GLFW_KEY_1:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_2:
      pseudo_write_byte(term, '2');
      break;
    case GLFW_KEY_3:
      pseudo_write_byte(term, '3');
      break;
    case GLFW_KEY_4:
      pseudo_write_byte(term, '4');
      break;
    case GLFW_KEY_5:
      pseudo_write_byte(term, '5');
      break;
    case GLFW_KEY_6:
      pseudo_write_byte(term, '6');
      break;
    case GLFW_KEY_7:
      pseudo_write_byte(term, '7');
      break;
    case GLFW_KEY_8:
      pseudo_write_byte(term, '8');
      break;
    case GLFW_KEY_9:
      pseudo_write_byte(term, '9');
      break;
    case GLFW_KEY_SEMICOLON:
      pseudo_write_byte(term, ';');
      break;
    case GLFW_KEY_EQUAL:
      pseudo_write_byte(term, '=');
      break;
    case GLFW_KEY_A:
      pseudo_write_byte(term, 'a');
      break;
    case GLFW_KEY_B:
      pseudo_write_byte(term, 'b');
      break;
    case GLFW_KEY_C:
      pseudo_write_byte(term, 'c');
      break;
    case GLFW_KEY_D:
      pseudo_write_byte(term, 'd');
      break;
    case GLFW_KEY_E:
      pseudo_write_byte(term, 'e');
      break;
    case GLFW_KEY_F:
      pseudo_write_byte(term, 'f');
      break;
    case GLFW_KEY_G:
      pseudo_write_byte(term, 'g');
      break;
    case GLFW_KEY_H:
      pseudo_write_byte(term, 'h');
      break;
    case GLFW_KEY_I:
      pseudo_write_byte(term, 'i');
      break;
    case GLFW_KEY_J:
      pseudo_write_byte(term, 'j');
      break;
    case GLFW_KEY_K:
      pseudo_write_byte(term, 'k');
      break;
    case GLFW_KEY_L:
      pseudo_write_byte(term, 'l');
      break;
    case GLFW_KEY_M:
      pseudo_write_byte(term, 'm');
      break;
    case GLFW_KEY_N:
      pseudo_write_byte(term, 'n');
      break;
    case GLFW_KEY_O:
      pseudo_write_byte(term, 'o');
      break;
    case GLFW_KEY_P:
      pseudo_write_byte(term, 'p');
      break;
    case GLFW_KEY_Q:
      pseudo_write_byte(term, 'q');
      break;
    case GLFW_KEY_R:
      pseudo_write_byte(term, 'r');
      break;
    case GLFW_KEY_S:
      pseudo_write_byte(term, 's');
      break;
    case GLFW_KEY_T:
      pseudo_write_byte(term, 't');
      break;
    case GLFW_KEY_U:
      pseudo_write_byte(term, 'u');
      break;
    case GLFW_KEY_V:
      pseudo_write_byte(term, 'v');
      break;
    case GLFW_KEY_W:
      pseudo_write_byte(term, 'w');
      break;
    case GLFW_KEY_X:
      pseudo_write_byte(term, 'x');
      break;
    case GLFW_KEY_Y:
      pseudo_write_byte(term, 'y');
      break;
    case GLFW_KEY_Z:
      pseudo_write_byte(term, 'z');
      break;
    case GLFW_KEY_LEFT_BRACKET:
      pseudo_write_byte(term, '[');
      break;
    case GLFW_KEY_BACKSLASH:
      pseudo_write_byte(term, '\\');
      break;
    case GLFW_KEY_RIGHT_BRACKET:
      pseudo_write_byte(term, ']');
      break;
    case GLFW_KEY_GRAVE_ACCENT:
      pseudo_write_byte(term, '`');
      break;
    case GLFW_KEY_WORLD_1:
      pseudo_write_byte(term, 0); // TODO find this out
      break;
    case GLFW_KEY_WORLD_2:
      pseudo_write_byte(term, 0);
      break;
    case GLFW_KEY_ESCAPE:
      pseudo_write_byte(term, 27);
      break;
    case GLFW_KEY_ENTER:
      pseudo_write_byte(term, 13);
      break;
    case GLFW_KEY_TAB:
      pseudo_write_byte(term, 9);
      break;
    case GLFW_KEY_BACKSPACE:
      pseudo_write_byte(term, 8);
      break;
    case GLFW_KEY_INSERT:
      pseudo_write_byte(term, 0);
      break;
    case GLFW_KEY_DELETE:     pseudo_write_byte(term, 0); break;

    // Arrows
    case GLFW_KEY_RIGHT:      pseudo_write(term, "\033[C", 3); break;
    case GLFW_KEY_LEFT:       pseudo_write(term, "\033[D", 3); break;
    case GLFW_KEY_DOWN:       pseudo_write(term, "\033[B", 3); break;
    case GLFW_KEY_UP:         pseudo_write(term, "\033[A", 3); break;
    case GLFW_KEY_PAGE_UP:    pseudo_write(term, "\033[A", 3); break;
    case GLFW_KEY_PAGE_DOWN:  pseudo_write(term, "\033[B", 3); break;

    case GLFW_KEY_HOME:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_END:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_CAPS_LOCK:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_SCROLL_LOCK:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_NUM_LOCK:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_PRINT_SCREEN:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_PAUSE:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F1:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F2:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F3:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F4:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F5:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F6:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F7:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F8:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F9:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F10:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F11:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F12:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F13:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F14:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F15:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F16:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F17:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F18:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F19:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F20:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F21:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F22:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F23:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F24:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_F25:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_0:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_1:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_2:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_3:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_4:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_5:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_6:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_7:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_8:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_9:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_DECIMAL:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_DIVIDE:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_MULTIPLY:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_SUBTRACT:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_ADD:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_ENTER:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_KP_EQUAL:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_LEFT_SHIFT:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_LEFT_CONTROL:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_LEFT_ALT:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_LEFT_SUPER:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_RIGHT_SHIFT:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_RIGHT_CONTROL:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_RIGHT_ALT:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_RIGHT_SUPER:
      pseudo_write_byte(term, '1');
      break;
    case GLFW_KEY_MENU:
      pseudo_write_byte(term, '1');
      break;
    default:
      break;
  }
}
