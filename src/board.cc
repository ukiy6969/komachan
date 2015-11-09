#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "board.h"
#include <random>

const int Board::Attack_Rook_shift[] = {
  19, 20, 20, 20, 19,
  20, 21, 21, 21, 20,
  20, 21, 21, 21, 20,
  20, 21, 21, 21, 20,
  19, 20, 20, 20, 19
};

const int Board::Attack_Bishop_shift[] = {
  22, 23, 23, 23, 22,
  23, 23, 23, 23, 23,
  23, 23, 21, 23, 23,
  23, 23, 23, 23, 23,
  22, 23, 23, 23, 22
};

Board::Board(){
}

Board::~Board() {
}

#define FILE_CREATE( variable, size ) \
  if( fwrite( variable, sizeof( unsigned long long ), size, fp) != size ){ \
    printf(" fwrite \"%s\" failed.\n", #variable); \
    fclose( fp ); return -1; }\

int Board::zobrist_create(std::string zobrist_name) {
  FILE *fp;
  int i, j, k;
  std::random_device rd;
  std::mt19937_64 mt(rd());

  if ( (fp = fopen(zobrist_name.c_str(), "wb")) == NULL ) {
    return -1;
  }

  for(i = 0; i < 32; i++) {
    for(j = 0; j < 32; j++) {
      PIECE_INFO_RAND[i][j] = mt();
      FILE_CREATE( &PIECE_INFO_RAND[i][j] , 1);
    }
  }

  for(i = 0; i < 2; i++) {
    for(j = 0; j < 4; j++) {
      for (k = 0; k < 8; k++){
        HAND_INFO_RAND[i][j][k] = mt();
        FILE_CREATE( &HAND_INFO_RAND[i][j][k] , 1);
      }
    }
  }

  for(i = 0; i < 2; i++) {
    TURN_RAND[i] = mt();
    FILE_CREATE( &TURN_RAND[i] , 1);
  }

  return fclose(fp) == 0;
}

#undef FILE_CREATE

#define FILE_READ( variable, size ) \
  if( fread( variable, sizeof( unsigned long long ), size, fp) != size ){ \
    printf(" fwrite \"%s\" failed.\n", #variable); \
    fclose( fp ); return -1; }\

int Board::zobrist_init(std::string binPath) {
  /** int long bit **/
  FILE *fp;
  int ret;
  int i, j;
  std::string bp(binPath);
  bp.append("/Zobrist_rand.bin");

  if ( (fp = fopen(bp.c_str(), "rb")) == NULL) {
    ret = zobrist_create(bp);
    if (!ret) { return -1; }
    fp = fopen(bp.c_str(), "rb");
  }

  for(i = 0; i < 32; i++) {
    FILE_READ( PIECE_INFO_RAND[i] , 32);
  }
  for(i = 0; i < 2; i++) {
    for(j = 0; j < 4; j++){
      FILE_READ( HAND_INFO_RAND[i][j] , 8);
    }
  }
  for(i = 0; i < 2; i++) {
    FILE_READ( &TURN_RAND[i] , 1);
  }
  return fclose(fp) == 0;
}

#undef FILE_READ

int Board::zobrist_check() {
  int i,j,k;
  int a,b,c;
  int checkCnt;
  unsigned long long check;
  for(i = 0; i < 32; i++) {
    for(j = 0; j < 32; j++) {
      check = PIECE_INFO_RAND[i][j];
      checkCnt = 0;
      for(a = 0; a < 32; a++) {
        for(b = 0; b < 32; b++) {
          if (check == PIECE_INFO_RAND[a][b]) {
            checkCnt++;
          }
        }
      }
      for(a = 0; a < 2; a++) {
        for(b = 0; b < 4; b++) {
          for (c = 0; c < 8; c++){
            if (check == HAND_INFO_RAND[a][b][c]) {
              checkCnt++;
            }
          }
        }
      }
      for(a = 0; a < 2; a++) {
        if (check == TURN_RAND[a]) {
          checkCnt++;
        }
      }
      if (checkCnt > 1) {
        return 0;
      }
    }
  }

  for(i = 0; i < 2; i++) {
    for(j = 0; j < 4; j++) {
      for (k = 0; k < 8; k++){
        check = HAND_INFO_RAND[i][j][k];
        checkCnt = 0;
        for(a = 0; a < 32; a++) {
          for(b = 0; b < 32; b++) {
            if (check == PIECE_INFO_RAND[a][b]) {
              checkCnt++;
            }
          }
        }
        for(a = 0; a < 2; a++) {
          for(b = 0; b < 4; b++) {
            for (c = 0; c < 8; c++){
              if (check == HAND_INFO_RAND[a][b][c]) {
                checkCnt++;
              }
            }
          }
        }
        for(a = 0; a < 2; a++) {
          if (check == TURN_RAND[a]) {
            checkCnt++;
          }
        }
        if (checkCnt > 1) {
          return 0;
        }
      }
    }
  }

  for(i = 0; i < 2; i++) {
    check = TURN_RAND[i];
    checkCnt = 0;
    for(a = 0; a < 32; a++) {
      for(b = 0; b < 32; b++) {
        if (check == PIECE_INFO_RAND[a][b]) {
          checkCnt++;
        }
      }
    }
    for(a = 0; a < 2; a++) {
      for(b = 0; b < 4; b++) {
        for (c = 0; c < 8; c++){
          if (check == HAND_INFO_RAND[a][b][c]) {
            checkCnt++;
          }
        }
      }
    }
    for(a = 0; a < 2; a++) {
      if (check == TURN_RAND[a]) {
        checkCnt++;
      }
    }
    if (checkCnt > 1) {
      return 0;
    }
  }
  return 1;

}

void Board::printZobristHashed() {
  int i,j,k;

  for(i = 0; i < 32; i++) {
    for(j = 0; j < 32; j++) {
      printf("PIECE_INFO_RAND[%d][%d] = %llu\n",i, j, PIECE_INFO_RAND[i][j]);
    }
  }

  for(i = 0; i < 2; i++) {
    for(j = 0; j < 4; j++) {
      for (k = 0; k < 8; k++){
        printf("HAND_INFO_RAND[%d][%d][%d] = %llu\n", i, j,k, HAND_INFO_RAND[i][j][k]);
      }
    }
  }

  for(i = 0; i < 2; i++) {
    printf("TURN_RAND[%d] = %llu\n",i, TURN_RAND[i]);
  }
}

inline void Board::update_zobrist(unsigned long long u){
  ZOBRIST ^= u;
}

unsigned long long Board::get_zobrist() {
  int index = 0, type = 0;

  unsigned long long res = TURN_RAND[(int) TURN ];

  for( int i=0; i<5; i++ ) {
    for( int j=0; j<5; j++ ) {

      index = i*5 + j;
      if( (type = get_piece_on_sq_w( index )) != no_piece ) {
        res ^= PIECE_INFO_RAND[ type ][ index];
      }
      else if( (type = get_piece_on_sq_b( index )) != no_piece ) {
        res ^= PIECE_INFO_RAND[ type ][ index];
      }

    }
  }

  for( int i=0; i<8; i++) {
    res ^= HAND_INFO_RAND[white][W_HAND(i)][i];
    res ^= HAND_INFO_RAND[black][B_HAND(i)][i];
  }

  return res;
}

int Board::is_attack(unsigned int *attack_pieces) {
  int nmove = 0;
  if( TURN ){
    nmove = attacks_to_w( SQ_B_KING, attack_pieces);
  } else {
    nmove = attacks_to_b( SQ_W_KING, attack_pieces);
  }
  return nmove;
}


int Board::gen_legalmoves( unsigned int moves[] )
{
  unsigned int pin[32], attack_on_king;
  unsigned int att;
  int nmove = 0;

  if( TURN )
    {
      pinInfo_b( pin );
      attack_on_king = attacks_to_w( SQ_B_KING, &att );
      if( attack_on_king > 0 )
        {
          nmove = gen_evasion_b( moves, 0, attack_on_king, att, pin );
        }
      else
        {
          nmove = gen_cap_b( moves, nmove, pin );
          nmove = gen_drop_b( moves, nmove );
          nmove = gen_nocap_b( moves, nmove, pin );
        }
    }
  else
    {
      pinInfo_w( pin );
      attack_on_king = attacks_to_b( SQ_W_KING, &att );
      if( attack_on_king > 0 )
        {
          nmove = gen_evasion_w( moves, 0, attack_on_king, att, pin );
        }
      else{
        nmove = gen_cap_w( moves, nmove, pin );
        nmove = gen_drop_w( moves, nmove );
        nmove = gen_nocap_w( moves, nmove, pin );
      }
    }

  return nmove;
}

inline int Board::gen_evasion_w( unsigned int moves[], int count, int nAttacks,
                   unsigned int attack_pieces, unsigned int pin[]  )
{
  unsigned int dests, att;
  int to, cap, index;

  dests = Attack_King[ SQ_W_KING ] & nOccupiedW;

  Occupied0   ^= Bit( SQ_W_KING );      /* assume that the king is absent */
  while( (to = FirstOne( dests )) != -1 )
    {
      if( attacks_to_b( to, &att ) == 0 )
        {
          cap = ( Bit( to ) & (~nOccupiedB) ) ? 1 : 0 ;
          moves[ count ] = MOVE( w_king, SQ_W_KING, to, 0, cap );
          count ++;
        }
      dests ^= Bit( to );
    }
  Occupied0   ^= Bit( SQ_W_KING );

  if( nAttacks >= 2 )
    { return count; }


  int sq = FirstOne( attack_pieces );
  assert( sq >= 0 );
  count = gen_attacks_to_w( moves, count, sq, 1, pin );


#define DROP( piece ) \
  if( IsHand_W( piece ) ){ \
    moves[ count ] = MOVE( w_ ## piece, move_drop, to, 0, 0 ); \
    count ++;}

  if( attack_pieces & ( Attack_King[ SQ_W_KING ] ) )
    {
      /* nothing to do */
    }
  else if( attack_pieces & ( BB_B_ROOK | BB_B_DRAGON ) )
    {
      index  = (( (Occupied0 & Attack_Rook_mask[sq]) * Attack_Rook_magic[sq])
               >> Attack_Rook_shift[ sq ]) & mask_magic;
      dests  = Attack_Rook[ sq ][ index ];

      index  = (( (Occupied0 & Attack_Rook_mask[ SQ_W_KING ])
                * Attack_Rook_magic[ SQ_W_KING ])
               >> Attack_Rook_shift[ SQ_W_KING ]) & mask_magic;
      dests &= Attack_Rook[ SQ_W_KING ][ index ];

      assert( dests );

      while( (to = FirstOne( dests )) != -1 )
        {
          if( W_HAND_A )
            {
              if( to >= 5 && IsHand_W( pawn ) )
                {
                  int dp = FirstOne( BB_W_PAWN );
                  if( dp < 0 || Bit(to) & ~DoublePawn[ dp ] )
                    {
                      moves[ count ] = MOVE( w_pawn, move_drop, to, 0, 0 );
                      count ++;
                    }
                }
              DROP( silver );
              DROP( gold );
              DROP( bishop );
              DROP( rook );
            }

          count = gen_attacks_to_w( moves, count, to, 0, pin );

          dests ^= Bit( to );
        }
    }
  else if( attack_pieces & ( BB_B_BISHOP | BB_B_HORSE ) )
    {
      index  = (( (Occupied0 & Attack_Bishop_mask[sq])
                  * Attack_Bishop_magic[sq])
                >> Attack_Bishop_shift[ sq ]) & mask_magic;
      dests  = Attack_Bishop[ sq ][ index ];

      index  = (( (Occupied0 & Attack_Bishop_mask[ SQ_W_KING ])
                * Attack_Bishop_magic[ SQ_W_KING ])
               >> Attack_Bishop_shift[ SQ_W_KING ]) & mask_magic;
      dests &= Attack_Bishop[ SQ_W_KING ][ index ];

      assert( dests );

      while( (to = FirstOne( dests )) != -1 )
        {
          if( W_HAND_A )
            {
              if( to >= 5 && IsHand_W( pawn ) )
                {
                  int dp = FirstOne( BB_W_PAWN );
                  if( dp < 0 || Bit(to) & ~DoublePawn[ dp ] )
                    {
                      moves[ count ] = MOVE( w_pawn, move_drop, to, 0, 0 );
                      count ++;
                    }
                }
              DROP( silver );
              DROP( gold );
              DROP( bishop );
              DROP( rook );
            }

          count = gen_attacks_to_w( moves, count, to, 0, pin );

          dests ^= Bit( to );
        }
    }

#undef DROP

  return count;
}

inline int Board::gen_evasion_b( unsigned int moves[], int count, int nAttacks,
                   unsigned int attack_pieces, unsigned int pin[] )
{
  unsigned int dests, att;
  int to, cap, index;

  dests = Attack_King[ SQ_B_KING ] & nOccupiedB;

  Occupied0   ^= Bit( SQ_B_KING );      /* assume that the king is absent */
  while( (to = FirstOne( dests )) != -1 )
    {
      if( attacks_to_w( to, &att ) == 0 )
        {
          cap = ( Bit( to ) & (~nOccupiedW) ) ? 1 : 0 ;
          moves[ count ] = MOVE( b_king, SQ_B_KING, to, 0, cap );
          count ++;
        }
      dests ^= Bit( to );
    }
  Occupied0   ^= Bit( SQ_B_KING );

  if( nAttacks >= 2 )
    { return count; }


  int sq = FirstOne( attack_pieces );
  assert( sq >= 0 );
  count = gen_attacks_to_b( moves, count, sq, 1, pin );


#define DROP( piece ) \
  if( IsHand_B( piece ) ){ \
    moves[ count ] = MOVE( b_ ## piece, move_drop, to, 0, 0 ); \
    count ++;}

  if( attack_pieces & ( Attack_King[ SQ_B_KING ] ) )
    {
      /* nothing to do */
    }
  else if( attack_pieces & ( BB_W_ROOK | BB_W_DRAGON ) )
    {
      index  = (( (Occupied0 & Attack_Rook_mask[sq]) * Attack_Rook_magic[sq])
               >> Attack_Rook_shift[ sq ]) & mask_magic;
      dests  = Attack_Rook[ sq ][ index ];

      index  = (( (Occupied0 & Attack_Rook_mask[ SQ_B_KING])
                * Attack_Rook_magic[ SQ_B_KING ])
               >> Attack_Rook_shift[ SQ_B_KING ]) & mask_magic;
      dests &= Attack_Rook[ SQ_B_KING ][ index ];

      assert( dests );

      while( (to = FirstOne( dests )) != -1 )
        {
          if( B_HAND_A )
            {
              if( to < 20 && IsHand_B( pawn ) )
                {
                  int dp = FirstOne( BB_B_PAWN );
                  if( dp < 0 || Bit(to) & ~DoublePawn[ dp ] )
                    {
                      moves[ count ] = MOVE( b_pawn, move_drop, to, 0, 0 );
                      count ++;
                    }
                }
              DROP( silver );
              DROP( gold );
              DROP( bishop );
              DROP( rook );
            }

          count = gen_attacks_to_b( moves, count, to, 0, pin );

          dests ^= Bit( to );
        }
    }
  else if( attack_pieces & ( BB_W_BISHOP | BB_W_HORSE ) )
    {
      index  = (( (Occupied0 & Attack_Bishop_mask[sq])
                  * Attack_Bishop_magic[sq])
                >> Attack_Bishop_shift[ sq ]) & mask_magic;
      dests  = Attack_Bishop[ sq ][ index ];

      index  = (( (Occupied0 & Attack_Bishop_mask[ SQ_B_KING ])
                * Attack_Bishop_magic[ SQ_B_KING ])
               >> Attack_Bishop_shift[ SQ_B_KING ]) & mask_magic;
      dests &= Attack_Bishop[ SQ_B_KING ][ index ];

      assert( dests );

      while( (to = FirstOne( dests )) != -1 )
        {
          if( B_HAND_A )
            {
              if( to < 20 && IsHand_B( pawn ) )
                {
                  int dp = FirstOne( BB_B_PAWN );
                  if( dp < 0 || Bit(to) & ~DoublePawn[ dp ] )
                    {
                      moves[ count ] = MOVE( b_pawn, move_drop, to, 0, 0 );
                      count ++;
                    }
                }
              DROP( silver );
              DROP( gold );
              DROP( bishop );
              DROP( rook );
            }

          count = gen_attacks_to_b( moves, count, to, 0, pin );

          dests ^= Bit( to );
        }
    }

#undef DROP

  return count;
}

inline int Board::mate_by_dropping_pawn_w( int sq )
{
 /* ret = 1: illegal
        = 0: legal   */
  unsigned int dests, att;
  int to;
  unsigned int moves[ SIZE_LEGALMOVES ];

  dests = Attack_King[ SQ_B_KING ] & nOccupiedB;

  /* not necessary to assume king's absence 'cause the king cannot
     be attacked by sliding piece in this case.                 */

  Occupied0   ^= Bit( sq );
  while( (to = FirstOne( dests )) != -1 )
    {
      if( attacks_to_w( to, &att ) == 0 )
        {
          Occupied0   ^= Bit( sq );
          return 0;
        }
      dests ^= Bit( to );
    }
  Occupied0   ^= Bit( sq );

  unsigned int pin[32];
  pinInfo_b( pin );
  if( gen_attacks_to_b( moves, 0, sq, 0, pin ) )
    { return 0; }

  return 1;
}

inline int Board::mate_by_dropping_pawn_b( int sq )
{
 /* ret = 1: illegal
        = 0: legal   */
  unsigned int dests, att;
  int to;
  unsigned int moves[ SIZE_LEGALMOVES ];

  dests = Attack_King[ SQ_W_KING ] & nOccupiedW;

  /* not necessary to assume king's absence 'cause the king cannot
     be attacked by sliding piece in this case.                 */

  Occupied0   ^= Bit( sq );
  while( (to = FirstOne( dests )) != -1 )
    {
      if( attacks_to_b( to, &att ) == 0 )
        {
          Occupied0   ^= Bit( sq );
          return 0;
        }
      dests ^= Bit( to );
    }
  Occupied0   ^= Bit( sq );


  unsigned int pin[32];
  pinInfo_w( pin );
  if( gen_attacks_to_w( moves, 0, sq, 0, pin ) )
    { return 0; }

  return 1;
}

#define GEN_ATTACK( piece, promote ) \
  while( ( from = FirstOne( dests ) ) != -1){ \
    int prom = 0; \
    if( ~pin[ from ] & Bit( sq ) ){ \
      if( promote && (from < 5 || sq < 5) ){ prom = 1; } \
      moves[ count ] = MOVE( piece, from, sq, prom, cap ); \
      count ++; } \
    dests ^= Bit( from ); }

inline int Board::gen_attacks_to_w( unsigned int moves[], int count, int sq, int cap, unsigned int pin[] )
{
  /* King is not considered. */
  unsigned int dests;
  int from, index;

  if( (dests = (Attack_BPawn[ sq ] & BB_W_PAWN)) != 0 )
    {
      from = FirstOne( dests );
      if( ~pin[ from ] & Bit( sq ) )
        {
          if( sq < 5 )
            {
              moves[ count ] = MOVE( w_pawn, from, sq, 1, cap );
            }
          else
            {
              moves[ count ] = MOVE( w_pawn, from, sq, 0, cap );
            }
          count ++;
        }
    }

  dests  = Attack_BSilver[ sq ] & BB_W_SILVER;
  while( ( from = FirstOne( dests ) ) != -1)
    {
      if( ~pin[ from ] & Bit( sq ) ){
        if( from < 5 || sq < 5 )
          { moves[ count ] = MOVE( w_silver, from, sq, 1, cap );
            count ++; }
        moves[ count ] = MOVE( w_silver, from, sq, 0, cap );
        count ++; }
      dests ^= Bit( from );
    }

  dests  = Attack_BGold[ sq ] & BB_W_GOLD;
  GEN_ATTACK( w_gold, 0 );

  dests  = Attack_BGold[ sq ] & BB_W_PRO_SILVER;
  GEN_ATTACK( w_pro_silver, 0 );

  dests  = Attack_BGold[ sq ] & BB_W_PRO_PAWN;
  GEN_ATTACK( w_pro_pawn, 0 );

  dests  = Attack_King[ sq ];
  index =(((Occupied0 & Attack_Bishop_mask[sq])
           * Attack_Bishop_magic[sq])
          >> Attack_Bishop_shift[sq]) & mask_magic;
  dests |= Attack_Bishop[ sq ][ index ];
  dests &= BB_W_HORSE;
  GEN_ATTACK( w_horse, 0 );

  dests  = Attack_King[ sq ];
  index  = (( (Occupied0 & Attack_Rook_mask[sq]) * Attack_Rook_magic[sq])
            >> Attack_Rook_shift[ sq ]) & mask_magic;
  dests |= Attack_Rook[ sq ][ index ];
  dests &= BB_W_DRAGON;
  GEN_ATTACK( w_dragon, 0 );


  index =(((Occupied0 & Attack_Bishop_mask[sq])
           * Attack_Bishop_magic[sq])
          >> Attack_Bishop_shift[sq]) & mask_magic;
  dests = Attack_Bishop[ sq ][ index ];
  dests &= BB_W_BISHOP;
  GEN_ATTACK( w_bishop, 1 );

  index  = (( (Occupied0 & Attack_Rook_mask[sq]) * Attack_Rook_magic[sq])
            >> Attack_Rook_shift[ sq ]) & mask_magic;
  dests  = Attack_Rook[ sq ][ index ];
  dests &= BB_W_ROOK;
  GEN_ATTACK( w_rook, 1 );

  return count;
}
#undef GEN_ATTACK

#define GEN_ATTACK( piece, promote ) \
  while( ( from = FirstOne( dests ) ) != -1){ \
    int prom = 0; \
    if( ~pin[ from ] & Bit( sq ) ){ \
      if( promote && (from > 19 || sq > 19) ){ prom = 1; } \
      moves[ count ] = MOVE( piece, from, sq, prom, cap ); \
      count ++; } \
    dests ^= Bit( from ); }

inline int Board::gen_attacks_to_b( unsigned int moves[], int count, int sq, int cap, unsigned int pin[] )
{
  /* King is not considered. */
  unsigned int dests;
  int from, index;

  if( (dests = (Attack_WPawn[ sq ] & BB_B_PAWN)) != 0 )
    {
      from = FirstOne( dests );
      if( ~pin[ from ] & Bit( sq ) )
        {
          if( sq > 19 )
            {
              moves[ count ] = MOVE( b_pawn, from, sq, 1, cap );
            }
          else
            {
              moves[ count ] = MOVE( b_pawn, from, sq, 0, cap );
            }
          count ++;
        }
    }

  dests  = Attack_WSilver[ sq ] & BB_B_SILVER;
  while( ( from = FirstOne( dests ) ) != -1)
    {
      if( ~pin[ from ] & Bit( sq ) ){
        if( from > 19 || sq > 19 )
          { moves[ count ] = MOVE( b_silver, from, sq, 1, cap );
            count ++; }
        moves[ count ] = MOVE( b_silver, from, sq, 0, cap );
        count ++; }
      dests ^= Bit( from );
    }

  dests  = Attack_WGold[ sq ] & BB_B_GOLD;
  GEN_ATTACK( b_gold, 0 );

  dests  = Attack_WGold[ sq ] & BB_B_PRO_SILVER;
  GEN_ATTACK( b_pro_silver, 0 );

  dests  = Attack_WGold[ sq ] & BB_B_PRO_PAWN;
  GEN_ATTACK( b_pro_pawn, 0 );

  dests  = Attack_King[ sq ];
  index =(((Occupied0 & Attack_Bishop_mask[sq])
           * Attack_Bishop_magic[sq])
          >> Attack_Bishop_shift[sq]) & mask_magic;
  dests |= Attack_Bishop[ sq ][ index ];
  dests &= BB_B_HORSE;
  GEN_ATTACK( b_horse, 0 );

  dests  = Attack_King[ sq ];
  index  = (( (Occupied0 & Attack_Rook_mask[sq]) * Attack_Rook_magic[sq])
            >> Attack_Rook_shift[ sq ]) & mask_magic;
  dests |= Attack_Rook[ sq ][ index ];
  dests &= BB_B_DRAGON;
  GEN_ATTACK( b_dragon, 0 );

  index =(((Occupied0 & Attack_Bishop_mask[sq])
           * Attack_Bishop_magic[sq])
          >> Attack_Bishop_shift[sq]) & mask_magic;
  dests = Attack_Bishop[ sq ][ index ];
  dests &= BB_B_BISHOP;
  GEN_ATTACK( b_bishop, 1 );

  index  = (( (Occupied0 & Attack_Rook_mask[sq]) * Attack_Rook_magic[sq])
            >> Attack_Rook_shift[ sq ]) & mask_magic;
  dests  = Attack_Rook[ sq ][ index ];
  dests &= BB_B_ROOK;
  GEN_ATTACK( b_rook, 1 );

  return count;
}

#undef GEN_ATTACK

int Board::attacks_to_w( int sq, unsigned int *attack_pieces )
{
  int count = 0, from, index;
  unsigned int att;

  att  = Attack_BPawn[ sq ] & BB_W_PAWN;
  att |= Attack_BSilver[ sq ] & BB_W_SILVER;
  att |= Attack_BGold[ sq ] &
    ( BB_W_GOLD | BB_W_PRO_PAWN | BB_W_PRO_SILVER );
  att |= Attack_King[ sq ] &
    ( BB_W_HORSE | BB_W_DRAGON | BB_W_KING );

  index = (( (Occupied0 & Attack_Rook_mask[sq]) * Attack_Rook_magic[sq])
            >> Attack_Rook_shift[ sq ]) & mask_magic;
  att |= Attack_Rook[ sq ][ index ] & ( BB_W_ROOK | BB_W_DRAGON );

  index = (( (Occupied0 & Attack_Bishop_mask[sq]) * Attack_Bishop_magic[sq])
            >> Attack_Bishop_shift[ sq ]) & mask_magic;
  att |= Attack_Bishop[ sq ][ index ] & ( BB_W_BISHOP | BB_W_HORSE );

  *attack_pieces = att;

  while( (from = FirstOne(att)) != -1 )
    {
      count ++;
      att ^= Bit( from );
    }

  return count;
}

int Board::attacks_to_b( int sq, unsigned int *attack_pieces )
{
  int count = 0, from;
  unsigned int att, index;

  att  = Attack_WPawn[ sq ] & BB_B_PAWN;
  att |= Attack_WSilver[ sq ] & BB_B_SILVER;
  att |= Attack_WGold[ sq ] &
    ( BB_B_GOLD | BB_B_PRO_PAWN | BB_B_PRO_SILVER );
  att |= Attack_King[ sq ] &
    ( BB_B_HORSE | BB_B_DRAGON | BB_B_KING  );

  index  = (( (Occupied0 & Attack_Rook_mask[sq]) * Attack_Rook_magic[sq])
            >> Attack_Rook_shift[ sq ]) & mask_magic;
  att |= Attack_Rook[ sq ][ index ] & ( BB_B_ROOK | BB_B_DRAGON );

  index = (( (Occupied0 & Attack_Bishop_mask[sq]) * Attack_Bishop_magic[sq])
            >> Attack_Bishop_shift[ sq ]) & mask_magic;
  att |= Attack_Bishop[ sq ][ index ] & ( BB_B_BISHOP | BB_B_HORSE );

  *attack_pieces = att;

  while( (from = FirstOne(att)) != -1 )
    {
      count ++;
      att ^= Bit( from );
    }

  return count;
}


inline void Board::pinInfo_w( unsigned int pin[] ){
  memset( pin, 0, sizeof(unsigned int)*32 );

  unsigned bb;
  int pIndex, sq = SQ_W_KING;
  int index;

  bb = ( (BB_B_ROOK | BB_B_DRAGON) ) & Attack_Rook[sq][0];
  while( (pIndex = FirstOne( bb )) != -1 )
    {
      index  = (( (Occupied0 & Attack_Rook_mask[ pIndex ])
                 * Attack_Rook_magic[pIndex])
                >> Attack_Rook_shift[pIndex]) & mask_magic;
      pin[ Pin_Rook[pIndex][sq][index][0] ]
        |= Pin_Rook[pIndex][sq][index][1];
      bb ^= Bit( pIndex );
    }

  bb = ( (BB_B_BISHOP | BB_B_HORSE) ) & Attack_Bishop[sq][0];
  while( (pIndex = FirstOne( bb )) != -1 )
    {
      index  = (( (Occupied0 & Attack_Bishop_mask[ pIndex ])
                 * Attack_Bishop_magic[pIndex])
                >> Attack_Bishop_shift[pIndex]) & mask_magic;
      pin[ Pin_Bishop[pIndex][sq][index][0] ]
        |= Pin_Bishop[pIndex][sq][index][1];
      bb ^= Bit( pIndex );
    }

  return;
}

inline void Board::pinInfo_b( unsigned int pin[] ){
  memset( pin, 0, sizeof(unsigned int)*32 );

  unsigned bb;
  int pIndex, sq = SQ_B_KING;
  int index;

  bb = ( (BB_W_ROOK | BB_W_DRAGON) ) & Attack_Rook[sq][0];
  while( (pIndex = FirstOne( bb )) != -1 )
    {
      index  = (( (Occupied0 & Attack_Rook_mask[ pIndex ])
                 * Attack_Rook_magic[pIndex])
                >> Attack_Rook_shift[pIndex]) & mask_magic;
      pin[ Pin_Rook[pIndex][sq][index][0] ]
        |= Pin_Rook[pIndex][sq][index][1];
      bb ^= Bit( pIndex );
    }

  bb = ( (BB_W_BISHOP | BB_W_HORSE) ) & Attack_Bishop[sq][0];
  while( (pIndex = FirstOne( bb )) != -1 )
    {
      index  = (( (Occupied0 & Attack_Bishop_mask[ pIndex ])
                 * Attack_Bishop_magic[pIndex])
                >> Attack_Bishop_shift[pIndex]) & mask_magic;
      pin[ Pin_Bishop[pIndex][sq][index][0] ]
        |= Pin_Bishop[pIndex][sq][index][1];
      bb ^= Bit( pIndex );
    }

  return;
}


inline int Board::gen_drop_w( unsigned int moves[], int count )
{
  unsigned long dests;
  int to;

  if( IsHand_W( gold ) )
    {
      dests = ~Occupied0 & bb_mask;
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_gold, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }

  if( IsHand_W( silver ) )
    {
      dests = ~Occupied0 & bb_mask;
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_silver, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }

  if( IsHand_W( bishop) )
    {
      dests = ~Occupied0 & bb_mask;
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_bishop, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }

  if( IsHand_W( rook ) )
    {
      dests = ~Occupied0 & bb_mask;
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_rook, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }

  if( IsHand_W( pawn ) )
    {
      dests = ~Occupied0 & bb_mask & ~b1_1111;
      to = FirstOne( BB_W_PAWN );
      if( to >= 0 )
        { dests &= ~DoublePawn[ to ]; }

      while( (to = FirstOne(dests)) != -1 )
        {
          if( to == SQ_B_KING + 5 &&  /* test "mate by dropping pawn" */
              mate_by_dropping_pawn_w( to ) )
            {
              dests ^= Bit( to );
              continue;
            }
          moves[ count ] = MOVE( w_pawn, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }

  return count;
}

inline int Board::gen_drop_b( unsigned int moves[], int count )
{
  unsigned long dests;
  int to;

  if( IsHand_B( gold ) )
    {
      dests = ~Occupied0 & bb_mask;
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_gold, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }

  if( IsHand_B( silver ) > 0 )
    {
      dests = ~Occupied0 & bb_mask;
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_silver, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }


  if( IsHand_B( bishop ) )
    {
      dests = ~Occupied0 & bb_mask;
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_bishop, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }

  if( IsHand_B( rook ) )
    {
      dests = ~Occupied0 & bb_mask;
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_rook, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }

  if( IsHand_B( pawn ) )
    {
      dests = ~Occupied0 & bb_mask & ~( b1_1111 << 20 );
      to = FirstOne( BB_B_PAWN );
      if( to >= 0 )
        { dests &= ~DoublePawn[ to ]; }

      while( (to = FirstOne(dests)) != -1 )
        {
          if( to == SQ_W_KING - 5 &&  /* test "mate by dropping pawn" */
              mate_by_dropping_pawn_b( to ) )
            {
              dests ^= Bit( to );
              continue;
            }
          moves[ count ] = MOVE( b_pawn, move_drop, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
    }

  return count;
}

inline int Board::gen_cap_w( unsigned int moves[], int count, unsigned int pin[] )
{
  /* ret: number of total moves */
  unsigned int dests, bb, att;
  int from, to, index;

  bb = BB_W_PAWN;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WPawn[ from ] & ~nOccupiedB;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( to < 5 )
            { moves[ count ] = MOVE( w_pawn, from, to, 1, 1 ); }
          else
            { moves[ count ] = MOVE( w_pawn, from, to, 0, 1 ); }
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_PRO_PAWN;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WGold[ from ] & ~nOccupiedB;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_pro_pawn, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_SILVER;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WSilver[ from ] & ~nOccupiedB;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from < 5 || to < 5 )/* inside of the opponent's territory */
            { moves[ count ] = MOVE( w_silver, from, to, 1, 1 );
              count ++;                                                     }
          moves[ count ] = MOVE( w_silver, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_PRO_SILVER;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WGold[ from ] & ~nOccupiedB;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_pro_silver, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_GOLD;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WGold[ from ] & ~nOccupiedB;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_gold, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_BISHOP;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Bishop_mask[from])
               * Attack_Bishop_magic[from])
              >> Attack_Bishop_shift[from]) & mask_magic;
      dests = Attack_Bishop[ from ][ index ];
      dests &= ~nOccupiedB & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from < 5 || to < 5){
            moves[ count ] = MOVE( w_bishop, from, to, 1, 1 );
            count ++;
          }
          else{
            moves[ count ] = MOVE( w_bishop, from, to, 0, 1 );
            count ++;
          }
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_ROOK;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Rook_mask[from]) * Attack_Rook_magic[from])
             >> Attack_Rook_shift[from]) & mask_magic;
      dests = Attack_Rook[ from ][ index ];
      dests &= ~nOccupiedB & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from < 5 || to < 5){
            moves[ count ] = MOVE( w_rook, from, to, 1, 1 );
            count ++;
          }
          else{
            moves[ count ] = MOVE( w_rook, from, to, 0, 1 );
            count ++;
          }
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_HORSE;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Bishop_mask[from])
               * Attack_Bishop_magic[from])
              >> Attack_Bishop_shift[from]) & mask_magic;
      dests = Attack_Bishop[ from ][ index ];
      dests |= Attack_King[ from ];
      dests &= ~nOccupiedB & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_horse, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_DRAGON;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Rook_mask[from]) * Attack_Rook_magic[from])
             >> Attack_Rook_shift[from]) & mask_magic;
      dests = Attack_Rook[ from ][ index ];
      dests |= Attack_King[ from ];
      dests &= ~nOccupiedB & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_dragon, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  dests = Attack_King[ SQ_W_KING ] & ~nOccupiedB;
  while( (to = FirstOne( dests )) != -1 )
    {
      if( attacks_to_b( to, &att ) == 0 )
        {
          moves[ count ] = MOVE( w_king, SQ_W_KING, to, 0, 1 );
          count ++;
        }
      dests ^= Bit( to );
    }

  return count;
}


inline int Board::gen_cap_b( unsigned int moves[], int count, unsigned int pin[] )
{
  /* ret: number of total moves */
  unsigned int dests, bb, att;
  int from, to, index;

  bb = BB_B_PAWN;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BPawn[ from ] & ~nOccupiedW;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( to > 19 )
            { moves[ count ] = MOVE( b_pawn, from, to, 1, 1 ); }
          else
            { moves[ count ] = MOVE( b_pawn, from, to, 0, 1 ); }
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_PRO_PAWN;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BGold[ from ] & ~nOccupiedW;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_pro_pawn, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_SILVER;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BSilver[ from ] & ~nOccupiedW;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from > 19 || to > 19 )/* inside of the opponent's territory */
            { moves[ count ] = MOVE( b_silver, from, to, 1, 1 );
              count ++;                                                     }
          moves[ count ] = MOVE( b_silver, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_PRO_SILVER;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BGold[ from ] & ~nOccupiedW;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_pro_silver, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_GOLD;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BGold[ from ] & ~nOccupiedW;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_gold, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_BISHOP;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Bishop_mask[from])
               * Attack_Bishop_magic[from])
              >> Attack_Bishop_shift[from]) & mask_magic;
      dests = Attack_Bishop[ from ][ index ];
      dests &= ~nOccupiedW & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from > 19 || to > 19){
            moves[ count ] = MOVE( b_bishop, from, to, 1, 1 );
            count ++;
          }
          else{
            moves[ count ] = MOVE( b_bishop, from, to, 0, 1 );
            count ++;
          }
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_ROOK;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Rook_mask[from]) * Attack_Rook_magic[from])
             >> Attack_Rook_shift[from]) & mask_magic;
      dests = Attack_Rook[ from ][ index ];
      dests &= ~nOccupiedW & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from > 19 || to > 19){
            moves[ count ] = MOVE( b_rook, from, to, 1, 1 );
            count ++;
          }
          else{
            moves[ count ] = MOVE( b_rook, from, to, 0, 1 );
            count ++;
          }
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_HORSE;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Bishop_mask[from])
               * Attack_Bishop_magic[from])
              >> Attack_Bishop_shift[from]) & mask_magic;
      dests = Attack_Bishop[ from ][ index ];
      dests |= Attack_King[ from ];
      dests &= ~nOccupiedW & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_horse, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_DRAGON;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Rook_mask[from]) * Attack_Rook_magic[from])
             >> Attack_Rook_shift[from]) & mask_magic;
      dests = Attack_Rook[ from ][ index ];
      dests |= Attack_King[ from ];
      dests &= ~nOccupiedW & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_dragon, from, to, 0, 1 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  dests = Attack_King[ SQ_B_KING ] & ~nOccupiedW;
  while( (to = FirstOne( dests )) != -1 )
    {
      if( attacks_to_w( to, &att ) == 0 )
        {
          moves[ count ] = MOVE( b_king, SQ_B_KING, to, 0, 1 );
          count ++;
        }
      dests ^= Bit( to );
    }

  return count;
}


inline int Board::gen_nocap_w( unsigned int moves[], int count, unsigned int pin[] )
{
  /* ret: number of generated moves */
  unsigned int dests, bb, att;
  int from, to, index;

  bb = BB_W_HORSE;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Bishop_mask[from])
               * Attack_Bishop_magic[from])
              >> Attack_Bishop_shift[from]) & mask_magic;
      dests = Attack_Bishop[ from ][ index ];
      dests |= Attack_King[ from ];
      dests &= ~Occupied0 & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_horse, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_DRAGON;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Rook_mask[from]) * Attack_Rook_magic[from])
             >> Attack_Rook_shift[from]) & mask_magic;
      dests = Attack_Rook[ from ][ index ];
      dests |= Attack_King[ from ];
      dests &= ~Occupied0 & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_dragon, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_SILVER;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WSilver[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from < 5 || to < 5 )/* inside of the opponent's territory */
            { moves[ count ] = MOVE( w_silver, from, to, 1, 0 );
              count ++;                                                     }
          moves[ count ] = MOVE( w_silver, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_GOLD;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WGold[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_gold, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_PRO_SILVER;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WGold[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_pro_silver, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_BISHOP;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Bishop_mask[from])
               * Attack_Bishop_magic[from])
              >> Attack_Bishop_shift[from]) & mask_magic;
      dests = Attack_Bishop[ from ][ index ];
      dests &= ~Occupied0 & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from < 5 || to < 5){
            moves[ count ] = MOVE( w_bishop, from, to, 1, 0 );
            count ++;
          }
          else{
            moves[ count ] = MOVE( w_bishop, from, to, 0, 0 );
            count ++;
          }
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_ROOK;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Rook_mask[from]) * Attack_Rook_magic[from])
             >> Attack_Rook_shift[from]) & mask_magic;
      dests = Attack_Rook[ from ][ index ];
      dests &= ~Occupied0 & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from < 5 || to < 5){
            moves[ count ] = MOVE( w_rook, from, to, 1, 0 );
            count ++;
          }
          else{
            moves[ count ] = MOVE( w_rook, from, to, 0, 0 );
            count ++;
          }
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_PRO_PAWN;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WGold[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( w_pro_pawn, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_W_PAWN;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_WPawn[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( to < 5 )
            { moves[ count ] = MOVE( w_pawn, from, to, 1, 0 ); }
          else
            { moves[ count ] = MOVE( w_pawn, from, to, 0, 0 ); }
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  dests = Attack_King[ SQ_W_KING ] & ~Occupied0;
  while( (to = FirstOne( dests )) != -1 )
    {
      if( attacks_to_b( to, &att ) == 0 )
        {
          moves[ count ] = MOVE( w_king, SQ_W_KING, to, 0, 0 );
          count ++;
        }
      dests ^= Bit( to );
    }

  return count;
}

inline int Board::gen_nocap_b( unsigned int moves[], int count, unsigned int pin[]  )
{
  /* ret: number of generated moves */
  unsigned int dests, bb, att;
  int from, to, index;

  bb = BB_B_HORSE;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Bishop_mask[from])
               * Attack_Bishop_magic[from])
              >> Attack_Bishop_shift[from]) & mask_magic;
      dests = Attack_Bishop[ from ][ index ];
      dests |= Attack_King[ from ];
      dests &= ~Occupied0 & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_horse, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_DRAGON;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Rook_mask[from]) * Attack_Rook_magic[from])
             >> Attack_Rook_shift[from]) & mask_magic;
      dests = Attack_Rook[ from ][ index ];
      dests |= Attack_King[ from ];
      dests &= ~Occupied0 & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_dragon, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_SILVER;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BSilver[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from > 19 || to > 19 )/* inside of the opponent's territory */
            { moves[ count ] = MOVE( b_silver, from, to, 1, 0 );
              count ++;                                                     }
          moves[ count ] = MOVE( b_silver, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_GOLD;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BGold[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_gold, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_PRO_SILVER;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BGold[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_pro_silver, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_BISHOP;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Bishop_mask[from])
               * Attack_Bishop_magic[from])
              >> Attack_Bishop_shift[from]) & mask_magic;
      dests = Attack_Bishop[ from ][ index ];
      dests &= ~Occupied0 & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from > 19 || to > 19){
            moves[ count ] = MOVE( b_bishop, from, to, 1, 0 );
            count ++;
          }
          else{
            moves[ count ] = MOVE( b_bishop, from, to, 0, 0 );
            count ++;
          }
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_ROOK;
  while( (from = FirstOne(bb)) != -1 )
    {
      index =(((Occupied0 & Attack_Rook_mask[from]) * Attack_Rook_magic[from])
             >> Attack_Rook_shift[from]) & mask_magic;
      dests = Attack_Rook[ from ][ index ];
      dests &= ~Occupied0 & ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( from > 19 || to > 19){
            moves[ count ] = MOVE( b_rook, from, to, 1, 0 );
            count ++;
          }
          else{
            moves[ count ] = MOVE( b_rook, from, to, 0, 0 );
            count ++;
          }
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_PRO_PAWN;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BGold[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          moves[ count ] = MOVE( b_pro_pawn, from, to, 0, 0 );
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  bb = BB_B_PAWN;
  while( (from = FirstOne(bb)) != -1 )
    {
      dests = Attack_BPawn[ from ] & ~Occupied0;
      dests &= ~(pin[ from ]);
      while( (to = FirstOne(dests)) != -1 )
        {
          if( to > 19 )
            { moves[ count ] = MOVE( b_pawn, from, to, 1, 0 ); }
          else
            { moves[ count ] = MOVE( b_pawn, from, to, 0, 0 ); }
          count ++;
          dests ^= Bit( to );
        }
      bb ^= Bit( from );
    }

  dests = Attack_King[ SQ_B_KING ] & ~Occupied0;
  while( (to = FirstOne( dests )) != -1 )
    {
      if( attacks_to_w( to, &att ) == 0 )
        {
          moves[ count ] = MOVE( b_king, SQ_B_KING, to, 0, 0 );
          count ++;
        }
      dests ^= Bit( to );
    }

  return count;
}


void Board::make_move_w( const unsigned int& move )
{
  const int from    = MOVE_FROM( move );
  const int to      = MOVE_TO( move );
  const int type    = MOVE_TYPE( move );
  const int promote = move & move_mask_promote;
  const int cap     = move & move_mask_capture;
  int cap_type = 0;

  assert( type < 16 );

  if( from == move_drop )
    {
      UPDATE_ZOBRIST(HAND_INFO_RAND[white][W_HAND(type)][type]);
      UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
      W_HAND_A -= HAND_ADD( type );
      BB_N( type ) |= Bit( to );

      nOccupiedW ^= Bit( to );
      Occupied0  ^= Bit( to );

      UPDATE_ZOBRIST(HAND_INFO_RAND[white][W_HAND(type)][type]);
    }
  else if( cap )
    {
      cap_type = get_piece_on_sq_b( to );
      int cap_nopro = cap_type & mask_nopro;
      assert( cap_type != no_piece && cap_type != b_king );

      UPDATE_ZOBRIST(HAND_INFO_RAND[white][W_HAND(cap_nopro)][cap_nopro]);
      if( promote ) /* cap / pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type + m_promote][to]);
          BB_N( type ) ^= Bit( from );
          BB_N( type + m_promote ) ^= Bit( to );
          BB_N( cap_type ) ^= Bit( to );
          W_HAND_A += HAND_ADD( cap_nopro );

          nOccupiedW ^= Bit( from ) | Bit( to );
          nOccupiedB ^= Bit( to );
          Occupied0  ^= Bit( from );
        }
      else /* cap / no pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
          BB_N( type ) ^= Bit( from ) | Bit( to );
          BB_N( cap_type ) ^= Bit( to );
          W_HAND_A += HAND_ADD( cap_type & mask_nopro );

          nOccupiedW ^= Bit( from ) | Bit( to );
          nOccupiedB ^= Bit( to );
          Occupied0  ^= Bit( from );

        }
        UPDATE_ZOBRIST(PIECE_INFO_RAND[cap_type][to]);
        UPDATE_ZOBRIST(HAND_INFO_RAND[white][W_HAND(cap_nopro)][cap_nopro]);
    }
  else
    {
      if( promote ) /* no cap / pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type + m_promote][to]);
          BB_N( type ) ^= Bit( from );
          BB_N( type + m_promote ) ^= Bit( to );

          nOccupiedW ^= Bit( from ) | Bit( to );
          Occupied0  ^= Bit( from ) | Bit( to );
        }
      else /* no cap / no pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
          BB_N( type ) ^= Bit( from ) | Bit( to );

          nOccupiedW ^= Bit( from ) | Bit( to );
          Occupied0  ^= Bit( from ) | Bit( to );
        }
    }
  SQ_W_KING = FirstOne( BB_W_KING );
  assert( SQ_W_KING >= 0 );

  UPDATE_ZOBRIST(TURN_RAND[black]);
  UPDATE_ZOBRIST(TURN_RAND[white]);
  history[N_PLY].move =
    ( move & ~move_mask_captured ) + CAPTURED_2_MOVE( cap_type );

  FLIP_TURN;
  N_PLY ++;
  history[N_PLY].zobrist = get_zobrist();
  /*
  std::cout << " from " << from
            << " to "   << to
            << " type " << type
            << " promote " << promote
            << " cap "  << cap
            << " cap_type" << cap_type
            << std::endl;
  */
  return;
}

void Board::make_move_b( const unsigned int& move )
{
  const int from    = MOVE_FROM( move );
  const int to      = MOVE_TO( move );
  const int type    = MOVE_TYPE( move );
  const int promote = move & move_mask_promote;
  const int cap     = move & move_mask_capture;
  int cap_type = 0;

  assert( type >= 16 );

  if( from == move_drop )
    {
      UPDATE_ZOBRIST(HAND_INFO_RAND[black][B_HAND(type & mask_nopro)][type & mask_nopro]);
      UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
      B_HAND_A -= HAND_ADD( type & mask_nopro );
      BB_N( type ) |= Bit( to );

      nOccupiedB  ^= Bit( to );
      Occupied0   ^= Bit( to );

      UPDATE_ZOBRIST(HAND_INFO_RAND[black][B_HAND(type & mask_nopro)][type & mask_nopro]);
    }
  else if( cap )
    {
      cap_type = get_piece_on_sq_w( to );
      int cap_nopro = cap_type & mask_nopro;
      assert( cap_type != no_piece && cap_type != w_king );

      UPDATE_ZOBRIST(HAND_INFO_RAND[black][B_HAND(cap_nopro)][cap_nopro]);
      if( promote ) /* cap / pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type + m_promote][to]);
          BB_N( type ) ^= Bit( from );
          BB_N( type + m_promote ) ^= Bit( to );
          BB_N( cap_type ) ^= Bit( to );
          B_HAND_A += HAND_ADD( cap_nopro );

          nOccupiedB ^= Bit( from ) | Bit( to );
          nOccupiedW ^= Bit( to );
          Occupied0  ^= Bit( from );
        }
      else /* cap / no pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
          BB_N( type ) ^= Bit( from ) | Bit( to );
          BB_N( cap_type ) ^= Bit( to );
          B_HAND_A += HAND_ADD( cap_type & mask_nopro );

          nOccupiedB ^= Bit( from ) | Bit( to );
          nOccupiedW ^= Bit( to );
          Occupied0  ^= Bit( from );
        }
      UPDATE_ZOBRIST(PIECE_INFO_RAND[cap_type][to]);
      UPDATE_ZOBRIST(HAND_INFO_RAND[black][B_HAND(cap_nopro)][cap_nopro]);
    }
  else
    {
      if( promote ) /* no cap / pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type + m_promote][to]);
          BB_N( type ) ^= Bit( from );
          BB_N( type + m_promote ) ^= Bit( to );

          nOccupiedB ^= Bit( from ) | Bit( to );
          Occupied0  ^= Bit( from ) | Bit( to );
        }
      else /* no cap / no pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
          BB_N( type ) ^= Bit( from ) | Bit( to );

          nOccupiedB ^= Bit( from ) | Bit( to );
          Occupied0  ^= Bit( from ) | Bit( to );
        }
    }
  SQ_B_KING = FirstOne( BB_B_KING );
  assert( SQ_B_KING >= 0 );

  UPDATE_ZOBRIST(TURN_RAND[black]);
  UPDATE_ZOBRIST(TURN_RAND[white]);
  history[N_PLY].move =
    ( move & ~move_mask_captured ) + CAPTURED_2_MOVE( cap_type );

  FLIP_TURN;
  N_PLY ++;
  history[N_PLY].zobrist = get_zobrist();
  /*
  std::cout << " from " << from
            << " to "   << to
            << " type " << type
            << " promote " << promote
            << " cap "  << cap
            << " cap_type" << cap_type
            << std::endl;
  */
  return;
}

void Board::unmake_move_w()
{
  FLIP_TURN;
  N_PLY --;
  int move = history[N_PLY].move;

  const int from     = MOVE_FROM( move );
  const int to       = MOVE_TO( move );
  const int type     = MOVE_TYPE( move );
  const int promote  = move & move_mask_promote;
  const int cap_type = MOVE_CAPTURED_TYPE( move );

  if( from == move_drop )
    {
      UPDATE_ZOBRIST(HAND_INFO_RAND[white][W_HAND(type)][type]);
      UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
      W_HAND_A += HAND_ADD( type );
      BB_N( type ) ^= Bit( to );

      nOccupiedW ^= Bit( to );
      Occupied0  ^= Bit( to );
      UPDATE_ZOBRIST(HAND_INFO_RAND[white][W_HAND(type)][type]);
    }
  else if( cap_type )
    {
      int cap_nopro = cap_type & mask_nopro;
      assert( cap_type != no_piece && cap_type != b_king );
      UPDATE_ZOBRIST(HAND_INFO_RAND[white][W_HAND(cap_nopro)][cap_nopro]);
      if( promote ) /* cap / pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type + m_promote][to]);
          BB_N( type ) ^= Bit( from );
          BB_N( type + m_promote ) ^= Bit( to );
          BB_N( cap_type ) ^= Bit( to );
          W_HAND_A -= HAND_ADD( cap_type & mask_nopro );

          nOccupiedW ^= Bit( from ) | Bit( to );
          nOccupiedB ^= Bit( to );
          Occupied0  ^= Bit( from );
        }
      else /* cap / no pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
          BB_N( type ) ^= Bit( from ) | Bit( to );
          BB_N( cap_type ) ^= Bit( to );
          W_HAND_A -= HAND_ADD( cap_type & mask_nopro );

          nOccupiedW ^= Bit( from ) | Bit( to );
          nOccupiedB ^= Bit( to );
          Occupied0  ^= Bit( from );
        }
      UPDATE_ZOBRIST(PIECE_INFO_RAND[cap_type][to]);
      UPDATE_ZOBRIST(HAND_INFO_RAND[white][W_HAND(cap_nopro)][cap_nopro]);
    }
  else
    {
      if( promote ) /* no cap / pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type + m_promote][to]);
          BB_N( type ) ^= Bit( from );
          BB_N( type + m_promote ) ^= Bit( to );

          nOccupiedW ^= Bit( from ) | Bit( to );
          Occupied0  ^= Bit( from ) | Bit( to );
        }
      else /* no cap / no pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
          BB_N( type ) ^= Bit( from ) | Bit( to );

          nOccupiedW ^= Bit( from ) | Bit( to );
          Occupied0  ^= Bit( from ) | Bit( to );
        }
    }
  SQ_W_KING = FirstOne( BB_W_KING );

  UPDATE_ZOBRIST(TURN_RAND[black]);
  UPDATE_ZOBRIST(TURN_RAND[white]);
  return;
}

void Board::unmake_move_b()
{
  FLIP_TURN;
  N_PLY --;
  int move = history[N_PLY].move;

  const int from     = MOVE_FROM( move );
  const int to       = MOVE_TO( move );
  const int type     = MOVE_TYPE( move );
  const int promote  = move & move_mask_promote;
  const int cap_type = MOVE_CAPTURED_TYPE( move );

  if( from == move_drop )
    {
      UPDATE_ZOBRIST(HAND_INFO_RAND[black][B_HAND(type & mask_nopro)][type & mask_nopro]);
      UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
      B_HAND_A += HAND_ADD( type & mask_nopro );
      BB_N( type ) ^= Bit( to );

      nOccupiedB ^= Bit( to );
      Occupied0  ^= Bit( to );

      UPDATE_ZOBRIST(HAND_INFO_RAND[black][B_HAND(type & mask_nopro)][type & mask_nopro]);
    }
  else if( cap_type )
    {
      int cap_nopro = cap_type & mask_nopro;
      assert( cap_type != no_piece && cap_type != b_king );
      UPDATE_ZOBRIST(HAND_INFO_RAND[black][B_HAND(cap_nopro)][cap_nopro]);
      if( promote ) /* cap / pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type + m_promote][to]);
          BB_N( type ) ^= Bit( from );
          BB_N( type + m_promote ) ^= Bit( to );
          BB_N( cap_type ) ^= Bit( to );
          B_HAND_A -= HAND_ADD( cap_type & mask_nopro );

          nOccupiedB ^= Bit( from ) | Bit( to );
          nOccupiedW ^= Bit( to );
          Occupied0  ^= Bit( from );
        }
      else /* cap / no pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
          BB_N( type ) ^= Bit( from ) | Bit( to );
          BB_N( cap_type ) ^= Bit( to );
          B_HAND_A -= HAND_ADD( cap_type & mask_nopro );

          nOccupiedB ^= Bit( from ) | Bit( to );
          nOccupiedW ^= Bit( to );
          Occupied0  ^= Bit( from );
        }
      UPDATE_ZOBRIST(PIECE_INFO_RAND[cap_type][to]);
      UPDATE_ZOBRIST(HAND_INFO_RAND[black][B_HAND(cap_nopro)][cap_nopro]);
    }
  else
    {
      if( promote ) /* no cap / pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type + m_promote][to]);
          BB_N( type ) ^= Bit( from );
          BB_N( type + m_promote ) ^= Bit( to );

          nOccupiedB ^= Bit( from ) | Bit( to );
          Occupied0  ^= Bit( from ) | Bit( to );
        }
      else /* no cap / no pro */
        {
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][from]);
          UPDATE_ZOBRIST(PIECE_INFO_RAND[type][to]);
          BB_N( type ) ^= Bit( from ) | Bit( to );

          nOccupiedB ^= Bit( from ) | Bit( to );
          Occupied0  ^= Bit( from ) | Bit( to );
        }
    }
  SQ_B_KING = FirstOne( BB_B_KING );

  UPDATE_ZOBRIST(TURN_RAND[black]);
  UPDATE_ZOBRIST(TURN_RAND[white]);
  return;
}


inline int Board::FirstOne( int bb )
{
  return bb ? __builtin_ctz( bb ) : -1;
}

int Board::get_piece_on_sq_w( int sq )
{
#define TEST( piece ) \
  if( BB_N( piece ) & Bit( sq ) ) \
    return piece; \

  TEST( w_silver );
  TEST( w_gold );
  TEST( w_bishop );
  TEST( w_rook );
  TEST( w_pro_silver );
  TEST( w_horse );
  TEST( w_dragon );
  TEST( w_pawn );
  TEST( w_pro_pawn );
  if( SQ_W_KING == sq )
    return w_king;

  return no_piece;
}

int Board::get_piece_on_sq_b( int sq )
{
  TEST( b_silver );
  TEST( b_gold );
  TEST( b_bishop );
  TEST( b_rook );
  TEST( b_pro_silver );
  TEST( b_horse );
  TEST( b_dragon );
  TEST( b_pawn );
  TEST( b_pro_pawn );
  if( SQ_B_KING == sq )
    return b_king;

  return no_piece;
#undef TEST
}

int Board::get_piece_on_sq( int sq )
{
  int ret;

  if( (ret = get_piece_on_sq_w( sq )) != no_piece )
    { return ret; }

  return get_piece_on_sq_b( sq );
}

#define FILE_READ( variable, size ) \
  if( fread( variable, sizeof( unsigned int ), size, fp) != size ){ \
    printf(" fread \"%s\" failed.\n", #variable); \
    fclose( fp ); return -1; }\

int Board::starting_initialize(const std::string binPath)
{
  /*
    return -1: failed
            0: succeeded
   */

  FILE *fp;
  std::string name = "/BB_Attack.bin";
  std::string bp(binPath);

  bp.append(name);
  //std::cout << "binPath: " << bp << std::endl;

  fp = fopen(bp.c_str(),"rb");
  if(fp == NULL){
    return -1;
  }

  FILE_READ( Attack_WPawn, 32 );
  FILE_READ( Attack_WSilver, 32 );
  FILE_READ( Attack_WGold, 32 );
  FILE_READ( Attack_BPawn, 32 );
  FILE_READ( Attack_BSilver, 32 );
  FILE_READ( Attack_BGold, 32 );
  FILE_READ( Attack_King, 32 );
  FILE_READ( Attack_Rook, 32 * 64 );
  FILE_READ( Attack_Rook_magic, 32 );
  FILE_READ( Attack_Rook_mask, 32 );
  FILE_READ( Attack_Bishop, 32 * 64 );
  FILE_READ( Attack_Bishop_magic, 32 );
  FILE_READ( Attack_Bishop_mask, 32 );
  FILE_READ( DoublePawn, 32 );
  FILE_READ( Pin_Rook, 32 * 32 * 64 * 2 );
  FILE_READ( Pin_Bishop, 32 * 32 * 64 * 2 );

  fclose( fp );

  return 0;
}

#undef FILE_READ

void Board::clear_game()
{
  TURN      = white;
  N_PLY  = 0;

  memset( BB_ALL, 0, sizeof(BB_ALL) );
  W_HAND_A = 0;
  B_HAND_A = 0;

  BB_W_PAWN       = Bit( XY2INDEX( 5, 4 ) );
  BB_W_SILVER     = Bit( XY2INDEX( 3, 5 ) );
  BB_W_GOLD       = Bit( XY2INDEX( 4, 5 ) );
  BB_W_BISHOP     = Bit( XY2INDEX( 2, 5 ) );
  BB_W_ROOK       = Bit( XY2INDEX( 1, 5 ) );
  BB_W_KING       = Bit( XY2INDEX( 5, 5 ) );
  SQ_W_KING       =      XY2INDEX( 5, 5 )  ;
  BB_W_PRO_PAWN   = 0;
  BB_W_PRO_SILVER = 0;
  BB_W_HORSE      = 0;
  BB_W_DRAGON     = 0;

  BB_B_PAWN       = Bit( XY2INDEX( 1, 2 ) );
  BB_B_SILVER     = Bit( XY2INDEX( 3, 1 ) );
  BB_B_GOLD       = Bit( XY2INDEX( 2, 1 ) );
  BB_B_BISHOP     = Bit( XY2INDEX( 4, 1 ) );
  BB_B_ROOK       = Bit( XY2INDEX( 5, 1 ) );
  BB_B_KING       = Bit( XY2INDEX( 1, 1 ) );
  SQ_B_KING       =      XY2INDEX( 1, 1 )  ;
  BB_B_PRO_PAWN   = 0;
  BB_B_PRO_SILVER = 0;
  BB_B_HORSE      = 0;
  BB_B_DRAGON     = 0;

  calc_occupied_sq();

  ZOBRIST = get_zobrist();
  history[0].zobrist = get_zobrist();

  return;
}

inline void Board::calc_occupied_sq()
{
  int i,index, tmp;

  nOccupiedW = bb_mask;
  nOccupiedB = bb_mask;
  Occupied0  = 0;

  for( i=0; i < bbs_size; i++ )
    {
      if( i == w_king )
        { nOccupiedW &= ~Bit( SQ_W_KING );
          Occupied0   ^= Bit( SQ_W_KING );
          continue;                       }
      else if(i == b_king )
        { nOccupiedB &= ~Bit( SQ_B_KING );
          Occupied0   ^= Bit( SQ_B_KING );
          continue;                       }
      tmp = BB_N(i);
      while( (index = FirstOne( tmp ) )  >= 0 )
        {
          if( i < (bbs_size / 2) )
            nOccupiedW &= ~Bit(index);
          else
            nOccupiedB &= ~Bit(index);
          Occupied0   ^= Bit( index );
          tmp ^= Bit(index);
        }

    }

  return;
}


int Board::popuCount( int piece )
{
  int count = 0;
  unsigned bb = BB_N( piece );
  int b;

  while( ( b = FirstOne( bb ) ) != -1 )
    {
      count ++;
      bb &= ~Bit( b );
    }

  return count;
}

int Board::get_turn()
{
  return TURN ? black : white ;
}

int Board::get_nply()
{
  return N_PLY;
}

char Board::turn(){
  return TURN;
}

void Board::unmake_move(){
  TURN ? unmake_move_w() : unmake_move_b();
}

void Board::make_move(const int& move){
  TURN ? make_move_b( move ) : make_move_w( move );
}

int Board::w_hand(int piece){
  return (int)((W_HAND_A >> ((piece)*2)) & b0011);
}
int Board::b_hand(int piece){
    return (int)((B_HAND_A >> ((piece)*2)) & b0011);
}

void Board::print_tpt(){
  auto itr = TPT.begin();
  while(itr != TPT.end()){
    std::cout << (*itr).first << ":" << (*itr).second.eval << ":" <<  (*itr).second.depth<< std::endl;
    ++itr;
  }
}

void Board::set_tpt(unsigned long long key, int depth, short eval) {
  tpt_v new_val = { depth, (short)abs((int)eval)};
  tpt[key] = new_val;
}

std::unordered_map<unsigned long long, tpt_v>* Board::get_tpt() {
  return &tpt;
}

void Board::write_tpt(std::string binPath) {
  std::string tpt_bin_name = "/Tpt.bin";
  std::string bp(binPath);
  bp.append(tpt_bin_name);
  std::ofstream ofs(bp.c_str(), std::ios_base::out | std::ios_base::binary);
  if (ofs) {
    auto itr = TPT.begin();
    while(itr != TPT.end()){
      ofs.write(reinterpret_cast<const char *>(&(*itr).first), sizeof((*itr).first));
      ofs.write(reinterpret_cast<const char *>(&(*itr).second), sizeof((*itr).second));
      ++itr;
    }
  }
  ofs.close();
}

void Board::read_tpt(std::string binPath) {
  std::string tpt_bin_name = "/Tpt.bin";
  std::string bp(binPath);
  bp.append(tpt_bin_name);
  std::ifstream ifs(bp.c_str() ,std::ios_base::in | std::ios_base::binary);
  if (ifs) {
    std::pair<unsigned long long, tpt_v> tptv;
    while(!ifs.eof()){
      ifs.read(reinterpret_cast<char *>(&(tptv.first)), sizeof(tptv.first));
      ifs.read(reinterpret_cast<char *>(&(tptv.second)), sizeof(tptv.second));
      TPT.insert(tptv);
    }
  }
  ifs.close();
}

int Board::get_board_show_cnt() {
  if (N_PLY <= 0) { return 0; }
  unsigned long long check = history[N_PLY].zobrist;
  int cnt = 0;
  for (int i = 0; i < N_PLY ; i++) {
    if (history[i].zobrist == check) {
      cnt++;
    }
  }
  return cnt;
}
