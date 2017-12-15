DECLARE_RESOURCES

int main() {
   INIT_MAIN

_FDL   double c = 100.4;
_FDL   double v = 1.0;
_FDL   double x = 0.0;
_FDL   double t = 0;
_FDL   FLOAT_SPLIT_ALL(1, t >> v >> c >> x >> double::end(), c << v << x << t << double::end())
_FDL   while (x <= c) {
_FDL      t += 1.0;
_FDL      x += v*1.0;
_FDL      FLOAT_SPLIT_ALL(2, v >> c >> x >> double::end(), c << v << x << double::end())
_FDL      // if (v*v >= 0.08*(c-x-0.20))
_FDL      if (v >= 0.06*(c-x-0.20)) {
_FDL         v -= 0.04;
_FDL         if (v <= 0.2 /* 0.02 */) {
_FDL            v = 0.2 /* 0.02 */;
_FDL         };
_FDL      };
_FDL      FPRINT(v);
_FDL      FLOAT_MERGE_ALL(2, x << c << v << double::end(), x >> v >> c >> double::end())
_FDL      c += DBETWEEN(-0.001, 0.001);
_FDL      FPRINT(x);
_FDL      FPRINT(c);
_FDL      FPRINT(v);
_FDL      FLOAT_SPLIT_ALL_LOOP(1)
_FDL   };
_FDL   FLOAT_MERGE_ALL(1, x << c << v << t << double::end(), t >> x >> v >> c >> double::end())
_FDL   FPRINT(v);
_FDL   FPRINT(t);
_FDL   FPRINT(x);
_FDL   FPRINT(c);

   END_MAIN
}

