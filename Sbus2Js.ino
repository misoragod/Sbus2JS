/*
 *  Sbus2JS
 */
#include <Joystick.h>
#define NUM_BUTTON  32
#define PIN_X         0 
#define PIN_Y         1 
#define PIN_AI        2
#define BUTTON_ESTOP  1 
#define BUTTON_ERASE  3 
#define BUTTON_AI     9 
#define BUTTON_MODE  10

int count;
long interval;
int base[5] = {0,0,0,0,0};
int j;
boolean toggle_num[NUM_BUTTON];
int WIDTH = 672;     // WIDTH = 672 
float joystick[5] = {0,0,0,0,0};
Joystick_ Joystick = Joystick_(
 0x03,                    // reportid
 JOYSTICK_TYPE_GAMEPAD,   // type
 NUM_BUTTON,              // button count
 0,                       // hat switch count
 true, true, false,      // left x,y,z axis enable
 true, true, false,      // right x,y,zaxis enable
 false,                   // rudder enable
 false,                   // throttle enable
 false,                   // accelerator enable
 false,                   // brake enable
 false                    // steering enable
 );

void setup() {
  Serial.begin(115200); // Terminal
  Serial1.begin(100000,SERIAL_8E2); // S-BUS

 // Initialize joystick.
 Joystick.begin();
 Joystick.setXAxisRange(0, 100);
 Joystick.setRyAxisRange(0, 100);
 Joystick.setYAxisRange(0, 100);
 Joystick.setRxAxisRange(0, 100);

 // Initialize button.
 for(int i = 0; i < NUM_BUTTON; i++) {
   Joystick.setButton(i, false);
   toggle_num[i] = false;
 }

  
 count=0;
 j = 0;
}

void loop() {
 int data[26];
 int val[19];
 int i;
 if (Serial1.available() > 0) {
  data[count]=Serial1.read();
  interval=millis();
  count++;
 }
 if ((interval+4 < millis()) && (0 < count) ) {
   count=0;
   val[0] =((data[1] & 0xff)<<0) + ((data[2] & 0x07)<<8);
   val[1] =((data[2] & 0xf8)>>3) + ((data[3] & 0x3f)<<5);
   val[2] =((data[3] & 0xc0)>>6) + ((data[4] & 0xff)<<2) + ((data[5] & 0x01)<<10);
   val[3] =((data[5] & 0xfe)>>1) + ((data[6] & 0x0f)<<7);
   val[4] =((data[6] & 0x0f)>>4) + ((data[7] & 0x7f)<<4);
   val[5] =((data[7] & 0x80)>>7) + ((data[8] & 0xff)<<1) + ((data[9] & 0x03) <<9);
   val[6] =((data[9] & 0x7c)>>2) + ((data[10] & 0x1f)<<6);
   val[7] =((data[10] & 0xe0)>>5) + ((data[11] & 0xff)<<3);
  
   val[8] =((data[12] & 0xff)<<0) + ((data[13] & 0x07)<<8);
   val[9] =((data[13] & 0xf8)>>3) + ((data[14] & 0x3f)<<5);
   val[10]=((data[14] & 0xc0)>>6) + ((data[15] & 0xff)<<2) + ((data[16] & 0x01)<<10);
   val[11]=((data[16] & 0xfe)>>1) + ((data[17] & 0x0f)<<7);
   val[12]=((data[17] & 0x0f)>>4) + ((data[18] & 0x7f)<<4);
   val[13]=((data[18] & 0x80)>>7) + ((data[19] & 0xff)<<1) + ((data[20] & 0x03) <<9);
   val[14]=((data[20] & 0x7c)>>2) + ((data[21] & 0x1f)<<6);
   val[15]=((data[21] & 0xe0)>>5) + ((data[22] & 0xff)<<3);
   val[16] = (data[23] & 0x1) ? 0x7ff : 0 ;
   val[17] = (data[23] & 0x2) ? 0x7ff : 0 ;
   val[18] = (data[23] & 0x8) ? 0x7ff : 0 ; // Failsafe
   if(j++ < 10){
     base[0] = val[0];
     base[1] = val[1];
     base[2] = val[2];
     base[3] = val[3];
   }else{
    /*    joystick[0]   Steering
     *    joystick[1]   Throttle
     */
      joystick[0] = map(val[0], long(base[0] - WIDTH), long(base[0] + WIDTH), 0, 100);
      joystick[1] = map(val[2], long(base[2] - WIDTH), long(base[3] + WIDTH), 0, 100);
      joystick[2] = map(val[3], long(base[1] - WIDTH), long(base[1] + WIDTH), 0, 100);  
      joystick[3] = map(val[1], long(base[3] - WIDTH), long(base[3] + WIDTH), 0, 100);

/*
      joystick[2] = map(val[1], long(base[1] - WIDTH), long(base[1] + WIDTH), 0, 100);  
 //     joystick[2] = map(val[2], long(base[2] - WIDTH), long(base[2] + WIDTH), 0, 100);
      joystick[3] = map(val[3], long(base[3] - WIDTH), long(base[3] + WIDTH), 0, 100);
*/
      // Left  X Axis
      Joystick.setXAxis(joystick[0]);
      // Right Y Axis
      Joystick.setRyAxis(joystick[1]);
//      Joystick.setYAxis(joystick[1]);

      // Right X Axis
      Joystick.setRxAxis(joystick[2]);
      // Left Y Axis
      Joystick.setYAxis(joystick[3]);

/*
      for (i=0 ; i<4; i++ ) {
        Serial.print(joystick[i],DEC);
        Serial.print(F(" "));
      }
      Serial.print(F("\n"));

*/
     for (i=0 ; i<18; i++ ) {
      Serial.print(val[i],DEC);
      Serial.print(F(" "));
      }
      Serial.print(F("\n"));

  //   for (i=0 ; i<4; i++ ) {
  //    Serial.print(base[i],DEC);
  //    Serial.print(F(" "));
  //  }
  //   Serial.print(F("\n"));
  }
 }

}
