/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcPIDController.cpp
*
*  Created on: Mar 14, 2016
*      Author: anderssandstrom
*
\*************************************************************************/

#include "ecmcPIDController.h"

#include <stdio.h>
#include <stdlib.h>

ecmcPIDController::ecmcPIDController(ecmcAxisData *axisData,
                                     double        sampleTime) {
  data_ = axisData;
  initVars();

  if (!data_) {
    LOGERR("%s/%s:%d: DATA OBJECT NULL.\n", __FILE__, __FUNCTION__, __LINE__);
    exit(EXIT_FAILURE);
  }
}

ecmcPIDController::ecmcPIDController(ecmcAxisData *axisData,
                                     double        kp,
                                     double        ki,
                                     double        kd,
                                     double        kff,
                                     double        sampleTime,
                                     double        outMax,
                                     double        outMin) {
  data_ = axisData;
  initVars();

  if (!data_) {
    LOGERR("%s/%s:%d: DATA OBJECT NULL.\n", __FILE__, __FUNCTION__, __LINE__);
    exit(EXIT_FAILURE);
  }
  kp_         = kp;
  ki_         = ki;
  kd_         = kd;
  kff_        = kff;
  outputMax_  = outMax;
  outputMin_  = outMin;
  sampleTime_ = sampleTime;
}

void ecmcPIDController::initVars() {
  errorReset();
  outputP_            = 0;
  outputI_            = 0;
  outputD_            = 0;
  outputIMax_         = 0;  // For integrator
  outputIMin_         = 0;
  outputMax_          = 0;  // For combined PID output
  outputMin_          = 0;
  ff_                 = 0;
  controllerErrorOld_ = 0;
  kp_                 = 0;
  ki_                 = 0;
  kd_                 = 0;
  kff_                = 0;
  sampleTime_         = 0;
}

ecmcPIDController::~ecmcPIDController()
{}

void ecmcPIDController::reset() {
  outputP_            = 0;
  outputI_            = 0;
  outputD_            = 0;
  ff_                 = 0;
  controllerErrorOld_ = 0;
}

void ecmcPIDController::setIRange(double iMax, double iMin) {
  outputIMax_ = iMax;
  outputIMin_ = iMin;
}

double ecmcPIDController::getOutPPart() {
  return outputP_;
}

double ecmcPIDController::getOutIPart() {
  return outputI_;
}

double ecmcPIDController::getOutDPart() {
  return outputD_;
}

double ecmcPIDController::getOutFFPart() {
  return ff_;
}

double ecmcPIDController::getOutTot() {
  return data_->status_.cntrlOutput;
}

void ecmcPIDController::setKp(double kp) {
  kp_ = kp;
}

void ecmcPIDController::setKi(double ki) {
  ki_ = ki;
}

void ecmcPIDController::setKd(double kd) {
  kd_ = kd;
}

void ecmcPIDController::setKff(double kff) {
  kff_ = kff;
}

void ecmcPIDController::setOutMax(double outMax) {
  outputMax_ = outMax;
}

void ecmcPIDController::setOutMin(double outMin) {
  outputMin_ = outMin;
}

void ecmcPIDController::setIOutMax(double outMax) {
  outputIMax_ = outMax;
}

void ecmcPIDController::setIOutMin(double outMin) {
  outputIMin_ = outMin;
}

double ecmcPIDController::control(double error, double ff) {
  // Simple PID loop with FF.
  // Consider to make base class to derive other controller types

  if (!data_->command_.enable || data_->interlocks_.driveSummaryInterlock) {
    reset();
    return 0;
  }

  ff_                       = ff * kff_;
  
  data_->status_.cntrlError = error;                              
  outputP_ = data_->status_.cntrlError * kp_;
  outputI_ = outputI_ + data_->status_.cntrlError * ki_;

  // Enabled only when limits differ and max>min
  if ((outputIMax_ != outputIMin_) && (outputIMax_ > outputIMin_)) {
    if (outputI_ > outputIMax_) {
      outputI_ = outputIMax_;
    }

    if (outputI_ < outputIMin_) {
      outputI_ = outputIMin_;
    }
  }
  outputD_ =
    (data_->status_.cntrlError - controllerErrorOld_) * kd_;
  data_->status_.cntrlOutput = outputP_ + outputI_ + outputD_ + ff_;

  // Enabled only when limits differ and max>min
  if ((outputMax_ != outputMin_) && (outputMax_ > outputMin_)) {
    if (data_->status_.cntrlOutput > outputMax_) {
      data_->status_.cntrlOutput = outputMax_;
    }

    if (data_->status_.cntrlOutput < outputMin_) {
      data_->status_.cntrlOutput = outputMin_;
    }
  }
  controllerErrorOld_ = data_->status_.cntrlError;
  return data_->status_.cntrlOutput;
}

int ecmcPIDController::validate() {
  if (data_->sampleTime_ <= 0) {
    return setErrorID(__FILE__,
                      __FUNCTION__,
                      __LINE__,
                      ERROR_CNTRL_INVALID_SAMPLE_TIME);
  }
  return 0;
}
