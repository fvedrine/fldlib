/*
 * Patriot missible scheme
 * ==============
 *  - For each iteration of the loop, an imprecision is added.
 *
 * Fluctuat analysis tips
 * =============
 *  - Check the error of 'delta',
 *  - Unfold the loop and print evolution of 't' error.
 */

DECLARE_RESOURCES

int main() {
  INIT_MAIN
  float t=0, delta;
  int i, n = 500;
  delta = FFROM_STRING(0.1);

  for (i=0;i<n;i++) {
    t = t + delta;
    FPRINT(t);
  }

  END_MAIN
  return 0;
}
