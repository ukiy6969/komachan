#include "env.h"

#include <time.h>
#include <string.h>
#include <assert.h>
#include "header.h"

int search_root(double *search_time)
{
  /*
    ret =  0: succeeded
        = -1: failed
        = -2: the game is already finished
   */
  int imove, best;
  int nmove = 0;
  int depth = SEARCH_DEPTH;
  int d, start, end;
  int maxtime = 10000;
  double test = 0;
  short value;
  short beta, max = 0;
  unsigned int legalmoves[ SIZE_LEGALMOVES ];
  short evals[ SIZE_LEGALMOVES ];

  nmove = gen_legalmoves( legalmoves );

  if( nmove == 0 )
    {
      if(!server_mode){
        out( " This game was already concluded.\n" );
      }
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
    for( imove = 0; imove < nmove; imove++ )
      {
        MAKE_MOVE( legalmoves[ imove ] );
        value = -search( -beta, -max, d -1, 1 );
        evals[imove] = value;
        UNMAKE_MOVE;

        if( value > max )
          {
            max = value;
            best = imove;
          }

      }

    if (d != depth) {
      move_ordering(evals, legalmoves, nmove);
    }

    if ( ((double)clock() - start)/CLOCKS_PER_SEC > maxtime) {
      break;
    }

  }

  end = clock();

  *search_time = (((double)end - start)/CLOCKS_PER_SEC);

  MAKE_MOVE( legalmoves[ best ] );
  return 0;
}


void move_ordering(short *evals, int *legal_moves, int n) {
  /** insert sort **/
  short tmp_e;
  int i, j, tmp_l;
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

int search( short alpha, short beta, int depth, int ply )
{
  int imove;
  short value;
  short max = -SCORE_MAX;
  int nmove;

  unsigned int legalmoves[ SIZE_LEGALMOVES ];

  if( depth <= 0 )
    {
      return evaluate();
    }

  nmove = gen_legalmoves( legalmoves );

  if( nmove == 0 )
    { return SCORE_MATED + ply; }

  max = alpha;
  for( imove = 0; imove < nmove; imove++ )
    {
      MAKE_MOVE( legalmoves[ imove ] );
      value = -search( -beta, -max, depth -1, ply +1 );
      UNMAKE_MOVE;

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

short evaluate()
{
  int score = 0;

  score += ( popuCount( w_pawn )       - popuCount( b_pawn ) )       * 100;
  score += ( popuCount( w_silver )     - popuCount( b_silver ) )     * 300;
  score += ( popuCount( w_gold )       - popuCount( b_gold ) )       * 350;
  score += ( popuCount( w_bishop )     - popuCount( b_bishop ) )     * 350;
  score += ( popuCount( w_rook )       - popuCount( b_rook ) )       * 400;
  score += ( popuCount( w_pro_pawn )   - popuCount( b_pro_pawn ) )   * 200;
  score += ( popuCount( w_pro_silver ) - popuCount( b_pro_silver ) ) * 350;
  score += ( popuCount( w_horse )      - popuCount( b_horse ) )      * 450;
  score += ( popuCount( w_dragon )     - popuCount( b_dragon ) )     * 500;

  score += ( W_HAND( pawn )   - B_HAND( pawn ) )   * 100;
  score += ( W_HAND( silver ) - B_HAND( silver ) ) * 280;
  score += ( W_HAND( gold )   - B_HAND( gold ) )   * 370;
  score += ( W_HAND( bishop ) - B_HAND( bishop ) ) * 340;
  score += ( W_HAND( rook )   - B_HAND( rook ) )   * 390;

  return get_turn() ? -score: score;
}
