#include <LineAvoidance.h>
#include <Arduino.h>
#include <zirconLib.h>
#include <math.h>
#include<tuple>
#include<iostream>
#include <vector>

LineAvoidance::LineAvoidance() {

}

double LineAvoidance::findingLine() {
    move_angle = 5;
    crossLine = false;

    for (int i = 1; i <= 3; i++) {
        if (readLine(i) > calibrateVals[i-1]) {
            lineFound = true;
        }
    }

    if (readLine(backLeft) > calibrateVals[backLeft-1] && readLine(backRight) > calibrateVals[backRight-1] && !crossLine) {
        crossLine = true;
    }

    if (crossLine && readLine(front) > calibrateVals[front-1]) {
        crossLine = false;
    }

    if (readLine(backLeft) > calibrateVals[backLeft-1] && !crossLine) {
        Serial.println("Going to the right");
        return 90;
    } else if (readLine(backRight) > calibrateVals[backRight-1] && !crossLine) {
        Serial.println("Going to the left");
        return 270;
    } else if (readLine(front) < calibrateVals[front-1]) {
        if (crossLine) {
            return 180;
        } else {
            return 0;
        }
    }

    return move_angle;

}

double LineAvoidance::followingAngle () {
    // if (readLine(backLeft) < calibrateVals[backLeft-1]) {
    //     Serial.println("Going to the right to avoid line");
    //     return 90;
    // } else if (readLine(backRight) < calibrateVals[backRight-1]) {
    //     Serial.println("Going to the left to avoid line");
    //     return 270;
    // } else if (readLine(front) > calibrateVals[front-1]) {
    //     Serial.println("Going forward to avoid line");
    //     return 0;
    // }
    // else {
    //     Serial.println("No line found");
    //     return -1;
    // }

    if (readLine(front) > calibrateVals[backLeft-1] ) {
        delay(400);
        return 0;
    } else {
        return -1;
    }
}


std::vector<int> LineAvoidance::getLineValues() {
    std::vector<int> lineValues;
    for (int i = 1; i <= 3; i++) {
        lineValues.push_back(readLine(i));
    }
    return lineValues;
}

void LineAvoidance::foundLine() {
    std::vector<int> values = getLineValues();
    for (int i = 0; i < 3; i++) {
        if (values[i] > calibrateVals[i]) {
            lineFound = true;
        }
    }
}

double LineAvoidance::avoidingLine(int currMovementAngle) {
    foundLine();
    if (lineFound) {
        // if (currMovementAngle == 105) {
        //     Serial.println("Going to the left to avoid line");
        //     return 270;
        // } else if (currMovementAngle == 265) {
        //     Serial.println("Going to the right to avoid line");
        //     return 90;
        // } else if (currMovementAngle == 180) {
        //     Serial.println("Going forward to avoid line");
        //     return 0;
        // } 

        if (readBall(backLeft) > calibrateVals[backLeft-1] && readBall(backRight) > calibrateVals[backRight-1]) {
            Serial.println("Going forward");
            return 0;
        } else if (readBall(backLeft) > calibrateVals[backLeft-1]) {
            Serial.println("Going to right");
            return 95;
        } else if (readBall(backRight) > calibrateVals[backRight-1]) {
            Serial.println("Going left");
            return 265;
        }
    }
    return -1;
}


void LineAvoidance::calibrateLine() {
    std::vector<int> values = getLineValues();

    // front
    // br
    // bl


    for (int i = 0; i < 3; i++) {
        calibrateVals[i] = fmax(calibrateVals[i],values[i] + 300);
        Serial.println("Calibration sensor " + String(i+1) + ": " + String(calibrateVals[i]));
    }
    Serial.println();

}