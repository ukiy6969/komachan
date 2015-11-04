#include "search.h"

Search::Search(Board* b){
  board = b;
}

inline void move_ordering(short *evals, unsigned int *legal_moves, unsigned int n);

int Search::search_root()
{
  /*
    ret =  0: succeeded
        = -1: failed
        = -2: the game is already finished
   */
  int best;
  unsigned int nmove = 0, imove;
  int depth = SEARCH_DEPTH;
  int d;
  int maxtime = SEARCH_MAX_TIME;
  int search_time;
  short value;
  short beta, max = 0;
  unsigned int legalmoves[ SIZE_LEGALMOVES ];
  short evals[ SIZE_LEGALMOVES ];
  clock_t start, end;

  nmove = board->gen_legalmoves( legalmoves );

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

    best = 0;
    max   = -SCORE_MAX;
    beta  = SCORE_MAX;
    for( imove = 0; imove < nmove; imove++ ) {

        board->make_move( legalmoves[ imove ] );
        value = -search( -beta, -max, d -1, 1 );
        evals[imove] = value;
        board->unmake_move();

        if( value > max )
          {
            max = value;
            best = imove;
          }

    }

    if (d != depth) {
      move_ordering(evals, legalmoves, nmove);
    }

    /*
    if ( (((double)clock() - start)) > maxtime) {
      break;
    }
    */


  }

  end = clock();

  search_time = (double)(end - start);
  std::cout << search_time << std::endl;

  board->make_move( legalmoves[ best ] );
  return search_time;
}

int Search::search( short alpha, short beta, int depth, int ply )
{
  int imove;
  short value, _value;
  short max = -SCORE_MAX;
  int nmove;

  unsigned int legalmoves[ SIZE_LEGALMOVES ];

  if( depth <= 0 )
    {
      return evaluate();
    }

  nmove = board->gen_legalmoves( legalmoves );

  if( nmove == 0 )
    { return SCORE_MATED + ply; }

  max = alpha;
  for( imove = 0; imove < nmove; imove++ )
    {
      board->make_move( legalmoves[ imove ] );
      if (board->get_board_show_cnt() >= 4) {
        value = -SCORE_MAX;
      }else
      /*
      if (board->tpt.count(board->game.zobrist) != 0 && board->tpt[board->game.zobrist].depth >= depth) {
        //value = board->get_turn() ? -board->tpt[board->game.zobrist].eval : board->tpt[board->game.zobrist].eval;
        //value = board->tpt[board->game.zobrist].eval;
      } else {
        value = -search( -beta, -max, depth -1, ply + 1 );
        board->set_tpt(board->game.zobrist, depth -1, value);
      }
      */
      value = -search( -beta, -max, depth -1, ply + 1 );
      board->unmake_move();

      if( value >= beta )
        {
          return value;
        }
      if( value > max )
        {
          max = value;
        }

    }

  return max;
}

short Search::evaluate()
{
  int score = 0;

  score += ( board->popuCount( w_pawn )       - board->popuCount( b_pawn ) )       * 100;
  score += ( board->popuCount( w_silver )     - board->popuCount( b_silver ) )     * 300;
  score += ( board->popuCount( w_gold )       - board->popuCount( b_gold ) )       * 350;
  score += ( board->popuCount( w_bishop )     - board->popuCount( b_bishop ) )     * 350;
  score += ( board->popuCount( w_rook )       - board->popuCount( b_rook ) )       * 400;
  score += ( board->popuCount( w_pro_pawn )   - board->popuCount( b_pro_pawn ) )   * 200;
  score += ( board->popuCount( w_pro_silver ) - board->popuCount( b_pro_silver ) ) * 350;
  score += ( board->popuCount( w_horse )      - board->popuCount( b_horse ) )      * 450;
  score += ( board->popuCount( w_dragon )     - board->popuCount( b_dragon ) )     * 500;

  score += ( board->w_hand( pawn )   - board->b_hand( pawn ) )   * 100;
  score += ( board->w_hand( silver ) - board->b_hand( silver ) ) * 280;
  score += ( board->w_hand( gold )   - board->b_hand( gold ) )   * 370;
  score += ( board->w_hand( bishop ) - board->b_hand( bishop ) ) * 340;
  score += ( board->w_hand( rook )   - board->b_hand( rook ) )   * 390;

  return board->get_turn() ? -score: score;
}


inline void move_ordering(short *evals, unsigned int *legal_moves, unsigned int n) {
  /** insert sort **/
  short tmp_e;
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
