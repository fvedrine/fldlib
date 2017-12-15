DECLARE_RESOURCES

int main(int argc, char** argv) {
  INIT_MAIN

  double x = 0.7;
  double y = x - (double) 1e-18;
  double z, w;
  int i;
  for (i = 0; i < 2; ++i) {
     FLOAT_SPLIT_ALL(1, w >> z >> y >> x >> double::end(), x << y << double::end())
     if (x <= y) {
       double u, v;
       z = y-x;
       FLOAT_SPLIT_ALL(2, y >> x >> w >> v >> u >> double::end(), z << x << y << double::end())
       if (x > y) {
         u = x/1.2;
         v = y/1.2;
         w = u-v;
       }
       else {
         u = y/1.2;
         v = x/1.2;
         w = u-v;
       }
       FLOAT_MERGE_ALL(2, u << v << w << x << y << double::end(), y >> x >> z >> double::end())
     }
     else {
       z = x-y;
       w = z/1.2;
     }
     FLOAT_MERGE_ALL(1, x << y << z << w << double::end(), y >> x >> double::end())
     DPRINT(x);
     DPRINT(y);
     DPRINT(z);
     DPRINT(w);
  };

  END_MAIN
  return 0;
}


