#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RED(X) ((X & 0xFF0000) >> 16)
#define GREEN(X) ((X & 0x00FF00) >> 8)
#define BLUE(X) (X & 0x0000FF)
#define RGB(R,G,B) (R << 16 | G << 8 | B)

enum ESC_STATE {
  STATE_NUTRAL,  // No mode
  STATE_ESC,     // Escape
  STATE_CSI,     // Escape(CSI)
  STATE_OSC,     // Escape(OSC)
  STATE_DCS      // Escape(DCS)
};

struct point {
  unsigned int x, y;
};

struct esc_sequence {
  bytebuffer* buf;        // For storing esc codes
  enum ESC_STATE state;   // State of parser
};

enum CELL_ATTRIBUTE {
  ATTR_OFF          = 0,
  ATTR_BOLD         = 1,
  ATTR_ITALIC       = 1 < 1,
  ATTR_UNDERLINE    = 1 < 2,
  ATTR_BLINK        = 1 < 3,
  ATTR_INVERSE      = 1 < 4
};

struct cell {
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

bool init_terminal(struct terminal* t, unsigned int cols, unsigned int rows, font_data* font) {

  t->font = font;
  t->cols = cols;
  t->rows = rows;
  
  t->attr = ATTR_OFF;
  t->cursor.x = 0;
  t->cursor.y = 0;

  t->esc.buf = bytebuffer_init(50); // initial esc buffer size TODO
  t->screen = malloc(sizeof(struct cell) * cols * rows);

  for(int c = 0; c < 256; c++) t->custom_color[c] = -1;

  return true;
}

void test_setcells(struct terminal* t){
  for(int i = 0; i < t->cols * t->rows; i++) {
    t->screen[i].c = i%128;
    t->screen[i].fg = color256[i%256];
    t->screen[i].bg = color256[i%256];
  }
}
