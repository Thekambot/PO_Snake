#include "winsys.h"
#include "snake.h"

#include <iostream>

int main ()
{
  std::cout << "test" << std::endl;

  CDesktop d;
  d.insert(new CInputLine (CRect (CPoint (5, 7), CPoint (15, 15))));
  d.insert(new CWindow (CRect (CPoint (2, 3), CPoint (20, 10)), '#'));
  d.insert(new CSnake(CRect (CPoint (5, 5), CPoint (45, 20))));
  d.run ();
  return 0;
}
