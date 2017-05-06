/* Serial Passthrough code for driving a Nextion display panel
 * date: 05/06/2017
 * 
 * Serial1 is used to drive Nextion panel
 * 
 */

#include <Arduino.h>

void SendCmd(char *);
int getTouch(int);
int getSendme(int);
int getTouchPosition(int);
int getSleepPosition(int);
int getNumeric(int);
int getText(int);
int setSleep(int);
int setAwake(int);
int getPower(int);
int getSD(int);
int getTransparentEnd(int);
int getTransparent(int);
void event(int);

void startMaze(void);  // built in secondary program

char Buffer[256];
unsigned char C;
unsigned long wait;
int Index;
int Index2;
int i;


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

  wait = millis();

}

void loop() {
  // check for console data
  if (Serial.available() > 0)
  {
    Index = 0;
    while (Serial.available() >0)
    {
      C = Serial.read();
      if ((C != 0x0d) && (C != 0x0a)) // Strip these codes
        Buffer[Index++] = C;
      Serial.write(C);
    }
    Buffer[Index]=0;
  }

  // run special program code
  if (strcmp(Buffer, "run") == 0)
  {
    Buffer[0] = 0;
    Serial.println("Starting Program");
    startMaze();
    Serial.println("Program Finished");
    Index = 0;
  }
  
  // send command to Nextion panel
  if (Index > 0)
  {
    SendCmd(Buffer);
    Index=0;
  }

  // did we get some data back from panel
  if (Serial1.available() > 0)
  {
    C = Serial1.read();
//    Serial.write('<');Serial.print(C, HEX);Serial.print("> ");
    Buffer[Index2++] = C;
  }

  // Wait a second to fill buffer
  if (millis() > wait)
  {
    int j;
    for (int i=0;i<Index2;i++)
    {
      for (j=i+1;j<Index2;j++)
        if (Buffer[j] == 0xff)
          break;
      j = j - i;
      int C = Buffer[i];
//      Serial.print(C, HEX);Serial.print(" ");Serial.print(i);Serial.print(" ");Serial.println(j);
      switch (C)
      {
        case 0x00: if (j == 1) {i += 3;Serial.println("Invalid instruction");}
          break;
        case 0x01: if (j == 1) {i += 3;Serial.println("Success");}
          break;
        case 0x02: if (j == 1) {i += 3;Serial.println("Component ID invalid");}
          break;
        case 0x03: if (j == 1) {i += 3;Serial.println("Page ID invalid");}
          break;
        case 0x04: if (j == 1) {i += 3;Serial.println("Picture ID invalid");}
          break;
        case 0x05: if (j == 1) {i += 3;Serial.println("Font ID invalid");}
          break;
        case 0x11: if (j == 1) {i += 3;Serial.println("Baud rate settings invalid");}
          break;
        case 0x12: if (j == 1) {i += 3;Serial.println("Curve control ID number or channel number is invalid");}
          break;
        case 0x1a: if (j == 1) {i += 3;Serial.println("Variable name invalid");}
          break;
        case 0x1b: if (j == 1) {i += 3;Serial.println("Variable operation invalid");}
          break;
        case 0x1c: if (j == 1) {i += 3;Serial.println("Failed to assign");}
          break;
        case 0x1d: if (j == 1) {i += 3;Serial.println("Operate EEPROM failed");}
          break;
        case 0x1e: if (j == 1) {i += 3;Serial.println("Parameter quantity invalid");}
          break;
        case 0x1f: if (j == 1) {i += 3;Serial.println("IO operation failed");}
          break;
        case 0x20: if (j == 1) {i += 3;Serial.println("Undefined escape characters");}
          break;
        case 0x23: if (j == 1) {i += 3;Serial.println("Too long variable name");}
          break;
        case 0x65: if (j == 4) i = getTouch(i); // touch event
          break;
        case 0x66: if (j == 2) i = getSendme(i); // sendme
          break;
        case 0x67: if (j == 6) i = getTouchPosition(i); // touch position
          break;
        case 0x68: if (j == 6) i = getSleepPosition(i); // touch sleep position
          break;
        case 0x70: if (j < Index2) i = getText(i, j); // string variable
          break;
        case 0x71: if (j == 5) i = getNumeric(i); // numeric variable
          break;
        case 0x86: if (j == 1) i = setSleep(i); // sleep mode
          break;
        case 0x87: if (j == 1) i = setAwake(i); // awake mode
          break;
        case 0x88: if (j == 1) i = getPower(i); // powered on
          break;
        case 0x89: if (j == 1) i = getSD(i); // SD card update
          break;
        case 0xfd: if (j == 1) i = getTransparentEnd(i); // leaving transparent mode
          break;
        case 0xfe: if (j == 1) i = getTransparent(i); // entering transparent mode
          break;
        default: Serial.write(Buffer[i]);
      }
    }
    Index2=0;
    wait = millis() + 1000;
  }
}

// convert numeric panel value
int getNumeric(int i)
{
  int n;
  
  n = Buffer[i+4];
  n = n << 8;
  n = n | Buffer[i+3];
  n = n << 8;
  n = n | Buffer[i+2];
  n = n << 8;
  n = n | Buffer[i+1];
  Serial.println(n);
  return i+7;
}

// convert text data from panel
int getText(int i, int j)
{
  for (int k=1;k<j;k++)
    Serial.write(Buffer[i+k]);
  Serial.println();
  return j+2;
}

// touch event occured
int getTouch(int i)
{
  int p, b, e;
  p = Buffer[i+1];
  b = Buffer[i+2];
  e = Buffer[i+3];
  Serial.print("PageID:");Serial.print(p);Serial.print(" ");Serial.print("Button:");Serial.print(b);Serial.print(" ");
  event(e);Serial.println();
  return i+6;
}

// get current page
int getSendme(int i)
{
  int j = Buffer[i+1];
  Serial.print("PageID: ");Serial.println(j);
  return i+4;
}

// get touch position on screen
int getTouchPosition(int i)
{
  int x, y, e;

  x = Buffer[i+1] << 8;
  x = x | Buffer[i+2];
  y = Buffer[i+3] << 8;
  y = y | Buffer[i+4];
  e = Buffer[i+5];
  event(e);Serial.print(" (");Serial.print(x);Serial.print(",");Serial.print(y);Serial.print(")");
  Serial.println();
  return i+8;
}

// get touch position of sleeping panel
int getSleepPosition(int i)
{
  int x, y, e;
  
  x = Buffer[i+1] << 8;
  x = x | Buffer[i+2];
  y = Buffer[i+3] << 8;
  y = y | Buffer[i+4];
  e = Buffer[i+5];
  event(e);Serial.print(" (");Serial.print(x);Serial.print(",");Serial.print(y);Serial.print(")");
  Serial.println();
  return i+8;
}

// panel went to sleep
int setSleep(int i)
{
  Serial.println("Going to Sleep");
  return i+3;
}

// panel came out of sleep mode
int setAwake(int i)
{
  Serial.println("Panel Awake");  
  return i+3;
}

// panel has just powered up
int getPower(int i)
{
  Serial.println("Panel Powered up");
  return i+3;
}

// loading HMI file by SD card
int getSD(int i)
{
  Serial.println("SD card Load");
  return i+3;
}

int getTransparentEnd(int i)
{
  Serial.println("Transparent Mode Finished");
  return i+3;
}

int getTransparent(int i)
{
  Serial.println("Transparent Mode Entered");
  return i+3;
}

void event(int e)
{
  if (e == 0) Serial.print("Release");
  if (e == 1) Serial.print("Press");
}

void SendCmd(char *B)
{
  Serial1.print(B);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

