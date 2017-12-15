DECLARE_RESOURCES

int main()
{
  INIT_MAIN

  double sqrt2 = 1.41421353816986083943750;
  double S, I;
  I = FBETWEEN_WITH_ERROR(1.0, 2.0, 0, 0.001);
  FLOAT_SPLIT_ALL(1, S >> double::end(), I << double::end())
  if (I >= 2)
    S = sqrt2*(1+(I/2-1)*(.5-0.125*(I/2-1)));
  else
    S = 1 + (I-1)*(.5 + (I-1)*(-.125+(I-1)*.0625));
  FLOAT_MERGE_ALL(1, S << double::end(), I >> double::end())
  FPRINT(S);

  END_MAIN
  return 0;
}
