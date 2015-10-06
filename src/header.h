#include <stdio.h>
#include "board.h"

#define PROGRAM_NAME         "Sample 5x5 Shogi Player"
#define VERSION              "0.1.0"
#define REVISION             2

#define SEARCH_DEPTH         7

#define SCORE_MAX            8100
#define SCORE_MATED          (-SCORE_MAX)

#define SIZE_FILENAME        64

extern const char *ch_piece[16];
extern const char *ch_piece2[32];
extern const char *ch_piece_csa[16];
extern FILE *file_log;
extern int is_log_output;
extern int server_mode;

/* main.c */
void close_program();

/* io.c */
int cmd_prompt();
void out( const char *format, ... );
void out_file( FILE *fp, const char *format, ... );
void out_position();
void out_board();
void out_legalmoves( unsigned int moves[], int count );
void str_CSA_move( char *buf, unsigned int move );
unsigned int  CSA2Internal( const char *str );
int out_record( int resign );
void out_server();
void out_move_string(char* str);

/* ini.c */
void game_initialize();
void game_finalize();

/* search.c */
int search_root(double *time);
void move_ordering(short *evals, int *legal_moves, int n);
int search( short alpha, short beta, int depth, int ply);
short evaluate();
