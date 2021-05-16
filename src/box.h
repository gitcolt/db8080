#include <ncurses.h>

#ifndef BOX
#define BOX

static inline void color_box(WINDOW* win, short color_pair) {
  wattron(win, COLOR_PAIR(color_pair));
  box(win, 0, 0);
  wattroff(win, COLOR_PAIR(color_pair));
}

#endif
