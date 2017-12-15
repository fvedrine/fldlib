DECLARE_RESOURCES

int main()
{
  INIT_MAIN

  float x,y,z;
  x = 0.1;
  x += 2e-9;
  x += 2e-9;
  x += 2e-9;
  x += 2e-9;
  y = FBETWEEN_WITH_ERROR(0.1-1e-8, 0.1+1e-8, -1e-8, +1e-8);
  FLOAT_SPLIT_ALL(1, z >> y >> x >> double::end(), x << y << z << double::end())
  if (x >= 0.1) {
    FLOAT_SPLIT_ALL(2, z >> y >> double::end(), y << double::end())
    if (y != 0.1) {
      z = 0.1;
      z += y;
    }
    else {
      z = 0.2;
      z += y;
    }
    FLOAT_MERGE_ALL(2, y << z << double::end(), y >> double::end())
  };
  FLOAT_MERGE_ALL(1, x << y << z << double::end(), z >> y >> x >> double::end())
  FPRINT(x);
  FPRINT(y);
  FPRINT(z);

  END_MAIN
  return 0;
}
