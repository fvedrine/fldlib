DECLARE_RESOURCES

int main() {
  INIT_MAIN

  float x = FBETWEEN_WITH_ERROR(0.0, 2.0, -1e-5, 1e-5);
  float y;

  FLOAT_SPLIT_ALL(1, y >> double::end(), x << double::end())
  if (x < 1.0) {
    FAFFPRINT(x);
    y = 4*x-3;
  }
  else {
    FAFFPRINT(x);
    y = x;
  }
  FLOAT_MERGE_ALL(1, y << double::end(), x >> double::end())
  FPRINT(y);

  END_MAIN
  return 0;
}
