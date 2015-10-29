#include "io.h"

void close_program(Game* game);

int main( int argc, char *argv[] )
{
  int ret;
  char* bin_path;
  Game* game = new Game();
  Io* io = new Io(game);
  Board* board = io->get_board();

  if (argc > 1) {
    for(int i=1; i<argc; i++){
      if (strcmp(argv[i], "--no-logfile") == 0) {
      }
      if (strcmp(argv[i], "--server-mode") == 0){
      }
      if (strstr(argv[i], "--bin-path=") != NULL){
        strtok(argv[i], "=");
        bin_path = strtok(NULL, "");
      }
    }
  }

  if (bin_path == NULL) {
    bin_path = "./setting";
  }

  std::string binPath(bin_path);

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
