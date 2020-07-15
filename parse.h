/*
 * This design was inspired by yaft, yet it is not a clone
 * TODO: use table for state instead
 *
 * Resources:
 * http://www.vt100.net
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

// TODO rewrite entire parser

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "control_characters.h"
#include "control_sequences.h"
#include "escape_sequences.h"
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
  if(c >= 0 && c <= 0x1F)         return C0_CONTROL;
  else if(c == 0x20)              return SPACE;
  else if(c >= 0x20 && c <= 0x2F) return INTERMEDIATE;
  else if(c >= 0x30 && c <= 0x3F) return PARAMETER;
  else if(c >= 0x40 && c <= 0x5F) return UPPERCASE;
  else if(c >= 0x50 && c <= 0x7E) return LOWERCASE;
  else if(c == 0x7F)              return DELETE;
  else                            return OTHER;
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
 * Appends character to terminal->esc.buf, and if it's complete, return true
 *
 * 1. When the buffer is complete ( decided by esc_is_complete() ), return true
 * to run sequence handler.
 *
 * 2. Append given character to terminal->esc.buf, ONLY if when c is validated by
 * esc_is_valid(). When it's not valid, the buffer will be reset.
 *
 * TODO maybe check validity first
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
  if(handle_control_character(c)) handle_control_character(c)(t);
  else log_debug("Control code not found: %d", c);
}

/*
 * Handles escape sequence ( ESC )
 *
 * change state to control sequence ( CSI ) when [ is given
 * change state to device control sequence ( DCS ) when P is given
 * change state to operating system command ( OSC ) when ] is given
 *
 * escape_sequence() will be called when the full sequence is ready, and arguement c will contain
 * the final character. After finishing job, terminal->esc needs to be reset ( by esc_reset() ), unless 
 * the state was changed for further sequence ( CSI, DCS, OSC )
 */
void escape_sequence(struct terminal* t, char c) {
  if(handle_escape_sequence(c)) handle_escape_sequence(c)(t);
  else log_debug("Escape sequence not found: %c");
  if(t->esc.state != STATE_ESC) return;
  esc_reset(t);
}

/*
 * Handles control sequence ( CSI )
 *
 * control_sequence() will be called when the full sequence is ready, and argument c will contain
 * the final character. After finishing job, terminal->esc needs to be reset ( by esc_reset () ).
 * Unlike escape_sequence() terminal->esc has to be reset always since there is no event causing state change.
 */

int esc_arg_parse(char* start, char* end, char** args, int arg_max) {
  int cnt = 0;
  char* iter = start;
  args[0] = iter;
  while(*iter++ && iter < end)
    if(*iter == ';' && !(*iter = '\0'))
      args[++cnt] = ++iter;
  return (start == end) ? 0 : cnt + 1;
}

void control_sequence(struct terminal* t, char c) {
  for(int i = 0; i < bytebuffer_size(t->esc.buf); i++)log_debug("    %c", *(t->esc.buf->b+i));
  *(t->esc.buf->p - 1) = '\0';

  int argc;
  char* argv[16] = { NULL };
  bool dec = false; // when there is '?' after ESC[

  char* arg_start = t->esc.buf->b + 1; // +1 account for [
  char* arg_end = t->esc.buf->p - 1;   // -1 account for final character
  if(*arg_start == '?' && (dec = true)) ++arg_start; // check if ? is present
  while(char_categorize(*(--arg_end)) == INTERMEDIATE) *(arg_end + 1) = *(arg_end); // handle intermediates
  *(++arg_end) = '\0'; // make it cstring for intermediates
  argc = esc_arg_parse(arg_start, arg_end, argv, 16);

  if(handle_control_sequence(c))
    handle_control_sequence(c)(t, dec, argc, argv, (arg_end == t->esc.buf->p - 1) ? arg_end : arg_end + 1);
  esc_reset(t);
}

/*
 * device control sequence ( DCS ) & operating system command ( OSC )
 * 
 * device_control_sequence() & operating_system_command() will be called when the full sequence is ready, but
 * no final character argument is provided since its finalizer is always "\007" or "\003\".
 * terminal->esc had to be reset always ( by esc_reset() )
 */
void device_control_sequence(struct terminal* t) {
  log_debug("DCS complete: %.*s", bytebuffer_size(t->esc.buf), t->esc.buf->b);
  esc_reset(t);
}

void operating_system_command(struct terminal* t) {
  log_debug("OSC complete: %.*s", bytebuffer_size(t->esc.buf), t->esc.buf->b);
  esc_reset(t);
}

/*
 * Handles all input from terminal.
 *
 * In detail,
 * When state is neutral
 *  1) if given character is C0, handles C0 ( run control_character() )
 *  2) if given character is printable, append to terminal screen
 *  3) UTF8 decoding TODO
 *
 * When state is ESC, CSI, OSC, DCS
 *  - append character to terminal->esc.buf 
 *  - when character is final ( when esc_append() returns true ), handle sequence 
 *  with escape_sequence(), control_sequence(), device_control_sequence(), operating_system_command()
 */
void parse(struct terminal* term, char* buf, int size) {
  for(int i = 0; i < size; i++) {
    char c = buf[i];
    enum CHARACTER_TYPE chtype = char_categorize(c);

    if(chtype == C0_CONTROL) control_character(term, c);
    else if(term->esc.state == STATE_NEUTRAL) terminal_write_byte(term, c);
    else if(term->esc.state == STATE_ESC && esc_append(term, c)) escape_sequence(term, c);
    else if(term->esc.state == STATE_CSI && esc_append(term, c))   control_sequence(term, c);
    else if(term->esc.state == STATE_OSC && esc_append(term, c))   device_control_sequence(term);
    else if(term->esc.state == STATE_DCS && esc_append(term, c))   operating_system_command(term);
  }
}
