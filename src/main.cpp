#include "configuration.h"

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#if defined(SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
#include <elapsedMillis.h>
#endif

//#include "Pixel_t.h"
#include "Bn880qGps.h"

// -------------------------------------------------------------------------------------------------

struct Resources {
    struct {
        struct PreInit {
            PreInit() {
                Serial.begin(SERIAL_BAUD_RATE);
                while(!Serial)
                    delay(10);

                Serial.printf("\033c"); // clear terminal
                Serial.print("\n\n\n");
                Serial.println("Resources::PreInit: done");
            }
        } _;
    } _;

    // ---------------------------------------------------------------------------------------------

    HardwareSerial &console = Serial;
    SoftwareSerial gps_serial{ GPS_SERIAL_RX_PIN, GPS_SERIAL_TX_PIN };
    Bn880qGps gps{ gps_serial };
#if defined(SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
    elapsedMillis print_timer;
#endif
    Adafruit_NeoPixel pixel{ 1, D6, NEO_GRB + NEO_KHZ400 };
    bool led_state{ LOW };

    // ---------------------------------------------------------------------------------------------
    void setup() {
        console.println("Resources::setup");

        // pixel.setup();
        pixel.begin();
        pixel.setPixelColor(0, SIGNAL_LED_WARN_COLOR);
        pixel.setBrightness(SIGNAL_LED_WARN_BRIGHTNESS);
        pixel.show();

        wifi_status_led_uninstall();
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, led_state);
        wifi_set_opmode(NULL_MODE);
        wifi_set_sleep_type(MODEM_SLEEP_T);
        wifi_fpm_open();
        wifi_fpm_do_sleep(0xFFFFFFF);

        gps_serial.begin(GPS_SERIAL_BAUD_RATE, GPS_SERIAL_CONFIG);
        gps.setup();
#if defined(SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
        print_timer = 0;
#endif
        console.println("Resources::setup: done");
    }

    // ---------------------------------------------------------------------------------------------

    void process() {
        if(gps.process()) {
#if defined(SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
            if(print_timer > SERIAL_PRINT_GPS_DATA_INTERVAL_MS) {
                // Serial.printf("\033c"); // clear terminal
                Serial.printf("\033[40A"); // move 28 lines up
                printTabular(gps.getData());
                print_timer = 0;
            }
#endif
        }
    }
} r;

// -------------------------------------------------------------------------------------------------

void setup() { r.setup(); }

// -------------------------------------------------------------------------------------------------

void loop() { r.process(); }
