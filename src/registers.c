#include "colors.h"
#include "registers.h"

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

  mvwprintw(win, 1, 2, "%02X", val);
  wrefresh(win);
}

struct Registers_Pane* registers_pane_new(int starty, int startx) {
  struct Registers_Pane* regs = malloc(sizeof(struct Registers_Pane));
  regs->win = newwin(14, 22, starty, startx);
  wattron(regs->win, COLOR_PAIR(YELLOW));
  box(regs->win, 0, 0);
  wattroff(regs->win, COLOR_PAIR(YELLOW));
  mvwprintw(regs->win, 0, 2, "Registers");

  regs->a_win = derwin(regs->win, 3, 6, 1, 5);
  regs->b_win = derwin(regs->win, 3, 6, 4, 5);
  regs->c_win = derwin(regs->win, 3, 6, 4, 11);
  regs->d_win = derwin(regs->win, 3, 6, 7, 5);
  regs->e_win = derwin(regs->win, 3, 6, 7, 11);
  regs->h_win = derwin(regs->win, 3, 6, 10, 5);
  regs->l_win = derwin(regs->win, 3, 6, 10, 11);

  wattron(regs->a_win, COLOR_PAIR(BLUE));
  box(regs->a_win, 0, 0);
  wattroff(regs->a_win, COLOR_PAIR(BLUE));
  wrefresh(regs->a_win);

  wattron(regs->b_win, COLOR_PAIR(BLUE));
  box(regs->b_win, 0, 0);
  wattroff(regs->b_win, COLOR_PAIR(BLUE));
  wrefresh(regs->b_win);

  wattron(regs->c_win, COLOR_PAIR(BLUE));
  box(regs->c_win, 0, 0);
  wattroff(regs->c_win, COLOR_PAIR(BLUE));
  wrefresh(regs->c_win);

  wattron(regs->d_win, COLOR_PAIR(BLUE));
  box(regs->d_win, 0, 0);
  wattroff(regs->d_win, COLOR_PAIR(BLUE));
  wrefresh(regs->d_win);

  wattron(regs->e_win, COLOR_PAIR(BLUE));
  box(regs->e_win, 0, 0);
  wattroff(regs->e_win, COLOR_PAIR(BLUE));
  wrefresh(regs->e_win);

  wattron(regs->h_win, COLOR_PAIR(BLUE));
  box(regs->h_win, 0, 0);
  wattroff(regs->h_win, COLOR_PAIR(BLUE));
  wrefresh(regs->h_win);

  wattron(regs->l_win, COLOR_PAIR(BLUE));
  box(regs->l_win, 0, 0);
  wattroff(regs->l_win, COLOR_PAIR(BLUE));
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
