#include "game.h"

Game::Game(){
  board = new Board();
  search = new Search(board);
}

Board* Game::get_board(){
  return board;
}

Search* Game::get_search(){
  return search;
}

void Game::game_initialize(){
  board->clear_game();
}

void Game::game_finalize(){
}
