#include <math.h>

DECLARE_RESOURCES

int main() {
  INIT_MAIN

  double x, y, resx, resy, res;
  x = DBETWEEN_WITH_ERROR(1,2,-0.0001,0.0001);
  y = DBETWEEN_WITH_ERROR(.1,.2,-0.0001,0.0001);
  resx = y/x;
  resx = 1.0/(sqrt(1.0+resx*resx));
  resy = x/y;
  resy = 1.0/(sqrt(resy*resy+1.0));
  res = resx*resx+resy*resy;

  END_MAIN
  return 0;
}
