/***************************************************
  This is a library for the Adafruit 1.8" SPI display. This library has
  been modified from the original version to work with a non-Adafruit
  1.8in display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ---->  http://www.adafruit.com/products/358

The 1.8" TFT shield
  ---->  http://www.adafruit.com/products/358

The 1.44" TFT breakout
  ---->  http://www.adafruit.com/products/358

as well as Adafruit raw 1.8" TFT display
  ---->  http://www.adafruit.com/products/358


  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


/*
	STM32-based CO2 sensor(MQ135 based)
	This sketch will poll analog input and draw graphic representation on TFT display.
	In this version, only current, minimum and maximum values are displayed. 
*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>


// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     PB12
#define TFT_RST    PB13 // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     PB14

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK PA6   // set these to be whatever pins you like!
#define TFT_MOSI PA7   // set these to be whatever pins you like!
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//(60/5)*24*3 (трое суток) = 12*72=720+144 = 864

#define ARCH_CNT 128
//values archive
int values[ARCH_CNT], i;

float p = 3.1415926;

void setup(void) {
  
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW);
  
  for(i=0;i<=ARCH_CNT;i++)
  {
      values[i]=0;
  }
  
  Serial.begin(9600);
  Serial.print("Hello! ST7735 TFT Test");

  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

  // Use this initializer (uncomment) if you're using a 1.44" TFT
  //tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab

  Serial.println("Initialized");

  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  // large block of text
  tft.fillScreen(ST7735_BLACK);
  testdrawtext(5, 45, "LCD test...", ST7735_WHITE,  ST7735_BLACK);
  delay(500);
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(5,15,122,22,5,ST7735_WHITE);
  tft.fillRoundRect(7,17,118,18,5,ST7735_BLACK);
  
}

void loop() {
  int x=0, y=0;
  static int cur_time=0;
  
  float val,f;
  static float val_max=0.0, val_min=4095.0;
  int val_i=0;
  String s;
  char buf[20];
  uint16_t color;
  
  val = analogRead(PA1);
  //Update min/max values
  if(val_max<val)
  {
    val_max=val;
  }
  if(val_min>val)
  {
    val_min=val;
  }
  
  //sprintf(buf, "%f", val);
  
  //Normalize value to 0...100
  val_i=round(((val-val_min)/(val_max-val_min))*100.0);
  sprintf(buf, "RAW/nrmCO2:%d/%u  ", round(val), val_i);
  //testdrawtext(5, 45, "                 ", ST7735_WHITE);
  // tft.fillScreen(ST77XX_BLACK);
  color=ST77XX_WHITE;
  /*if(val_i<90)
  {
    color=ST77XX_GREEN;
  }
  if((val_i>90)&&(val_i<110))
  {
    color=ST77XX_WHITE;
  }
  if((val_i>110)&&(val_i<120))
  {
    color=ST77XX_YELLOW;
  }
  if((val_i>120)&&(val_i<135))
  {
    color=ST77XX_ORANGE;
  }
  if((val_i>135)&&(val_i<999))
  {
    color=ST77XX_RED;
  }*/
  testdrawtext(5, 5, buf, color,ST77XX_BLACK);
  
  digitalWrite(PC13, HIGH);

  //draw bar indicator
  f=(1.14)*(val_i);
  x=round(f);
  tft.fillRoundRect(7,17,118,18,5,ST7735_BLACK);
  tft.fillRoundRect(9,18,x,16,3,color);
  delay(500);

  //every 225 seconds - save data in archive (archive length is 128 elements, 8 hours if we will save one in 225 sec).
  //assume that "tick" occuring every 0.5 sec -> 5 mins = 600 ticks.
  //if(cur_time>=600)
  if(cur_time>=40)
  {
    //Redraw graph
    tft.fillRect(0,40,128,120,ST7735_WHITE);
    for(i=ARCH_CNT-1;i>0;i--)
    {
      values[i]=values[i-1];
      //draw point on graph
      x=127-i;
      y=159-values[i];
      //y=159-i;
      tft.drawPixel(x,y,ST7735_RED);
    }
    values[0]=val_i;
    cur_time=0;
  }
  cur_time++;
}


void testdrawtext(int wid, int hei, char *text, uint16_t color, uint16_t bg_color) {
  tft.setCursor(wid, hei);
  tft.setTextColor(color,bg_color);
  tft.setTextWrap(true);
  tft.print(text);
}
