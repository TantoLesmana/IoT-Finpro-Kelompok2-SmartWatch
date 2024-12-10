#define BLYNK_TEMPLATE_ID "TMPL6Ic1HFZR2"
#define BLYNK_TEMPLATE_NAME "finpro iot"
#define BLYNK_AUTH_TOKEN "xPCktTn6TcbyRq6AQ6jMq2i0aaOUd-Jn"
/*  Rui Santos & Sara Santos - Random Nerd Tutorials
    THIS EXAMPLE WAS TESTED WITH THE FOLLOWING HARDWARE:
    1) ESP32-2432S028R 2.8 inch 240×320 also known as the Cheap Yellow Display (CYD): https://makeradvisor.com/tools/cyd-cheap-yellow-display-esp32-2432s028r/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/cyd-lvgl/
    2) REGULAR ESP32 Dev Board + 2.8 inch 240x320 TFT Display: https://makeradvisor.com/tools/2-8-inch-ili9341-tft-240x320/ and https://makeradvisor.com/tools/esp32-dev-board-wi-fi-bluetooth/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/esp32-tft-lvgl/
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

/*  Install the "lvgl" library version 9.2 by kisvegabor to interface with the TFT Display - https://lvgl.io/
    *** IMPORTANT: lv_conf.h available on the internet will probably NOT work with the examples available at Random Nerd Tutorials ***
    *** YOU MUST USE THE lv_conf.h FILE PROVIDED IN THE LINK BELOW IN ORDER TO USE THE EXAMPLES FROM RANDOM NERD TUTORIALS ***
    FULL INSTRUCTIONS AVAILABLE ON HOW CONFIGURE THE LIBRARY: https://RandomNerdTutorials.com/cyd-lvgl/ or https://RandomNerdTutorials.com/esp32-tft-lvgl/   */
#include <lvgl.h>

/*  Install the "TFT_eSPI" library by Bodmer to interface with the TFT Display - https://github.com/Bodmer/TFT_eSPI
    *** IMPORTANT: User_Setup.h available on the internet will probably NOT work with the examples available at Random Nerd Tutorials ***
    *** YOU MUST USE THE User_Setup.h FILE PROVIDED IN THE LINK BELOW IN ORDER TO USE THE EXAMPLES FROM RANDOM NERD TUTORIALS ***
    FULL INSTRUCTIONS AVAILABLE ON HOW CONFIGURE THE LIBRARY: https://RandomNerdTutorials.com/cyd-lvgl/ or https://RandomNerdTutorials.com/esp32-tft-lvgl/   */
#include <TFT_eSPI.h>

// Install the "XPT2046_Touchscreen" library by Paul Stoffregen to use the Touchscreen - https://github.com/PaulStoffregen/XPT2046_Touchscreen - Note: this library doesn't require further configuration
#include <XPT2046_Touchscreen.h>

#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <time.h>

const char ntpServer[] = "id.pool.ntp.org";
String currentTime;

char ssid[] = "loltotan";
char pass[] = "ntnt1234";

double suhu, kelembapan, oxygen; // var global

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// If logging is enabled, it will inform the user about what is happening in the library
void log_print(lv_log_level_t level, const char * buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}

String currentHourMinute;

void updateTimeTask(void *pvParameters) {
  while (1) {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char timeString[40];
      strftime(timeString, sizeof(timeString), "%A, %d %B %Y %H:%M:%S", &timeinfo);
      currentTime = String(timeString);

      // Extract hour and minute
      char hourMinuteString[6];
      strftime(hourMinuteString, sizeof(hourMinuteString), "%H:%M", &timeinfo);
      currentHourMinute = String(hourMinuteString);
    }
    delay(5000); // Update time every 5 seconds
  }
}

// Get the Touchscreen data
void touchscreen_read(lv_indev_t * indev, lv_indev_data_t * data) {
  // Checks if Touchscreen was touched, and prints X, Y and Pressure (Z)
  if(touchscreen.tirqTouched() && touchscreen.touched()) {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    data->state = LV_INDEV_STATE_PRESSED;

    // Set the coordinates
    data->point.x = x;
    data->point.y = y;

    // Print Touchscreen info about X, Y and Pressure (Z) on the Serial Monitor
    /* Serial.print("X = ");
    Serial.print(x);
    Serial.print(" | Y = ");
    Serial.print(y);
    Serial.print(" | Pressure = ");
    Serial.print(z);
    Serial.println();*/
  }
  else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

int btn1_count = 0;
// Callback that is triggered when btn1 is clicked
static void event_handler_btn1(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_CLICKED) {
    btn1_count++;
    LV_LOG_USER("Button clicked %d", (int)btn1_count);
  }
}

// Callback that is triggered when btn2 is clicked/toggled
static void event_handler_btn2(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t*) lv_event_get_target(e);
  if(code == LV_EVENT_VALUE_CHANGED) {
    LV_UNUSED(obj);
    LV_LOG_USER("Toggled %s", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "on" : "off");
  }
}

static lv_obj_t * slider_label;
// Callback that prints the current slider value on the TFT display and Serial Monitor for debugging purposes
static void slider_event_callback(lv_event_t * e) {
  lv_obj_t * slider = (lv_obj_t*) lv_event_get_target(e);
  char buf[8];
  lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
  lv_label_set_text(slider_label, buf);
  lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  LV_LOG_USER("Slider changed to %d%%", (int)lv_slider_get_value(slider));
}

void lv_create_main_gui(void) {
  char suhu_buffer[10];
  char kelembapan_buffer[10];
  char oxygen_buffer[10];

  dtostrf(suhu, 6, 2, suhu_buffer); // Convert suhu_blynk to string
  dtostrf(kelembapan, 6, 2, kelembapan_buffer); // Convert kelembapan_blynk to string
  dtostrf(oxygen, 6, 2, oxygen_buffer); // Convert oxygen_blynk to string

  // Create a text label aligned center on top ("Hello, world!")
  lv_obj_t * text_label = lv_label_create(lv_screen_active());
  lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  lv_label_set_text(text_label, "SMARTWATCH");
  lv_obj_set_width(text_label, 150);    // Set smaller width to make the lines wrap
  lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_label, LV_ALIGN_CENTER, 0, -90);

  lv_obj_t * text_label2 = lv_label_create(lv_screen_active());
  lv_label_set_long_mode(text_label2, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  lv_label_set_text(text_label2, "Temperature:");
  lv_obj_set_width(text_label2, 150);    // Set smaller width to make the lines wrap
  lv_obj_set_style_text_align(text_label2, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_label2, LV_ALIGN_CENTER, 0, -10);

  // lv_obj_t * text_label3 = lv_label_create(lv_screen_active());
  // lv_label_set_long_mode(text_label3, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  // lv_label_set_text(text_label3, suhu_buffer);
  // lv_obj_set_width(text_label3, 150);    // Set smaller width to make the lines wrap
  // lv_obj_set_style_text_align(text_label3, LV_TEXT_ALIGN_CENTER, 0);
  // lv_obj_align(text_label3, LV_ALIGN_CENTER, 0, 15);

    lv_obj_t * text_labelL2 = lv_label_create(lv_screen_active());
  lv_label_set_long_mode(text_labelL2, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  lv_label_set_text(text_labelL2, "Humidty:");
  lv_obj_set_width(text_labelL2, 150);    // Set smaller width to make the lines wrap
  lv_obj_set_style_text_align(text_labelL2, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_labelL2, LV_ALIGN_CENTER, -110, -10);

  // lv_obj_t * text_labelL3 = lv_label_create(lv_screen_active());
  // lv_label_set_long_mode(text_labelL3, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  // lv_label_set_text(text_labelL3, kelembapan_buffer);
  // lv_obj_set_width(text_labelL3, 150);    // Set smaller width to make the lines wrap
  // lv_obj_set_style_text_align(text_labelL3, LV_TEXT_ALIGN_CENTER, 0);
  // lv_obj_align(text_labelL3, LV_ALIGN_CENTER, -110, 15);

  lv_obj_t * text_labelT2 = lv_label_create(lv_screen_active());
  lv_label_set_long_mode(text_labelT2, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  lv_label_set_text(text_labelT2, "Oxygen:");
  lv_obj_set_width(text_labelT2, 150);    // Set smaller width to make the lines wrap
  lv_obj_set_style_text_align(text_labelT2, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_labelT2, LV_ALIGN_CENTER, 110, -10);

  // lv_obj_t * text_labelT3 = lv_label_create(lv_screen_active());
  // lv_label_set_long_mode(text_labelT3, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  // lv_label_set_text(text_labelT3, oxygen_buffer);
  // lv_obj_set_width(text_labelT3, 150);    // Set smaller width to make the lines wrap
  // lv_obj_set_style_text_align(text_labelT3, LV_TEXT_ALIGN_CENTER, 0);
  // lv_obj_align(text_labelT3, LV_ALIGN_CENTER, 110, 15);

  // lv_obj_t * text_labelclock = lv_label_create(lv_screen_active());
  // lv_label_set_long_mode(text_labelclock, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  // lv_label_set_text(text_labelclock, "12:34");
  // lv_obj_set_width(text_labelclock, 150);    // Set smaller width to make the lines wrap
  // lv_obj_set_style_text_align(text_labelclock, LV_TEXT_ALIGN_CENTER, 0);
  // lv_obj_align(text_labelclock, LV_ALIGN_CENTER, 0, -50);

  // static lv_style_t style;
  // lv_style_init(&style);
  // lv_style_set_text_font(&style, &lv_font_montserrat_20); // Set font size to 20

  // // Apply the style to the label
  // lv_obj_add_style(text_labelclock, &style, 0);

  lv_obj_t * btn_label;
  // Create a Button (btn1)
  lv_obj_t * btn1 = lv_button_create(lv_screen_active());
  lv_obj_add_event_cb(btn1, event_handler_btn1, LV_EVENT_ALL, NULL);
  lv_obj_align(btn1, LV_ALIGN_CENTER, -100, -50);
  lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

  btn_label = lv_label_create(btn1);
  lv_label_set_text(btn_label, "-");
  lv_obj_center(btn_label);

  // Create a Button (btn1)
  lv_obj_t * btn2 = lv_button_create(lv_screen_active());
  lv_obj_add_event_cb(btn2, event_handler_btn1, LV_EVENT_ALL, NULL);
  lv_obj_align(btn2, LV_ALIGN_CENTER, 100, -50);
  lv_obj_remove_flag(btn2, LV_OBJ_FLAG_PRESS_LOCK);

  btn_label = lv_label_create(btn2);
  lv_label_set_text(btn_label, "+");
  lv_obj_center(btn_label);

  lv_obj_t * text_labelD2 = lv_label_create(lv_screen_active());
  lv_label_set_long_mode(text_labelD2, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  lv_label_set_text(text_labelD2, "Note:");
  lv_obj_set_width(text_labelD2, 150);    // Set smaller width to make the lines wrap
  lv_obj_set_style_text_align(text_labelD2, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_labelD2, LV_ALIGN_CENTER, 0, 50);

  lv_obj_t * text_labelD3 = lv_label_create(lv_screen_active());
  lv_label_set_long_mode(text_labelD3, LV_LABEL_LONG_WRAP);    // Breaks the long lines
  lv_label_set_text(text_labelD3, "Everything is normal :D");
  lv_obj_set_width(text_labelD3, 250);    // Set smaller width to make the lines wrap
  lv_obj_set_style_text_align(text_labelD3, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_labelD3, LV_ALIGN_CENTER, 0, 75);
}

void blynkTask(void *parameters) {
  while (1) {
    if (WiFi.status() == WL_CONNECTED) {
      Blynk.run();  // Process Blynk tasks
    } else {
      // Reconnect to Wi-Fi if disconnected
      if (WiFi.status() != WL_CONNECTED) {
          WiFi.begin(ssid, pass);
      }
    }

    Blynk.syncVirtual(V0); // Sync temperature
    Blynk.syncVirtual(V1); // Sync humidity
    Blynk.syncVirtual(V2); // Sync Oxygen

    // Use currentTime for any time-dependent logic
    Serial.print("Current Time: ");
    Serial.println(currentHourMinute);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

BLYNK_WRITE(V0) { // mengambil suhu dari rumah
  suhu = param.asDouble();
  Serial.print("Updated suhu: ");
  Serial.println(suhu);
}

BLYNK_WRITE(V1) { // mengambil kelembapan dari rumah
  kelembapan = param.asDouble();
  Serial.print("Updated kelembapan: ");
  Serial.println(kelembapan);
}

BLYNK_WRITE(V2) { // mengambil kelembapan dari rumah
  oxygen = param.asDouble();
  Serial.print("Updated oxygen: ");
  Serial.println(oxygen);
}

void setup() {
  Blynk.config(BLYNK_AUTH_TOKEN);
  WiFi.begin(ssid, pass);
  configTime(7 * 3600, 0, ntpServer);
  String LVGL_Arduino = String("LVGL Library Version: ") + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);
  
  // Start LVGL
  lv_init();
  // Register print function for debugging
  lv_log_register_print_cb(log_print);

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 0: touchscreen.setRotation(0);
  touchscreen.setRotation(2);

  // Create a display object
  lv_display_t * disp;
  // Initialize the TFT display using the TFT_eSPI library
  disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);
    
  // Initialize an LVGL input device object (Touchscreen)
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  // Set the callback function to read Touchscreen input
  lv_indev_set_read_cb(indev, touchscreen_read);

  // Function to draw the GUI (text, buttons and sliders)
  lv_create_main_gui();

  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  xTaskCreatePinnedToCore(
    updateTimeTask,
    "Update Time Task",
    2048,
    NULL,
    1,
    NULL,
    1 // Core 1
  );
  
  // create task blynk
  xTaskCreatePinnedToCore(blynkTask,
                          "Run Blynk",
                          4096,
                          NULL,
                          1,
                          NULL,
                          1);
}

void loop() {
  lv_task_handler();  // let the GUI do its work
  lv_tick_inc(5);     // tell LVGL how much time has passed

  Serial.print("Temperature (suhu): ");
  Serial.println(suhu);
  Serial.print("Humidity (kelembapan): ");
  Serial.println(kelembapan);
  Serial.print("Oxygen Level: ");
  Serial.println(oxygen);

  Serial.println(Blynk.connected());
  updateTemperatureDisplay(suhu);
  updateHumidityDisplay(kelembapan);
  updateOxygenDisplay(oxygen);
  updateTimeDisplay(currentHourMinute);

  delay(1000);           // let this time pass
}

void updateTemperatureDisplay(double temperature) {
    // Find the temperature label in the GUI and update its text
    static lv_obj_t * text_label3 = lv_label_create(lv_screen_active());
    char tempStr[16];
    snprintf(tempStr, sizeof(tempStr), "%.1f C", temperature);
    lv_label_set_long_mode(text_label3, LV_LABEL_LONG_WRAP);    // Breaks the long lines
    lv_label_set_text(text_label3, tempStr);
    lv_obj_set_width(text_label3, 150);    // Set smaller width to make the lines wrap
    lv_obj_set_style_text_align(text_label3, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(text_label3, LV_ALIGN_CENTER, 0, 15);
}

void updateHumidityDisplay(double humidity) {
    // Find the temperature label in the GUI and update its text
    static lv_obj_t * text_labelL3 = lv_label_create(lv_screen_active());
    char tempStr[16];
    snprintf(tempStr, sizeof(tempStr), "%.1f %", humidity);
    lv_label_set_long_mode(text_labelL3, LV_LABEL_LONG_WRAP);    // Breaks the long lines
    lv_label_set_text(text_labelL3, tempStr);
    lv_obj_set_width(text_labelL3, 150);    // Set smaller width to make the lines wrap
    lv_obj_set_style_text_align(text_labelL3, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(text_labelL3, LV_ALIGN_CENTER, -110, 15);
}

void updateOxygenDisplay(double oxygen) {
    // Find the temperature label in the GUI and update its text
    static lv_obj_t * text_labelT3 = lv_label_create(lv_screen_active());
    char tempStr[16];
    snprintf(tempStr, sizeof(tempStr), "%.1f %", oxygen);
    lv_label_set_long_mode(text_labelT3, LV_LABEL_LONG_WRAP);    // Breaks the long lines
    lv_label_set_text(text_labelT3, tempStr);
    lv_obj_set_width(text_labelT3, 150);    // Set smaller width to make the lines wrap
    lv_obj_set_style_text_align(text_labelT3, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(text_labelT3, LV_ALIGN_CENTER, 110, 15);
}

void updateTimeDisplay(String time) {
    // Find the temperature label in the GUI and update its text
    static lv_obj_t * text_labelclock = lv_label_create(lv_screen_active());
    // char tempStr[16];
    // snprintf(tempStr, sizeof(tempStr), "%.1f %", oxygen);
    lv_label_set_long_mode(text_labelclock, LV_LABEL_LONG_WRAP);    // Breaks the long lines
    lv_label_set_text(text_labelclock, time.c_str());
    lv_obj_set_width(text_labelclock, 150);    // Set smaller width to make the lines wrap
    lv_obj_set_style_text_align(text_labelclock, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(text_labelclock, LV_ALIGN_CENTER, 0, -50);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &lv_font_montserrat_20); // Set font size to 20

    // Apply the style to the label
    lv_obj_add_style(text_labelclock, &style, 0);
}