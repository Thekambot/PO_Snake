#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <iostream>
#include <vector>

#include "winsys.h"

enum class GameState
{
  PAUSE = 0, INFO, GAMEOVER
};

enum class Direction
{
  IN_PLACE = 0, LEFT, RIGHT, UP, DOWN
};

enum class Symbols
{
  FOOD = 0, HEAD, BODY
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

  void display_start_menu();
  void display_sneak();

  void generate_new_food();
  void generate_new_snake();

};

#endif
