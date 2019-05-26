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

#define ARCH_CNT 864
//values archive
int values[ARCH_CNT]

float p = 3.1415926;

void setup(void) {
  for(i=0;i=ARCH_NUM;i++)
  {
      values[i]=0;
  }
  pinMode(PC13, OUTPUT);
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
  int abs_min=85, abs_max=180, x=0, y=0;
  static int local_min=9999, local_max=0, cur_time=0;
  
  float val,f;
  int val_i=0;
  String s;
  char buf[20];
  uint16_t color;
  
  val = analogRead(PA1);
  
  //sprintf(buf, "%f", val);
  val_i=round(val);
  sprintf(buf, "RAW CO2 val: %d     ", val_i);
  //testdrawtext(5, 45, "                 ", ST7735_WHITE);
  // tft.fillScreen(ST77XX_BLACK);
  color=ST77XX_WHITE;
  if(val_i<90)
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
  }
  testdrawtext(5, 5, buf, color,ST77XX_BLACK);
  
  digitalWrite(PC13, HIGH);

  //draw bar indicator

  //x=x0+(len_x)*(CO2max-CO2)/(CO2)
  if(val_i>abs_max)
  {
    val_i=abs_max;
  }
  f=9.0+(107.0)*((val_i-abs_min))/(abs_max-abs_min);
  x=round(f);
  tft.fillRoundRect(7,17,118,18,5,ST7735_BLACK);
  tft.fillRoundRect(9,18,x,16,3,color);

  //draw local min/local max lines
  f=9.0+(107.0)*((local_min-abs_min))/(abs_max-abs_min);
  x=round(f);
  tft.drawFastVLine(x, 18, 17, ST7735_CYAN);
  tft.drawFastVLine(x+1, 18, 17, ST7735_CYAN);
  f=9.0+(107.0)*((local_max-abs_min))/(abs_max-abs_min);
  x=round(f);
  tft.drawFastVLine(x, 18, 17, ST77XX_MAGENTA);
  tft.drawFastVLine(x+1, 18, 17, ST77XX_MAGENTA),
  
  //tft.invertDisplay(true);
  delay(500);
  //tft.invertDisplay(false);
  //digitalWrite(PC13, LOW);
  //delay(500);
  //digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);              // wait for a second
  //digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);              // wait for a second

  //Sensor warmup
  if(cur_time<120)
  {
    //cur_time++;
  }
  else
  {
    if(val_i<local_min)
    {
      local_min=val_i;
    }
    if(val_i>local_max)
    {
      local_max=val_i;
    }
     
    x=40;
    sprintf(buf, "Min CO2 val: %d     ", local_min);
    testdrawtext(5, x, buf, ST7735_CYAN,ST7735_BLACK);
    sprintf(buf, "MAX CO2 val: %d     ", local_max);
    x=x+10;
    testdrawtext(5, x, buf, ST77XX_MAGENTA,ST7735_BLACK);
  }

  //every 5 minutes - save data in archive.
  //assume that "tick" occuring every 0.5 sec -> 5 mins = 600 ticks.
  if(cur_time>=600)
  {
    for(i=0;i=ARCH_NUM-1;i++)
    {
      values[i+1]=values[i];
    }
    values[0]=val_i;
  }
  

  cur_time++;
}


void testdrawtext(int wid, int hei, char *text, uint16_t color, uint16_t bg_color) {
  tft.setCursor(wid, hei);
  tft.setTextColor(color,bg_color);
  tft.setTextWrap(true);
  tft.print(text);
}
