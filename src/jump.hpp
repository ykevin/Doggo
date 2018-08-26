#include "ChRt.h"
#include "Arduino.h"
#include "ODriveArduino.h"
#include "config.hpp"
#include "globals.hpp"
#include "position_control.hpp"

#ifndef POSITION_CONTROL_H
#define POSITION_CONTROL_H

void TragectoryJump();
void TrajectoryPosControlJump();
void MoveLegJump(ODriveArduino& odrive, float t, float FREQ, float gait_offset,
  float stanceHeight, float flightPercent, float stepLength,
  float upAMP, float downAMP, float leg_direction, float sign);


/**
* Drives the ODrives in an open-loop, position-control sinTrajectory.
*/
void TrajectoryPosControlJump() {

  // min radius = 0.8
  // max radius = 0.25
  float t = millis()/1000.0;
  const float launchTime = 2.0;
  const float stanceHeight = 0.05; // Desired height of body from ground prior to jump (m)
  const float downAMP = 0.1; // Peak amplitude below stanceHeight in sinusoidal trajectory (m)

  bool hasCompletedJump = false;
  while(!hasCompletedJump) {
    if (t>launchTime) {
      hasCompletedJump = true;
    }
    Serial.print(t);
    Serial.print(" ");

    const float leg0_sign = -1.0;
    const float leg0_direction = -1.0;
    MoveLegJump(odrv0Interface, t, launchTime, stanceHeight, downAMP,
      leg0_direction, leg0_sign);

    const float leg1_sign = -1.0;
    const float leg1_direction = -1.0;
    MoveLegJump(odrv1Interface, t, launchTime, stanceHeight, downAMP,
      leg1_direction, leg1_sign);

    const float leg2_sign = -1.0;
    const float leg2_direction = 1.0;
    MoveLegJump(odrv2Interface, t, launchTime, stanceHeight, downAMP,
      leg2_direction, leg2_sign);

    const float leg3_sign = -1.0;
    const float leg3_direction = 1.0;
    MoveLegJump(odrv3Interface, t, launchTime, stanceHeight, downAMP,
      leg3_direction, leg3_sign);
      Serial.println();

    chThdSleepMilliseconds(10);

  }
}


/**
* Sinusoidal function of vertical movement for jump
*/
void TrajectoryJump (float t, float launchTime, float stanceHeight,
   float downAMP, float& x, float& y) {
    //Need to check if n works
    n = t/launchTime;
    x = 0;
    y = downAMP*sin(PI/4 + PI/4*n) + stanceHeight;
  }


void MoveLegJump(ODriveArduino& odrive, float t, float launchTime,
  float stanceHeight, float downAMP, float leg_direction, float sign) {
    float enc0;
    float enc1;
    float x; // float x for leg to be set by the sin trajectory
    float y;
    TrajectoryJump(t, launchTime, stanceHeight, downAMP, x, y);
    CartesianToEncoder(x, y, leg_direction, sign, enc0, enc1);
    odrive.SetPosition(0,(int)enc0);
    odrive.SetPosition(1,(int)enc1);
  }
