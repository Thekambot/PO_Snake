#include "winsys.h"
#include "snake.h"

#include <iostream>

int main ()
{
  getchar();

  CDesktop d;
  d.insert(new CInputLine (CRect (CPoint (5, 7), CPoint (15, 15))));
  d.insert(new CWindow (CRect (CPoint (1, 1), CPoint (4, 50)), 'O'));
  d.insert(new CSnake(CRect (CPoint (5, 5), CPoint (45, 20))));
  d.run ();
  return 0;
}
