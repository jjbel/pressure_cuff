struct Param {
  const char *name;
  float value = 0;
  float inc = 1;
  float dec = 2;
  float min = 0;
  float max = 10;

  void increment() {
    value += inc;
    if (value > max) {
      value = max;
    }
  }

  void decrement() {
    value -= dec;
    if (value < min) {
      value = min;
    }
  }
};
