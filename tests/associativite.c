DECLARE_RESOURCES

int main()
{
  INIT_MAIN

  double x,y,z,t,u;
  x = DBETWEEN(0,3);
  y = DBETWEEN(0,3);
  z = x+y;
  t = z-x;
  u = t-y;

  DPRINT(z);
  DPRINT(t);
  DPRINT(u);

  END_MAIN
  return 0;
}
