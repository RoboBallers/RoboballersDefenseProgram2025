#ifndef BALLFINDING_H
#define BALLFINDING_H

#include <Arduino.h>
#include <zirconLib.h>
#include <math.h>


class BallFinding {
    public:
        BallFinding();
        double ballAngle();
        double cos_sensors_sum;
        double sin_sensors_sum;
        double ballangle;
        double multiplier;
        double distance;
        bool ball_in_robot;
        double orbit_val;
        double multi;
        double move_angle;
        const int num_sensors = 8;
        bool isBallVisible();


};

#endif