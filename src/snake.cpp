#include "snake.h"

CSnake::CSnake(CRect r, char _c /*=' '*/):
  CFramedWindow(r, _c), s_level(1), 
  s_score(0), s_state(GameState::GS_INFO),
  s_speed(1.0f)
{
}
