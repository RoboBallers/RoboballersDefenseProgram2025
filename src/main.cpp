#include <Arduino.h>
#include <zirconLib.h>
#include <iostream>
#include <math.h>

#include <BallFinding.h>
#include <movement.h>
#include <LineAvoidance.h>
#include <CompassSensor.h>
#include <elapsedMillis.h>

// use compass sensor to continuously make sure that the attachment is in the front
// fix weird wheel thing



LineAvoidance line;
BallFinding ballFinding;
CompassSensor compassSensor;
movement Movement(compassSensor);

elapsedMillis lostBallTimer;
bool searchingForBall = false;
bool movingRight = true;

double correct_val;
double goalDirection;
double direction;
double difference;

bool start = false;
bool prevStart = LOW;
bool currentStart = LOW;

bool calibrateLine = false;
bool prevCalibrateLine = LOW;
bool currentCalibrateLine = LOW;

bool compassdone = false;
bool compassDone2 = false;

double universalSpeed = 0.45;

void setup() {
    InitializeZircon();
    Serial.begin(9600);
}


void lineTesting() {
    for (int i = 1; i <= 3; i++) {
        Serial.println("Sensor " + String(i) + ": " + String(readLine(i)));
    }
}

void IRtesting() {
    for (int i = 1; i <= 8; i++) {
        Serial.println("Ball Sensor " + String(i) + ": " + String(readBall(i)));
    }
    Serial.println("BallAngle: " + String(ballFinding.ballAngle()));
}

void searchForBallMotion() {
    if (movingRight) {
        Serial.println("Searching - moving right");
        Movement.movementfunc(90, 0.2,0);
    } else {
        Serial.println("Searching - moving left");
        Movement.movementfunc(270, 0.2,0);
    }

    if (lostBallTimer > 1000) { 
        movingRight = !movingRight;
        lostBallTimer = 0;
    }
}

void testingCompass() {
    currentStart = readButton(1);
    currentCalibrateLine = readButton(2);

    if (currentStart == HIGH && prevStart == LOW) {
        motor3(100,1);
        delay(100);
        start = !start;
    }

    if (currentCalibrateLine == HIGH && prevCalibrateLine == LOW) {
        motor2(100,1);
        delay(400);
        calibrateLine = !calibrateLine;
    }


    if (start) {
        Serial.println("Calibrating Comapss sensor");
        Movement.stopMotors();
        compassSensor.zeroedAngle = compassSensor.getOrientation();
        Serial.println("Compass Zeroed Angle: " + String(Movement.compassSensor.zeroedAngle));
    } else {
        Movement.movementfunc(315,0.3,0);
    }

    prevStart = currentStart;
    prevCalibrateLine = currentCalibrateLine;
}

void process() {
    currentStart = readButton(1);
    currentCalibrateLine = readButton(2);


    if (currentStart == HIGH && prevStart == LOW) {
        start = !start;
         if (start) {
             motor3(100,1);
             delay(100);
        }
    }

    if (currentCalibrateLine == HIGH && prevCalibrateLine == LOW) {
        calibrateLine = !calibrateLine;
        if (calibrateLine) {
            motor2(100,1);
            delay(400);
        }
    }

    if (start) {
        double lineAngle = line.avoidingLine(Movement.currMovementAngle);
            
        if (lineAngle != -1) {
            Movement.movementfunc(lineAngle, universalSpeed,0);
            delay(20);
        }
        // if (lineAngle != -1) {
        //     Serial.println("Line detected");
        //     delay(10);
        // }

        if (!ballFinding.isBallVisible()) {
            searchingForBall = true;
            searchForBallMotion();
            Serial.println("Ball not detected - entering search mode");
        } else {
            searchingForBall = false;
        }

        if (!searchingForBall) {
            double angle = ballFinding.ballAngle();

            Serial.println("Ball Angle: " + String(angle));

            if (30 < angle && angle < 90) {
                Serial.println("Going right as ball is on the right");
                Movement.movementfunc(105,universalSpeed,0);
            } else if (270 < angle && angle < 345) {
                Serial.println("Going left as ball Angle is on the left");
                Movement.movementfunc(265,universalSpeed,0);
            } else if (90 < angle && angle < 270) {
                Serial.println("Moving back");
                Movement.movementfunc(180, universalSpeed,0);
            } else {
                Movement.stopMotors();
            }
            // delay(40);
        }
      
    } else {
        Movement.stopMotors();
        compassSensor.zeroedAngle = compassSensor.getOrientation(); // + 10 previously worked

        compassdone = true;
        if (compassdone && !compassDone2) {
            compassDone2 = true;
            motor1(100,1);
            delay(400);
        }

        if (calibrateLine) {
            Serial.println("Calibrating line");
            line.calibrateLine();
        }

    }

    prevStart = currentStart;
    prevCalibrateLine = currentCalibrateLine;

    line.lineFound = false;

}

void creatingCompassFunction() {
    currentStart = readButton(1);
    currentCalibrateLine = readButton(2);


    if (currentStart == HIGH && prevStart == LOW) {
        start = !start;
        // if (start) {
        //     motor3(100,1);
        //     delay(100);
        // }
    }

    if (currentCalibrateLine == HIGH && prevCalibrateLine == LOW) {
        calibrateLine = !calibrateLine;
        if (calibrateLine) {
            motor2(100,1);
            delay(400);
        }
    }

    if (start) {
        IRtesting();

        int compassAngle = compassSensor.currentOffset();
        Serial.println("Comapss Offset: " + String(compassAngle));
            // delay(40);
        
        } else {
            Movement.stopMotors();
            compassSensor.zeroedAngle = compassSensor.getOrientation(); // + 10 previously worked

            compassdone = true;
            if (compassdone && !compassDone2) {
                compassDone2 = true;
                motor1(100,1);
                delay(400);
            }

    }

    prevStart = currentStart;
    prevCalibrateLine = currentCalibrateLine;

    line.lineFound = false;

}

void loop () {
    process();
    // Movement.rotateCorrection(0.3);
    // creatingCompassFunction();
    Serial.println();

    // lineTesting();
    // Serial.println();

    // IRtesting();


    // delay(400);
    // Serial.println();

}







    // getIntersectionAngle();
    // double chordLength = lineDetection.getChord();

    // if (lineDetection.lineDetected && chordLength < (2-maxChordOffset)) {
    //     if (315 < lineDetection.intersectionAngle || lineDetection.intersectionAngle < 45) {
    //         Serial.println("Going forward");
    //         movement.movement(0,0,0.2);
    //     } else if (135 < lineDetection.intersectionAngle && lineDetection.intersectionAngle < 225) {
    //         Serial.println("Going backward");
    //         movement.movement(0, 0, -0.2);
    //     } else {
    //         Serial.println("Work on tuning those values");
    //     }
    // }

    // delay(500);

    