DECLARE_RESOURCES

#define _EPS 0.000001
#define _15SUR8     1.875
#define _5SUR4      1.25
#define _3SUR8      0.375

int main ()
{   
  INIT_MAIN

  double xn, xnp1, residu, lsup, linf, Output, should_be_zero;
  int i, cond;
  double Input = DBETWEEN(16.0,16.1); 
  if (Input <= 1.0) 
    xn = 1.0; 
  else 
    xn = 1.0/Input;
  xnp1 = xn;    
  residu = 2.0*_EPS*(xn+xnp1);
  lsup = _EPS * (xn+xnp1);     
  linf = -lsup;
  cond = ((residu > lsup) || (residu < linf)) ;      
  i = 0;
#if defined(FLOAT_DIAGNOSIS) && (defined(FLOAT_INTERVAL) || defined(FLOAT_AFFINE)) && defined(FLOAT_LOOP_UNSTABLE)
  int loop_counter = 0;
#endif
  while (cond)
  { 
    xnp1 = xn * (_15SUR8 + Input*xn*xn*(-_5SUR4+_3SUR8*Input*xn*xn));
    residu = 2.0*(xnp1-xn);
    DPRINT(xnp1);
    DPRINT(residu);
    xn = xnp1;
    lsup = _EPS * (xn+xnp1);     
    linf = -lsup;
    cond = ((residu > lsup) || (residu < linf)) ; i++; 
#if defined(FLOAT_DIAGNOSIS) && (defined(FLOAT_INTERVAL) || defined(FLOAT_AFFINE)) && defined(FLOAT_LOOP_UNSTABLE)
    ++loop_counter;
    if (loop_counter >= 100)
        return 0;
#endif
  }
  Output = 1.0 / xnp1; 
  should_be_zero = Output*Output-Input;

  END_MAIN
  return 0;
}
