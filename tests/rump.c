DECLARE_RESOURCES

int main(){
  INIT_MAIN

  double f;
  double x = 77617;
  double y = 33096;

  f = 333.75*y*y*y*y*y*y;
  f += x*x*(11*x*x*y*y - y*y*y*y*y*y - 121*y*y*y*y - 2);
  f += 5.5*y*y*y*y*y*y*y*y;
  f += x / (2*y);

  END_MAIN
  return 0;
}
