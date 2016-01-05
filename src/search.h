#include "board.h"

#include <time.h>
#include <string.h>
#include <assert.h>

// トランスポジションのバリュー
typedef struct _tpt_v
{
  int depth;     // 探索の深さ
  char turn;
  int eval;             // 評価値
} tpt_v;

class Search {
private:
  static const int SCORE_MAX = 81000;
  static const int SCORE_MATED = (-SCORE_MAX);
  static constexpr int TPT_SIZE_MAX = 10000000;
  unsigned int legal_moves[25][ SIZE_LEGALMOVES ];
  int imove[25];
  int value[25];
  int max[25];
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

  // 駒に関する評価値
  int pawnScore;
  int silverScore;
  int goldScore;
  int bishopScore;
  int rookScore;
  int proPawnScore;
  int proSilverScore;
  int horseScore;
  int dragonScore;
  int handPawnScore;
  int handSilverScore;
  int handGoldScore;
  int handBishopScore;
  int handRookScore;

  // 駒の位置に関する評価値
  int pawnPosiScore[25];
  int silverPosiScore[25];
  int goldPosiScore[25];
  int bishopPosiScore[25];
  int rookPosiScore[25];
  int proPawnPosiScore[25];
  int proSilverPosiScore[25];
  int horsePosiScore[25];
  int dragonPosiScore[25];

  double search_root();
  int search(int alpha, int beta, int depth, int ply, int *_depthed);
  int evaluate();
  int evaluatePosition();
  int is_conti_search();
  void set_tpt(unsigned long long key, int depth, char turn, int value);
  std::unordered_map<unsigned long long, tpt_v>* get_tpt();
  void print_tpt();
  void write_tpt(std::string binPath);
  void read_tpt(std::string binPath);
};
