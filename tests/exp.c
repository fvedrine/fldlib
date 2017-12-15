DECLARE_RESOURCES

int main(int argc, char** argv) {
  INIT_MAIN

  double x = DBETWEEN(0.43, 7.315);
  double y = exp(x);
  DPRINT(x);
  DPRINT(y);

  END_MAIN
  return 0;
}
