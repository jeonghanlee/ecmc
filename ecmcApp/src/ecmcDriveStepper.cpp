#include "ecmcDriveStepper.hpp"

ecmcDriveStepper::ecmcDriveStepper()
{
  initVars();
}

ecmcDriveStepper::ecmcDriveStepper(double scale)
{
  initVars();
  scale_=scale;
}
ecmcDriveStepper::~ecmcDriveStepper()
{
  ;
}

bool ecmcDriveStepper::getEnable()
{
  return enableOutput_;
}

bool ecmcDriveStepper::getEnabled()
{
  return enabledInput_;
}

int ecmcDriveStepper::setEnable(bool enable)
{
  if(interlock_ && enable){
    enableOutput_=false;
    return setErrorID(ERROR_DRV_DRIVE_INTERLOCKED);
  }

  if(enableBrake_){
    if(!enable ){
      brakeOutput_=0;  //brake locked when 0 . TODO: Apply brake some cycles before enable is low
    }
    else{
      brakeOutput_=1;  //brake open when 1
    }
  }

  enableOutput_=enable;
  return 0;
}

int ecmcDriveStepper::validate()
{
  int errorCode=validateEntry(0); //Enable entry output
  if(errorCode){
    return setErrorID(errorCode);
  }

  errorCode=validateEntry(1); //Velocity Setpoint entry output
  if(errorCode){
    return setErrorID(errorCode);
  }

  errorCode=validateEntry(2); //Enabled entry input
  if(errorCode){
    return setErrorID(errorCode);
  }

  if(enableBrake_){
    errorCode=validateEntry(3); //brake output
    if(errorCode){
      return setErrorID(errorCode);
    }
  }

  if(enableReduceTorque_){
    errorCode=validateEntry(4); //reduce torque output
    if(errorCode){
      return setErrorID(errorCode);
    }
  }

  if(scaleDenom_==0){
    return setErrorID(ERROR_DRV_SCALE_DENOM_ZERO);
  }
  return 0;
}
