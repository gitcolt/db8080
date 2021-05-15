#include "colors.h"
#include "memory.h"
#include "flags.h"

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
  init_pair(MAGENTA, COLOR_MAGENTA, -1);
  init_pair(WHITE, COLOR_WHITE, -1);
  init_pair(CYAN, COLOR_CYAN, -1);
  refresh(); // Do I need to call this?
}

int main() {
  init();

  struct Memory_Pane* mem = memory_pane_new(1, 40);

  char* filename = "invaders";
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Unable to open $s\n", filename);
    fflush(stdout);
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);
  unsigned char* bytes = malloc(MAX_BYTES);
  fread(bytes, fsize, 1, file);
  fclose(file);

  load_memory(mem, bytes, fsize);

  struct Flags_Pane* flags = flags_pane_new(0, 0);

  int ch;
  while (ch = getch()) {
    switch(ch) {
      case KEY_UP:
      case 'k':
        mem_move_up(mem);
        break;
      case KEY_DOWN:
      case 'j':
        mem_move_down(mem);
        break;
      case 'l':
        mem_move_right(mem);
        break;
      case 'h':
        mem_move_left(mem);
        break;
    }
  }

  getch(); // Wait for user input
  endwin();
}
