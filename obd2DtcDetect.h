/*
 *  MODIFIED BY PACHARAPOL OUMOLARN
 *  15/5/2018
 */
#ifndef _OBD_II_DTC_H_
#define _OBD_II_DTC_H_
#include <Arduino.h>
#include "obd2DtcDetect_def.h"

class obd2DtcDetect_h
{
  private:
  INT8U m_cMode;
  INT8U m_cPid;
  INT8U m_cData[8];
  
  private:
  void sendOrder();
  void setModePid(INT8U cMode, INT8U cPid);
  
  public:
  void init(void);
  void showMsgLCD(String line1,String line2);
  void showValLCD(String line1,INT32U line2);
  INT32U getRevData();
  INT32U getTempData();
};

extern obd2DtcDetect_h OBD2DTC;
#endif
