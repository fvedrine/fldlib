DECLARE_RESOURCES

int main()
{
  INIT_MAIN

  float x,y,z;
  x = FBETWEEN_WITH_ERROR(-1.0, +1.0, -1e-7, +1e-7);
  y = FBETWEEN_WITH_ERROR(-1.0, +1.0, -1e-7, +1e-7);
  FLOAT_SPLIT_ALL(1, z >> y >> x >> double::end(), y << x << double::end())
  z = x-y;
  if (x < y)
    z = -z;
  FLOAT_MERGE_ALL(1, x << y << z << double::end(), x >> y >> double::end())
  FPRINT(x);
  FPRINT(y);
  FPRINT(z);

  END_MAIN
  return 0;
}
