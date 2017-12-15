DECLARE_RESOURCES

int main() {
  INIT_MAIN

  double x, loop;
  int i, n;
  loop = DBETWEEN(0, 9.99);
  FLOAT_SPLIT_ALL(1, x >> double::end(), loop << double::end());
  n = (int) loop;
    
  x = DBETWEEN_WITH_ERROR(0.0,0.95,0.0,0.1); 

  for(i=0;i<n;i++) {
    x = x * x;
  };

  FLOAT_MERGE_ALL(1, x << double::end(), loop >> double::end());
  DPRINT(x);

  END_MAIN
  return 0;
}
