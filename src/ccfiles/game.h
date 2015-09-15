#include "search.h"

class Game {
private:
  Board* board;
  Search* search;
public:
  Game();
  Board* get_board();
  Search* get_search();
  void game_initialize();
  void game_finalize();
};
