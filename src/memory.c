#include "colors.h"
#include "memory.h"
#include "box.h"

#include <stdlib.h>

static int cursory;
static int cursorx;

void refresh_bytes(struct Memory_Pane* mem, int content_starty, int content_startx) {
  prefresh(mem->outer_pad,
    content_starty,
    content_startx,
    mem->outer_win_starty + mem->border_height,
    mem->outer_win_startx + mem->border_width,
    mem->outer_win_starty + mem->border_height + mem->outer_pad_rect_height,
    mem->outer_win_startx + mem->border_width + mem->outer_pad_rect_width);
}

struct Memory_Pane* memory_pane_new(int starty, int startx) {
  struct Memory_Pane* mem = malloc(sizeof(struct Memory_Pane));
  mem->border_height = 2;
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

  wattron(mem->addr_pad, A_BOLD);
  for (int row = 0; row < fsize/16; row++) {
    wprintw(mem->addr_pad, "%07X\n", row * 16);
  }
  wattroff(mem->addr_pad, A_BOLD);

  color_box(mem->outer_win, RED);
  mvwprintw(mem->outer_win, 0, 2, "Memory");
  wrefresh(mem->outer_win);

  //mvwprintw(mem->outer_win, 0, mem->border_width + mem->addr_rect_width + 1, "0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");

  refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);

  wrefresh(mem->outer_win);

  return mem;
}

void mem_move_left(struct Memory_Pane* mem) {
  int minx = getbegx(mem->bytes_pad);
  if (cursorx > minx) {
    mvwchgat(mem->outer_pad, cursory, cursorx, 2, A_BOLD, -1, NULL);
    cursorx -= 3;
    mvwchgat(mem->outer_pad, cursory, cursorx, 2, A_STANDOUT, -1, NULL);
    refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
  }
}

void mem_move_right(struct Memory_Pane* mem) {
  int maxx = getmaxx(mem->bytes_pad) + 3;
  if (cursorx < maxx) {
    mvwchgat(mem->outer_pad, cursory, cursorx, 2, A_BOLD, -1, NULL);
    cursorx += 3;
    mvwchgat(mem->outer_pad, cursory, cursorx, 2, A_STANDOUT, -1, NULL);
    refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
  }
}

void mem_move_up(struct Memory_Pane* mem) {
  if (cursory <= mem->outer_pad_content_starty)
    mem_scroll_up(mem);
  if (cursory > 0) {
    mvwchgat(mem->outer_pad, cursory, cursorx, 2, A_BOLD, -1, NULL);
    mvwchgat(mem->outer_pad, --cursory, cursorx, 2, A_STANDOUT, -1, NULL);
    refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
  }
}

void mem_move_down(struct Memory_Pane* mem) {
  if (cursory >= mem->outer_pad_content_starty + mem->outer_pad_rect_height)
    mem_scroll_down(mem);
  if (cursory < mem->outer_pad_content_height - 1) {
    mvwchgat(mem->outer_pad, cursory, cursorx, 2, A_BOLD, -1, NULL);
    mvwchgat(mem->outer_pad, ++cursory, cursorx, 2, A_STANDOUT, -1, NULL);
    refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
  }
}

void mem_scroll_up(struct Memory_Pane* mem) {
  if (mem->outer_pad_content_starty > 0)
    refresh_bytes(mem, --mem->outer_pad_content_starty, mem->outer_pad_content_startx);
}

void mem_scroll_down(struct Memory_Pane* mem) {
  if (mem->outer_pad_content_starty < (mem->bytes_content_height - mem->outer_pad_rect_height - 1))
    refresh_bytes(mem, ++mem->outer_pad_content_starty, mem->outer_pad_content_startx);
}

void load_memory(struct Memory_Pane* mem, unsigned char* bytes, size_t size) {
  if (size > MAX_BYTES)
    return;
  wattron(mem->bytes_pad, A_BOLD);
  for (int row = 0; row < size/16; row++) {
    for (int i = 0; i < 16; i++) {
      wprintw(mem->bytes_pad, "%02X ", *(bytes + (row * 16) + i));
    }
  }
  wattroff(mem->bytes_pad, A_BOLD);

  getbegyx(mem->bytes_pad, cursory, cursorx);
  mvwchgat(mem->outer_pad, cursory, cursorx, 2, A_STANDOUT, -1, NULL);

  refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
}

int update_byte(struct Memory_Pane* mem, char ch, size_t off) {
  if (off > mem->bytes_size - 1)
    return -1;
  mem->bytes[off] = 0xFF;
  mvwprintw(mem->bytes_pad, off/16, (off % 16) * 3, "%02X", mem->bytes[off]);
  refresh_bytes(mem, 0, 0);
  return 0;
}

