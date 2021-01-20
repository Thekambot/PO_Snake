#include <time.h>

#include "snake.h"
#include "screen.h"

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

  display_sneak();

  switch (s_state)
  {
    case GameState::INFO:
      display_start_menu(); break;

    case GameState::PAUSE: 
      break;

    case GameState::GAMEOVER 
      break;
  }
}

void CSnake::set_window()
{
  s_state = GameState::INFO;
  s_level = 1;
  s_score = 0;
  s_speed = 1.0;
  s_dir = Direction::IN_PLACE;

  generate_new_food();
  generate_new_snake();

  display_start_menu();
}

void CSnake::generate_new_food()
{
  srand(time(NULL));

  s_food_coord.x = rand() % (geom.size.x - 1) + 1;
  s_food_coord.y = rand() % (geom.size.y - 1) + 1;
}

void CSnake::generate_new_snake()
{
  s_snake.clear();
  s_snake.reserve(3);

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