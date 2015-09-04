#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"


int main( int argc, char *argv[] )
{
  int ret;

  out(" %s  ver. %s  / revision %d\n\n", PROGRAM_NAME, VERSION, REVISION );
  is_log_output = 1;
  if (argc > 1) {
    if (strcmp(argv[1], "--no-logfile") == 0) {
      is_log_output = 0;
    }
  }

  if( starting_initialize() )
    { exit(1); }
  game_initialize();

  out_position();

  while( 1 )
    {
      ret = cmd_prompt();

      if( ret == -1 )
        close_program();
    }
}


void close_program()
{
  game_finalize();
  exit(0);
}
