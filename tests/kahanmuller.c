/*
 * Kahan-Muller sequence
 * ===============
 *  - This sequence has two fixpoints, a repulsive (6) and an attractive (100).
 *  - Starting from the given initial values, the exact computation gives 6. But any imprecision leads to the attractive point 100,
 *
 * Fluctuat analysis tips
 * =============
 *  - Unfold the loop and increase the internal accuracy (MPFRbits) to follow the evolution of the imprecision.
 *  - In simple precision, attractive field catchs the sequence at the 7th iteration.
 *  - Increasing MPFRbits, it is possible to evaluate accurately the real value and errors due to float of xi.
 */

DECLARE_RESOURCES

int main(void)
{
  INIT_MAIN

  float x0, x1, x2;
  int i;
  int n;

  n = 100;
  x0 = 11/2.0;
  x1 = 61;
  x1 /= 11.0;

  for (i=1 ; i<=n ; i++)
  {  
    x2 = 111 - (1130 - 3000/x0) / x1;
    x0 = x1;
    x1 = x2; 
    FPRINT(x1);
   }

  END_MAIN
  return 0;
}
