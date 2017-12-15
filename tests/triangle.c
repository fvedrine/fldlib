DECLARE_RESOURCES

int main() {
  INIT_MAIN

  float a, b, c;
  float s,sa,sb,sc,A;
  a = 1.9999999;
  b = 1;
  c = 1;
  s = (a+b+c)/2;
  sa = s-a;
  sb = s-b;
  sc = s-c;
  A = sqrt(s*(s-a)*(s-b)*(s-c));
  FPRINT(A);

  END_MAIN
  return 0;
}
