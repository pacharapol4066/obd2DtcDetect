/*
 *  MODIFIED BY PACHARAPOL OUMOLARN
 *  15/5/2018
 */
#ifndef _OBD_II_DTC_DEF_H_
#define _OBD_II_DTC_DEF_H_

#ifndef INT8U
#define INT8U unsigned char
#endif
#ifndef INT16U
#define INT16U unsigned short
#endif
#ifndef INT32U
#define INT32U unsigned long
#endif


/*Reference : https://en.wikipedia.org/wiki/OBD-II_PIDs */
#define DEBUGMODE 1
#define PID_ENGREV  0x0c
#define PID_ENGTEMP 0x05
#define CAN_ID      0x7df
#define MODE1       0x01

#endif
