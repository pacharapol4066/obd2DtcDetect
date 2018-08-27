 /*
 *  MODIFIED BY PACHARAPOL OUMOLARN
 *  27/8/2018
 */

//Libraries
#include "obd2DtcDetect.h"
#include "obd2DtcDetect_def.h"


//Setup Configuration
void setup()
{
#if DEBUGMODE
  Serial.begin(115200);
#endif
  OBD2DTC.init();
}

//Loop
void loop()
{
    OBD2DTC.showValLCD("REV  : ",OBD2DTC.getRevData());
    OBD2DTC.showValLCD("TEMP : ",OBD2DTC.getTempData());
    delay(500);
}
