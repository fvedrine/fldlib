/*
 * Absorption
 * =======
 *  - At the end of program, 'x' is equal to 'z' due to absorption phenomenon.
 *
 * Fluctuat analysis tips
 * =============
 *  - Floating-point value of 'z' is 1.0, but real value is different,
 *  - Switch to extended precision to improve the accuracy.
 */

DECLARE_RESOURCES

int main(void)
{
  INIT_MAIN

  float x,y,z;

  x = 1.0F;
  y = ((float) 1.0F)/(((float) 10000.0F)*((float) 10000.0F));
  FPRINT(y);
  z = x + y;
  FPRINT(z);

  END_MAIN

  return 0;
}
