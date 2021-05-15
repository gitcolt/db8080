struct Flags_Pane {
  WINDOW* win;
  WINDOW* s_win;
  WINDOW* z_win;
  WINDOW* a_win;
  WINDOW* p_win;
  WINDOW* c_win;
};

struct Flags_Pane* flags_pane_new(int starty, int startx);
