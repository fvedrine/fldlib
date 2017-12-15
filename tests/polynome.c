DECLARE_RESOURCES

int main(void)
{
  INIT_MAIN
  float x,y,z,t;

  x = FBETWEEN(0,1);
  y = (x-1)*(x-1)*(x-1)*(x-1);
  FPRINT(y);
  z = x*x;
  FPRINT(z);
  z = z*z - 4*x*z + 6*z - 4*x + 1;
  FPRINT(z);
  t = z-y;
  FPRINT(t);

  END_MAIN
  return 0;
}
