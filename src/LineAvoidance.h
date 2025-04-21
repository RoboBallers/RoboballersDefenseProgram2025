#ifndef LINE_AVOIDANCE_H
#define LINE_AVOIDANCE_H
#include <Arduino.h>
#include <zirconLib.h>
#include <math.h>
#include <movement.h>
#include <BallFinding.h>


class LineAvoidance {
    public:
        LineAvoidance();
        double findingLine();
        void calibrateLine();
        std::vector<int> getLineValues();
        double followingAngle();
        double avoidingLine(int currMovementAngle);
        void foundLine();
        bool lineFound = false;

    private:
        const int white_line = 100;
        double move_angle;
        int calibrateVals[3];
        bool crossLine;

        int front = 1;
        int backRight = 2;
        int backLeft = 3;

};

#endif