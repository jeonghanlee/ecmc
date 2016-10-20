#ifndef ECMCDRIVEBASE_H_
#define ECMCDRIVEBASE_H_

#include <stdio.h>
#include <cmath>

#include "ecmcDefinitions.h"
#include "ecmcEcEntry.h"
#include "ecmcEcEntryLink.h"
#include "ecmcEcPdo.h"
#include "ecmcError.h"

//DRIVE
#define ERROR_DRV_DRIVE_INTERLOCKED 0x14600
#define ERROR_DRV_ASSIGN_ENTRY_FAILED 0x14601
#define ERROR_DRV_SCALE_DENOM_ZERO 0x14602
#define ERROR_DRV_ENABLE_ENTRY_NULL 0x14603
#define ERROR_DRV_VEL_SET_ENTRY_NULL 0x14604
#define ERROR_DRV_ENABLED_ENTRY_NULL 0x14605
#define ERROR_DRV_ENABLED_READ_ENTRY_FAIL 0x14606
#define ERROR_DRV_BRAKE_ENTRY_NULL  0x14607
#define ERROR_DRV_REDUCE_TORQUE_ENTRY_NULL  0x14608

class ecmcDriveBase : public ecmcEcEntryLink
{
public:
  ecmcDriveBase();
  ecmcDriveBase(double scale);
  virtual ~ecmcDriveBase();
  void initVars() ;
  void setScaleNum(double scaleNum);
  int setScaleDenom(double scaleDenom);
  double getScale();
  int setVelSet(double vel);
  double getVelSet();
  int setVelSetRaw(int rawVel);
  int getVelSetRaw();
  virtual bool getEnable()=0;
  virtual bool getEnabled()=0;
  virtual int setEnable(bool enable)=0;
  void setInterlock(bool interlock);
  virtual int validate()=0;
  bool getInterlock();
  void readEntries();
  void writeEntries();
  int setEnableBrake(bool enable);
  int setEnableReduceTorque(bool enable);
  int setBrake(bool value);
  int setReduceTorque(bool value);
  int getEnableBrake();
  int getEnableReduceTorque();
  int setAtTarget(bool atTarget);
protected:
  double scale_;
  double scaleNum_;
  double scaleDenom_;
  int velSetRawOutput_;
  double velSet_;
  bool enableOutput_;
  bool enabledInput_;
  bool interlock_;
  bool enableBrake_;
  bool enableReduceTorque_;
  int brakeOutput_;
  int reduceTorqueOutput_;
  operationMode opeationMode_;
};
#endif
