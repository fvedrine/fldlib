DECLARE_RESOURCES

int main() {
  INIT_MAIN

  int i;
  double y=0.7;
  double x=y;
  for (i=1; i<=20; i++) {
    x=11*x-7;
    DPRINT(x);
  }

  END_MAIN
  return 0;
}
