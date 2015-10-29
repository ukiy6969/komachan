#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"

int is_log_output; // is output logfile flag
int server_mode;  // server_mode


int main( int argc, char *argv[] )
{
  int ret,i;
  char* bin_path = "BB_Attack.bin";
  char* zobrist_path = "Zobrist_rand.bin";

  is_log_output = 1;
  if (argc > 1) {
    for(i=1; i<argc; i++){
      if (strcmp(argv[i], "--no-logfile") == 0) {
        is_log_output = 0;
      }
      if (strcmp(argv[i], "--server-mode") == 0){
        is_log_output = 0;
        server_mode = 1;
      }
      if (strstr(argv[i], "--bin-path=") != NULL){
        strtok(argv[i], "=");
        bin_path = strtok(NULL, "");
      }
      if (strstr(argv[i], "--zobrist-path=") != NULL){
        strtok(argv[i], "=");
        zobrist_path = strtok(NULL, "");
      }
    }
  }

  if (!server_mode) {
    out(" %s  ver. %s  / revision %d\n\n", PROGRAM_NAME, VERSION, REVISION );
  }

  if( starting_initialize(bin_path) )
    { exit(1); }
  game_initialize();
  zobrist_init(zobrist_path);

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
