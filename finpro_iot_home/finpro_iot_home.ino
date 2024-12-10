#define BLYNK_TEMPLATE_ID "TMPL6tkliGpGD"
#define BLYNK_TEMPLATE_NAME "finpro iot"
#define BLYNK_AUTH_TOKEN "SAcbd3I4mzH2_bVaYn8A3i93_kZ83vwy"

#include "DHT.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define DHTPIN 4
#define DHTTYPE DHT11

char ssid[] = "BananaXD";
char pass[] = "&*kA4gu$x800=D";

static const TickType_t DHT_TIMEOUT = 5000 / portTICK_PERIOD_MS; // dht timeout timer 5s
static TimerHandle_t DHT_TIMER = NULL;

float t, h;
float lastT = 0;
float lastH = 0;


// callback function untuk merggunakan nilai terakhir suhu
void DHTLatest(TimerHandle_t xTimer) {
  Serial.println("DHT22 timed out");
  Serial.print(F("Latest Humidity: "));
  Serial.print(lastT);
  Serial.println(F("%"));
  Serial.print("Latest Temperature: ");
  Serial.print(lastH);
  Serial.println(F("°C "));
}


// Iniitialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// receive input from dht and reset timer
void doDHT(void *parameters) {
  dht.begin();

  while (1) {
    t = dht.readTemperature();
    h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      lastT = t;
      lastH = h;
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.println(F("%"));
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(F("°C "));

      Blynk.virtualWrite(V0, t);
      Blynk.virtualWrite(V1, h);

      xTimerStart(DHT_TIMER, portMAX_DELAY);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("System Start");

  // create timeout timer
  DHT_TIMER = xTimerCreate(
                      "DHT timer",    // Name of timer
                      DHT_TIMEOUT,         // Period of timer (in ticks)
                      pdFALSE,              // one-shot
                      (void *)0,           // Timer ID
                      DHTLatest);          // Callback function

  xTimerStart(DHT_TIMER, portMAX_DELAY);

  // create task DHT
  xTaskCreatePinnedToCore(doDHT,
                          "Do DHT",
                          4096,
                          NULL,
                          1,
                          NULL,
                          0);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}