#include "ncurses.h"
#include "flags.h"
#include "colors.h"

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
  wattron(flags->win, COLOR_PAIR(CYAN));
  box(flags->win, 0, 0);
  wattroff(flags->win, COLOR_PAIR(CYAN));
  mvwprintw(flags->win, 0, 2, "Flags");
  flags->s_win = derwin(flags->win, 3, 3, 1, 2);
  flags->z_win = derwin(flags->win, 3, 3, 1, 6);
  flags->a_win = derwin(flags->win, 3, 3, 1, 10);
  flags->p_win = derwin(flags->win, 3, 3, 1, 14);
  flags->c_win = derwin(flags->win, 3, 3, 1, 18);

  wattron(flags->s_win, COLOR_PAIR(MAGENTA));
  box(flags->s_win, 0, 0);
  wattroff(flags->s_win, COLOR_PAIR(MAGENTA));
  wattron(flags->z_win, COLOR_PAIR(MAGENTA));
  box(flags->z_win, 0, 0);
  wattroff(flags->z_win, COLOR_PAIR(MAGENTA));
  wattron(flags->a_win, COLOR_PAIR(MAGENTA));
  box(flags->a_win, 0, 0);
  wattroff(flags->a_win, COLOR_PAIR(MAGENTA));
  wattron(flags->p_win, COLOR_PAIR(MAGENTA));
  box(flags->p_win, 0, 0);
  wattroff(flags->p_win, COLOR_PAIR(MAGENTA));
  wattron(flags->c_win, COLOR_PAIR(MAGENTA));
  box(flags->c_win, 0, 0);
  wattroff(flags->c_win, COLOR_PAIR(MAGENTA));

  set_flag(flags, 's', 0);
  set_flag(flags, 'z', 0);
  set_flag(flags, 'a', 0);
  set_flag(flags, 'p', 0);
  set_flag(flags, 'c', 1);

  wrefresh(flags->win);

  return flags;
}
