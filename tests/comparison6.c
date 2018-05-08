DECLARE_RESOURCES

int main()
{
  INIT_MAIN

  float R1[3], E, res;
  R1[0] = 0;
  R1[1] = 5*2.25;
  R1[2] = R1[1] + 20*1.1;

  E = FBETWEEN_WITH_ERROR(0.0, +100.0, -1e-5, +1e-5);

  struct ResultIterator {
    float* container;
    int row;

    ResultIterator(float* acontainer, int arow)
      : container(acontainer), row(arow) {}
    ResultIterator& operator++() { ++row; return *this; }
    int operator-(const ResultIterator& source) { return row - source.row; }
    float& operator*() const { return container[row]; }
    float* operator->() const { return &container[row]; }
    bool operator<(const ResultIterator& source) const
      { return row < source.row; }
    bool operator!=(const ResultIterator& source) const
      { return row != source.row; }
    typedef float value_type;
  };

  FLOAT_SPLIT_ALL(1,
        double::MergeBranches::packer(ResultIterator(R1, 0), ResultIterator(R1, 3)) >> res >> double::end(),
        E << double::MergeBranches::packer(ResultIterator(R1, 0), ResultIterator(R1, 3)) << double::end())
  if (E < 5)
    res = E*2.25 + R1[0];
  else if (E < 25)
    res = (E-5)*1.1 + R1[1];
  else
    res = R1[2];

  FLOAT_MERGE_ALL(1,
        res << double::MergeBranches::packer(ResultIterator(R1, 0), ResultIterator(R1, 3)) << double::end(),
        double::MergeBranches::packer(ResultIterator(R1, 0), ResultIterator(R1, 3)) >> E >> double::end())
  FPRINT(res);

  END_MAIN
  return 0;
}
