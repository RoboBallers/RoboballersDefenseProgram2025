#include <movement.h>
#include <Arduino.h>
#include <zirconLib.h>
#include <math.h>
#include <algorithm>
#include<tuple>
#include<iostream>

movement::movement (CompassSensor& compassSensor) : compassSensor(compassSensor) {
    myPID = new PID(&Input, &Output, &Setpoint, kp, ki, kd, REVERSE);
    myPID->SetMode(AUTOMATIC);
}

int movement:: sign(double var) {
    if (var < 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void movement:: movementfunc(double degrees, double speedFactor, double desiredOrientation) {
  currMovementAngle = degrees;
    if (degrees >= 360) {
        degrees -= 360;
    }
    FrontRight = sin((degrees-theta1)*(PI/180.0));
    Back = sin((degrees-theta2)*(PI/180.0));
    FrontLeft = sin((degrees-theta3)*(PI/180.0));

    maxVal = max(max(abs(FrontRight), abs(FrontLeft)), abs(Back));

    FRpower  = FrontRight / maxVal;
    Backpower  = Back / maxVal;
    FLpower  = FrontLeft / maxVal;


    double correction = findCorrection(desiredOrientation);

    FRpower += correction;
    Backpower += correction;
    FLpower += correction;

    maxVal = max(max(abs(FrontRight), abs(FrontLeft)), abs(Back));

    FRpower  = FRpower / maxVal;
    Backpower  = Backpower / maxVal;
    FLpower  = FLpower / maxVal;

    if (FRpower > 1) {
      FRpower = 1;
    } else if (FRpower < -1) {
      FRpower = -1;
    }

    if (FLpower > 1) {
      FLpower = 1;
    } else if (FLpower < -1) {
      FLpower = -1;
    }

    if (Backpower > 1) {
      Backpower = 1;
    } else if (Backpower < -1) {
      Backpower = -1;
    }

// change back to 100
    motor3(255*speedFactor*abs(Backpower), sign(Back));
    motor2(255*speedFactor*abs(FRpower), sign(FrontRight));
    motor1(255*speedFactor*abs(FLpower), sign(FrontLeft));
}


double movement::findCorrection(double desiredOrientation) {
  double correction = 0;
  double orientationDiff = compassSensor.currentOffset() - desiredOrientation;

  Input = abs(orientationDiff);
  myPID->Compute();

  if (orientationDiff > 90) {
    correction = -1;
  } else if (orientationDiff < -90) {
    correction = 1;
  } else if (orientationDiff > 0) {
    correction = -1 * (Output / 100);
  } else if (orientationDiff < 0) {
    correction = (Output / 100);
  }

  // Serial.println("Correction: " + String(correction));

  return correction;
}

// double movement::findCorrection2() {
//   double correction = 0;
//   double orientationDiff = compassSensor.currentOffset();
//   Serial.println("Current Orientation Diff: " + String(orientationDiff));


//   Input2 = abs(orientationDiff);
//   myPID2->Compute();

//   if (abs(orientationDiff) < 5) {
//     correction = 0;
//   } else if (orientationDiff > 90) {
//     correction = -1;
//   } else if (orientationDiff < -90) {
//     correction = 1;
//   } else if (orientationDiff > 0) {
//     correction = -1 * (Output2 / 100);
//   } else if (orientationDiff < 0) {
//     correction = (Output2 / 100);
//   }


//   return correction;
// }


void movement::stopMotors() {
    motor1(0, 0);
    motor2(0, 0);
    motor3(0, 0);
}