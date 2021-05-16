#include "colors.h"
#include "memory.h"
#include "box.h"

#include <stdlib.h>
#include <stdio.h>

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
  if (mem->cursorx > minx) {
    mvwchgat(mem->outer_pad, mem->cursory, mem->cursorx, 2, A_BOLD, -1, NULL);
    mem->cursorx -= 3;
    mvwchgat(mem->outer_pad, mem->cursory, mem->cursorx, 2, A_STANDOUT, -1, NULL);
    refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
  }
}

void mem_move_right(struct Memory_Pane* mem) {
  int maxx = getmaxx(mem->bytes_pad) + 3;
  if (mem->cursorx < maxx) {
    mvwchgat(mem->outer_pad, mem->cursory, mem->cursorx, 2, A_BOLD, -1, NULL);
    mem->cursorx += 3;
    mvwchgat(mem->outer_pad, mem->cursory, mem->cursorx, 2, A_STANDOUT, -1, NULL);
    refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
  }
}

void mem_move_up(struct Memory_Pane* mem) {
  if (mem->cursory <= mem->outer_pad_content_starty)
    mem_scroll_up(mem);
  if (mem->cursory > 0) {
    mvwchgat(mem->outer_pad, mem->cursory, mem->cursorx, 2, A_BOLD, -1, NULL);
    mvwchgat(mem->outer_pad, --mem->cursory, mem->cursorx, 2, A_STANDOUT, -1, NULL);
    refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
  }
}

void mem_move_down(struct Memory_Pane* mem) {
  if (mem->cursory >= mem->outer_pad_content_starty + mem->outer_pad_rect_height)
    mem_scroll_down(mem);
  if (mem->cursory < mem->outer_pad_content_height - 1) {
    mvwchgat(mem->outer_pad, mem->cursory, mem->cursorx, 2, A_BOLD, -1, NULL);
    mvwchgat(mem->outer_pad, ++mem->cursory, mem->cursorx, 2, A_STANDOUT, -1, NULL);
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

  getbegyx(mem->bytes_pad, mem->cursory, mem->cursorx);
  mvwchgat(mem->outer_pad, mem->cursory, mem->cursorx, 2, A_STANDOUT, -1, NULL);
  refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
}

void edit_byte(struct Memory_Pane* mem, int row, int col, char ch) {
  static int is_waiting_for_input = 0;
  static char buf[] = "00";

  // Break out if directional character
  if (ch == 'h' || ch == 'j' || ch == 'k' || ch == 'l') {
    is_waiting_for_input = 0;
    ungetch(ch);
    return;
  }

  // Ignore non-hex, non-directional characters
  if ( !(ch >= '0' && ch <= '9') &&
       !(ch >= 'a' && ch <= 'f')    ) {
    edit_byte(mem, row, col, getch());
    return;
  }

  if (!is_waiting_for_input) {
    buf[0] = ch;
    buf[1] = '0';
  } else
    buf[1] = ch;

  int hexval;
  sscanf(buf, "%02X", &hexval);

  int off = row * 16 + col;
  mem->bytes[off] = hexval;
  update_bytes(mem, off, 1);
  is_waiting_for_input = !is_waiting_for_input;
  if (is_waiting_for_input)
    edit_byte(mem, row, col, getch());
}

int update_bytes(struct Memory_Pane* mem, size_t off, int count) {

  if (off + count > mem->bytes_size)
    return -1;
  while (count-- > 0) {
    wattron(mem->bytes_pad, A_REVERSE);
    mvwprintw(mem->bytes_pad, off/16, (off % 16) * 3, "%2X", mem->bytes[off]);
    wattroff(mem->bytes_pad, A_REVERSE);
    off++;
  }
  refresh_bytes(mem, mem->outer_pad_content_starty, mem->outer_pad_content_startx);
  return 0;
}

