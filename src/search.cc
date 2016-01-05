#include "search.h"

Search::Search(Board* b):
  pawnPosiScore {
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 2,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 0
  },
  silverPosiScore {
    0, 0, 0, 0, 0,
    1, 2, 2, 2, 1,
    1, 2, 3, 2, 1,
    1, 2, 2, 2, 1,
    1, 1, 1, 1, 1
  },
  goldPosiScore {
    1, 1, 1, 2, 0,
    1, 2, 2, 3, 2,
    1, 2, 3, 2, 1,
    1, 2, 2, 2, 1,
    0, 2, 1, 1, 1
  },
  bishopPosiScore {
    0, 0, 0, 1, 2,
    0, 2, 2, 1, 1,
    0, 2, 3, 2, 0,
    0, 1, 2, 2, 0,
    2, 1, 1, 1, 1
  },
  rookPosiScore {
    2, 2, 2, 2, 1,
    2, 1, 2, 2, 2,
    2, 1, 1, 2, 2,
    2, 1, 1, 1, 2,
    1, 2, 2, 2, 2
  },
  proPawnPosiScore {
    1, 1, 1, 2, 2,
    1, 2, 2, 3, 2,
    1, 2, 2, 2, 1,
    1, 2, 2, 2, 1,
    1, 1, 1, 1, 1
  },
  proSilverPosiScore {
    1, 2, 2, 2, 2,
    1, 2, 2, 2, 2,
    1, 2, 3, 2, 2,
    1, 2, 2, 2, 2,
    1, 1, 1, 1, 1
  },
  horsePosiScore {
    1, 1, 1, 1, 1,
    1, 2, 2, 2, 1,
    1, 2, 3, 2, 1,
    1, 2, 2, 2, 1,
    1, 1, 1, 1, 1
  },
  dragonPosiScore {
    2, 2, 2, 2, 3,
    1, 2, 2, 2, 2,
    1, 2, 3, 2, 2,
    1, 2, 2, 2, 2,
    1, 1, 1, 1, 2
  },
  useTpt (0),
  searchDepth (SEARCH_DEPTH),
  searchMaxTime (SEARCH_MAX_TIME),
  searchSumTime (0),
  currentEval (0),
  pawnScore (100),
  silverScore (300),
  goldScore (350),
  bishopScore (350),
  rookScore (400),
  proPawnScore (200),
  proSilverScore (350),
  horseScore (450),
  dragonScore (500),
  handPawnScore (100),
  handSilverScore (280),
  handGoldScore (370),
  handBishopScore (340),
  handRookScore (390)
{
  board = b;
}

inline void move_ordering(int *evals, unsigned int *legal_moves, unsigned int n);

double Search::search_root()
{
  /*
    ret =  0: succeeded
        = -1: failed
        = -2: the game is already finished
   */
  int best;
  unsigned int nmove = 0, imove;
  int depth = searchDepth;
  int depthed = 0;
  int d;
  double maxtime = searchMaxTime;
  double search_time;
  int value;
  int beta, max = 0;
  //unsigned int legalmoves[ SIZE_LEGALMOVES ];
  int evals[ SIZE_LEGALMOVES ];
  clock_t start, end;

  nmove = board->gen_legalmoves( legal_moves[0] );

  if( nmove == 0 )
    {
      //out( " This game was already concluded.\n" );
      return -2;
    }

  //out( " Root move generation -> %d moves.\n", nmove );

  if( nmove <= 1 )
    {
      best = 0;
      max = 0;
    }

  best = 0;
  max   = -SCORE_MAX;
  beta  = SCORE_MAX;

  start = clock();

  for(d = 2; d <= depth; d++  ){
    //std::cout << "depth: " << d << std::endl;
    if (nmove <= 1) {
      break;
    }

    best = 0;
    max   = -SCORE_MAX;
    beta  = SCORE_MAX;
    for( imove = 0; imove < nmove; imove++ ) {

        board->make_move( legal_moves[ 0 ][ imove ] );
        if (board->get_board_show_cnt() >= 2) {
          value = -SCORE_MAX;
        }
        else if (useTpt) {
          if (tpt.count(board->game.zobrist) != 0 && tpt[board->game.zobrist].depth > d) {
            value = tpt[board->game.zobrist].eval;
            assert ( board->get_turn() == tpt[board->game.zobrist].turn );
          } else {
            depthed = 0;
            value = -search( -beta, -max, d -1, 1, &depthed );
            set_tpt(board->game.zobrist, d-1, board->game.turn, value);
          }
        }
        else {
          depthed = 0;
          value = -search( -beta, -max, d -1, 1, &depthed);
        }
        evals[imove] = value;
        board->unmake_move();

        if( value > max )
          {
            max = value;
            best = imove;
          }

    }

    if (d < (depth - 2)) {
      move_ordering(evals, legal_moves[ 0 ], nmove);
    }

    if ( ((double)(clock() - start) / CLOCKS_PER_SEC) > (double)maxtime) {
      //std::cout << "time up" << std::endl;
      break;
    }


  }

  end = clock();

  search_time = (double)(end - start) / CLOCKS_PER_SEC;
  searchSumTime += search_time;

  board->make_move( legal_moves[ 0 ][ best ] );
  currentEval = max;
  return search_time;
}

inline int Search::search( int alpha, int beta, int depth, int ply, int* _depthed)
{
  const int cdepth = 1 + (*_depthed);
  imove[ cdepth] = 0;
  value[ cdepth] = 0;
  max[ cdepth] = -SCORE_MAX;
  nmove[ cdepth] = 0;
  isConti[ cdepth] = 0;
  depthed[ cdepth] = cdepth;
  maxDepth[ cdepth] = cdepth;
  //std::cout << "depthed" << depthed << std::endl;


  if( depth <= 0 )
    {
      return evaluate();
    }

  nmove[cdepth] = board->gen_legalmoves( legal_moves[ cdepth ] );

  if( nmove[cdepth] == 0 ) {
    return SCORE_MATED + ply;
  }

  max[cdepth] = alpha;
  for( imove[cdepth] = 0; imove[cdepth] < nmove[cdepth]; imove[cdepth]++ )
    {
      board->make_move( legal_moves[ cdepth ][ imove[cdepth] ] );
      if (board->get_board_show_cnt() >= 3) {
        value[cdepth] = -SCORE_MAX;
      }else if (useTpt) {
        if (tpt.count(board->game.zobrist) != 0 && tpt[board->game.zobrist].depth > depth - 1) {
          value[cdepth] = tpt[board->game.zobrist].eval;
          assert( board->game.turn == tpt[board->game.zobrist].turn );
        } else {
          depthed[cdepth] = cdepth;
          if ( (depth-1) == 0 && MOVE_CAPTURE( legal_moves[ cdepth ][ imove[ cdepth ] ]) ) {
            value[ cdepth ] = -search( -beta, -max[ cdepth ], depth, ply + 1, &depthed[ cdepth ] );
          } else {
            value[cdepth] = -search( -beta, -max[cdepth], depth -1, ply + 1, &depthed[cdepth] );
            set_tpt(board->game.zobrist, depth-1, board->game.turn, value[cdepth]);
         }
        }
      }else {
        depthed[cdepth] = cdepth;
        if ( (depth-1) == 0 && MOVE_CAPTURE( legal_moves[ cdepth ][ imove[ cdepth ] ]) ) {
          value[ cdepth ] = -search( -beta, -max[ cdepth ], depth, ply + 1, &depthed[ cdepth ] );
        } else {
          value[cdepth] = -search( -beta, -max[cdepth], depth -1, ply + 1, &depthed[cdepth] );
        }
      }
      board->unmake_move();

      if( value[cdepth] >= beta )
        {
          *_depthed = depthed[cdepth];
          return value[cdepth];
        }
      if( value[cdepth] > max[cdepth] )
        {
          max[cdepth] = value[cdepth];
          maxDepth[cdepth] = depthed[cdepth];
        }

    }

  *_depthed = maxDepth[cdepth];

  return max[cdepth];
}

inline int Search::evaluate()
{
  int score = 0;

  score += ( board->popuCount( w_pawn )       - board->popuCount( b_pawn ) )       * pawnScore;
  score += ( board->popuCount( w_silver )     - board->popuCount( b_silver ) )     * silverScore;
  score += ( board->popuCount( w_gold )       - board->popuCount( b_gold ) )       * goldScore;
  score += ( board->popuCount( w_bishop )     - board->popuCount( b_bishop ) )     * bishopScore;
  score += ( board->popuCount( w_rook )       - board->popuCount( b_rook ) )       * rookScore;
  score += ( board->popuCount( w_pro_pawn )   - board->popuCount( b_pro_pawn ) )   * proPawnScore;
  score += ( board->popuCount( w_pro_silver ) - board->popuCount( b_pro_silver ) ) * proSilverScore;
  score += ( board->popuCount( w_horse )      - board->popuCount( b_horse ) )      * horseScore;
  score += ( board->popuCount( w_dragon )     - board->popuCount( b_dragon ) )     * dragonScore;

  score += ( board->w_hand( pawn )   - board->b_hand( pawn ) )   * handPawnScore;
  score += ( board->w_hand( silver ) - board->b_hand( silver ) ) * handSilverScore;
  score += ( board->w_hand( gold )   - board->b_hand( gold ) )   * handGoldScore;
  score += ( board->w_hand( bishop ) - board->b_hand( bishop ) ) * handBishopScore;
  score += ( board->w_hand( rook )   - board->b_hand( rook ) )   * handRookScore;

  //score += evaluatePosition();

  return board->game.turn ? -score: score;
}

inline int Search::evaluatePosition() {
  int score = 0;

  if (board->game.BBs[ w_pawn ]) {
    assert( FIRSTONE( board->game.BBs[w_pawn] ) < 25);
    score += pawnPosiScore[ FIRSTONE( board->game.BBs[w_pawn] ) ];
  }
  if (board->game.BBs[ b_pawn ]){
    assert( FIRSTONE( board->game.BBs[b_pawn] ) < 25);
    score -= pawnPosiScore[24 - FIRSTONE( board->game.BBs[b_pawn] ) ];
  }

  if (board->game.BBs[ w_silver ]) {
    assert( FIRSTONE( board->game.BBs[w_silver] ) < 25);
    score += silverPosiScore[ FIRSTONE( board->game.BBs[w_silver] ) ];
  }
  if (board->game.BBs[ b_silver ]){
    assert( FIRSTONE( board->game.BBs[b_silver] ) < 25);
    score -= silverPosiScore[24 - FIRSTONE( board->game.BBs[b_silver] ) ];
  }

  if (board->game.BBs[ w_gold ]) {
    assert( FIRSTONE( board->game.BBs[w_gold] ) < 25);
    score += goldPosiScore[ FIRSTONE( board->game.BBs[w_gold] ) ];
  }
  if (board->game.BBs[ b_gold ]){
    assert( FIRSTONE( board->game.BBs[b_gold] ) < 25);
    score -= goldPosiScore[24 -  FIRSTONE( board->game.BBs[b_gold] ) ];
  }

  if (board->game.BBs[ w_bishop ]) {
    assert( FIRSTONE( board->game.BBs[w_bishop] ) < 25);
    score += bishopPosiScore[ FIRSTONE( board->game.BBs[w_bishop] ) ];
  }
  if (board->game.BBs[ b_bishop ]){
    assert( FIRSTONE( board->game.BBs[b_bishop] ) < 25);
    score -= bishopPosiScore[24 - FIRSTONE( board->game.BBs[b_bishop] ) ];
  }

  if (board->game.BBs[ w_rook ]) {
    assert( FIRSTONE( board->game.BBs[w_rook] ) < 25);
    score += rookPosiScore[ FIRSTONE( board->game.BBs[w_rook] ) ];
  }
  if (board->game.BBs[ b_rook ]){
    assert( FIRSTONE( board->game.BBs[b_rook] ) < 25);
    score -= rookPosiScore[24 - FIRSTONE( board->game.BBs[b_rook] ) ];
  }

  if (board->game.BBs[ w_pro_pawn ]) {
    assert( FIRSTONE( board->game.BBs[w_pro_pawn] ) < 25);
    score += proPawnPosiScore[ FIRSTONE( board->game.BBs[w_pro_pawn] ) ];
  }
  if (board->game.BBs[ b_pro_pawn ]){
    assert( FIRSTONE( board->game.BBs[b_pro_pawn] ) < 25);
    score -= proPawnPosiScore[24 - FIRSTONE( board->game.BBs[b_pro_pawn] ) ];
  }

  if (board->game.BBs[ w_pro_silver ]) {
    assert( FIRSTONE( board->game.BBs[w_pro_silver] ) < 25);
    score += proSilverPosiScore[ FIRSTONE( board->game.BBs[w_pro_silver] ) ];
  }
  if (board->game.BBs[ b_pro_silver ]){
    assert( FIRSTONE( board->game.BBs[b_pro_silver] ) < 25);
    score -= proSilverPosiScore[24 - FIRSTONE( board->game.BBs[b_pro_silver] ) ];
  }

  if (board->game.BBs[ w_horse ]) {
    assert( FIRSTONE( board->game.BBs[w_horse] ) < 25);
    score += horsePosiScore[ FIRSTONE( board->game.BBs[w_horse] ) ];
  }
  if (board->game.BBs[ b_horse ]){
    assert( FIRSTONE( board->game.BBs[b_horse] ) < 25);
    score -= horsePosiScore[24 - FIRSTONE( board->game.BBs[b_horse] ) ];
  }

  if (board->game.BBs[ w_dragon ]) {
    assert( FIRSTONE( board->game.BBs[w_dragon] ) < 25);
    score += dragonPosiScore[ FIRSTONE( board->game.BBs[w_dragon] ) ];
  }
  if (board->game.BBs[ b_dragon ]){
    assert( FIRSTONE( board->game.BBs[b_dragon] ) < 25);
    score -= dragonPosiScore[24 - FIRSTONE( board->game.BBs[b_dragon] ) ];
  }

  return score;
}


inline void move_ordering(int *evals, unsigned int *legal_moves, unsigned int n) {
  /** insert sort **/
  int tmp_e;
  unsigned int i, j, tmp_l;
  for (i = 1; i < n; i++) {
    tmp_e = evals[i];
    tmp_l = legal_moves[i];
    if (evals[i - 1] < tmp_e) {
      j = i;
      do {
        evals[j] = evals[j - 1];
        legal_moves[j] = legal_moves[j - 1];
        j--;
      } while(j > 0 && evals[j - 1] < tmp_e);
      evals[j] = tmp_e;
      legal_moves[j] = tmp_l;
    }
  }
}

inline int Search::is_conti_search() {
  if (MOVE_CAPTURE(board->history[board->game.n_ply].move)){
    return 1;
  }
  return 0;
}

inline void Search::set_tpt(unsigned long long hash, int depth, char turn, int eval) {
  if ( /*tpt.size() < TPT_SIZE_MAX &&*/ depth > 3) {
    tpt_v new_val = { depth, turn, eval};
    tpt[hash] = new_val;
  }
}
void Search::print_tpt(){
  auto itr = tpt.begin();
  while(itr != tpt.end()){
    std::cout << (*itr).first << ":" << (*itr).second.eval << ":" <<  (*itr).second.depth<< std::endl;
    ++itr;
  }
}

std::unordered_map<unsigned long long, tpt_v>* Search::get_tpt() {
  return &tpt;
}

void Search::write_tpt(std::string binPath) {
  std::string tpt_bin_name = "/Tpt.bin";
  std::string bp(binPath);
  bp.append(tpt_bin_name);
  std::ofstream ofs(bp.c_str(), std::ios_base::out | std::ios_base::binary);
  if (ofs) {
    auto itr = tpt.begin();
    while(itr != tpt.end()){
      ofs.write(reinterpret_cast<const char *>(&(*itr).first), sizeof((*itr).first));
      ofs.write(reinterpret_cast<const char *>(&(*itr).second), sizeof((*itr).second));
      ++itr;
    }
  }
  ofs.close();
}

void Search::read_tpt(std::string binPath) {
  std::string tpt_bin_name = "/Tpt.bin";
  std::string bp(binPath);
  bp.append(tpt_bin_name);
  std::ifstream ifs(bp.c_str() ,std::ios_base::in | std::ios_base::binary);
  if (ifs) {
    std::pair<unsigned long long, tpt_v> tptv;
    while(!ifs.eof()){
      ifs.read(reinterpret_cast<char *>(&(tptv.first)), sizeof(tptv.first));
      ifs.read(reinterpret_cast<char *>(&(tptv.second)), sizeof(tptv.second));
      tpt.insert(tptv);
    }
  }
  ifs.close();
}
