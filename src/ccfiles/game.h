#include "search.h"
#include <iostream>

class Game {
private:
  Board* board;
  Search* search;
  std::string binPath;
public:
  Game(std::string binPath = "./");
  ~Game();
  Board* get_board();
  Search* get_search();
  void game_initialize();
  void game_finalize();
  unsigned int move(double fromX, double fromY, double toX, double toY, const char *piece);
};
