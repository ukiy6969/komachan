#include "search.h"
#include <iostream>

class Game {
private:
  Board* _board;
  Search* _search;
  std::string binPath;
public:
  Game(std::string binPath = "./");
  ~Game();
  Board* get_board();
  Search* get_search();
  void game_initialize();
  void game_finalize();
  unsigned int move(double *_fromX, double *_fromY, double *_toX, double *_toY, const char *piece, double *_color, double *_promote, std::string* cap, double *isAttack);
  double search(double *_fromX, double *_fromY, double *_toX, double *_toY, std::string *piece, double *_color, double *_promote, std::string* cap, double *isAttack);
  void legal(const unsigned int& lmove, double *_fromX, double *_fromY, double *_toX, double *_toY, std::string *_piece);
};
