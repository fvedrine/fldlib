DECLARE_RESOURCES

double x[10] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
double y[10] = { 0.0, 2.0, 3.0, 2.0, 0.0, -2.0, -3.0, -1.9, 0.1, 2.1 };
int MIN, MAX;

int main() {
  INIT_MAIN
  MIN = 0;
  MAX = 9;

  double in = DBETWEEN(0, 10);
  double out, out1;
  double in2 = in;

  FLOAT_SPLIT_ALL(1, out >> in >> double::end(), in << double::end())

  int index = (int) in;
  DPRINT(in); in.flushOut();
  if (index < 0)
    out = y[0];
  else if (index >= 9)
    out = y[9];
  else
    out = y[index] + (in - index) * (y[index+1] - y[index]);
  DPRINT(out);

  FLOAT_MERGE_ALL(1, out << in << double::end(), in >> double::end())
  DPRINT(out);
  DPRINT(in2);
  in = in2;

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

