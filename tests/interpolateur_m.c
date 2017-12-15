DECLARE_RESOURCES

typedef struct 
{
  float x;
  float y;
} paire;

int main()
{
  INIT_MAIN

  paire R1[3];
  float R2[3];
  float E, res;

  R2[0] = 2.25;
  R2[1] = 1.1;
  R2[2] = 0;

  R1[0].x = 0;
  R1[1].x = 5;
  R1[2].x = 25;

  R1[0].y = 0;
  R1[1].y = R1[1].x * R2[0];
  R1[2].y = R1[1].y + (R1[2].x - R1[1].x) * R2[1];
  

  E = FBETWEEN(0.0,10.0);
  E = E*10;
  FPRINT(E);

  FLOAT_SPLIT_ALL(1, res >> double::end(), E << double::end())
  if (E < R1[1].x)
    res = (E-R1[0].x)*R2[0] + R1[0].y;
  else if (E < R1[2].x)
    res = (E-R1[1].x)*R2[1] + R1[1].y;
  else
    res = (E-R1[2].x)*R2[2] + R1[2].y;
  FLOAT_MERGE_ALL(1, res << double::end(), E >> double::end())

  FPRINT(res);

  END_MAIN
  return 0;
}
