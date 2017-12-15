DECLARE_RESOURCES

int main()
{
  INIT_MAIN

  float x,y;
_FDL  x = FBETWEEN_WITH_ERROR(1, 3, -0.1, +0.1);
_FDL  FLOAT_SPLIT_ALL(1, y >> double::end(), x << double::end())
_FDL  if (x <= 2) {
_FDL    y = x-1;
_FDL    if (x <= 1.95) {
_FDL      y = x-1.95;
_FDL    }
        else {
_FDL      y = 2*(x-1.95);
_FDL    }
_FDL  }
      else {
_FDL    y = x-2;
_FDL  };
_FDL  FLOAT_MERGE_ALL(1, y << double::end(), x >> double::end())
_FDL  FPRINT(y);

  END_MAIN
  return 0;
}
