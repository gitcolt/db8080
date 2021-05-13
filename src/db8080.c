#include "colors.h"
#include "memory.h"

#include <ncurses.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void init() {
  initscr();
  noecho();
  keypad(stdscr, 1); // Access arrows keys
  //curs_set(0); // Hide cursor
  start_color();
  use_default_colors(); // ?
  init_pair(RED, COLOR_RED, -1);
  refresh(); // Do I need to call this?
}

int main() {
  init();

  struct Memory_Pane mem;
  new_mem_pane(&mem, 1, 40);

  int ch;
  while (ch = getch()) {
    switch(ch) {
      case KEY_UP:
      case 'k':
        mem_move_up(&mem);
        break;
      case KEY_DOWN:
      case 'j':
        mem_move_down(&mem);
        break;
      case 'l':
        mem_move_right(&mem);
        break;
      case 'h':
        mem_move_left(&mem);
        break;
    }
  }

  getch(); // Wait for user input
  endwin();
}
