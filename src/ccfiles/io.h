#include "game.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

//#include "header.h"
class Io{
private:
  Board* board;
  Search* search;
  Game* game;
  FILE *file_log;
  int server_mode;
  int is_log_output;
public:
  Io(Game* g);
  ~Io();
  Board* get_board();
  static const int SIZE_FILENAME = 64;
  int cmd_prompt();
  int out_record(int resign);
  void new_game();
  void search_start();
  void back();
  void manual_move(char **last);
  int manual_move_str(char *str);
  void out_position();
  void out_legalmoves(unsigned int moves[], int cnt);
  void str_CSA_move(char *str, unsigned int moves);
  unsigned int CSA2Internal(const char *str);
  void out(const char *format, ... );
  void out_file(FILE *fp, const char *format, ...);
  void out_board();
  void out_server();
  void out_move_string(char *str);
};
