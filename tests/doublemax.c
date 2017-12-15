DECLARE_RESOURCES

int main() {
  INIT_MAIN

  float a, b, zero, max1, max2;
  a = FBETWEEN(-10.0, 10.0);
  b = FBETWEEN(-10.0, 10.0);
  zero = 0.0;
  FLOAT_SPLIT_ALL(1, max2 >> max1 >> b >> a >> double::end(), a << b << double::end())
  if (a < zero)
    max1 = zero;
  else
    max1 = a;
  if (b < zero)
    max2 = zero;
  else
    max2 = b;
  FLOAT_MERGE_ALL(1, a << b << max1 << max2 << double::end(), b >> a >> double::end())
  FPRINT(a);
  FPRINT(b);
  FPRINT(max1);
  FPRINT(max2);

  END_MAIN
  return 0;
}
