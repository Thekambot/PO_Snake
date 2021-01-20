#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <time.h>
#include <vector>
#include <iostream>

#include "cpoint.h"
#include "winsys.h"
#include "snake.h"

enum class GameState
{
  PAUSE = 0, UNPAUSE, INFO, GAMEOVER
};

enum class Direction
{
  IN_PLACE = 0, LEFT, RIGHT, UP, DOWN
};

enum class Symbols
{
  FOOD = 0, HEAD, BODY
};

enum class Controls
{
  not_set = 0,

  up = 1, down = 2, right = 3, left = 4,
  r = 5, R = 5,
  p = 6, P = 6,
  h = 7, H = 7
};

class CSnake:public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');

  void paint();
  bool handleEvent(int key);

  GameState s_state;

  size_t s_level;
  size_t s_score;
  CPoint s_food_coord;
  
  std::vector<CPoint> s_snake;
  double s_speed;
  Direction s_dir;

  void set_window();

  char symbol(Symbols s);
  Controls key_to_control(int key);

  bool game_controls(Controls key);
  void game_interval();

  void snake_move();

  void display_start_menu();
  void display_gameover_screen();
  void display_snake();

  void generate_new_food();
  void generate_new_snake();

};

#endif
