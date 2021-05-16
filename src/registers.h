#include <ncurses.h>

struct Registers_Pane {
  WINDOW* win;
  WINDOW* a_win;
  WINDOW* b_win;
  WINDOW* c_win;
  WINDOW* d_win;
  WINDOW* e_win;
  WINDOW* h_win;
  WINDOW* l_win;
};

struct Registers_Pane* registers_pane_new(int starty, int startx);

void set_register(struct Registers_Pane* regs, char ch, int val);
