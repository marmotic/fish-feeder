#include <AccelStepper.h>

const int stepsPerRevolution = 2048;
const int stepsPerContainer = -330;
const int firstContainerExtraSteps = -30; // first container needs to move some extra steps
const int containers = 21;
const int feedingIntervalInMillis = 24*60*60*1000;
int totalStepsTravelled = 0;
int currentContainer = 0;
int nextDoseTime = millis();
int paused = false;

// ULN2003 Motor Driver Pins
#define IN1 D5
#define IN2 D6
#define IN3 D7
#define IN4 D8

// initialize the stepper library
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);

  delay(5000);

  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(200);
}

void loop() {
  if (!stepper.isRunning() && !paused) {
    if (currentContainer < containers) {
      int currentTime = millis();
      if (currentTime > nextDoseTime) {
        moveOneContainer();
      } 
    } else {
      resetToInitial();
    }
  }

  stepper.run();
}

void moveOneContainer() {
  bool firstContainer = currentContainer == 0;
  int stepsToMove = stepsPerContainer + (firstContainer ? firstContainerExtraSteps : 0);
  stepper.move(stepsToMove);
  totalStepsTravelled += stepsToMove;
  int currentTime = millis(); 
  nextDoseTime = currentTime + feedingIntervalInMillis;
  currentContainer += 1;
}

void resetToInitial() {
  stepper.move(-totalStepsTravelled);
  totalStepsTravelled = 0;
  paused = true;
}
