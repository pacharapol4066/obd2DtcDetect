/*
 *  MODIFIED BY PACHARAPOL OUMOLARN
 *  15/5/2018
 *  https://www.csselectronics.com/screen/page/simple-intro-to-can-bus/language/en
 */
#include "obd2DtcDetect.h"
#include <mcp_can.h>            //CAN Bus Shield Compatibility Library
#include <SPI.h>                //CAN Bus Shield SPI Pin Library            
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>            //String support

LiquidCrystal_I2C lcd(0x3F, 16, 2);
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);  // Set CS pin

// Class init
void obd2DtcDetect_h::init(void)
{
    m_cMode      = 0;
    m_cPid       = 0;
  
    // CAN bus INIT 500k
    if(CAN.begin(CAN_500KBPS) == CAN_OK)
    {
        //Initialize LCD
        lcd.begin();
        lcd.backlight();
        showMsgLCD("Hilux Vigo Dashboard","by VT11257");
        delay(2000);
        showMsgLCD("Hilux Vigo Dashboard","booting...");
        delay(2000);
        showMsgLCD("CAN BUS Shield init ok!","");
    }
    else 
    {
        showMsgLCD("CAN INIT FAIL!!","");
    }

    CAN.init_Filt(0, 0, 0x7e0);
    CAN.init_Mask(0, 0, 0x7f0);
    CAN.init_Filt(1, 0, 0x7e0);
    CAN.init_Mask(1, 0, 0x7f0);
    
    CAN.init_Filt(2, 0, 0x7e0);
    CAN.init_Filt(3, 0, 0x7e0);
    CAN.init_Filt(4, 0, 0x7e0);
    CAN.init_Filt(5, 0, 0x7e0);
    
    delay(10);

}

//Reload LC Display
void obd2DtcDetect_h::showMsgLCD(String line1,String line2)
{  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
//    Serial.println(line1);
//    Serial.println(line2);
}

//Reload LC Display
void obd2DtcDetect_h::showValLCD(String line1,INT32U line2)
{  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(7, 0);
    lcd.print(line2);
//    Serial.println(line1);
//    Serial.println(line2);
}


void obd2DtcDetect_h::sendOrder()
{

  INT8U len = 0;
  /* 
    EACH ELEMENT REFER BYTE OF DATA IN CONTAINER   
    orBuf[8] = {LENGTH,MODE,PID,Ah,Bh,Ch,Dh,Unused};
    IF LENGTH IS 0x02 = REQUEST MESSAGE
    IF LENGTH IS > 0x02 = RECIEVED MESSAGE
  */
  INT8U orBuf[8]= {0x02, m_cMode, m_cPid, 0x00, 0x00, 0x00, 0x00, 0x00};
  
  /*
   * sendMsgBuf(id, ext, len, buf)
   * id ：Device ID for the remote CAN device  > 0x7df
   * ext：Data transmission format             > 0 (standard data format)
   * len：size of the array                    > 8 bytes of data
   * buf：data array                           > orBuf[8]
   */
  CAN.sendMsgBuf(CAN_ID, 0, 8, orBuf);
  
  if(CAN.checkReceive()== CAN_MSGAVAIL)      // Check to see whether data is received.
  {
    len = 0;
    memcpy(m_cData, 0, 8);          // DESC : set '0' to variable
    CAN.readMsgBuf(&len, m_cData);  // DESC : read data from the CAN bus
  }
}

void obd2DtcDetect_h::setModePid(INT8U cMode, INT8U cPid)
{
  m_cMode = cMode;
  m_cPid  = cPid;
}

/*
 **** RECIEVED DATA CONTAINER FORMAT ****
    m_cData[0] = LENGTH
    m_cData[1] = MODE
    m_cData[2] = PID
    m_cData[3] = A
    m_cData[4] = B
    m_cData[5] = C
    m_cData[6] = D
    m_cData[7] = Unused
*****************************************    
*/

/* Use for unsign long plus(+) method */
INT32U char_merger(INT8U iHigh, INT8U iLow)
{
  INT32U t = 0;
  t = iHigh;
  t<<=8;
  return (t+iLow);
}

INT32U obd2DtcDetect_h::getRevData()
{
  setModePid(MODE1, PID_ENGREV);                      //Engine REV (SAE J1979) with MODE 01
  sendOrder();                                        //Send command to ECU and retrieve data by reference
  return (char_merger(m_cData[3], m_cData[4])/4);     //Return to main ( formular A+B/4 )
}


INT32U obd2DtcDetect_h::getTempData()
{
  setModePid(MODE1, PID_ENGTEMP);       //Coolant temperature (SAE J1979) with MODE 01
  sendOrder();                          //Send command to ECU and retrieve data by reference
  return (m_cData[3] - 40);             //Return to main
}

obd2DtcDetect_h OBD2DTC;
