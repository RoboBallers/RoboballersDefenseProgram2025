#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Arduino.h>
#include <zirconLib.h>
#include <math.h>
#include <PID_v1.h>
#include <CompassSensor.h>

class movement {
    public:
        movement(CompassSensor& compassSensor);
        void movementfunc(double degrees, double speedFactor, double desiredOrientation);
        double findCorrection(double desiredOrientation);
        void rotateCorrection(double speedFactor);
        void stopMotors();

        double kp = 1;
        double ki = 0;
        double kd = 0.0015;

        CompassSensor& compassSensor;

        int currMovementAngle = 0;

        
    private:
        PID* myPID;
        PID* myPID2;

        double Setpoint, Input, Output;

        const int theta1 = 60;
        const int theta2 = 180;
        const int theta3 = 300;    
        double FrontRight, Back, FrontLeft;
        double maxVal;
        double FRpower, Backpower, FLpower;
        double avg, average, num_sens;
        int sign(double var);

};

#endif

