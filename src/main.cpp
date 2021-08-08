#include "configuration.h"

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

//#include "../.pio/libdeps/gpsgadget/SparkFun u-blox Arduino Library/src/SparkFun_Ublox_Arduino_Library.h"
#include "SparkFun_Ublox_Arduino_Library.h"

#if defined(SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
#include <elapsedMillis.h>
#endif

//#include "Pixel_t.h"
//#include "Bn880qGps.h"

// -------------------------------------------------------------------------------------------------

struct Resources {
    struct {
        struct PreInit {
            PreInit() {
                Serial.begin(SERIAL_BAUD_RATE, SERIAL_CONFIG);
                while(!Serial)
                    delay(10);

                // Serial.printf("\033c"); // clear terminal
                Serial.print("\n\n\n");
                Serial.println("Resources::PreInit: done");
            }
        } _;
    } _;

    // ---------------------------------------------------------------------------------------------

    HardwareSerial &console = Serial;
    SoftwareSerial gps_serial{ GPS_SERIAL_RX_PIN, GPS_SERIAL_TX_PIN };
    // Bn880qGps gps{ gps_serial };
    SFE_UBLOX_GPS gps;

#if defined(SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
    elapsedMillis print_timer;
#endif
    Adafruit_NeoPixel pixel{ 1, D6, NEO_GRB + NEO_KHZ400 };
    bool led_state{ LOW };

    // ---------------------------------------------------------------------------------------------
    void setup() {
#define stringify_macro(x) #x
#define stringify_value(x) stringify_macro(x)

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

        gps_serial.begin(GPS_SERIAL_INITIAL_BAUD_RATE, GPS_SERIAL_CONFIG);
        uint8_t retry{ 6 };
        while(!gps.begin(gps_serial) && retry-- > 1) {
            console.println(
            F("Resources::setup: failed to detect GPS initial baud rate " stringify_value(
            GPS_SERIAL_INITIAL_BAUD_RATE) " " stringify_value(GPS_SERIAL_CONFIG)));
            if(retry != 0) {
                delay(2000);
            }
        }

        if(retry == 0) {
            console.println(("Resources::setup: gave up, try operational baud rate"));
        } else {
            console.println(
            F("Resources::setup: connected GPS with initial baud rate " stringify_value(
            GPS_SERIAL_INITIAL_BAUD_RATE) " " stringify_value(GPS_SERIAL_CONFIG)));
        }

#if defined(GPS_SERIAL_TARGET_BAUD_RATE)
        console.println(
        F("Resources::setup: switching GPS baud rate from " stringify_value(GPS_SERIAL_INITIAL_BAUD_RATE) " " stringify_value(
        GPS_SERIAL_CONFIG) " to " stringify_value(GPS_SERIAL_TARGET_BAUD_RATE) " " stringify_value(GPS_SERIAL_CONFIG)));

        delay(2000);
        gps.setSerialRate(GPS_SERIAL_TARGET_BAUD_RATE);
        gps_serial.begin(GPS_SERIAL_TARGET_BAUD_RATE, GPS_SERIAL_CONFIG);
        while(!gps.begin(gps_serial)) // Connect to the Ublox module using Wire port
        {
            console.println(
            F("Resources::setup: failed to detected GPS with target baud rate " stringify_value(
            GPS_SERIAL_TARGET_BAUD_RATE) " " stringify_value(GPS_SERIAL_CONFIG)));
            delay(2000);
        }
#endif

        if(retry == 0) {
            console.println(F("Resources::setup: connected but no clean re-connect detected"));
            gps.factoryReset();
            console.println(F("Resources::setup: factory reset GPS"));
            delay(2000);
            console.println(F("Resources::setup: reboot device, bye"));
            ESP.restart();
        } else {
            console.println(
            F("Resources::setup: connected GPS with target baud rate " stringify_value(
            GPS_SERIAL_TARGET_BAUD_RATE) " " stringify_value(GPS_SERIAL_CONFIG)));
        }

        gps.setI2COutput(COM_TYPE_UBX);
        gps.setUART1Output(COM_TYPE_UBX);
#if defined(GPS_NAVIGATION_FREQUENCY)
        gps.setNavigationFrequency(GPS_NAVIGATION_FREQUENCY);
#endif
#if defined(GPS_SET_AUTO_PVT)
        gps.setAutoPVT(true); // Tell the GPS to "send" each solution
#elif defined(GPS_ASSUME_AUTO_PVT)
        gps.assumeAutoPVT(true, );
#endif

#if defined(DEBUG_GPS_COMMUNICATION)
#if defined(GPS_AUTO_PVT)
        gps.enableDebugging(Serial, true);
#else
        gps.enableDebugging();
#endif
#endif

#if defined(SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
        print_timer = 0;
#endif
        console.println(F("Resources::setup: done"));
#undef stringify_macro
#undef stringify_value
    }

    // ---------------------------------------------------------------------------------------------

    void process() {
        /* if(gps.process()) {
 #if defined(SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
             if(print_timer > SERIAL_PRINT_GPS_DATA_INTERVAL_MS) {
                 // Serial.printf("\033c"); // clear terminal
                 printTabular(gps.getData());
                 Serial.printf("\033[40A"); // move 28 lines up
                 print_timer = 0;
             }
 #endif
             static String serial_buffer;
             for(int c{ Serial.read() }; c > -1; c = Serial.read()) {

                 if(c == '\r' || c == '\n') {
                     if(!serial_buffer.isEmpty()) {
                         gps.sendSentence(serial_buffer.c_str());
                         serial_buffer.clear();
                     }
                 } else {
                     serial_buffer += static_cast<char>(c);
                 }
             }
         }*/

        // Query module only every second. Doing it more often will just cause I2C traffic.
        // The module only responds when a new position is available
#if defined(SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
#if defined(GPS_AUTO_PVT)
        if(gps.getPVT()) {
#endif
            if(print_timer >= SERIAL_PRINT_GPS_DATA_INTERVAL_MS)
            {
                console.print(F("\nLat: "));
                console.println(gps.getLatitude());

                console.print(F("\nLong: "));
                console.println(gps.getLongitude());
                // Serial.printf("\033[2A"); // move 28 lines up
                /*long latitude = gps.getLatitude();
                console.print(F("Lat: "));
                console.print(latitude);

                long longitude = gps.getLongitude();
                Serial.print(F(" Long: "));
                Serial.print(longitude);
                Serial.print(F(" (degrees * 10^-7)"));

                long altitude = gps.getAltitude();
                Serial.print(F(" Alt: "));
                Serial.print(altitude);
                Serial.print(F(" (mm)"));

                byte SIV = gps.getSIV();
                Serial.print(F(" SIV: "));
                Serial.print(SIV);

                Serial.println();
                Serial.print(gps.getgetYear());
                Serial.print("-");
                Serial.print(gps.getMonth());
                Serial.print("-");
                Serial.print(gps.getDay());
                Serial.print(" ");
                Serial.print(gps.getHour());
                Serial.print(":");
                Serial.print(gps.getMinute());
                Serial.print(":");
                Serial.println(gps.getSecond());

                Serial.println();*/
                print_timer = 0;
            }
#if defined(GPS_AUTO_PVT)
        }
#endif
#endif // SERIAL_PRINT_GPS_DATA_INTERVAL_MS
    }
} r;

// -------------------------------------------------------------------------------------------------

void setup() { r.setup(); }

// -------------------------------------------------------------------------------------------------

void loop() { r.process(); }
