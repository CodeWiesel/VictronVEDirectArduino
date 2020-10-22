/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, Brendan McLearie
 Modified 2020, Jim Smith
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: VEDirect.h
 - Class / enums / API

 Updates:
 - 2020-10--8:
 		Added additional labels to read MPPT devices
 - 2019-07-14:
  	  - Rewrite of read - cleaner.
  	  - Target labels extendible with enum and PROGMEM strings
  	  - Retired copy_raw_to_serial0() code - use VE_DUMP on read
  	  - Added some tunable parameters see #defines
******************************************************************/

#ifndef VEDIRECT_H_
#define VEDIRECT_H_

#include <Arduino.h>

// Tunable parameters - defaults tested on mega2560 R3
#define VED_LINE_SIZE 30	// Seems to be plenty. VE.Direct protocol could change
#define VED_MAX_LEBEL_SIZE 10	 // Max length of all labels of interest + '\0'. See ved_labels[]
#define VED_MAX_READ_LOOPS 6000 // How many read loops to be considered a read time-out
#define VED_MAX_READ_LINES 100	 // How many lines to read looking for a value
				// before giving up. Also determines lines for diag dump
#define VED_BAUD_RATE 19200

enum VE_DIRECT_DATA {
  VE_DUMP = 0,
  VE_BMV_SOC,
  VE_BMV_VOLTAGE,
  VE_BMV_STARTV,
  VE_BMV_POWER,
  VE_BMV_CURRENT,
  VE_BMV_ALARM,
  VE_BMV_RELAY,
  VE_MPPT_VOLTAGE,
  VE_MPPT_PANELV,
  VE_MPPT_POWER,
  VE_MPPT_CURRENT,
  VE_MPPT_ERR,
  VE_MPPT_STATE,
  VE_MPPT_H19,
  VE_MPPT_H20,
  VE_MPPT_H21,
  VE_MPPT_H22,
  VE_MPPT_H23,
  VB_DC_VBATT,    //VE BUS readings via USB port
  VB_DC_IINV,
  VB_DC_ICHG,
  VB_DC_FREQ, 
  VB_DC_PBATT,
  VB_AC_VMAINS,
  VB_AC_IMAINS,
  VB_AC_PMAINS,
  VB_AC_VINV,
  VB_AC_IINV,
  VB_AC_FREQ, 
  VB_AC_PINV,
  VB_LED_STATE,
  VC_MPPT_ABSORB,
  VC_MPPT_STATE,
  VC_MPPT_FLOAT,
  VC_MULTI_MAINS,
  VC_MULTI_ABSORB,
  VC_MULTI_STATE,
  VC_MULTI_FLOAT,
  VC_MULTI_INVERTER,
  VC_MULTI_OVERLOAD,
  VC_MULTI_LOWBATT,
  VC_MULTI_TEMP,
  VE_LAST_LABEL,
};

const char ved_labels[VE_LAST_LABEL][VED_MAX_LEBEL_SIZE]  = {
		"Dump",	// a string that won't match any label
		"SOC",
		"V",
		"VS",
		"P",
		"I",
		"Alarm",
		"Relay",
		"V",
		"VPV",
		"PPV",
		"I",
		"ERR",
		"CS",
};

class VEDirect {
public:
	VEDirect(HardwareSerial& port);
	virtual ~VEDirect();
	uint8_t begin();
	int32_t read(uint8_t target);
	void copy_raw_to_serial0(); // kept for backwards compatibility
private:
	HardwareSerial& VESerial;
};

#endif /* VEDIRECT_H_ */
