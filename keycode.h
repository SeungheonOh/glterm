/*
 * This need a re-writing
 * currently, multiple modifers at a time does not work.
 * need to check shift/capslock first then check other masks
 */

/*
 * This functions handles modifers and sends corresponding char* to terminal
 */
void handle_modifer(struct terminal* term, int mod, char* neutral, char* shift, char* control, char* alt, char* numlock) {
  if(mod == GLFW_MOD_SHIFT || mod == GLFW_MOD_CAPS_LOCK && strlen(shift) > 0)
    pseudo_write(term, shift, strlen(shift));
  else if(mod == GLFW_MOD_CONTROL && strlen(control) > 0)
    pseudo_write(term, control, strlen(control));
  else if(mod == GLFW_MOD_ALT && strlen(alt) > 0)
    pseudo_write(term, alt, strlen(alt));
  else if(mod == GLFW_MOD_NUM_LOCK && strlen(numlock) > 0)
    pseudo_write(term, numlock, strlen(numlock));
  else 
    pseudo_write(term, neutral, strlen(neutral));
}
 
/*
 * This function sends input to terminal given
 * May missing some codes, PR/Issue is welcomed
 */
void send_glfw_keycode(struct terminal* term, int key, int mod) {
  switch(key) {
    // NULL character
    case GLFW_KEY_UNKNOWN:    pseudo_write_byte(term, 0); break;

    // Basic keys
    case GLFW_KEY_SPACE:         handle_modifer(term, mod, " ", "", "", "", ""); break;
    case GLFW_KEY_APOSTROPHE:    handle_modifer(term, mod, "\'", "\"", "", "", ""); break;
    case GLFW_KEY_COMMA:         handle_modifer(term, mod, ",", "<", "", "", ""); break;
    case GLFW_KEY_MINUS:         handle_modifer(term, mod, "-", "_", "", "", ""); break;
    case GLFW_KEY_PERIOD:        handle_modifer(term, mod, ".", ">", "", "", ""); break;
    case GLFW_KEY_SLASH:         handle_modifer(term, mod, "/", "?", "", "", ""); break;
    case GLFW_KEY_0:             handle_modifer(term, mod, "0", ")", "", "", ""); break;
    case GLFW_KEY_1:             handle_modifer(term, mod, "1", "!", "", "", ""); break;
    case GLFW_KEY_2:             handle_modifer(term, mod, "2", "@", "", "", ""); break;
    case GLFW_KEY_3:             handle_modifer(term, mod, "3", "#", "", "", ""); break;
    case GLFW_KEY_4:             handle_modifer(term, mod, "4", "$", "", "", ""); break;
    case GLFW_KEY_5:             handle_modifer(term, mod, "5", "%", "", "", ""); break;
    case GLFW_KEY_6:             handle_modifer(term, mod, "6", "^", "", "", ""); break;
    case GLFW_KEY_7:             handle_modifer(term, mod, "7", "&", "", "", ""); break;
    case GLFW_KEY_8:             handle_modifer(term, mod, "8", "*", "", "", ""); break;
    case GLFW_KEY_9:             handle_modifer(term, mod, "9", "(", "", "", ""); break;
    case GLFW_KEY_SEMICOLON:     handle_modifer(term, mod, ";", ":", "", "", ""); break;
    case GLFW_KEY_EQUAL:         handle_modifer(term, mod, "=", "+", "", "", ""); break;
    case GLFW_KEY_A:             handle_modifer(term, mod, "a", "A", "\x01", "", ""); break;
    case GLFW_KEY_B:             handle_modifer(term, mod, "b", "B", "\x02", "", ""); break;
    case GLFW_KEY_C:             handle_modifer(term, mod, "c", "C", "\x03", "", ""); break;
    case GLFW_KEY_D:             handle_modifer(term, mod, "d", "D", "\x04", "", ""); break;
    case GLFW_KEY_E:             handle_modifer(term, mod, "e", "E", "\x05", "", ""); break;
    case GLFW_KEY_F:             handle_modifer(term, mod, "f", "F", "\x06", "", ""); break;
    case GLFW_KEY_G:             handle_modifer(term, mod, "g", "G", "\x07", "", ""); break;
    case GLFW_KEY_H:             handle_modifer(term, mod, "h", "H", "\x08", "", ""); break;
    case GLFW_KEY_I:             handle_modifer(term, mod, "i", "I", "\x09", "", ""); break;
    case GLFW_KEY_J:             handle_modifer(term, mod, "j", "J", "\x0A", "", ""); break;
    case GLFW_KEY_K:             handle_modifer(term, mod, "k", "K", "\x0B", "", ""); break;
    case GLFW_KEY_L:             handle_modifer(term, mod, "l", "L", "\x0C", "", ""); break;
    case GLFW_KEY_M:             handle_modifer(term, mod, "m", "M", "\x0D", "", ""); break;
    case GLFW_KEY_N:             handle_modifer(term, mod, "n", "N", "\x0E", "", ""); break;
    case GLFW_KEY_O:             handle_modifer(term, mod, "o", "O", "\x0F", "", ""); break;
    case GLFW_KEY_P:             handle_modifer(term, mod, "p", "P", "\x10", "", ""); break;
    case GLFW_KEY_Q:             handle_modifer(term, mod, "q", "Q", "\x11", "", ""); break;
    case GLFW_KEY_R:             handle_modifer(term, mod, "r", "R", "\x12", "", ""); break;
    case GLFW_KEY_S:             handle_modifer(term, mod, "s", "S", "\x13", "", ""); break;
    case GLFW_KEY_T:             handle_modifer(term, mod, "t", "T", "\x14", "", ""); break;
    case GLFW_KEY_U:             handle_modifer(term, mod, "u", "U", "\x15", "", ""); break;
    case GLFW_KEY_V:             handle_modifer(term, mod, "v", "V", "\x16", "", ""); break;
    case GLFW_KEY_W:             handle_modifer(term, mod, "w", "W", "\x17", "", ""); break;
    case GLFW_KEY_X:             handle_modifer(term, mod, "x", "X", "\x18", "", ""); break;
    case GLFW_KEY_Y:             handle_modifer(term, mod, "y", "Y", "\x19", "", ""); break;
    case GLFW_KEY_Z:             handle_modifer(term, mod, "z", "Z", "\x1A", "", ""); break;
    case GLFW_KEY_LEFT_BRACKET:  handle_modifer(term, mod, "[", "{", "\x1B", "", ""); break;
    case GLFW_KEY_BACKSLASH:     handle_modifer(term, mod, "\\", "|", "\x1C", "", ""); break;
    case GLFW_KEY_RIGHT_BRACKET: handle_modifer(term, mod, "]", "}", "\x1D", "", ""); break;
    case GLFW_KEY_GRAVE_ACCENT:  handle_modifer(term, mod, "`", "~", "\x1E", "", ""); break;

    case GLFW_KEY_WORLD_1: break;
    case GLFW_KEY_WORLD_2: break;

    case GLFW_KEY_ESCAPE:    pseudo_write_byte(term, 27); break;
    case GLFW_KEY_ENTER:     pseudo_write_byte(term, 13); break;
    case GLFW_KEY_TAB:       pseudo_write_byte(term, 9); break;
    case GLFW_KEY_BACKSPACE: pseudo_write_byte(term, 8); break;

    // Arrows
    case GLFW_KEY_RIGHT:      pseudo_write(term, "\033[C", 3); break;
    case GLFW_KEY_LEFT:       pseudo_write(term, "\033[D", 3); break;
    case GLFW_KEY_DOWN:       pseudo_write(term, "\033[B", 3); break;
    case GLFW_KEY_UP:         pseudo_write(term, "\033[A", 3); break;
    
    case GLFW_KEY_HOME:         handle_modifer(term, mod, "\033[1~", "\033[7$", "", "", ""); break;
    case GLFW_KEY_END:          handle_modifer(term, mod, "\033[4~", "\033[8$", "", "", ""); break;
    case GLFW_KEY_PRINT_SCREEN: break; // Does nothing (?)
    case GLFW_KEY_PAUSE:        handle_modifer(term, mod, "\033[P",  "",        "", "", ""); break; // Is it right?
    case GLFW_KEY_INSERT:       handle_modifer(term, mod, "\033[2~", "",        "", "", ""); break;
    case GLFW_KEY_DELETE:       handle_modifer(term, mod, "\033[3~", "\033[3$", "", "", ""); break;
    case GLFW_KEY_PAGE_UP:      handle_modifer(term, mod, "\033[5~", "",        "", "", ""); break;
    case GLFW_KEY_PAGE_DOWN:    handle_modifer(term, mod, "\033[6~", "",        "", "", ""); break;

    // F keys, no mod mask needed
    case GLFW_KEY_F1:  pseudo_write(term, "\033[11~",   5); break;
    case GLFW_KEY_F2:  pseudo_write(term, "\033[12~",   5); break;
    case GLFW_KEY_F3:  pseudo_write(term, "\033[13~",   5); break;
    case GLFW_KEY_F4:  pseudo_write(term, "\033[14~",   5); break;
    case GLFW_KEY_F5:  pseudo_write(term, "\033[15~",   5); break;
    case GLFW_KEY_F6:  pseudo_write(term, "\033[17~",   5); break;
    case GLFW_KEY_F7:  pseudo_write(term, "\033[18~",   5); break;
    case GLFW_KEY_F8:  pseudo_write(term, "\033[19~",   5); break;
    case GLFW_KEY_F9:  pseudo_write(term, "\033[20~",   5); break;
    case GLFW_KEY_F10: pseudo_write(term, "\033[21~",   5); break;
    case GLFW_KEY_F11: pseudo_write(term, "\033[23~",   5); break;
    case GLFW_KEY_F12: pseudo_write(term, "\033[24~",   5); break;
    case GLFW_KEY_F13: pseudo_write(term, "\033[11;2~", 7); break;
    case GLFW_KEY_F14: pseudo_write(term, "\033[12;2~", 7); break;
    case GLFW_KEY_F15: pseudo_write(term, "\033[13;2~", 7); break;
    case GLFW_KEY_F16: pseudo_write(term, "\033[14;2~", 7); break;
    case GLFW_KEY_F17: pseudo_write(term, "\033[15;2~", 7); break;
    case GLFW_KEY_F18: pseudo_write(term, "\033[17;2~", 7); break;
    case GLFW_KEY_F19: pseudo_write(term, "\033[18;2~", 7); break;
    case GLFW_KEY_F20: pseudo_write(term, "\033[19;2~", 7); break;
    case GLFW_KEY_F21: pseudo_write(term, "\033[20;2~", 7); break;
    case GLFW_KEY_F22: pseudo_write(term, "\033[21;2~", 7); break;
    case GLFW_KEY_F23: pseudo_write(term, "\033[23;2~", 7); break;
    case GLFW_KEY_F24: pseudo_write(term, "\033[24;2~", 7); break;
    case GLFW_KEY_F25: pseudo_write(term, "\033[11;5~", 7); break;

    case GLFW_KEY_KP_0: handle_modifer(term, mod, "\033[2~", "", "", "", "0"); break;
    case GLFW_KEY_KP_1: handle_modifer(term, mod, "\033[8~", "", "", "", "1"); break;
    case GLFW_KEY_KP_2: handle_modifer(term, mod, "\033[B",  "", "", "", "2"); break;
    case GLFW_KEY_KP_3: handle_modifer(term, mod, "\033[6~", "", "", "", "3"); break;
    case GLFW_KEY_KP_4: handle_modifer(term, mod, "\033[D",  "", "", "", "4"); break;
    case GLFW_KEY_KP_5: handle_modifer(term, mod, "",        "", "", "", "5"); break;
    case GLFW_KEY_KP_6: handle_modifer(term, mod, "\033[C",  "", "", "", "6"); break;
    case GLFW_KEY_KP_7: handle_modifer(term, mod, "\033[7~", "", "", "", "7"); break;
    case GLFW_KEY_KP_8: handle_modifer(term, mod, "\033[A",  "", "", "", "8"); break;
    case GLFW_KEY_KP_9: handle_modifer(term, mod, "\033[5~", "", "", "", "9"); break;

    case GLFW_KEY_KP_DECIMAL:  handle_modifer(term, mod, "\033[3~", "", "", "", "."); break;
    case GLFW_KEY_KP_DIVIDE:   handle_modifer(term, mod, "//", "", "", "", ""); break;
    case GLFW_KEY_KP_MULTIPLY: handle_modifer(term, mod, "*", "", "", "", "*"); break;
    case GLFW_KEY_KP_SUBTRACT: handle_modifer(term, mod, "-", "", "", "", "-"); break;
    case GLFW_KEY_KP_ADD:      handle_modifer(term, mod, "+", "", "", "", "+"); break;
    case GLFW_KEY_KP_ENTER:    pseudo_write_byte(term, 13); break;
    case GLFW_KEY_KP_EQUAL:    handle_modifer(term, mod, "=", "", "", "", ""); break;

    // Ignore modifer keys
    case GLFW_KEY_SCROLL_LOCK:
    case GLFW_KEY_NUM_LOCK:
    case GLFW_KEY_CAPS_LOCK:
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_LEFT_ALT:
    case GLFW_KEY_LEFT_SUPER:
    case GLFW_KEY_RIGHT_SHIFT:
    case GLFW_KEY_RIGHT_CONTROL:
    case GLFW_KEY_RIGHT_ALT:
    case GLFW_KEY_RIGHT_SUPER:
    case GLFW_KEY_MENU:
    default:
      break;
  }
}
