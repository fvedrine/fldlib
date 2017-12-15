DECLARE_RESOURCES

int main(int argc, char** argv) {
  INIT_MAIN

  double x = DBETWEEN(0.1, 100);
  double xx = 3*x;
  double y = x * x;
  double z = x * DBETWEEN(0.1, 100);
  DPRINT(x);
  DPRINT(xx);
  DPRINT(y);
  DPRINT(z);

  END_MAIN
  return 0;
}


