#pragma once

#include <HardwareSerial.h>
#include <NMEA0183.h>
#include <WString.h>
#include <inttypes.h>
class Stream;

// -------------------------------------------------------------------------------------------------

struct GXgsvSatelliteChannel
{
    uint8_t id;
    uint8_t elevation;
    uint16_t azimuth;
    uint8_t snr;
};

// -------------------------------------------------------------------------------------------------

void print(const GXgsvSatelliteChannel &g, const String &prefix = "", const String &indent = "");

// -------------------------------------------------------------------------------------------------

struct GXgsv
{
    uint8_t numberOfMessages;
    uint8_t messageNumber;
    uint8_t numberOfSatellitesInView;

    GXgsvSatelliteChannel channel1;
    GXgsvSatelliteChannel channel2;
    GXgsvSatelliteChannel channel3;
    GXgsvSatelliteChannel channel4;
};

// -------------------------------------------------------------------------------------------------

void print(const GXgsv &g, const String &prefix = "", const String &indent = "");

// -------------------------------------------------------------------------------------------------

struct GXgga {
    float utc_time_seconds;
    float latitude;
    char north_south_indicator{' '};
    float longitude;
    char east_west_indicator{' '};
    //! position_fix_indicator:
    //! 0 no or invalid fix
    //! 1 gps sps, fix valid
    //! 2 differential gps sps, fix valid
    //! 3-5 not supported
    //! 6 dead reckoning, fix valid
    uint8_t position_fix_indicator;
    uint8_t satellites_used;
    float horizontal_dilution_of_precision;
    float mean_sea_level_altitude;
    char msl_alt_units{' '};
    float geoid_separation;
    char geoid_separation_units{' '};
    float age_of_diff_corr_seconds; // TODO: what is the correct term?
    uint16_t diff_ref_station_id; // TODO: what is the correct term?
};

// -------------------------------------------------------------------------------------------------

void print(const GXgga &g, const String &prefix = "", const String &indent = "");

// -------------------------------------------------------------------------------------------------

struct GXgll {
    float latitude;
    char north_south_indicator{' '}; // N or S
    float longitude;
    char east_west_indicator{' '}; // E or W
    float utc_time_seconds;
    char status{' '}; // A=data valid, V=data not valid
    char mode{' '}; // NMEA >= v3.0, A=autonomous, D=DGPS, E=DR
};

// -------------------------------------------------------------------------------------------------

void print(const GXgll &g, const String &prefix = "", const String &indent = "");

// -------------------------------------------------------------------------------------------------

struct GXrmc {
    float utc_time_seconds;
    char status{' '}; // A=data valid, V=data not valid
    float latitude;
    char north_south_indicator{' '}; // N or S
    float longitude;
    char east_west_indicator{' '}; // E or W
    float speed_over_ground_kts;
    float course_over_ground_deg;
    String date{};
    float magnetic_variation; //
    char east_west_indicator_2{' '}; // E
    char mode{' '}; // only NMEA >= v2.3, A=autonomous, D=DGPS, E=DR
};

// -------------------------------------------------------------------------------------------------

void print(const GXrmc &g, const String &prefix = "", const String &indent = "");

// -------------------------------------------------------------------------------------------------

struct GXvtg {
    float course_deg;
    char course_deg_reference{' '}; // T
    float course_magnetic_deg;
    char course_magnetic_deg_reference{' '}; // M
    float speed_horizontal_kts;
    float speed_horizontal_kts_unit; // N
    float speed_horizontal_kmh;
    float speed_horizontal_kmh_unit; // K
    char mode{' '}; // only NMEA >= 2.3, A=autonomous, D=DGPS, E=DR
};

// -------------------------------------------------------------------------------------------------

void print(const GXvtg &g, const String &prefix = "", const String &indent = "");

// -------------------------------------------------------------------------------------------------

class Bn880qGps
{
public:
    Bn880qGps(Stream &serial);
    void sendSentence(const char *sentence);
    void setup();
    bool process();
    void print();

    bool onGpgsv(const String &sentence, uint8_t fields_count);
    bool onGngsv(const String &sentence, uint8_t fields_count);
    bool onGlgsv(const String &sentence, uint8_t fields_count);

    bool onGpgga(const String &sentence, uint8_t fields_count);
    bool onGngga(const String &sentence, uint8_t fields_count);
    bool onGlgga(const String &sentence, uint8_t fields_count);

    bool onGpgll(const String &sentence, uint8_t fields_count);
    bool onGngll(const String &sentence, uint8_t fields_count);
    bool onGlgll(const String &sentence, uint8_t fields_count);

    bool onGprmc(const String &sentence, uint8_t fields_count);
    bool onGnrmc(const String &sentence, uint8_t fields_count);
    bool onGlrmc(const String &sentence, uint8_t fields_count);

    bool onGpvtg(const String &sentence, uint8_t fields_count);
    bool onGnvtg(const String &sentence, uint8_t fields_count);
    bool onGlvtg(const String &sentence, uint8_t fields_count);

    struct
    {
        GXgsv gpsgv;
        GXgsv gnsgv;
        GXgsv glsgv;

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
    } data;

protected:
    static uint8_t computeCrc(const char *sentence);

    bool parseSentence();

    //! {@
    //! \param sentence string to parse data from
    //! \param fields_count number of data fields found in sentence
    //! \param dest where to store parsed data
    //! \return true if at least one filed was parsed
    bool onGXgsv(const String &sentence, uint8_t fields_count, GXgsv &dest);
    bool onGXgga(const String &sentence, uint8_t fields_count, GXgga &dest);
    bool onGXgll(const String &sentence, uint8_t fields_count, GXgll &dest);
    bool onGXrmc(const String &sentence, uint8_t fields_count, GXrmc &dest);
    bool onGXvtg(const String &sentence, uint8_t fields_count, GXvtg &dest);
    //! @}
    Stream &m_serial;
    NMEA0183 m_sentence_parser;
    // NMEAParser<5> m_nmea_data_parser;
};
