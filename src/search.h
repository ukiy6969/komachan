#include "board.h"

#include <time.h>
#include <string.h>
#include <assert.h>

class Search {
private:
  static const int SCORE_MAX = 8100;
  static const int SCORE_MATED = (-SCORE_MAX);
  Board* board;
public:
  Search(Board* b);
  static constexpr int SEARCH_DEPTH = 6;
  static constexpr double SEARCH_MAX_TIME = 20.0;
  int useTpt;
  int searchDepth;
  double searchMaxTime;
  double searchSumTime;
  double currentEval;
  double search_root();
  inline int search(short alpha, short beta, int depth, int ply);
  inline short evaluate();
};
