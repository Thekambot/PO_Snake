#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <iostream>
#include <vector>

#include "winsys.h"

enum class GameState
{
  GS_PAUSE = 0, GS_INFO
};

enum class Direction
{
  DIR_IN_PLACE = 0, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN
};

class CSnake:public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');

  void paint();
  bool handleEvent(int key);

  size_t s_level;
  size_t s_score;
  
  GameState s_state;
  double s_speed;

  CPoint s_food_coord;

  std::vector<CPoint> s_snake;

  void display_start_menu();
  void display_sneak();

  void set_window();
};

#endif
