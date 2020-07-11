#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RED(X) ((X & 0xFF0000) >> 16)
#define GREEN(X) ((X & 0x00FF00) >> 8)
#define BLUE(X) (X & 0x0000FF)
#define RGB(R,G,B) (R << 16 | G << 8 | B)

enum ESC_STATE {
  STATE_NEUTRAL,  // No mode
  STATE_ESC,     // Escape
  STATE_CSI,     // Escape(CSI)
  STATE_OSC,     // Escape(OSC)
  STATE_DCS      // Escape(DCS)
};

enum CELL_ATTRIBUTE {
  ATTR_OFF          = 0,
  ATTR_BOLD         = 1,
  ATTR_ITALIC       = 1 < 1,
  ATTR_UNDERLINE    = 1 < 2,
  ATTR_BLINK        = 1 < 3,
  ATTR_INVERSE      = 1 < 4
};

struct esc_sequence {
  bytebuffer* buf;        // For storing esc codes
  enum ESC_STATE state;   // State of parser
};

struct point {
  unsigned int x, y;
};

struct cell {
  bool d;                       // Dirty
  unsigned int c;               // Chacter
  unsigned long fg;             // Foreground
  unsigned long bg;             // Background
  enum CELL_ATTRIBUTE attr;     // Attribute
};

struct terminal {
  int pid;
  int fd_master;
  int fd_slave; // we don't need to take care for this
  font_data* font;
  unsigned int cols, rows;
  enum CELL_ATTRIBUTE attr;
  struct cell* screen;        // For screen data only, not for saving lines
  struct esc_sequence esc;
  struct point cursor;
  unsigned long custom_color[COLORS];
};

bool terminal_init(struct terminal* t, unsigned int cols, unsigned int rows, font_data* font) {

  t->font = font;
  t->cols = cols;
  t->rows = rows;
  
  t->attr = ATTR_OFF;
  t->cursor.x = 0;
  t->cursor.y = 0;

  t->esc.state = STATE_NEUTRAL;
  t->esc.buf = bytebuffer_init(50); // initial esc buffer size TODO
  t->screen = malloc(sizeof(struct cell) * cols * rows);

  for(int c = 0; c < 256; c++) t->custom_color[c] = -1;

  return true;
}

void terminal_clear(struct terminal* t) {
  for(int i = 0; i < t->cols * t->rows; i++)
    t->screen[i].d = false;
}

void terminal_erase_cell(struct terminal* t, unsigned int x, unsigned int y) {
  t->screen[x + y * t->cols].d = false;
}

void terminal_set_cursor(struct terminal* t, unsigned int x, unsigned int y) {
  t->cursor.x = (x > t->cols) ? t->cols : x;
  t->cursor.y = (y > t->rows) ? t->rows : y;
}

void terminal_move_cursor(struct terminal* t, int x, int y) {
  terminal_set_cursor(t, t->cursor.x + x, t->cursor.y + y);
}

// TODO attribute
void terminal_set_cell(struct terminal* t, unsigned int c, unsigned int x, unsigned y) {
  int index = x + y * t->cols;
  t->screen[index].d = true;
  t->screen[index].c = c;
  t->screen[index].fg = RGB(255, 255, 255);
  t->screen[index].bg = RGB(255, 255, 255);
}

void terminal_scroll(struct terminal* t, int l) {
  //terminal_clear(t);
  for(int i = 0; i < t->rows; i++)
    for(int j = 0; j < t->cols; j++)
      if(j + (i + l) * t->cols < t->rows * t->cols && t->screen[j + (i + l) * t->cols].d)
        terminal_set_cell(t, t->screen[j + (i + l) * t->cols].c, j, i);
      else 
        terminal_erase_cell(t, j, i);
}

void terminal_cursor_next(struct terminal* t) {
  t->cursor.x++;
  if(t->cursor.x >= t->cols) {
    t->cursor.x = 0;
    t->cursor.y++;
  }
  if(t->cursor.y >= t->rows) {
    terminal_scroll(t, 1);
    t->cursor.y = t->rows-1;
  }
}

void terminal_cursor_next_line(struct terminal* t) {
  t->cursor.y++;
  if(t->cursor.y >= t->rows) {
    terminal_scroll(t, 1);
    t->cursor.y = t->rows-1;
  }
}

void terminal_write_byte(struct terminal* t, unsigned int c) {
  if(t->cursor.x + t->cursor.y * t->cols > t->cols * t->rows) return;
  terminal_set_cell(t, c, t->cursor.x, t->cursor.y);
  terminal_cursor_next(t);
}

void terminal_write(struct terminal* t, unsigned int* s, size_t l) {
  for(int i = 0; i < l; i++)
    terminal_write_byte(t, s[i]);
}

void test_setcells(struct terminal* t){
  for(int i = 0; i < t->cols * t->rows; i++) {
    t->screen[i].d = false;
    continue;
    t->screen[i].c = i%128;
    t->screen[i].fg = color256[i%256];
    t->screen[i].bg = color256[i%256];
  }
}
