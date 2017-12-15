DECLARE_RESOURCES

int main(void)
{ 
  INIT_MAIN

  float t;
  int i,n;
  t=1;
  n = 200;
  for (i=1; i<=n; i++) {
    t=t*(sqrt(5)-1)/2;
    FPRINT(t);
  } 
  FPRINT(t);

  END_MAIN
  return 0;
}
