DECLARE_RESOURCES

int main() {
  INIT_MAIN

  float x = FBETWEEN(0, 1);
  float y = x*x;
  FAFFPRINT(y);

  END_MAIN
  return 0;
}

