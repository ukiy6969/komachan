#include "board.h"

#include <time.h>
#include <string.h>
#include <assert.h>

// トランスポジションのバリュー
typedef struct _tpt_v
{
  int depth;     // 探索の深さ
  char turn;
  short eval;             // 評価値
} tpt_v;

class Search {
private:
  static const int SCORE_MAX = 8100;
  static const int SCORE_MATED = (-SCORE_MAX);
  static constexpr int TPT_SIZE_MAX = 10000000;
  unsigned int legal_moves[25][ SIZE_LEGALMOVES ];
  int imove[25];
  short value[25];
  short max[25];
  int nmove[25];
  int depthed[25];
  int maxDepth[25];
  int isConti[25];
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
  int search(short alpha, short beta, int depth, int ply, int *_depthed);
  short evaluate();
  int is_conti_search();
  void set_tpt(unsigned long long key, int depth, char turn, short value);
  std::unordered_map<unsigned long long, tpt_v>* get_tpt();
  void print_tpt();
  void write_tpt(std::string binPath);
  void read_tpt(std::string binPath);
};
