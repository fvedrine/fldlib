/*
 * Second order filter
 * ============
 *  - Sensors are modeled by "DBETWEEN" assertions
 *  - Output is stored in variable S
 *
 * Fluctuat analysis tips
 * =============
 *  - Increase analyzer internal accuracy to 500 bits and unfold the entire loop.
 */

DECLARE_RESOURCES

int main(int argc, char** argv) {
  INIT_MAIN

  double S,S0,S1,E,E0,E1;
  int i,N;

  S = 0.0;
  S0= 0.0;
  S1= 0.0;
  E = DBETWEEN(-1.0,1.0);
  E0= DBETWEEN(-1.0,1.0);
  N = 200;

  for (i=1;i<N;i++) {
    E1 = E0;
    E0 = E;
    E  = DBETWEEN(-1.0,1.0);
    S1 = S0;
    S0 = S;
    S  = 0.7*E - 1.3*E0 + 1.1*E1 + 1.4*S0 - 0.7*S1; 
    DPRINT(S0);
    DPRINT(S1); 
  }
  DPRINT(S);

  DSENSITIVITY(S);

  END_MAIN
}

