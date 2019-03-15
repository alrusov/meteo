#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN    8
#define DHTTYPE   DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

uint32_t delayMS;

void setup() {
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  bmp.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void loop() {
  delay(delayMS);

  int p = int(bmp.readPressure() / 133.322 + 0.5);

  float t1 = bmp.readTemperature();
  t1 = isnan(t1) ? -99 : t1;

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float t2 = event.temperature;
  t2 = isnan(t2) ? -99 : t2;

  dht.humidity().getEvent(&event);
  int h = isnan(event.relative_humidity) ? -99 : int(event.relative_humidity + 0.5);


  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(45, 13);
  display.print(int((t1 + t2) / 2 + 0.5));
  display.print(" C");

  display.setTextSize(1);
  display.setCursor(10, 1);
  display.print(h);
  display.print(" %");

  display.setCursor(90, 1);
  display.print(p);
  display.print(" mm");

  display.display();
}
