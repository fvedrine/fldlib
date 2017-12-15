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
  float E;
  int compteur;
  float result;

  R2[0] = 2.25;
  R2[1] = 1.1;
  R2[2] = 0;

  R1[0].x = 0;
  R1[1].x = 5;
  R1[2].x = 25;

  R1[0].y = 0;
  R1[1].y = R1[1].x * R2[0];
  R1[2].y = R1[1].y + (R1[2].x - R1[1].x) * R2[1];
  

  E = FBETWEEN(0.0,100.0);

  FLOAT_SPLIT_ALL(1, result >> double::end(), E << double::end())
  compteur = 0;
  while ((compteur<2) && (E >= R1[compteur+1].x))
    compteur++;
  result = (E-R1[compteur].x)*R2[compteur] + R1[compteur].y;
  FLOAT_MERGE_ALL(1, result << double::end(), E >> double::end())
  FPRINT(result);

  END_MAIN
  return 0;
}
