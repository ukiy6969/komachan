#include "game.h"


Game::Game(std::string bp): binPath(bp){
  board = new Board();
  search = new Search(board);
}

Game::~Game() {
  delete board;
  delete search;
}

Board* Game::get_board(){
  return board;
}

Search* Game::get_search(){
  return search;
}

void Game::game_initialize(){
  if ( board->zobrist_init(binPath) < 0 ) {
    exit(1);
  }
  board->clear_game();
  if ( board->starting_initialize(binPath) < 0 ) {
    exit(1);
  }
}

void Game::game_finalize(){
}

unsigned int Game::move(double _fromX, double _fromY, double _toX, double _toY, const char *piece){
  int move;
  int from, to, type, type_c, prom, cap, cap_type;
  int fromX, fromY, toX, toY, i;
  const char *buf;
  const char *_ch_piece_csa[16] =
  { "--", "FU", "--", "--", "GI", "KI", "KA", "HI",
    "OU", "TO", "--", "--", "NG", "--", "UM", "RY" };

  fromX = int(_fromX);
  fromY = int(_fromY);
  toX   = int(_toX);
  toY   = int(_toY);
  buf   = piece;

  from = 5 - fromX + ( fromY -1 )*5;
  to   = 5 - toX   + ( toY   -1 )*5;

  if( fromX == 0 && fromY == 0 )
    {
      from = move_drop;
      for( i=0; i < 16; i++ )
        {
          if( !strcmp( buf, _ch_piece_csa[ i ] ) )
            {
              type_c = board->get_turn() ? i+16 : i;
              break;
            }
        }
      if( i >= 16 )
        type_c = 0;
      prom = 0;
      cap = 0;
      cap_type = 0;
    }
  else
    {
      if( from < 0 || from >= 25 || to < 0 || to >= 25 )
        { return MOVE_NULL; }

      type_c = board->get_turn() ? board->get_piece_on_sq_b( from ) : board->get_piece_on_sq_w( from );
      type   = ( type_c & ~mask_piece_color );

      if( !strcmp( buf, _ch_piece_csa[ 0 ] ) )
        { return MOVE_NULL; }
      if( ( type < m_promote ) && !strcmp( buf, _ch_piece_csa[ type + m_promote ] ) )
        { prom = 1; }
      else if( !strcmp( buf, _ch_piece_csa[ type ] ) )
        { prom = 0; }
      else
        { return MOVE_NULL; }
      cap_type = board->get_turn() ? board->get_piece_on_sq_w( to ) : board->get_piece_on_sq_b( to );
      if( cap_type == no_piece )
        {
          cap = 0;
          cap_type = 0;
        }
      else
        {
          cap = 1;
        }
    }

  move = MOVE_C( type_c, from, to, prom, cap, cap_type );
  board->make_move( move );
  return move;

}
