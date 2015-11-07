#include "game.h"


const char *_ch_piece_csa[16] =
    { "--", "FU", "--", "--", "GI", "KI", "KA", "HI",
    "OU", "TO", "--", "--", "NG", "--", "UM", "RY" };

Game::Game(std::string bp): binPath(bp){
  _board = new Board();
  _search = new Search(_board);
}

Game::~Game() {
  delete _board;
  delete _search;
}

Board* Game::get_board(){
  return _board;
}

Search* Game::get_search(){
  return _search;
}

void Game::game_initialize(){
  if ( _board->zobrist_init(binPath) < 0 ) {
    std::cout << "could not load Zobrist" << std::endl;
    exit(1);
  }
  //_board->printZobristHashed();
  _board->clear_game();
  if ( _board->starting_initialize(binPath) < 0 ) {
    std::cout << "could not load BB_Attack" << std::endl;
    exit(1);
  }
  if ( !_board->zobrist_check() ) {
    _board->printZobristHashed();
    std::cout << "zobrist is duplicate" << std::endl;
    exit(1);
  }
  if ( _search->useTpt) {
    _board->read_tpt(binPath);
  }
}

void Game::game_finalize(){
  if ( _search->useTpt ) {
    _board->write_tpt(binPath);
  }
  std::cout << "searchALL: " << _search->searchSumTime << std::endl;
}

unsigned int Game::move(double *_fromX, double *_fromY, double *_toX, double *_toY, const char *_piece, double *_color, double *_promote, std::string* _cap, double *_isAttack){
  int move;
  int from, to, type, type_c, prom, cap, cap_type;
  int fromX, fromY, toX, toY, i;
  unsigned int attack_pieces;
  const char *buf;

  fromX = int(*_fromX);
  fromY = int(*_fromY);
  toX   = int(*_toX);
  toY   = int(*_toY);
  buf   = _piece;

  from = 5 - fromX + ( fromY -1 )*5;
  to   = 5 - toX   + ( toY   -1 )*5;

  if( fromX == 0 && fromY == 0 )
    {
      from = move_drop;
      for( i=0; i < 16; i++ )
        {
          if( !strcmp( buf, _ch_piece_csa[ i ] ) )
            {
              type_c = _board->get_turn() ? i+16 : i;
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

      type_c = _board->get_turn() ? _board->get_piece_on_sq_b( from ) : _board->get_piece_on_sq_w( from );
      type   = ( type_c & ~mask_piece_color );

      if( !strcmp( buf, _ch_piece_csa[ 0 ] ) )
        { return MOVE_NULL; }
      if( ( type < m_promote ) && !strcmp( buf, _ch_piece_csa[ type + m_promote ] ) )
        { prom = 1; }
      else if( !strcmp( buf, _ch_piece_csa[ type ] ) )
        { prom = 0; }
      else
        { return MOVE_NULL; }
      cap_type = _board->get_turn() ? _board->get_piece_on_sq_w( to ) : _board->get_piece_on_sq_b( to );
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

  *_promote = (double)prom;
  if (cap_type == 0) {
    *_cap = "";
  } else {
    *_cap = _ch_piece_csa[cap_type & ~mask_piece_color];
  }

  if (_board->is_attack(&attack_pieces)) {
    *_isAttack = 1;
  } else {
    *_isAttack = 0;
  }
  *_color = _board->turn();

  move = MOVE_C( type_c, from, to, prom, cap, cap_type );
  _board->make_move( move );
  return move;

}

double Game::search(double *_fromX, double *_fromY, double *_toX, double *_toY, std::string *_piece, double *_color, double *_promote, std::string* _capture, double *_isAttack){
  double search_time = _search->search_root();
  unsigned int move = _board->history[ _board->get_nply() - 1].move;
  unsigned int attack_pieces;
  int type_c = MOVE_TYPE( move );
  int type   = type_c & ~mask_piece_color;
  int from   = MOVE_FROM( move );
  int to     = MOVE_TO( move );
  int prom   = MOVE_PROMOTE( move );
  int cap    = MOVE_CAPTURE( move );
  int cap_type = MOVE_CAPTURED_TYPE( move );

  if(cap_type > 16){
    cap_type -= 16;
  }

  if( from == move_drop ) {
    *_fromX = 0;
    *_fromY = 0;
    *_toX = 5-( to%5 );
    *_toY = ( to/5 +1);
  } else {
    *_fromX = 5-( from%5 );
    *_fromY = ( from/5 +1);
    *_toX = 5-( to%5 );
    *_toY = ( to/5 +1);
  }

  *_promote = (double)prom;


  if( prom ) {
    *_piece = _ch_piece_csa[ type + m_promote ];
  } else {
    *_piece = _ch_piece_csa[ type ];
  }

  if (cap) {
    *_capture = _ch_piece_csa[ cap_type ];
  }else {
    *_capture = "";
  }
  if (_board->is_attack(&attack_pieces)) {
    *_isAttack = 1;
  } else {
    *_isAttack = 0;
  }

  *_color = _board->turn();
  return search_time;
}

void Game::legal(const unsigned int& lmove, double *_fromX, double *_fromY, double *_toX, double *_toY, std::string *_piece) {
  int type_c = MOVE_TYPE( lmove );
  int type   = type_c & ~mask_piece_color;
  int from   = MOVE_FROM( lmove );
  int to     = MOVE_TO( lmove );
  int prom   = MOVE_PROMOTE( lmove );

  if( from == move_drop ) {
    *_fromX = 0;
    *_fromY = 0;
    *_toX = 5-( to%5 );
    *_toY = ( to/5 +1);
  } else {
    *_fromX = 5-( from%5 );
    *_fromY = ( from/5 +1);
    *_toX = 5-( to%5 );
    *_toY = ( to/5 +1);
  }

  if (prom) {
    *_piece = _ch_piece_csa[ type + m_promote ];
  }else {
    *_piece = _ch_piece_csa[ type ];
  }
}
