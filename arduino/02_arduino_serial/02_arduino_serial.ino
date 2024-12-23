#include <AFMotor.h>

float lerp(float value, float from_min, float from_max) {
  return (value - from_min) / (from_max - from_min);
}

float lerp(float value, float from_min, float from_max, float to_min, float to_max) {
  return lerp(value, from_min, from_max) * (to_max - to_min) + to_min;
}


struct Time {
  unsigned long start_ms = 0;
  unsigned long current_ms = 0;
};

struct State {
  int params[4] = {
    4000,  // ramp_up_ms
    4000,  // steady_high_ms
    4000,  // ramp_down_ms
    2000   // steady_low_ms
  };

  float max_pressure = 50;

  void deflate() {
    Serial.println("deflating...");
    // release solenoid
    // TODO
  }

  void off() {
    deflate();
  }

  float ramp_speed(Time& time) {
    // 0 -ramp_up- t1 -steady_high- t2 -ramp_down- t3 -steady_low- t4

    const int t1 = params[0];
    const int t2 = t1 + params[1];
    const int t3 = t2 + params[2];
    const int t4 = t3 + params[3];

    time.current_ms = millis();
    const unsigned long elapsed_ms = time.current_ms - time.start_ms;

    if (elapsed_ms > t4) {
      time.start_ms = millis();
      return 0;
    } else if (elapsed_ms > t3) {
      return 0;
    } else if (elapsed_ms > t2) {
      return 1 - lerp(elapsed_ms, t2, t3);
    } else if (elapsed_ms > t1) {
      return 1;
    } else {
      return lerp(elapsed_ms, 0, t1);
    }
  }
};

struct Reader {
  static constexpr uint64_t MAX_SIZE = 32;
  char buffer[MAX_SIZE];
  uint64_t length = 0;
  // bool newData

  void read(State& state) {
    if (Serial.available() <= 0) { return; }
    const char ch = Serial.read();
    if (ch == '\n') {
      process(state);
      clear();
    } else if (length == MAX_SIZE) {
      Serial.println("warning: buffer overflow");
      clear();
    } else {
      buffer[length] = ch;
      length++;
    }
  }

  void process(State& state) {
    if (length == 1) {
      state.off();
    } else if (length == 2) {
      // TODO
    }
  }

  void clear() {
    length = 0;
  }
};

AF_DCMotor motor1(1);
Time time;
State state;
Reader reader;


void setup() {
  Serial.begin(9600);
  delay(3000);
}

void loop() {
  // cud also use signed integers and declare var in if condn and use comma op
  float factor = 255 * state.ramp_speed(time);
  motor1.setSpeed(factor);
  motor1.run(FORWARD);

  if (Serial.available() > 0) {
    time.start_ms = millis();
    Serial.println(Serial.read());
  }
}
