#ifndef MEMORY
#define MEMORY

#include <ncurses.h>

#define MAX_BYTES 8192

struct Memory_Pane {
  int border_height;
  int border_width;
  WINDOW* outer_win;
  int outer_win_starty;
  int outer_win_startx;
  unsigned char bytes[MAX_BYTES];
  size_t bytes_size;

  WINDOW* outer_pad;
  int outer_pad_content_height;
  int outer_pad_content_width;
  int outer_pad_content_starty;
  int outer_pad_content_startx;
  int outer_pad_rect_height;
  int outer_pad_rect_width;

  WINDOW* bytes_pad;
  int bytes_content_height;
  int bytes_content_width;
  int bytes_rect_height;
  int bytes_rect_width;

  WINDOW* addr_pad;
  int addr_content_height;
  int addr_content_width;
  int addr_rect_height;
  int addr_rect_width;
};

struct Memory_Pane* memory_pane_new(int starty, int startx);
void mem_move_left(struct Memory_Pane* mem);
void mem_move_right(struct Memory_Pane* mem);
void mem_move_up(struct Memory_Pane* mem);
void mem_move_down(struct Memory_Pane* mem);
void mem_scroll_up(struct Memory_Pane* mem);
void mem_scroll_down(struct Memory_Pane* mem);
void load_memory(struct Memory_Pane* mem, unsigned char* bytes, size_t size);
int update_byte(struct Memory_Pane* mem, char ch, size_t off);

#endif
