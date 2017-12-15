DECLARE_RESOURCES

#define T1   1.0
#define T2   0.176575
#define R1 -14.13341245
#define SI  41.17619681
#define A0   1.0
#define A1  -6.0000000056
#define A2  -3.3333337415
#define A3  11.4544368439
#define A4   9.6717198021
#define A5  -8.6822950335
#define TRUE  1
#define FALSE 0
#define PI  3.141592654
#define DPI 6.283185308
#define PID 1.570796327
#define PIQ 0.785398164

#define fonc_sin sin2
/* #define fonc_sin sin4 */ /* TRY THIS IF YOU WANT */

typedef double value;

static struct SSA {
  value A0S, AS, BS, CS, DS, ES; 
} SS = {
  A0,
  (A4*A5+A2*(A3+A5)),
  (A2*A3*A4*A5),
  (A1+A3+A5),
  (A1*A2*(A3+A5)+(A1+A3)*A4*A5),
  (A1*A2*A3*A4*A5) 
};

typedef struct _test
{        
  value ac;
  value s1, c1, ss;
} Test;

value a0,s1,c1;
value s2,c2;
value norme1,norme2;
value s1er, c1er;
value err = 4.0e-7;
int num;
value i1;
value _C0, _C1, _C2, _C3, _C4, _C5;

value fabs2(value x){
  if( x < 0) 
    return -x;
  else 
    return x;
}

value TRUNC( value x){
  int tr;
  tr = (int) x;
  return(tr);
}

value F_SQRT(value x) {
  /* sqrt for x between 0.5 and 1 */
  value res;
  /*res = sqrt(x);*/ 
  res = _C1+(_C2+(_C3+(_C4+_C5*x)*x)*x)*x; /* UNCOMMENT IF YOU WANT */
  return res;
}

value sin1( value x){
  value x2 = x*x;
  return( ((R1 / (x2+SI)+ T2) * x2 + T1) * x);
}

value sin2( value x){
  value x2 = x*x;
  return((((-x2/(6.0*7)+1.0)*x2/(5.0*4)-1.0)*x2/(2.0*3)+1.0)*x);
}

value sin3( value x){
  value x2 = x*x;
  return( (x2/(x2/(x2/(x2/(x2/A5+A4)+A3)+A2)+A1)+A0)*x );
}

value sin4( value x){
  value x2 = x*x;
  return( ((((x2+SS.AS)*x2+SS.BS)*x2)
           /((SS.CS*x2+SS.DS)*x2+SS.ES)+SS.A0S)*x );
}

void sincospi2(value x, value *const sinx, value *const cosx){
  value vax,z1;
  value yy1, a;
  
  if(x < 10.0e-10){    /* epsilon */
      yy1 = x;
      z1 = F_SQRT((value)(1.0-yy1*yy1));
  }else{
    if(x == PIQ){
      yy1 = F_SQRT((value)2.0)/2.0;
      z1 = F_SQRT((value)(1.0-yy1*yy1));
    }else{ 
      if(x > PIQ){                     
        x = (PI/2.0)-x;
        z1 = fonc_sin(x);
        yy1 = F_SQRT((value)(1.0-z1*z1));
      }else{
        yy1 = fonc_sin(x);
        z1 = F_SQRT((value)(1.0-yy1*yy1));
      }
    }
  }
  a = x;
  *sinx = yy1;
  *cosx = z1;
}

void sincos1(value x, value *const sinx, value *const cosx){
  value vax,z1;
  value yy1, a;
  vax = fabs2(x);
  if(vax < 1.0e7){
      if (x < 0.0){
        x=vax;
        if(x > DPI)
          x -= TRUNC(x/(2.0*PI))*2.0*PI;
        if(x > PI){
          x -= PI;
          if(x > PID){
            x = PI - x;
            sincospi2(x, &yy1, &z1);
          }else{
            sincospi2(x, &yy1, &z1);
            z1 = - z1;
          }
        }else{
          if(x > PID){
            x = PI - x;
            sincospi2(x, &yy1, &z1);
            z1 = - z1;
            yy1 = - yy1;
          }else{
            sincospi2(x, &yy1, &z1);
            yy1 = - yy1;
          }
        }
      }else{
        if(x > DPI)
          x -= TRUNC(x/(2.0*PI))*2.0*PI;
        if(x > PI){
          x -= PI;
          if(x > PID){
            x = PI - x;
            sincospi2(x, &yy1, &z1);
            yy1 = - yy1;
          }else{
            sincospi2(x, &yy1, &z1);
            yy1 = - yy1;
            z1 = - z1;
          }
        }else{
          if(x > PID){
            x = PI - x;
            sincospi2(x, &yy1, &z1);
            z1 = - z1;
          }else
            sincospi2(x, &yy1, &z1);
        }
      }
  }else
    yy1 = 9.9e+27;
  
  a = x;
  *sinx = yy1;
  *cosx = z1;
}

int main(){
  INIT_MAIN

  value should_be_zero, result;
  Test test;
  _C0 = 1.414213538169860839843750;
  _C1 =  0.2297613918781280517578125;
  _C2 =  1.296735525131225585937500;
  _C3 = -0.9010983705520629882812500;
  _C4 =  0.4935534000396728515625000;
  _C5 = -0.1189586669206619262695313125;
  test.ac = DBETWEEN(PI/10.0,PI/4.0);
  sincos1(test.ac,&test.s1,&test.c1);
  result = test.s1;
  should_be_zero = test.s1*test.s1+test.c1*test.c1-1.0;
  DPRINT(result);
  DPRINT(should_be_zero);

  SS.A0S = SS.AS = SS.BS = SS.CS = SS.DS = SS.ES = 0;
  a0 = s1 = c1 = s2 = c2 = norme1 = norme2 = s1er = c1er = err = i1 = 0;
  _C0 = _C1 = _C2 = _C3 = _C4 = _C5 = 0;

  END_MAIN

  return 0;
}
