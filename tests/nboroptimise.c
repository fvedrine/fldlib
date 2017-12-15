#include <math.h>

DECLARE_RESOURCES

int main(void)
{ 
  INIT_MAIN

  float x,y,z;
  int i;
  x=1;
  y=(sqrt(5)-1)/2;
  for (i=1;i<=100;i++) 
  {
    z=x;
    x=y;
    y=z-y;
    FPRINT(y);
  } 

  END_MAIN
  return 0;
}
