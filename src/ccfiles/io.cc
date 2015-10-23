#include "io.h"
#include <iostream>

#define DELIM   " "

const char *ch_piece[16] =
{ "--", "歩", "--", "--", "銀", "金", "角", "飛",
  "王", "と", "--", "--", "全", "--", "馬", "竜" };

const char *ch_piece2[32] =
{ " --", "^歩", " --", " --", "^銀", "^金", "^角", "^飛",
  "^王", "^と", " --", " --", "^全", " --", "^馬", "^竜",
  " --", "v歩", " --", " --", "v銀", "v金", "v角", "v飛",
  "v王", "vと", " --", " --", "v全", " --", "v馬", "v竜" };

const char *ch_piece_csa[16] =
{ "--", "FU", "--", "--", "GI", "KI", "KA", "HI",
  "OU", "TO", "--", "--", "NG", "--", "UM", "RY" };

Io::Io(Game* g){
  int i,iret;
  server_mode = 0;
  is_log_output = 1;
  game = g;
  board = game->get_board();
  search = game->get_search();

  char str_file[ SIZE_FILENAME ];
  for ( i = 0; i < 1000; i++ )
    {
      snprintf( str_file, SIZE_FILENAME, "log/game%03d.log", i );
      file_log = fopen( str_file, "r" );
      if ( file_log == NULL ) { break; }
      iret = fclose( file_log );
      if ( iret < 0 )
        {
          file_log = NULL;
          i = -1;
          break;
        }
    }

  if( i >= 0 && is_log_output)
    { file_log = fopen( str_file, "w" ); }
}

Board* Io::get_board(){
  return board;
}

int Io::cmd_prompt(){
  /*
    ret = 0: continue
         -1: exit
    */
  char buf[256];
  const char *token;
  char *last;
  int count_byte, ret;

  if( board->get_turn() && !server_mode)
    {
      out("Black %d> ", board->get_nply() +1 );
    }
  else if(!server_mode)
    {
      out("White %d> ", board->get_nply() +1 );
    }

  memset( buf, 0, sizeof(buf) );
  fgets( buf, sizeof( buf ), stdin );
  count_byte = strlen( buf );
  if( !strcmp( buf, "\n") )
    { return 0; }

  buf[count_byte-1] = '\0';

  token = strtok_r( buf, DELIM, &last );
  if( token == NULL )
    { return 0; }

  if( !strcmp( token, "quit" ) )
    { return -1; }
  else if( !strcmp( token, "board" ) )
    { out_position(); }
  else if( !strcmp( token, "back" ) )
    { back(); }
  else if( !strcmp( token, "move" ) )
    { manual_move( &last ); }
  else if( !server_mode && !strcmp( token, "search" ) || !strcmp( token, "s" ) )
    { search_start(); }
  else if( !strcmp( token, "new" )  )
    { new_game(); }
  else if( !strcmp( token, "record" )  )
    { out_record( 1 ); }
  else if( server_mode && !strcmp(token, "search") )
    {
      search_start();
      out_server();
    }
  else
    { ret = -1; }

  if( ret == -1 )
    out(" Invalid command\n");

  return 0;
}

int Io::out_record( int resign )
{
  /* ret = 0; succeeded
          -1; failed    */
  FILE *fp;
  int i;
  char str[8], filename[ SIZE_FILENAME];
  struct tm *date;
  time_t timer;
  time( &timer );

  date = localtime(&timer);

  sprintf(filename, "records/%04d_%02d_%02d_%02d%02d%02d.msk",
           date->tm_year +1900, date->tm_mon +1, date->tm_mday,
           date->tm_hour      , date->tm_min,    date->tm_sec);

  fp = fopen(filename, "w");
  fp = fopen(filename, "w");

  if( fp == NULL )
    {
      out(" Error: Cannot create %s\n", filename);
      return -1;
    }

  out_file( fp, "\nV1_MSK\n\n" );
  out_file( fp, "\nPI\n+\n");


  for( i = 0; i < board->get_nply(); i++ )
    {
      str_CSA_move( str, board->history[ i ].move );
      if( i % 2 )
        { out_file( fp, "-" ); }
      else
        { out_file( fp, "+" ); }
      out_file( fp, "%s\n", str );
    }

  if( resign )
    out_file( fp, "%%TORYO\n" );

  fclose( fp );

  out( " %s has been output.\n", filename );
  return 0;
}

void Io::new_game()
{
  game->game_initialize();
  out_position();
}

void Io::search_start()
{
  int iret;
  iret = search->search_root();
  if( iret == -2 )
    {
      out( " This game was already concluded.\n");
      return;
    }
  else if( iret == -1 )
    {
      out( " Search() failed.\n" );
      return;
    }
  out_position();
  return;
}

void Io::back()
{
  if( board->get_nply() > 0 )
    {
      board->unmake_move();
      out_position();
    }
  else
    {
      out(" This is the starting position.\n");
    }
}

void Io::manual_move( char **last )
{
  const char *p = strtok_r( NULL, DELIM, last );
  unsigned int move;

  move = CSA2Internal( p );
  if( move == MOVE_NULL )
    {
      out(" Invalid Move\n");
      return;
    }

  char str_move[8];
  str_CSA_move( str_move, move );
  out("%s\n", str_move);
  board->make_move( move );
  out_position();

}

int Io::manual_move_str( char *str){
  unsigned int move;
  move = CSA2Internal(str);
  if( move == MOVE_NULL )
    {
      return -1;
    }
  board->make_move( move );
  return 0;
}

void Io::out_position()
{
  if(server_mode) {
    return;
  }
  out("\n");
  out_board();
  out(" Root position evaluation = %d\n", search->evaluate() );

  unsigned int moves[256];
  int nmove;
  nmove = board->gen_legalmoves( moves );
  out_legalmoves( moves, nmove );
/*
#ifndef _MSC_VER
  // TODO:
  assert( isCorrect_Occupied() );
#endif
*/
  return;
}

void Io::out_legalmoves( unsigned int moves[], int count )
{
  int i;
  char buf[8];

  out(" Legal Moves( %d ) =\n ", count);

  for( i=0; i < count; i++)
    {
      str_CSA_move( buf, moves[ i ] );
      out(" %s",buf);
      if( i%10 == 9 )
        out("\n ");
    }
  out("\n");
  return;
}

void Io::str_CSA_move( char *str, unsigned int move )
{
  int type_c = MOVE_TYPE( move );
  int type   = type_c & ~mask_piece_color;
  int from   = MOVE_FROM( move );
  int to     = MOVE_TO( move );
  int prom   = MOVE_PROMOTE( move );

  if( from == move_drop )
    {
      snprintf( str, 7, "%d%d%d%d%s",
                0, 0,
                5-( to%5),    ( to/5   +1 ),
                ch_piece_csa[ type ] );
    }
  else if( prom )
    {
      snprintf( str, 7, "%d%d%d%d%s",
                5-( from%5 ), ( from/5 +1 ),
                5-( to%5),    ( to/5   +1 ),
                ch_piece_csa[ type + m_promote ] );
    }
  else
    {
      snprintf( str, 7, "%d%d%d%d%s",
                5-( from%5 ), ( from/5 +1 ),
                5-( to%5),    ( to/5   +1 ),
                ch_piece_csa[ type ] );
    }
  return;
}

unsigned int Io::CSA2Internal( const char *str )
{
  /* return = MOVE_NULL : illegal
              others    : a move ( validity is not guaranteed ) */
  int move;
  int from, to, type, type_c, prom, cap, cap_type;
  int fromX, fromY, toX, toY, i;
  const char *buf;

  if( str == NULL || strlen( str ) != 6 )
    { return MOVE_NULL; }

  fromX = str[0] -'0';
  fromY = str[1] -'0';
  toX   = str[2] -'0';
  toY   = str[3] -'0';
  buf   = str + 4;

  from = 5 - fromX + ( fromY -1 )*5;
  to   = 5 - toX   + ( toY   -1 )*5;

  if( fromX == 0 && fromY == 0 )
    {
      from = move_drop;
      for( i=0; i < 16; i++ )
        {
          if( !strcmp( buf, ch_piece_csa[ i ] ) )
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

      if( !strcmp( buf, ch_piece_csa[ 0 ] ) )
        { return MOVE_NULL; }
      if( ( type < m_promote ) && !strcmp( buf, ch_piece_csa[ type + m_promote ] ) )
        { prom = 1; }
      else if( !strcmp( buf, ch_piece_csa[ type ] ) )
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
  return move;
}

void Io::out( const char *format, ... )
{
  va_list arg;

  va_start( arg, format );
  vprintf( format, arg );
  va_end( arg );
  fflush( stdout);

  if ( ( strchr( format, '\n' ) != NULL || strchr( format, '\r' ) == NULL )
       && file_log != NULL  && is_log_output)
    {
      va_start( arg, format );
      vfprintf( file_log, format, arg );
      va_end( arg );
      fflush( file_log );
    }

  return;
}

void Io::out_file( FILE *fp, const char *format, ... )
{
  va_list arg;

  va_start( arg, format );
  vfprintf( fp, format, arg );
  va_end( arg );
  fflush( fp );

  return;
}

void Io::out_board(){
  int i, j, index, type;

  if (server_mode) {
    out_server();
    return;
  }

  if( board->get_turn() )
    {
      out("[ %d 手目 後手 ]\n", board->get_nply() +1 );
    }
  else
    {
      out("[ %d 手目 先手 ]\n", board->get_nply() +1 );
    }

  out(" 後手持駒= ");
  for( i=0; i < 8; i++)
    {
      if( board->b_hand(i) > 0 )
        out(" %s%d", ch_piece[i], board->b_hand(i) );
    }
  out("\n");

  out("   5   4   3   2   1\n");
  for( i=0; i<5; i++ )
    {
      out(" ---------------------\n");
      out(" |");
    for( j=0; j<5; j++ )
      {
        index = i*5 + j;
        if( board->Occupied0 & Bit( index ) )
          {
            if( (type = board->get_piece_on_sq_w( index )) != no_piece )
              out("%s|", ch_piece2[ type ] );
            else if( (type = board->get_piece_on_sq_b( index )) != no_piece )
              out("%s|", ch_piece2[ type ] );
          }
        else
          { out("   |"); continue; }
      }
      switch(i)
        {
        case 0:
          out(" 一\n");
          break;
        case 1:
          out(" 二\n");
          break;
        case 2:
          out(" 三\n");
          break;
        case 3:
          out(" 四\n");
          break;
        case 4:
          out(" 五\n");
          break;
        }
    }
  out(" ---------------------\n");

  out(" 先手持駒= ");
  for( i=0; i < 8; i++)
    {
      if( board->w_hand(i) > 0 )
        out(" %s%d", ch_piece[i], board->w_hand(i) );
    }
  out("\n\n");

  return;
}

void Io::out_server(){
  char str[8];
  str_CSA_move( str, board->history[ board->get_nply() - 1].move );
  out("%s\n", str);
  return;
}

void Io::out_move_string(char* str){
  str_CSA_move( str, board->history[ board->get_nply() - 1].move );
}
