#ifndef MedianFilter_h
#define MedianFilter_h


class MedianFilter
{
  public:
    MedianFilter(int defaultValue = 150); //(int tempFilterLength)
    void UpdateFilter(int newValueAsInt);
    int getFilterValue();

  private:
    int filterLength = 7;
    float values[7];
    int rank[7];
    int currentPositionInFilter;
};


#endif
