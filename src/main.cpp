#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
//#include "Pixel_t.h"
#include "Bn880qGps.h"

// -------------------------------------------------------------------------------------------------

struct Resources
{
    struct
    {
        struct pre_setup
        {
            pre_setup()
            {
                Serial.begin(230400);
                while(!Serial)
                    delay(10);
                Serial.print("\n\n\n");
                Serial.println("pre_setup");

                //pinMode(D3, INPUT);
                //pinMode(D5, OUTPUT);
            }
        } _;
    } _;

    HardwareSerial &console = Serial;
    SoftwareSerial gps_serial{ D3, D5 };

    Adafruit_NeoPixel pixel{ 1, D6, NEO_GRB + NEO_KHZ400 };

    bool led_state{ LOW };

    Bn880qGps gps{gps_serial};

    void setup()
    {
        console.println("setup");

        // pixel.setup();
        pixel.begin();
        pixel.setPixelColor(0, 128, 0, 128);
        pixel.setBrightness(50);
        pixel.show();

        wifi_status_led_uninstall();
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, led_state);
        wifi_set_opmode(NULL_MODE);
        wifi_set_sleep_type(MODEM_SLEEP_T);
        wifi_fpm_open();
        wifi_fpm_do_sleep(0xFFFFFFF);

        gps_serial.begin(9600, SWSERIAL_8N1);
        gps.setup();
    }

    void process()
    {
        if(gps.process()){
            Serial.println();
            gps.print();
        }
    }
} r;

// -------------------------------------------------------------------------------------------------

void setup() { r.setup(); }

// -------------------------------------------------------------------------------------------------

void loop() { r.process(); }
