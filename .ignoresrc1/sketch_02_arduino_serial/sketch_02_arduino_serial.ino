#include <AFMotor.h>

float lerp(float value, float from_min, float from_max) {
  return (value - from_min) / (from_max - from_min);
}

float lerp(float value, float from_min, float from_max, float to_min, float to_max) {
  return lerp(value, from_min, from_max) * (to_max - to_min) + to_min;
}


struct Time {
  uint64_t start_ms = 0;
  uint64_t current_ms = 0;
};

struct State {
  AF_DCMotor motor = AF_DCMotor(1);

  static constexpr uint64_t PARAM_COUNT = 6;
  char in_params[PARAM_COUNT];

  bool running = false;
  float out_params[PARAM_COUNT] = {
    40,    // max P (mm Hg)
    0.3,   // session duration (min)
    4000,  // ramp_up_ms
    4000,  // steady_high_ms
    4000,  // ramp_down_ms
    2000   // steady_low_ms
  };

  void deflate() {
    Serial.println("deflating...");
    // release solenoid
    // TODO
  }

  void off() {
    running = false;
    motor.run(RELEASE);
    deflate();
  }

  void on() {
    running = true;
    Serial.println("on");
  }

  float ramp_speed(Time& time) {
    // 0 -ramp_up- t1 -steady_high- t2 -ramp_down- t3 -steady_low- t4

    const int t1 = out_params[2];
    const int t2 = t1 + out_params[3];
    const int t3 = t2 + out_params[4];
    const int t4 = t3 + out_params[5];

    time.current_ms = millis();
    const uint64_t elapsed_ms = time.current_ms - time.start_ms;

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

  // Non-blocking serial read
  // https://forum.arduino.cc/t/serial-input-basics-updated/382007
  // cud even do while(read()){} so if many reach at once?

  void read(State& state) {
    if (Serial.available() <= 0) { return; }

    const char ch = Serial.read();

    if (ch == '\n') {
      // Serial.print("processing... ");
      // Serial.println((int)length);

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
    // Serial.print((int)length);
    // Serial.print(' ');
    // Serial.println(buffer[0]);

    if (length == 1) {
      // Serial.print(int(buffer[0]));
      if (buffer[0] == 48) {
        state.off();
      } else if (buffer[0] == 49) {
        state.on();
      }
    } else if (length == 2) {
      Serial.print("got: ");
      Serial.print(int(buffer[0]));
      Serial.print(' ');
      Serial.println(int(buffer[1]));

      // TODO put a mapping function or a LUT instead of float
      state.out_params[buffer[0]] = float(buffer[1] - 48) * 1000;
      // Serial.println(state.out_params[buffer[0] - 48]);
      Serial.println(state.out_params[3]);
    }
  }

  void clear() {
    length = 0;
  }
};

Time time;
State state;
Reader reader;

uint64_t session_start = 0;

void setup() {
  Serial.begin(9600);
  delay(3000);
}

void loop() {
  reader.read(state);

  // Serial.println((float(millis()) - session_start) / (1000 * 60) > 0);
  // TODO
  if ((float(millis()) - session_start) / (1000 * 60) > state.out_params[1]) {
    session_start = 0;
    state.running = false;
    Serial.println("session done!");
  }

  if (!state.running) { return; }
  float factor = 255 * state.ramp_speed(time);
  state.motor.setSpeed(factor);
  state.motor.run(FORWARD);
}
