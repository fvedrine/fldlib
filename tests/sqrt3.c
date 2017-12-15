DECLARE_RESOURCES

// #define _EPS 0.000001
#define _EPS 0.00001
#define _15SUR8     1.875
#define _5SUR4      1.25
#define _3SUR8      0.375

int main ()
{   
  INIT_MAIN
  double xn, xnp1, residu, lsup, linf;
  double Input, Output;
  int i, cond;
  Input = DBETWEEN(16.0,16.1); 

  FLOAT_SPLIT_ALL(1, Input >> xn >> double::end(), xn << Input << double::end())
  if (Input <= 1.0) 
    xn = 1.0; 
  else 
    xn = 1.0/Input;
  FLOAT_MERGE_ALL(1, xn << Input << double::end(), Input >> xn >> double::end())

  xnp1 = xn;    
  residu = 2.0*_EPS*(xn+xnp1);
  lsup = _EPS * (xn+xnp1);     
  linf = -lsup;
#if defined(FLOAT_DIAGNOSIS) && defined(FLOAT_INTERVAL) && defined(FLOAT_LOOP_UNSTABLE)
  int loop_counter = 0;
#endif
  FLOAT_SPLIT_ALL(2, xnp1 >> double::end(), residu << lsup << linf << xn << xnp1 << Input << double::end())
  cond = ((residu > lsup) || (residu < linf)) ;      
  while (cond)
  { 
    xnp1 = xn * (_15SUR8 + Input*xn*xn*(-_5SUR4+_3SUR8*Input*xn*xn));
    residu = 2.0*(xnp1-xn);
    DPRINT(xnp1);
    DPRINT(residu);
    xn = xnp1;
    lsup = _EPS * (xn+xnp1);     
    linf = -lsup;
    DPRINT(residu);
    DPRINT(lsup);
    DPRINT(linf);
    FLOAT_SPLIT_ALL_LOOP(2)
    cond = ((residu > lsup) || (residu < linf)) ;
#if defined(FLOAT_DIAGNOSIS) && defined(FLOAT_INTERVAL) && defined(FLOAT_LOOP_UNSTABLE)
    ++loop_counter;
    if (loop_counter >= 1000)
        return 0;
#endif
  }
  FLOAT_MERGE_ALL(2, xnp1 << double::end(), Input >> xnp1 >> xn >> linf >> lsup >> residu >> double::end())
  Output = 1.0 / xnp1; 
  DPRINT(Output);

  END_MAIN
  return 0;
}
