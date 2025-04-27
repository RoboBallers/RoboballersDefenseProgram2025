#include <BallFinding.h>
#include <Arduino.h>
#include <zirconLib.h>
#include <math.h>
#include <algorithm>
#include<tuple>
#include<iostream>
// #include <cmath>


BallFinding::BallFinding () {

}

bool BallFinding::isBallVisible() {
    for (int i = 1; i <= num_sensors; i++) {
        if (readBall(i) >= 10) {
            return true;
        }
    }
    return false; 
}


double BallFinding::ballAngle() {
    double sin_sensors_sum = 0;
    double cos_sensors_sum = 0;
  
    for (int i = 0; i < num_sensors; i++) {
        int sensorVal = readBall(i+1);

        if (i == 7) {
            sensorVal += 100;
            if (sensorVal < 0) {
                sensorVal = 0;
            }
        }

        sin_sensors_sum += sensorVal * sin(((i+1)*45)*(PI/180));
        cos_sensors_sum += sensorVal * cos(((i+1)*45)*(PI/180));
    }


    ballangle = atan2(cos_sensors_sum, sin_sensors_sum) * (180/PI);

    if (ballangle < 0) {
        ballangle += 360;
    }

    ballangle -= 40;

    if (ballangle < 0) {
        ballangle += 360;
    }

    ballangle = (360-ballangle);


    return ballangle;
}