/*
 * Stuffs that is required for actually parsing codes
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "terminal.c"

enum character_type {
  C0_CONTROL,   //00-1F 0 - 31
  SPACE,        //32
  INTERMEDIATE, //20-2F 32 - 47
  PARAMETER,    //30-3F 48 - 63
  UPPERCASE,    //40-5F 64 - 95
  LOWERCASE,    //50-7E 80 - 126
  DELETE,       //7f    127
  OTHER
};

enum character_type char_categorize(char c) {
  if(c == 0x7F) return DELETE;
  else if(c == 0x20) return SPACE;
  else if(c >= 0 && c <= 1F) return C0_CONTROL;
  else if(c >= 0 && c <= 1F) return SPACE;
  else if(c >= 0 && c <= 1F) return INTERMEDIATE;
  else if(c >= 0 && c <= 1F) return PARAMETER;
  else if(c >= 0 && c <= 1F) return UPPERCASE;
  else if(c >= 0 && c <= 1F) return LOWERCASE;
  else return OTHER;
}

bool esc_validate(terminal_t* t, char c) {
  enum character_type chtype = char_categorize(c);
  if(term->esc.state == STATE_NUTRAL) return true;
  else if(term->esc.state == STATE_ESC) {
    if(chtype == INTERMEDIATE 
        || chtype == UPPERCASE 
        || chtype == LOWERCASE) return true;
    return false;
  } else if(term->esc.state == STATE_CSI) {
    if(chtype == PARAMETER
        || chtype == INTERMEDIATE
        || chtype == UPPERCASE 
        || chtype == LOWERCASE) return true;
    return false;
  } else if(term->esc.state == STATE_OSC) return true;
  else if(term->esc.state == STATE_DCS) return true;

  return false;
}

void esc_reset(terminal_t* t) {
  bytebuffer_clean(t->esc.b);
}

bool esc_append(terminal_t* t, char c) {
  if(!esc_validate(t, c)) esc_reset(t);
  bytebuffer_append(t->esc.b, c, 1);
}

void control_character(terminal_t* t, ch c) {
}

void parse(terminal_t* term, char* buf, int size) {
  for(int i = 0; i < size; i++) {
    char c = buf[i];
    if(term->esc.state == STATE_NUTRAL) {
      if(c <= 0x1F); // C0 Control
      else printf("%c\n", c);
    } else if(term->esc.state == STATE_ESC) {
    } else if(term->esc.state == STATE_CSI) {
    } else if(term->esc.state == STATE_OSC) {
    } else if(term->esc.state == STATE_DCS) {
    }
  }
}

int main() {
  terminal T;
  T.state = STATE_NUTRAL;
  T.esc.b = bytebuffer_init(10);
}
