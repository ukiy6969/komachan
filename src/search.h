#include "board.h"

#include <time.h>
#include <string.h>
#include <assert.h>

class Search {
private:
  static const int SEARCH_DEPTH = 4;
  static const int SCORE_MAX = 8100;
  static const int SCORE_MATED = (-SCORE_MAX);
  static const int SEARCH_MAX_TIME = 10000;
  Board* board;
public:
  Search(Board* b);
  int search_root();
  int search(short alpha, short beta, int depth, int ply);
  short evaluate();
};
