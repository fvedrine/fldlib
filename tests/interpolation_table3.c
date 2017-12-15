DECLARE_RESOURCES

double x[10] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
double y[10] = { 0.0, 2.0, 3.0, 2.0, 0.0, -2.0, -3.0, -1.9, 0.1, 2.1 };
int MIN, MAX;

int main() {
  INIT_MAIN
  MIN = 0; MAX = 9;

  double in = DBETWEEN_WITH_ERROR(0, 10, -1e-6, 1e-6);
  double out;

  FLOAT_SPLIT_ALL(1, out >> double::end(), in << double::end())

  int index = (int) in;
  DPRINT(in);
  if (index < 0)
    out = y[0];
  else if (index >= 9)
    out = y[9];
  else
    out = y[index] + (in - index) * (y[index+1] - y[index]);
  DPRINT(out);

  FLOAT_MERGE_ALL(1, out << double::end(), in >> double::end())
  DPRINT(out);

  END_MAIN
}

