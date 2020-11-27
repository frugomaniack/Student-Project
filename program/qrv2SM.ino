#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include "hsv.h"
#include "graficzki.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
#define MAXHUE 256*6
#define CNT 16
#define PIN       8 // Marked D1 on GEMMA
#define NUM_LEDS 16 //ilosc diód w pierścieniu

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB);
uint32_t color = strip.Color(75, 250, 100); // Change RGB color value here
int bluetooth;
int kolor=0;//red,green,blue 0,1,2
int red=100;
int green=0;
int blue=0;
int jasnosc=30;
int position = 0;
  int przycisk=0;
  int czas=millis();
  int pom=0;
  float x=8;
  float y=64;
  float x1=48;
  float y1=64;
  float x2=-32;
  float y2=32;

SoftwareSerial hc06(10,11);

void setup() 
{
  Serial.begin(115200);
  hc06.begin(9600);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), akcja, FALLING);

  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), akcjaminus, FALLING);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000);

  strip.begin();
  strip.show();            // Initialize all pixels to 'off'


  color = strip.Color(255, 255, 255);
  strip.setBrightness(50);
  for(int i=0; i<24; i++) 
  {
    strip.setPixelColor(i,color);     
    strip.show();
  } 

  Serial.println("Skonfigurowano");

}
///////////////////////////////////////void setup();

void rotatingred()
{
   for (int i = 0; i < CNT; i++)
   {
     strip.setPixelColor((i + position) % CNT, getPixelColorHsv(i, 0, 255, strip.gamma8(i * (255 / CNT))));
     strip.show();
     position++;
     position %= CNT;
     delay(50);
   }
}
///////////////////////////////////////void rotatingred();
void tencza()
{
     
    for (int i = 0; i < CNT; i++)
    {
     strip.setPixelColor((i + position) % CNT, getPixelColorHsv(i, i * (MAXHUE / CNT), 255, 10));
     strip.show();
     position++;
     position %= CNT;
     delay(50);
    }
}
///////////////////////////////////////void tencza();
void toster()
{
display.clearDisplay();  
  if(pom==0)
  {
    display.drawBitmap( x, y, toaster0, 32, 32, 1);
    display.drawBitmap( x1, y1, toaster0, 32, 32, 1);
  display.drawBitmap( x2, y2, toaster0, 32, 32, 1);
  }
  if(pom==1)
  {
    display.drawBitmap( x, y, toaster1, 32, 32, 1);
   display.drawBitmap( x1, y1, toaster1, 32, 32, 1);
    display.drawBitmap( x2, y2, toaster1, 32, 32, 1);
  }
  if(pom==2)
  {
    display.drawBitmap( x, y, toaster2, 32, 32, 1);
    display.drawBitmap( x1, y1, toaster2, 32, 32, 1);
    display.drawBitmap( x2, y2, toaster2, 32, 32, 1);
  }
delay(100);
display.display();
x=x+1.5;
y=y-1.5; 
x1=x1+2;
y1=y1-2;
x2++;
y2--;
pom++;
if(pom>2)
{
  pom=0;
}
    if(x>120||y<-32)
  {
   x=0;
   y=64;
  }
     if(x1>120||y1<-32)
  {
  x1=48;
  y1=64;
  }
    if(x2>120||y2<-32)
  {
  x2=-32;
  y2=32;
  }
}
///////////////////////////////////////void toster();


void loop() 
{
  if(hc06.available())
  {
    Serial.print("hc06:");
     bluetooth=hc06.read();
     
    Serial.println(bluetooth);
    if(bluetooth==3)
    {
      akcja();
    }
    else
     if(bluetooth==2)
    {
      akcjaminus();
    }
  }
 
if(przycisk==0)
  {
    hc06.println(0);
    display.clearDisplay(); 
display.drawBitmap( 0, 0, apkaQR, 128, 64, 1);

 display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_INVERSE); // Draw white text
  display.setCursor(5, 0);     // Start at top-left corner
  display.println("Pobierz APK");
  
display.display();
  }
   if(przycisk==1)
  {
    hc06.println(1);
    display.clearDisplay(); 
display.drawBitmap( 0, 0, renderQR, 128, 64, 1);

display.display();
  }
   
   if(przycisk==2)
  {
    hc06.println(2);

display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text

      display.setCursor(20, 10); 
      display.print(" Red: ");
      display.println(red);
      display.setCursor(20, 20); 
      display.print(" Green: ");
      display.println(green);
      display.setCursor(20, 30); 
      display.print(" Blue: ");
      display.println(blue);
      display.setCursor(20, 40); 
      display.print(" brightness: ");
      display.println(jasnosc);
      
      display.display();

 bluetooth=hc06.read();
 if(bluetooth==251)
    {
      kolor=1;
      //Serial.println("Red");
    }
    else
    if(bluetooth==252)
    {
      kolor=2;
      //Serial.println("Green");
    }
    else
     if(bluetooth==253)
    {
      kolor=3;
      //Serial.println("Blue");
    }
    else
    if(bluetooth==5)
    {
      kolor=4;
      //Serial.println("jasnosc");
    }
    else
    {
      kolor=0;
       if(bluetooth==2||bluetooth==3)
      {
        przycisk++;
      }
    }
      bluetooth=hc06.read();
    if(kolor==1)
    {
      red=bluetooth;
      red=map(red,10,250,0,255);
      Serial.print("Red:");
      Serial.print(red);
      Serial.print(" green");
      Serial.print(green);
      Serial.print(" blue");
      Serial.println(blue);  
    }
    else
    if(kolor==2)
    {
      green=bluetooth;
      green=map(green,10,250,0,255);
      Serial.print("Red:");
      Serial.print(red);
      Serial.print(" green");
      Serial.print(green);
      Serial.print(" blue");
      Serial.println(blue);
    }
    else
     if(kolor==3)
    {
      blue=bluetooth;
      blue=map(blue,10,250,0,255);
      Serial.print("Red:");
      Serial.print(red);
      Serial.print(" green");
      Serial.print(green);
      Serial.print(" blue");
      Serial.println(blue);
    }
     else    
    if(kolor==4)
    {
      jasnosc=bluetooth;
      jasnosc=map(jasnosc,10,255,0,100);
      Serial.print(" jasnosc");
      Serial.println(jasnosc);
    }


      strip.setBrightness(jasnosc); 
    for(int i=0; i<16; i++) 
  {
    color = strip.Color(red, green, blue);
    strip.setPixelColor(i,color);     
    strip.show();
  } 
  
  }
  
  
  if(przycisk==3)
  {
    hc06.println(3);
    strip.clear();
    strip.show();
    toster();
  }
  if(przycisk==4)
  {
    hc06.println(4);
    strip.clear();
    strip.show();
    display.clearDisplay(); 
display.drawBitmap( 0, 0, lenny128x64, 128, 64, 1);
display.display();
  }

  if(przycisk==5)
  {
    hc06.println(5);
    tencza();
  }
  
}
///////////////////////////////////////void loop();
void akcja()
{
  przycisk++;
  Serial.println(przycisk);
  if(przycisk>5)
  {
    przycisk=0;
  }
}
///////////////////////////////////////void akcja();
void akcjaminus()
{
  przycisk--;
  Serial.println(przycisk);
  if(przycisk<0)
  {
    przycisk=5;
  }
}
///////////////////////////////////////void akcjaminus();
