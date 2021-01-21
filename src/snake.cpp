#include <time.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "cpoint.h"
#include "screen.h"
#include "winsys.h"
#include "snake.h"

CSnake::CSnake(CRect r, char _c /*=' '*/):
  CFramedWindow(r, _c)
{
  set_window();
}

char CSnake::symbol(Symbols s)
{
  switch (s)
  {
  case Symbols::FOOD: return 'O';
  case Symbols::HEAD: return '#';
  case Symbols::BODY: return 'S';
  default: return '\0';
  }
}

void CSnake::paint()
{
  CFramedWindow::paint();

  gotoyx(geom.topleft.y - 1, geom.topleft.x);
  printl("| LEVEL: %d |", s_level);

  gotoyx(geom.topleft.y + s_food_coord.y, geom.topleft.x + s_food_coord.x);
  printc(symbol(Symbols::FOOD));

  display_snake();

  switch (s_state)
  {
    case GameState::INFO:
      display_start_menu(); 
      break;

    case GameState::PAUSE: 
      break;

    case GameState::GAMEOVER:
      display_gameover_screen();
      break;

    case GameState::UNPAUSE:
      break;
  }
}

bool CSnake::handleEvent(int key)
{
  //Controls button = key_to_control(key);
  //bool action = game_controls(button);
  bool action = game_controls(key);

  if (s_state != GameState::UNPAUSE)
    if (CFramedWindow::handleEvent(key))
      return true;

  if (s_state != GameState::GAMEOVER)
    game_interval();

  return action;
}

void CSnake::set_window()
{
  s_state = GameState::INFO;
  s_level = 1;
  s_score = 0;
  s_speed = 1;
  s_dir = Direction::IN_PLACE;

  generate_new_snake();
  generate_new_food();
}

bool CSnake::game_controls(int key)
{
  if (key == 'r' || key == 'R')
  {
    set_window();
    return true;
  }

  if (key == 'p' || key == 'P')
  {
    if (s_state == GameState::PAUSE)
      s_state = GameState::UNPAUSE;
    else
      s_state = GameState::PAUSE;
    return true;
  }

  if (key == 'h' || key == 'H')
  {
    if (s_state == GameState::INFO)
      s_state = GameState::UNPAUSE;
    else
      s_state = GameState::INFO;
    return true;
  }

  if (s_state != GameState::PAUSE && s_state != GameState::INFO && s_state != GameState::GAMEOVER)
  {
    if (key == KEY_UP)
    {
      if (s_dir != Direction::DOWN)
        s_dir = Direction::UP;
      return true;
    }

    if (key == KEY_DOWN)
    {
      if (s_dir != Direction::UP)
        s_dir = Direction::DOWN;
      return true;
    }

    if (key == KEY_RIGHT)
    {
      if (s_dir != Direction::LEFT)
        s_dir = Direction::RIGHT;
      return true;
    }

    if (key == KEY_LEFT)
    {
      if (s_dir != Direction::RIGHT)
        s_dir = Direction::LEFT;
      return true;
    }
  }
  
  return false;
}

bool CSnake::game_controls(Controls key)
{
  bool value = false;

  switch (key)
  {
    case Controls::r:
      set_window();
      value = true;
    break;

    case Controls::p:
      if (s_state == GameState::PAUSE)
        s_state = GameState::UNPAUSE;
      else
        s_state = GameState::PAUSE;
      value = true;
    break;

    case Controls::h:
      if (s_state == GameState::INFO)
        s_state = GameState::UNPAUSE;
      else
        s_state = GameState::INFO;
      value = true;
    break;

    default: value = false; break;
  }

  if (s_state != GameState::PAUSE && s_state != GameState::INFO && s_state != GameState::GAMEOVER)
  {
    switch (key)
    {
      case Controls::up:
      if (s_dir == Direction::DOWN) break;
      s_dir = Direction::UP;
      value = true;
      break;

      case Controls::down:
      if (s_dir == Direction::UP) break;
      s_dir = Direction::DOWN;
      value = true;
      break;

      case Controls::right:
      if (s_dir == Direction::LEFT) break;
      s_dir = Direction::RIGHT;
      value = true;
      break;

      case Controls::left:
      if (s_dir == Direction::RIGHT) break;
      s_dir = Direction::LEFT;
      value = true;
      break;

      default: value = false; break;
    }
  }
  
  return value;
}

void CSnake::game_interval()
{
  // clock_t start_time = clock();

	// double elapsed_time = 0.0;

	// while (s_speed * elapsed_time < 1)
	// {
	// 	elapsed_time = 1.0 * (clock() - start_time) / CLOCKS_PER_SEC;
	// }

  std::this_thread::sleep_for(std::chrono::milliseconds((110 - s_speed * 10)));

  snake_move();
  paint();
}

void CSnake::snake_move()
{
  if (s_dir == Direction::IN_PLACE || s_state != GameState::UNPAUSE)
    return;

  for (vector<CPoint>::reverse_iterator it = s_snake.rbegin(); it != std::prev(s_snake.rend(), 1); it++)
  {
    *it = *std::next(it, 1);
  }

  // for (size_t i = 0; i < s_snake.size() - 2; i++ )
  // {
  //   s_snake[i + 1] = s_snake[i];
  // }

  switch (s_dir)
  {
    case Direction::UP: 
    if (--(s_snake.front().y) < 1)
      s_snake.front().y = geom.size.y - 2;
    break;

    case Direction::DOWN:
    if (++(s_snake.front().y) > geom.size.y - 2)
      s_snake.front().y = 1;
    break;

    case Direction::RIGHT:
    if (++(s_snake.front().x) > geom.size.x - 2)
      s_snake.front().x = 1;
    break;

    case Direction::LEFT:
    if (--(s_snake.front().x) < 1)
      s_snake.front().x = geom.size.x - 2;
    break;

    case Direction::IN_PLACE:
    break;
  }

  vector<CPoint>::iterator it;
  for(it = std::next(s_snake.begin(), 1); it != s_snake.end(); it++)
  {
    if (s_snake.front() == *it)
    {
      s_state = GameState::GAMEOVER;
      return;
    }
  }

  // for (size_t i = 1; i < s_snake.size() - 1; i++ )
  // {
  //   if (s_snake.front().x == s_snake[i].x && 
  //       s_snake.front().y == s_snake[i].y)
  //   {
  //     s_state = GameState::GAMEOVER;
  //     return;
  //   }
  // }

  if (s_snake.front() == s_food_coord)
  {
    s_snake.push_back(s_snake.back());
    s_score++;

    if (s_score % 2 == 0 && s_level <= 10)
    {
      s_level++; s_speed += 1;
    }

    generate_new_food();
  }
}

void CSnake::generate_new_food()
{
  srand(time(NULL));
  bool inside_body = false;

  do
  {
    s_food_coord.x = rand() % (geom.size.x - 2) + 1;
    s_food_coord.y = rand() % (geom.size.y - 2) + 1;
    inside_body = false;

    vector<CPoint>::iterator it;
    for (it = std::next(s_snake.begin(), 1); it != s_snake.end(); it++)
    {
      if (s_food_coord == *it)
      {
        inside_body = true;
        break;
      }
    }

    // for (size_t i = 1; i < s_snake.size() - 1; i++ )
    // {
    //   if (s_food_coord.x == s_snake[i].x && 
    //       s_food_coord.y == s_snake[i].y)
    //   {
    //     inside_body = true;
    //     break;
    //   }
    // }
  }
  while (inside_body);
}

void CSnake::generate_new_snake()
{
  s_snake.clear();
  s_snake.reserve(50);

  s_snake.push_back(CPoint(4, 2));
  s_snake.push_back(CPoint(3, 2));
  s_snake.push_back(CPoint(2, 2));
}

void CSnake::display_start_menu()
{
	gotoyx(geom.topleft.y + 4, geom.topleft.x + 2);
	printl("h - toggle help information");	
		
	gotoyx(geom.topleft.y + 5, geom.topleft.x + 2);
	printl("p - toggle pause/play mode");

	gotoyx(geom.topleft.y + 6, geom.topleft.x + 2);
	printl("r - reset game");

	gotoyx(geom.topleft.y + 7, geom.topleft.x + 2);
	printl("arrows - move snake (in play mode) or");

	gotoyx(geom.topleft.y + 8, geom.topleft.x + 11);
	printl("move window (in pause mode)");
}

void CSnake::display_gameover_screen()
{
  gotoyx(geom.topleft.y + 4, geom.topleft.x + 2);
	printl("GAME OVER!");	
		
	gotoyx(geom.topleft.y + 5, geom.topleft.x + 2);
	printl("Score: %d", s_score);

	gotoyx(geom.topleft.y + 6, geom.topleft.x + 2);
	printl("r - reset game");
}

void CSnake::display_snake()
{
  int left = geom.topleft.x;
	int top = geom.topleft.y;

  vector<CPoint>::iterator it;  
  for (it = std::next(s_snake.begin(), 1); it != s_snake.end(); it++)
  {
    gotoyx(top + (*it).y, left + (*it).x);
    printc(symbol(Symbols::BODY));
  }

  gotoyx(top + s_snake.front().y, left + s_snake.front().x);
  printc(symbol(Symbols::HEAD));

  // for (size_t i = 1; i < s_snake.size() - 1; i++ )
  // {
  //   gotoyx(top + s_snake[i].y, left + s_snake[i].x);
  //   printc(symbol(Symbols::BODY));
  // }
}

Controls CSnake::key_to_control(int key)
{
  switch (key)
  {
    case 'r': return Controls::r;
    case 'R': return Controls::R;
    case 'p': return Controls::p;
    case 'P': return Controls::P;
    case 'h': return Controls::h;
    case 'H': return Controls::H;
    case KEY_UP: return Controls::up;
    case KEY_DOWN: return Controls::down;
    case KEY_RIGHT: return Controls::right;
    case KEY_LEFT: return Controls::left;

    default: return Controls::not_set;
  }
}
