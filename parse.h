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
 *  C0 Control	    00-1F hex  (G0 is 041-176 octal, 21-7E hex)
 *  SPACE		        20+A0 hex  Always and everywhere a blank space
 *  Intermediate	  20-2F hex   !"#$%&'()*+,-./
 *  Parameters	    30-3F hex  0123456789:;<=>?
 *  Uppercase	      40-5F hex  @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_
 *  Lowercase	      60-7E hex  `abcdefghijlkmnopqrstuvwxyz{|}~
 *  Alphabetic	    40-7E hex  (all of upper and lower case)
 *  Delete		      7F    hex  Always and everywhere ignored
 *  C1 Control	    80-9F hex  32 additional control characters <- not sure about this
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

enum CHARACTER_TYPE{
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

bool esc_validate(enum ESC_STATE state, char c) {
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

void esc_reset(struct terminal* t) {
  bytebuffer_clear(t->esc.buf);
}

bool esc_append(struct terminal* t, char c) {
  if(!esc_validate(t->esc.state, c)) {
    log_debug("invalid sequence");
    esc_reset(t);
    t->esc.state = STATE_NEUTRAL;
    return false;
  }
  bytebuffer_append(t->esc.buf, &c, 1);
}

/*
 * Handles control_characters
 * like SOH, XON, ESC, CR, etc...
 *
 * change state to STATE_ESC when ESC is given
 */
void control_character(struct terminal* t, char c) {
  log_debug("control character : %d", c);
  t->esc.state = STATE_NEUTRAL;

  if(c == '\033')t->esc.state = STATE_ESC;
}

/*
 * Handles escape sequence
 * which is all sequences starts with \033
 *
 * change state to control sequence(csi) when [ is given
 * change state to device control sequence(dcs) when P is given
 * change state to operating system command(ocs) when ] is given
 */
void escape_sequence(struct terminal* t, char c) {
  log_debug("escape_sequence : %d %c", c, c);
  //t->esc.state = STATE_NEUTRAL;
  if(c == '[') t->esc.state = STATE_CSI;
}

void control_sequence(struct terminal* t, char c) {
  
  if(c >= 64 && c <= 126) {
    log_debug("CSI complete: %.*s", bytebuffer_size(t->esc.buf), t->esc.buf->buf);

    esc_reset(t);
    t->esc.state = STATE_NEUTRAL;
  }
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
      if(esc_append(term, c));
    } else if(term->esc.state == STATE_DCS) {
      if(esc_append(term, c));
    }
  }
}
