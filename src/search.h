#include "board.h"

#include <time.h>
#include <string.h>
#include <assert.h>

class Search {
private:
  static const int SCORE_MAX = 8100;
  static const int SCORE_MATED = (-SCORE_MAX);
  static constexpr int TPT_SIZE_MAX = 100000;
  Board* board;
public:
  Search(Board* b);
  static constexpr int SEARCH_DEPTH = 8;
  static constexpr double SEARCH_MAX_TIME = 20.0;

  // トランスポジションテーブル
  std::unordered_map<unsigned long long, tpt_v> tpt;

  int useTpt;
  int searchDepth;
  double searchMaxTime;
  double searchSumTime;
  double currentEval;

  short pawnScore;
  short silverScore;
  short goldScore;
  short bishopScore;
  short rookScore;
  short proPawnScore;
  short proSilverScore;
  short horseScore;
  short dragonScore;
  short handPawnScore;
  short handSilverScore;
  short handGoldScore;
  short handBishopScore;
  short handRookScore;
  double search_root();
  int search(short alpha, short beta, int depth, int ply);
  short evaluate();
  void set_tpt(unsigned long long key, int depth, short value);
  std::unordered_map<unsigned long long, tpt_v>* get_tpt();
  void print_tpt();
  void write_tpt(std::string binPath);
  void read_tpt(std::string binPath);
};
