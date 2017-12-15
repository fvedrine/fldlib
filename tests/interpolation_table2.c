DECLARE_RESOURCES

double x[10] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
double y[10] = { 0.0, 2.0, 3.0, 2.0, 0.0, -2.0, -3.0, -1.9, 0.1, 2.1 };
int MIN, MAX;

int main() {
  INIT_MAIN
  MIN = 0;
  MAX = 9;

  double in = DBETWEEN_WITH_ERROR(0, 10, -1e-8, +1e-8);
  double out1;

  FLOAT_SPLIT_ALL(2, out1 >> double::end(), in << double::end())

  int index1, index2;
  if (in < x[MIN])
    out1 = y[MIN];
  else if (in > x[MAX])
    out1 = y[MAX];
  else {
    index1 = MIN;
    index2 = MAX;
    while (index1 < index2-1) {
      int mid = (index1 + index2) / 2;
      if (in < x[mid])
        index2 = mid;
      else
        index1 = mid;
    };
    /* index1+1 >= index2 >= index1 */
    if (in < x[index2])
      out1 = y[index1] + (in - x[index1]) * (y[index1+1] - y[index1]) / (x[index1+1] - x[index1]);
    else if (index2 >= 9)
      out1 = y[9];
    else
      out1 = y[index2] + (in - x[index2]) * (y[index2+1] - y[index2]) / (x[index2+1] - x[index2]);
  };

  FLOAT_MERGE_ALL(2, out1 << double::end(), in >> double::end())
  DPRINT(out1);

  END_MAIN
}

