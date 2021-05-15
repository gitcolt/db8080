#include "colors.h"
#include "memory.h"

#include <stdlib.h>

struct Memory_Pane* memory_pane_new(int starty, int startx) {
  struct Memory_Pane* mem = malloc(sizeof(struct Memory_Pane));
  mem->border_height = 1;
  mem->border_width = 2;
  char* filename = "invaders";
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Unable to open %s\n", filename);
    fflush(stdout);
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);
  fread(mem->bytes, fsize, 1, file);
  fclose(file);

  mem->bytes_size = fsize;
  mem->addr_rect_height = 15;
  mem->addr_rect_width = 8;
  mem->addr_content_height = fsize / 16;
  mem->addr_content_width = 8;
  mem->bytes_rect_height = 15;
  mem->bytes_rect_width = 16 * 3; // 16 bytes * (2 characters per byte + 1 space character)
  mem->bytes_content_height = fsize / 16;
  mem->bytes_content_width = 16 * 3;
  mem->outer_pad_rect_height = mem->bytes_rect_height;
  mem->outer_pad_rect_width = mem->addr_rect_width + mem->bytes_rect_width;
  mem->outer_pad_content_height = mem->bytes_content_height;
  mem->outer_pad_content_width = mem->addr_content_width + mem->bytes_content_width;
  mem->outer_pad = newpad(mem->outer_pad_content_height, mem->outer_pad_content_width);
  mem->outer_pad_content_starty = 0;
  mem->outer_pad_content_startx = 0;

  mem->bytes_pad = subpad(mem->outer_pad,
      mem->bytes_content_height,
      mem->bytes_content_width,
      0,
      mem->addr_rect_width);
  if (mem->bytes_pad == NULL) {
    mvprintw(LINES - 1, 0, "Unable to create memory bytes pad.\n");
    endwin();
    return NULL;
  }

  mem->addr_pad = subpad(mem->outer_pad,
      mem->addr_content_height,
      mem->addr_content_width,
      0,
      0);
  if (mem->addr_pad == NULL) {
    mvprintw(LINES - 1, 0, "Unable to create memory address pad.\n");
    endwin();
    return NULL;
  }

  mem->outer_win_starty = starty;
  mem->outer_win_startx = startx;
  mem->outer_win = newwin(mem->outer_pad_rect_height + mem->border_height * 2 + 1,
                          mem->outer_pad_rect_width + mem->border_width * 2 - 1,
                          mem->outer_win_starty,
                          mem->outer_win_startx);
  if (mem->outer_win == NULL) {
    mvprintw(LINES - 1, 0, "Unable to create memory window.\n");
    endwin();
    return NULL;
  }

  for (int row = 0; row < fsize/16; row++) {
    wprintw(mem->addr_pad, "%07X\n", row * 16);
  }

  wattron(mem->outer_win, COLOR_PAIR(RED));
  box(mem->outer_win, 0, 0);
  wattroff(mem->outer_win, COLOR_PAIR(RED));
  mvwprintw(mem->outer_win, 0, 2, "Memory");
  wrefresh(mem->outer_win);

  //mvwprintw(mem->outer_win, 0, mem->border_width + mem->addr_rect_width + 1, "0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
  prefresh(mem->outer_pad,
           mem->outer_pad_content_starty,
           mem->outer_pad_content_startx,
           mem->outer_win_starty + mem->border_height,
           mem->outer_win_startx + mem->border_width,
           mem->outer_win_starty + mem->border_height + mem->outer_pad_rect_height ,
           mem->outer_win_startx + mem->border_width + mem->outer_pad_rect_width);

  wmove(mem->outer_win, mem->border_height, mem->border_width + mem->addr_rect_width + 1);
  wrefresh(mem->outer_win);

  return mem;
}

void mem_move_left(struct Memory_Pane* mem) {
  int y, x;
  getyx(mem->outer_win, y, x);
  if (x > mem->border_width + mem->addr_rect_width + 1) {
    wmove(mem->outer_win, y, x - 3);
    wrefresh(mem->outer_win);
  }
}

void mem_move_right(struct Memory_Pane* mem) {
  int y, x;
  getyx(mem->outer_win, y, x);
  if (x < mem->border_width + mem->addr_rect_width + mem->bytes_rect_width - 2) {
    wmove(mem->outer_win, y, x + 3);
    wrefresh(mem->outer_win);
  }
}

void mem_move_up(struct Memory_Pane* mem) {
  int y, x;
  getyx(mem->outer_win, y, x);
  if (y > mem->border_height) {
    wmove(mem->outer_win, --y, x);
    wrefresh(mem->outer_win);
  } else
    mem_scroll_up(mem);
}

void mem_move_down(struct Memory_Pane* mem) {
  int y, x;
  getyx(mem->outer_win, y, x);
  if (y < mem->border_height + mem->bytes_rect_height) {
    wmove(mem->outer_win, ++y, x);
    wrefresh(mem->outer_win);
  } else
    mem_scroll_down(mem);
}

void mem_scroll_up(struct Memory_Pane* mem) {
  if (mem->outer_pad_content_starty > 0) {
    prefresh(mem->outer_pad,
             --mem->outer_pad_content_starty,
             mem->outer_pad_content_startx,
             mem->outer_win_starty + mem->border_height,
             mem->outer_win_startx + mem->border_width,
             mem->outer_win_starty + mem->border_height + mem->outer_pad_rect_height,
             mem->outer_win_startx + mem->border_width + mem->outer_pad_rect_width);
    wrefresh(mem->outer_win);
  }
}

void mem_scroll_down(struct Memory_Pane* mem) {
  if (mem->outer_pad_content_starty < (mem->bytes_content_height - mem->outer_pad_rect_height - 1)) {
    prefresh(mem->outer_pad,
             ++mem->outer_pad_content_starty,
             mem->outer_pad_content_startx,
             mem->outer_win_starty + mem->border_height,
             mem->outer_win_startx + mem->border_width,
             mem->outer_win_starty + mem->border_height + mem->outer_pad_rect_height,
             mem->outer_win_startx + mem->border_width + mem->outer_pad_rect_width);
    wrefresh(mem->outer_win);
  }
}

void load_memory(struct Memory_Pane* mem, unsigned char* bytes, size_t size) {
  if (size > MAX_BYTES)
    return;
  for (int row = 0; row < size/16; row++) {
    for (int i = 0; i < 16; i++) {
      wprintw(mem->bytes_pad, "%02X ", *(bytes + (row * 16) + i));
    }
  }
  prefresh(mem->outer_pad,
           mem->outer_pad_content_starty,
           mem->outer_pad_content_startx,
           mem->outer_win_starty + mem->border_height,
           mem->outer_win_startx + mem->border_width,
           mem->outer_win_starty + mem->border_height + mem->outer_pad_rect_height,
           mem->outer_win_startx + mem->border_width + mem->outer_pad_rect_width);
  wrefresh(mem->outer_win);
}

