#pragma once

// -------------------------------------------------------------------------------------------------
// ------------------------------------- SECTION INPUT ---------------------------------------------
// -------------------------------------------------------------------------------------------------
/**
 * Keyboard - MPR121 Capacitive
 */

#define DEBUG_KEYBOARD

/**
 * GPS - Beitian BN-880Q
 */

//#define DEBUG_GPS_COMMUNICATION
//#define DEBUG_GPS_DATA_PROCESSING

#define GPS_SERIAL_RX_PIN D3
#define GPS_SERIAL_TX_PIN D5

#define GPS_SERIAL_BAUD_RATE 9600
#define GPS_SERIAL_CONFIG SWSERIAL_8N1 // see SoftwareSerial.h SoftwareSerialConfig

// -------------------------------------------------------------------------------------------------
// ------------------------------------- SECTION OUTPUT --------------------------------------------
// -------------------------------------------------------------------------------------------------

/**
 * Serial
 */

#define SERIAL_BAUD_RATE 230400

/**
 * Oled - 0.66" 64x48
 */

/**
 * Visual - WS2812B RGB Shield
 */

#define SIGNAL_LED_WARN_BRIGHTNESS 50 // 0 to 255
#define SIGNAL_LED_WARN_COLOR ((128) >> 16 /*R*/ | (0) >> 8 /*G*/ | (128) >> 0 /*B*/)

/**
 * Audio - Piezo Buzzer
 */

// -------------------------------------------------------------------------------------------------
// ------------------------------------- SECTION GENERAL -------------------------------------------
// -------------------------------------------------------------------------------------------------

#define DEBUG_MAIN

/**
 * Interval for printing GPS data (if new data is available since last print) to serial.
 * Undefine to disable feature.
 */
#define SERIAL_PRINT_GPS_DATA_INTERVAL_MS 1000