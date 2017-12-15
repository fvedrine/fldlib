DECLARE_RESOURCES

int main()
{
  INIT_MAIN

  float x,y,z;
  x = FBETWEEN_WITH_ERROR(1.0, 3.0, -1e-7, +1e-7);
  FLOAT_SPLIT_ALL(1, z >> y >> x >> double::end(), x << double::end())
  if (x <= 2.0F) {
    y = x+2;
    z = x*x;
  }
  else {
    y = x;
    z = x*x;
  };
  FLOAT_MERGE_ALL(1, x << y << z << double::end(), x >> double::end())
  FPRINT(x);
  FPRINT(y);
  FPRINT(z);

  END_MAIN
  return 0;
}
