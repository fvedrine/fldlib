DECLARE_RESOURCES

#ifdef DBL_EPSILON
#undef DBL_EPSILON
#endif

#define DBL_EPSILON 2.2204460492503131e-16

#define HORNER 1

typedef struct _test_SQR2
{
  double v0, v,r, er, er1;
} Test_Sqr2;


double i1,p2, y, r, rp;
int i, p; 

double fabs2(double x){
  if( x < 0) 
    return -x;
  else
    return x;
}

double cal(double r)
{
        r = r - 1.0;
#if HORNER
        r = ((r/16.0 - 1/8.0)*r + 1.0/2.0)*r + 1.0;
#else
        r = 1.0+.5*r-1/8.0*r*r+1/16.0*r*r*r;
#endif
        return r;
}

double sqrt2(double x)
{
  double sqrt_2 ;
  double sqrt_5 ;
_FDL sqrt_2 = 1.414213562373095;
_FDL sqrt_5 = 0.7071067811865475;
_FDL if( x == 0.0 )
    return 0.0;
  else if(x > 1.0) {
_FDL   p = 0; p2 = 1.0;
_FDL   y = x;
_FDL   while(y > 1.0)
         {
_FDL       y /= 2.0;
_FDL       p++;
_FDL       p2 = p2 * sqrt_2;
_FDL     }
_FDL   r = cal(y);
_FDL   rp = r*r;
_FDL   for(i=0; i < 5 ; i++)
         {
_FDL       rp = r;
_FDL       r = (rp + y / rp)/2.0 ;
_FDL     }
_FDL   i1 = (i-1);
_FDL   return r*p2;
  }
  else if (x == 1.0)
    {
_FDL     r=1.0;
_FDL     return r;
    }
  else if( x > 0.5)
    {
_FDL     y=x; 
_FDL     p = 0; 
_FDL     p2 = 1.0;
_FDL     r = cal(y);
_FDL     rp = r*r;
_FDL     for(i=0; i < 5 ; i++)
           {
_FDL         rp = r;
_FDL         r = (rp + y / rp)/2.0 ;
_FDL       }
_FDL     i1 = (i-1);
_FDL     return r*p2;
    }
  else if( x == 0.5)
    {
_FDL     r =  sqrt_5;
_FDL     return r;
    }
  else if (x < 0.5)
    {
_FDL     p = 0; 
_FDL     p2 = 1.0;
_FDL     y = x;
_FDL     while( y < 0.5)
           {
_FDL         y *= 2.0;
_FDL         p++;
_FDL         p2 = p2 / sqrt_2;
_FDL       }
_FDL     p = -p;
_FDL     r = cal(y);
_FDL     rp = r*r;
_FDL     for(i=0; i < 5 ; i++)
           {
_FDL         rp = r;
_FDL         r = (rp + y / rp)/2.0 ;
_FDL       }
_FDL     i1 = (i-1);
_FDL     return r*p2;
    }
  return 0;
}

int main()
{
  INIT_MAIN

  Test_Sqr2 t;
  double should_be_zero, min, max, courant;
  int j;
_FDL min = 4.0*(1.0+DBL_EPSILON);
_FDL max = 8.0;
_FDL t.v = DBETWEEN(4.0*(1.0+DBL_EPSILON),8.0);
_FDL t.r = 2.0;
_FDL for (j=0; j<5; j++) {
_FDL   courant = sqrt2(t.v);
_FDL   t.r = courant;
_FDL   DPRINT(t.r);
_FDL   should_be_zero = courant*courant-t.v;
_FDL   DPRINT(should_be_zero);
_FDL   t.v = 2*t.v;
_FDL }

  END_MAIN
  return 0;
}

