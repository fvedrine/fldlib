DECLARE_RESOURCES

int main() {
   INIT_MAIN

   double c = 100.4;
   double v = 1.0;
   double x = 0.0;
   double t = 0;
   FLOAT_SPLIT_ALL(1, t >> v >> c >> x >> double::end(), c << v << x << t << double::end())
   while (x <= c) {
      t += 1.0;
      x += v*1.0;
      FLOAT_SPLIT_ALL(2, v >> c >> x >> double::end(), c << v << x << double::end())
      if (v*v >= 0.08*(c-x-0.20))
      { // unfortunately, no constraint on v
         v -= 0.04;
         if (v <= 0.2 /* 0.02 */)
            v = 0.2 /* 0.02 */;
      };
      DPRINT(v);
      // big error on v: >= 0.04
      FLOAT_MERGE_ALL(2, x << c << v << double::end(), x >> v >> c >> double::end())
      c += DBETWEEN(-0.001, 0.001);
      DPRINT(x);
      DPRINT(c);
      DPRINT(v);
      FLOAT_SPLIT_ALL_LOOP(1)
   };
   FLOAT_MERGE_ALL(1, x << c << v << t << double::end(), t >> x >> v >> c >> double::end())
   // v <= 0.04
   // t <= 110.0
   DPRINT(v);
   DPRINT(t);
   DPRINT(x);
   DPRINT(c);

   END_MAIN
}

