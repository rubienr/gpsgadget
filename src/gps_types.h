#pragma once

#include <WString.h>
#include <limits>
// class Stream;
// -------------------------------------------------------------------------------------------------

struct GXgsvSatelliteChannel {
    // TODO: default construct with "invalid" values to denote the values come not from GPS
    uint8_t id{ 0 };
    uint8_t elevation{ 0 };
    uint16_t azimuth{ 0 };
    uint8_t snr{ 0 };
};

// -------------------------------------------------------------------------------------------------

struct GXgsv {
    // TODO: default construct with "invalid" values to denote the values come not from GPS
    uint8_t numberOfMessages{ 0 };
    uint8_t messageNumber{ 0 };
    uint8_t numberOfSatellitesInView{ 0 };

    GXgsvSatelliteChannel channel_1;
    GXgsvSatelliteChannel channel_2;
    GXgsvSatelliteChannel channel_3;
    GXgsvSatelliteChannel channel_4;
};

// -------------------------------------------------------------------------------------------------

struct GXgga {
    float utc_time_seconds{ std::numeric_limits<float>::quiet_NaN() };
    float latitude{ std::numeric_limits<float>::quiet_NaN() }; // ddmm.mmmm
    char north_south_indicator{ ' ' };
    float longitude{ std::numeric_limits<float>::quiet_NaN() }; // ddmm.mmmm
    char east_west_indicator{ ' ' };
    //! position_fix_indicator:
    //! 0 no or invalid fix
    //! 1 gps sps, fix valid
    //! 2 differential gps sps, fix valid
    //! 3-5 not supported
    //! 6 dead reckoning, fix valid
    int8_t position_fix_indicator{ -1 };
    int8_t satellites_used{ -1 };
    float horizontal_dilution_of_precision{ std::numeric_limits<float>::quiet_NaN() };
    float mean_sea_level_altitude{ std::numeric_limits<float>::quiet_NaN() };
    char msl_alt_units{ ' ' };
    float geoid_separation{ std::numeric_limits<float>::quiet_NaN() };
    char geoid_separation_units{ ' ' };
    float age_of_diff_corr_seconds{ std::numeric_limits<float>::quiet_NaN() }; // TODO: what is the correct term?
    int16_t diff_ref_station_id{ -1 };   // TODO: what is the correct term?
};

// -------------------------------------------------------------------------------------------------

struct GXgll {
    float latitude{ std::numeric_limits<float>::quiet_NaN() }; // ddmm.mmmm
    char north_south_indicator{ ' ' }; // N or S
    float longitude{ std::numeric_limits<float>::quiet_NaN() }; // ddmm.mmmm
    char east_west_indicator{ ' ' }; // E or W
    float utc_time_seconds{ std::numeric_limits<float>::quiet_NaN() };
    char status{ ' ' }; // A=data valid, V=data not valid
    char mode{ ' ' };   // NMEA >= v3.0, A=autonomous, D=DGPS, E=DR
};

// -------------------------------------------------------------------------------------------------

struct GXrmc {
    float utc_time_seconds{ std::numeric_limits<float>::quiet_NaN() };
    char status{ ' ' }; // A=data valid, V=data not valid
    float latitude{ std::numeric_limits<float>::quiet_NaN() }; // ddmm.mmmm
    char north_south_indicator{ ' ' }; // N or S
    float longitude{ std::numeric_limits<float>::quiet_NaN() }; // ddmm.mmmm
    char east_west_indicator{ ' ' }; // E or W
    float speed_over_ground_kts{ std::numeric_limits<float>::quiet_NaN() };
    float course_over_ground_deg{ std::numeric_limits<float>::quiet_NaN() };
    String date{ "" };
    float magnetic_variation { std::numeric_limits<float>::quiet_NaN() };          //
    char east_west_indicator_2{ ' ' }; // E
    char mode{ ' ' };                  // only NMEA >= v2.3, A=autonomous, D=DGPS, E=DR
};

// -------------------------------------------------------------------------------------------------

struct GXvtg {
    float course_deg{ std::numeric_limits<float>::quiet_NaN() };
    char course_deg_reference{ ' ' }; // T
    float course_magnetic_deg{ std::numeric_limits<float>::quiet_NaN() };
    char course_magnetic_deg_reference{ ' ' }; // M
    float speed_horizontal_kts{ std::numeric_limits<float>::quiet_NaN() };
    char speed_horizontal_kts_unit{ ' ' }; // N
    float speed_horizontal_kmh{ std::numeric_limits<float>::quiet_NaN() };
    char speed_horizontal_kmh_unit{ ' ' }; // K
    char mode{ ' ' };                     // only NMEA >= 2.3, A=autonomous, D=DGPS, E=DR
};

// -------------------------------------------------------------------------------------------------

struct GXgsa {
    //! mode_1:
    //! M ... Manual—forced to operate in 2D or 3D mode
    //! A ... 2D Automatic—allowed to automatically switch 2D/3D
    char mode_1{ ' ' };
    //! mode_2:
    //! 1 ... Fix not available
    //! 2 ... 2D (<4 SVs used)
    //! 3 ... 3D (>3 SVs used)
    char mode_2{ ' ' };
    int8_t sat_used_ch_1{ -1 };
    int8_t sat_used_ch_2{ -1 };
    int8_t sat_used_ch_3{ -1 };
    int8_t sat_used_ch_4{ -1 };
    int8_t sat_used_ch_5{ -1 };
    int8_t sat_used_ch_6{ -1 };
    int8_t sat_used_ch_7{ -1 };
    int8_t sat_used_ch_8{ -1 };
    int8_t sat_used_ch_9{ -1 };
    int8_t sat_used_ch_10{ -1 };
    int8_t sat_used_ch_11{ -1 };
    int8_t sat_used_ch_12{ -1 };
    float pdop{ std::numeric_limits<float>::quiet_NaN()  };
    float hdop{ std::numeric_limits<float>::quiet_NaN()  };
    float vdop{ std::numeric_limits<float>::quiet_NaN()  };
};

// -------------------------------------------------------------------------------------------------

struct GpsData {
    GXgsv gpgsv_1;
    GXgsv gngsv_1;
    GXgsv glgsv_1;

    GXgsv gpgsv_2;
    GXgsv gngsv_2;
    GXgsv glgsv_2;

    GXgsv gpgsv_3;
    GXgsv gngsv_3;
    GXgsv glgsv_3;

    GXgga gpgga;
    GXgga gngga;
    GXgga glgga;

    GXgll gpgll;
    GXgll gngll;
    GXgll glgll;

    GXrmc gprmc;
    GXrmc gnrmc;
    GXrmc glrmc;

    GXvtg gpvtg;
    GXvtg gnvtg;
    GXvtg glvtg;

    GXgsa gpgsa;
    GXgsa gngsa;
    GXgsa glgsa;
};

// -------------------------------------------------------------------------------------------------

void print(const GXgsvSatelliteChannel &g, const String &prefix = "", const String &indent = "");
void print(const GXgsv &g, const String &prefix = "", const String &indent = "");
void print(const GXgga &g, const String &prefix = "", const String &indent = "");
void print(const GXgll &g, const String &prefix = "", const String &indent = "");
void print(const GXrmc &g, const String &prefix = "", const String &indent = "");
void print(const GXvtg &g, const String &prefix = "", const String &indent = "");
void print(const GXgsa &g, const String &prefix = "", const String &indent = "");
void print(const GpsData &g, const String &prefix = "", const String &indent = "");
void printTabular(const GpsData &g, const String &prefix = "", const String &indent = "");