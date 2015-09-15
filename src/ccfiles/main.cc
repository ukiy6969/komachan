#include "io.h"

void close_program(Game* game);

int main( int argc, char *argv[] )
{
  int ret;
  Game* game = new Game();
  Io* io = new Io(game);
  Board* board = io->get_board();

  board->starting_initialize();
  //if( io->get_board()->starting_initialize() )
  //  { exit(1); }
  game->game_initialize();

  io->out_position();

  while( 1 )
    {
      ret = io->cmd_prompt();

      if( ret == -1 )
        close_program(game);
    }
}

void close_program(Game* game)
{
  game->game_finalize();
  exit(0);
}
