DECLARE_RESOURCES

int main()
{
  INIT_MAIN

  float x,y,z;
  x = FBETWEEN_WITH_ERROR(0.0, 1.0, -1e-7, +1e-7);
  z = x;
  FLOAT_SPLIT_ALL(1, y >> x >> double::end(), x << z << double::end())
  if (x < 0.5F)
    y = x;
  else
    y = 1;
  FLOAT_MERGE_ALL(1, x << y << double::end(), z >> x >> double::end())
  z = x + y;
  FPRINT(z);

  END_MAIN
  return 0;
}
