#pragma once

// -------------------------------------------------------------------------------------------------
// ------------------------------------- SECTION INPUT ---------------------------------------------
// -------------------------------------------------------------------------------------------------

// ----- Keyboard
/**
 * Keyboard - MPR121 Capacitive
 */

#define DEBUG_KEYBOARD

// ----- GPS

/**
 * GPS - Beitian BN-880Q
 */

#define DEBUG_GPS_COMMUNICATION

#define GPS_SERIAL_RX_PIN D3
#define GPS_SERIAL_TX_PIN D5

/**
 * Baud rate to start initial communication with GPS module.
 */
#define GPS_SERIAL_INITIAL_BAUD_RATE 9600

/**
 * Operational baud rate for communication with GPS module. Undefine to disable this feature.
 */
#define GPS_SERIAL_TARGET_BAUD_RATE (GPS_SERIAL_INITIAL_BAUD_RATE*12)

#define GPS_SERIAL_CONFIG SWSERIAL_8N1 // see SoftwareSerial.h SoftwareSerialConfig

///**
// * specify the line ending if necessary, or undefine if none needed
// */
//#define GPS_SERIAL_TRANSMIT_TO_GPS_LINE_END "\x0d\x0a" // "\r\n"

//#define GPS_SET_AUTO_PVT
//#define GPS_ASSUME_AUTO_PVT
#if defined(GPS_SET_AUTO_PVT) || defined(GPS_ASSUME_AUTO_PVT)
#define GPS_AUTO_PVT
#endif

#define GPS_NAVIGATION_FREQUENCY 2

// -------------------------------------------------------------------------------------------------
// ------------------------------------- SECTION OUTPUT --------------------------------------------
// -------------------------------------------------------------------------------------------------

// ----- Terminal communication

/**
 * Baud rate for terminal communication.
 */
#define SERIAL_BAUD_RATE 230400
#define SERIAL_CONFIG SERIAL_8N1 // see HardwareSerial.h

// ----- Oled - 0.66" 64x48

// ----- Visual - WS2812B RGB Shield

/**
 * Warning color and brightness
 */
#define SIGNAL_LED_WARN_BRIGHTNESS 50 // 0 to 255
#define SIGNAL_LED_WARN_COLOR ((128) >> 16 /*R*/ | (0) >> 8 /*G*/ | (128) >> 0 /*B*/)

// ----- Audio - Piezo Buzzer

// -------------------------------------------------------------------------------------------------
// ------------------------------------- SECTION GENERAL -------------------------------------------
// -------------------------------------------------------------------------------------------------

#define DEBUG_MAIN

/**
 * Interval for printing GPS data (if new data is available since last print) to serial.
 * Undefine to disable feature.
 */
#define SERIAL_PRINT_GPS_DATA_INTERVAL_MS 1000