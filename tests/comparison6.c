DECLARE_RESOURCES

int main()
{
  INIT_MAIN

  float R1[3], E, res;
  R1[0] = 0;
  R1[1] = 5*2.25;
  R1[2] = R1[1] + 20*1.1;

  E = FBETWEEN_WITH_ERROR(0.0, +100.0, -1e-5, +1e-5);
  FLOAT_SPLIT_ALL(1, res >> double::end(), E << double::end())
  if (E < 5)
    res = E*2.25 + R1[0];
  else if (E < 25)
    res = (E-5)*1.1 + R1[1];
  else
    res = R1[2];

  FLOAT_MERGE_ALL(1, res << double::end(), E >> double::end())
  FPRINT(res);

  END_MAIN
  return 0;
}
