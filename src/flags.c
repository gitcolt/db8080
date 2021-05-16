#include "ncurses.h"
#include "flags.h"
#include "colors.h"
#include "box.h"

#include <stdlib.h>
#include <ctype.h>

void set_flag(struct Flags_Pane* flags, char ch, int on) {
  WINDOW* win;
  switch(ch) {
    case 's':
      win = flags->s_win; break;
    case 'z':
      win = flags->z_win; break;
    case 'a':
      win = flags->a_win; break;
    case 'p':
      win = flags->p_win; break;
    case 'c':
      win = flags->c_win; break;
  }
  wattron(win, on ? COLOR_PAIR(RED) : COLOR_PAIR(WHITE));
  wattron(win, A_BOLD);
  mvwaddch(win, 1, 1, toupper(ch));
  wattroff(win, on ? COLOR_PAIR(RED) : COLOR_PAIR(WHITE));
  wattroff(win, A_BOLD);
  wrefresh(win);
}

struct Flags_Pane* flags_pane_new (int starty, int startx) {
  struct Flags_Pane* flags = malloc(sizeof(struct Flags_Pane));
  flags->win = newwin(5, 23, starty, startx);
  color_box(flags->win, CYAN);
  mvwprintw(flags->win, 0, 2, "Flags");
  flags->s_win = derwin(flags->win, 3, 3, 1, 2);
  flags->z_win = derwin(flags->win, 3, 3, 1, 6);
  flags->a_win = derwin(flags->win, 3, 3, 1, 10);
  flags->p_win = derwin(flags->win, 3, 3, 1, 14);
  flags->c_win = derwin(flags->win, 3, 3, 1, 18);

  color_box(flags->s_win, MAGENTA);
  color_box(flags->z_win, MAGENTA);
  color_box(flags->a_win, MAGENTA);
  color_box(flags->p_win, MAGENTA);
  color_box(flags->c_win, MAGENTA);

  set_flag(flags, 's', 0);
  set_flag(flags, 'z', 0);
  set_flag(flags, 'a', 0);
  set_flag(flags, 'p', 0);
  set_flag(flags, 'c', 1);

  wrefresh(flags->win);

  return flags;
}
