class State {
  class Param {
    String name;
    float value;
    float min;
    float max;
    float step;

    Param(String name, float value, float min, float max, float step) {
      this.name = name;
      this.value = value;
      this.min = min;
      this.max = max;
      this.step = step;
    }

    public void increment() {
      value += step;
      if (value > max) {
        value -= max - min;
      }
    }


    public void decrement() {
      value += step;
      if (value < min) {
        value += max - min;
      }
    }
  }

  boolean running = false;
  Param[] params = {new Param("max P", 40, 1, 100, 5),
    new Param("rise time", 4, 1, 60, 1),
    new Param("fall time", 4, 1, 60, 1)};
  int activeParamIndex = 0;
  boolean edit = false;

  final int fontSize = 21;
  final int activeMargin = 6;

  public void init() {
    strokeWeight(4);
    textSize(fontSize);
    textAlign(LEFT, TOP);
  }

  public void turnOn() {
    println("turn on");
    this.running = true;
  }

  public void turnOff() {
    println("turn off");
    this.running = false;
  }

  public void toggleRunning() {
    if (this.running) {
      this.turnOff();
    } else {
      this.turnOn();
    }
  }

  public void toggleMode() {
    edit = !edit;
  }

  public void up() {
    if (this.edit) {
      params[activeParamIndex].increment();
    } else {
      activeParamIndex--;
      if (activeParamIndex == -1) {
        activeParamIndex = params.length - 1;
      }
    }
  }

  public void down() {
    if (this.edit) {
      params[activeParamIndex].decrement();
    } else {

      activeParamIndex++;
      if (activeParamIndex == params.length) {
        activeParamIndex = 0;
      }
    }
  }

  public void draw() {
    fill(255, 255, 255);
    text(this.running ? "ON" : "OFF", 10, 10);

    if (edit) {
      fill(100, 100, 100);
      noStroke();
    } else {
      noFill();
      stroke(250, 255, 83);
    }

    rect(10 - activeMargin, 60 - activeMargin + activeParamIndex * 50, 290, 27);

    fill(100, 100, 100);
    for (int i = 0; i < params.length; i++) {
      if (i == activeParamIndex) {
        fill(255, 255, 255);
      }

      text(params[i].name, 15, 60 + i * 50);
      text(params[i].value, 200, 60 + i * 50);

      if (i == activeParamIndex) {
        fill(100, 100, 100);
      }
    }
  }
}

State state = new State();

void setup() {
  size(320, 240);
  state.init();
  textFont(createFont("VCR_OSD_MONO_1.001.ttf", 21));
}

void draw() {
  background(0, 0, 0);
  state.draw();
}

void keyPressed() {
  if (keyCode == 32) {// SPACE
    state.toggleRunning();
  } else if (keyCode == UP) {//UP
    state.up();
  } else if (keyCode == DOWN) {
    state.down();
  } else if (key == ENTER) {// enter
    state.toggleMode();
    println("Enter");
  }
}
