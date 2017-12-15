DECLARE_RESOURCES

#define GEAR 4

float table1[2][GEAR] = { { 0.0, 1.0, 2.0, 3.0 }, { 0.0, 0.0, 1000.0, 1000.0 } };
float table2[2][GEAR] = { { 0.0, 1.0, 2.0, 3.0 }, { 0.0, 0.0, 0.0, 1000.0 } };

float lookup(float table[2][GEAR], float gear) {
  int i;
  float result;
  FLOAT_SPLIT_ALL(1, result >> double::end(), gear << double::end())
  if (gear < table[0][0])
    result = table[1][GEAR];
  else if (gear >= table[0][GEAR-1])
    result = table[1][GEAR-1];
  else {
    int hasFound = 0;
    for (i = 0; !hasFound && i < GEAR; ++i)
      if (gear <= table[0][i+1]) {
        hasFound = 1;
        result = table[1][i] + (table[1][i+1]-table[1][i])*(gear - table[0][i])
                 / (table[0][i+1]-table[0][i]);
      };
    if (!hasFound)
      result = table[1][GEAR-1];
  }
  FLOAT_MERGE_ALL(1, result << double::end(), gear >> double::end())
  return result;
}

void calc_torque(float angle, float speed, float* pressure1, float* pressure2)
{
  float val, gear;
  FLOAT_SPLIT_ALL(1, *pressure2 >> *pressure1 >> double::end(), angle << speed << double::end())
  if (angle <= 45)
    val = 60;
  else
    val = 70;
  if ( 3 * speed <= val)
    gear = 3;
  else
    gear = 4;
  *pressure1 = lookup(table1, gear);
  *pressure2 = lookup(table2, gear);
  FLOAT_MERGE_ALL(1, *pressure1 << *pressure2 << double::end(), speed >> angle >> double::end())
}

int main()
{
  INIT_MAIN

  float angle, speed, pressure1, pressure2;
  angle = FBETWEEN_WITH_ERROR(0.0, 90.0, -1e-4, +1e-4);
  speed = FBETWEEN_WITH_ERROR(0.0, 40.0, -1e-4, +1e-4);
  calc_torque(angle, speed, &pressure1, &pressure2);
  FPRINT(pressure1);
  FPRINT(pressure2);

  END_MAIN
  return 0;
}
