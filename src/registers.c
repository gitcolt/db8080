#include "colors.h"
#include "registers.h"
#include "box.h"

#include <ncurses.h>
#include <stdlib.h>

void set_register(struct Registers_Pane* regs, char ch, int val) {
  WINDOW* win;
  switch(ch) {
    case 'a':
      win = regs->a_win; break;
    case 'b':
      win = regs->b_win; break;
    case 'c':
      win = regs->c_win; break;
    case 'd':
      win = regs->d_win; break;
    case 'e':
      win = regs->e_win; break;
    case 'h':
      win = regs->h_win; break;
    case 'l':
      win = regs->l_win; break;
  }

  wattron(win, A_BOLD | COLOR_PAIR(GREEN));
  mvwprintw(win, 1, 2, "%02X", val);
  wattroff(win, A_BOLD| COLOR_PAIR(GREEN));
  wrefresh(win);
}

struct Registers_Pane* registers_pane_new(int starty, int startx) {
  struct Registers_Pane* regs = malloc(sizeof(struct Registers_Pane));
  regs->win = newwin(15, 23, starty, startx);
  color_box(regs->win, YELLOW);
  mvwprintw(regs->win, 0, 2, "Registers");

  regs->a_win = derwin(regs->win, 3, 6, 2, 5);
  regs->b_win = derwin(regs->win, 3, 6, 5, 5);
  regs->c_win = derwin(regs->win, 3, 6, 5, 12);
  regs->d_win = derwin(regs->win, 3, 6, 8, 5);
  regs->e_win = derwin(regs->win, 3, 6, 8, 12);
  regs->h_win = derwin(regs->win, 3, 6, 11, 5);
  regs->l_win = derwin(regs->win, 3, 6, 11, 12);

  color_box(regs->a_win, BLUE);
  wrefresh(regs->a_win);

  color_box(regs->b_win, BLUE);
  wrefresh(regs->b_win);

  color_box(regs->c_win, BLUE);
  wrefresh(regs->c_win);

  color_box(regs->d_win, BLUE);
  wrefresh(regs->d_win);

  color_box(regs->e_win, BLUE);
  wrefresh(regs->e_win);

  color_box(regs->h_win, BLUE);
  wrefresh(regs->h_win);

  color_box(regs->l_win, BLUE);
  wrefresh(regs->l_win);

  int y, x;
  getparyx(regs->a_win, y, x);
  mvwaddch(regs->win, y + 1, x - 2, 'A');
  getparyx(regs->b_win, y, x);
  mvwaddch(regs->win, y + 1, x - 2, 'B');
  getparyx(regs->c_win, y, x);
  mvwaddch(regs->win, y + 1, x + 7, 'C');
  getparyx(regs->d_win, y, x);
  mvwaddch(regs->win, y + 1, x - 2, 'D');
  getparyx(regs->e_win, y, x);
  mvwaddch(regs->win, y + 1, x + 7, 'E');
  getparyx(regs->h_win, y, x);
  mvwaddch(regs->win, y + 1, x - 2, 'H');
  getparyx(regs->l_win, y, x);
  mvwaddch(regs->win, y + 1, x + 7, 'L');

  set_register(regs, 'a', 0xFF);
  set_register(regs, 'b', 0xFF);
  set_register(regs, 'c', 0xFF);
  set_register(regs, 'd', 0xFF);
  set_register(regs, 'e', 0xFF);
  set_register(regs, 'h', 0xFF);
  set_register(regs, 'l', 0xFF);

  wrefresh(regs->win);
}
