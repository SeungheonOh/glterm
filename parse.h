/*
 * This design was inspired by yaft, yet it is not a clone
 * TODO: use table for state instead
 *
 * Resources:
 * http://www.vt100.net/docs/vt102-ug/appendixd.htm
 * https://www.vt100.net/docs/vt320-uu/appendixe.html
 * https://www.inwap.com/pdp10/ansicode.txt
 *
 * Character types:
 *  C0 Control      00-1F hex  (G0 is 041-176 octal, 21-7E hex)
 *  SPACE           20+A0 hex  Always and everywhere a blank space
 *  Intermediate    20-2F hex   !"#$%&'()*+,-./
 *  Parameters      30-3F hex  0123456789:;<=>?
 *  Uppercase       40-5F hex  @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_
 *  Lowercase       60-7E hex  `abcdefghijlkmnopqrstuvwxyz{|}~
 *  Alphabetic      40-7E hex  (all of upper and lower case)
 *  Delete          7F    hex  Always and everywhere ignored
 *  C1 Control      80-9F hex  32 additional control characters <- not sure about this
 *
 * Sequences:
 * control character
 *   (c0 control)
 *
 * escape seq 
 *   ESC intermediate... (parameter|uppercase|lowercase)
 *   ex) \0330A
 *
 * control seq
 *   ESC '[' parameter... intermediate... (uppercase|lowercase)
 *   ex) \033[A
 *
 * operating system control
 *   ESC ']' (ALL)... (BEL | ESC '\')
 *   ex) \033]123;123;123\033
 *
 * device control seq
 *   ESC 'P' (ALL)... (BEL | ESC '\')
 *   ex) \033P123;123;123\033
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "control_characters.h"
#include "sequences.h"

enum CHARACTER_TYPE {
  C0_CONTROL,   //00-1F 0 - 31
  SPACE,        //32
  INTERMEDIATE, //20-2F 32 - 47
  PARAMETER,    //30-3F 48 - 63
  UPPERCASE,    //40-5F 64 - 95
  LOWERCASE,    //50-7E 80 - 126
  DELETE,       //7f    127
  OTHER
};

enum CHARACTER_TYPE char_categorize(char c) {
  if(c >= 0 && c <= 0x1F) return C0_CONTROL;
  else if(c == 0x20) return SPACE;
  else if(c >= 0x20 && c <= 0x2F) return INTERMEDIATE;
  else if(c >= 0x30 && c <= 0x3F) return PARAMETER;
  else if(c >= 0x40 && c <= 0x5F) return UPPERCASE;
  else if(c >= 0x50 && c <= 0x7E) return LOWERCASE;
  else if(c == 0x7F) return DELETE;
  else return OTHER;
}

/*
 * Check if character is vaild for state given
 * read comments on the top
 */
bool esc_is_valid(enum ESC_STATE state, char c) {
  enum CHARACTER_TYPE chtype = char_categorize(c);
  if(state == STATE_NEUTRAL) return true;
  else if(state == STATE_ESC) {
    if(chtype == INTERMEDIATE 
        || chtype == UPPERCASE 
        || chtype == LOWERCASE) return true;
    return false;
  } else if(state == STATE_CSI) {
    if(chtype == PARAMETER
        || chtype == INTERMEDIATE
        || chtype == UPPERCASE 
        || chtype == LOWERCASE) return true;
    return false;
  } else if(state == STATE_OSC) return true;
  else if(state == STATE_DCS) return true;

  return false;
}

/*
 * Check sequence is finished or not
 * read comments on the top
 */
bool esc_is_complete(struct esc_sequence esc) {
  int buf_size = bytebuffer_size(esc.buf);
  enum ESC_STATE state = esc.state;
  enum CHARACTER_TYPE chtype = char_categorize(esc.buf->b[bytebuffer_size(esc.buf) - 1]);
  if(state == STATE_NEUTRAL) return true;
  else if(state == STATE_ESC) {
    if(chtype == PARAMETER
        || chtype == UPPERCASE
        || chtype == LOWERCASE) return true;
    else return false;
  } else if(state == STATE_CSI) {
    if(chtype == UPPERCASE
        || chtype == LOWERCASE) return true;
    else return false;
  } else if(state == STATE_DCS || state == STATE_OSC) {
    if(buf_size < 2) return false;
    if(esc.buf->b[bytebuffer_size(esc.buf) - 1] == '\007'
        || (esc.buf->b[bytebuffer_size(esc.buf) - 1] == '\\'
        && esc.buf->b[bytebuffer_size(esc.buf) - 2] == '\033')) return true;
    else return false;
  } else return true;
}


void esc_reset(struct terminal* t) {
  bytebuffer_clear(t->esc.buf);
  t->esc.state = STATE_NEUTRAL;
}

/*
 * 1. When the buffer is complete ( decided by esc_is_complete() ), return true
 * to run sequence handler.
 *
 * 2. Append given character to terminal->esc.buf, ONLY if when c is validated by
 * esc_is_valid(). When it's not valid, the buffer will be reset.
 */
bool esc_append(struct terminal* t, char c) {
  bytebuffer_append(t->esc.buf, &c, 1);
  if (esc_is_complete(t->esc)) return true;
  if(!esc_is_valid(t->esc.state, c)) {
    log_debug("invalid sequence: %.*s", bytebuffer_size(t->esc.buf), t->esc.buf->b);
    esc_reset(t);
  }
  return false;
}

/*
 * Handles control_characters
 * like SOH, XON, ESC, CR, etc...
 *
 * change state to STATE_ESC when ESC is given
 *
 * Note: control characters are not being pushed into terminal->esc.buf
 */
void control_character(struct terminal* t, char c) {
  t->esc.state = STATE_NEUTRAL;
  if(handle_control_character(c))
    handle_control_character(c)(t);
  else
    log_debug("Control code not found: %d", c);
}

/*
 * Handles escape sequence
 * which is all sequences starts with \033
 *
 * change state to control sequence(csi) when [ is given
 * change state to device control sequence(dcs) when P is given
 * change state to operating system command(osc) when ] is given
 */
void escape_sequence(struct terminal* t, char c) {
  if(c == '[') t->esc.state = STATE_CSI;
  else if(c == ']') t->esc.state = STATE_DCS;
  else if(c == 'P') t->esc.state = STATE_OSC;
  else {
    log_debug("escape_sequence : %d %c", c, c);
    t->esc.state = STATE_NEUTRAL;
    esc_reset(t);
  }
}

void control_sequence(struct terminal* t, char c) {
  log_debug("CSI complete: %.*s", bytebuffer_size(t->esc.buf), t->esc.buf->b);

  bytebuffer_append(t->esc.buf, "\0", 1);
  if(strcmp(t->esc.buf->b, "[H") == 0) {
    terminal_set_cursor(t, 0, 0);
  } else if(strcmp(t->esc.buf->b, "[2J") == 0) {
    terminal_clear(t);
  } else if(strcmp(t->esc.buf->b, "[C") == 0) {
    t->cursor.x--;
    terminal_erase_cell(t, t->cursor.x, t->cursor.y);
  }
  esc_reset(t);
}

void device_control_sequence(struct terminal* t, char c) {
  log_debug("DCS complete: %.*s", bytebuffer_size(t->esc.buf), t->esc.buf->b);
  esc_reset(t);
}

void operating_system_command(struct terminal* t, char c) {
  log_debug("OSC complete: %.*s", bytebuffer_size(t->esc.buf), t->esc.buf->b);
  esc_reset(t);
}

void parse(struct terminal* term, char* buf, int size) {
  for(int i = 0; i < size; i++) {
    char c = buf[i];
    enum CHARACTER_TYPE chtype = char_categorize(c);

    if(term->esc.state == STATE_NEUTRAL) {
      if(chtype == C0_CONTROL) control_character(term, c);
      else terminal_write_byte(term, c);
    } else if(term->esc.state == STATE_ESC) {
      if(esc_append(term, c))
        escape_sequence(term, c);
    } else if(term->esc.state == STATE_CSI) {
      if(esc_append(term, c))
        control_sequence(term, c);
    } else if(term->esc.state == STATE_OSC) {
      if(esc_append(term, c))
        device_control_sequence(term, c);
    } else if(term->esc.state == STATE_DCS) {
      if(esc_append(term, c))
        operating_system_command(term, c);
    }
  }
}
