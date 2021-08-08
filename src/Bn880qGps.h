#pragma once

#include <HardwareSerial.h>
#include <NMEA0183.h>
#include <inttypes.h>

#include "gps_types.h"


// -------------------------------------------------------------------------------------------------

class Bn880qGps {
public:
    Bn880qGps(Stream &serial);
    void sendSentence(const char *sentence);
    void setup();
    bool process();
    const GpsData &getData();

protected:
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

    bool onGpgsa(const String &sentence, uint8_t fields_count);
    bool onGngsa(const String &sentence, uint8_t fields_count);
    bool onGlgsa(const String &sentence, uint8_t fields_count);

    bool onGntxt(const String &sentence, uint8_t fields_count);

    bool parseSentence();

    //! {@
    //! \param sentence string to parse data from
    //! \param fields_count number of data fields found in sentence
    //! \param dest where to store parsed data
    //! \return true if at least one filed was parsed
    static bool onGXgsv(const String &sentence, uint8_t fields_count, GXgsv &dest);
    static bool onGXgga(const String &sentence, uint8_t fields_count, GXgga &dest);
    static bool onGXgll(const String &sentence, uint8_t fields_count, GXgll &dest);
    static bool onGXrmc(const String &sentence, uint8_t fields_count, GXrmc &dest);
    static bool onGXvtg(const String &sentence, uint8_t fields_count, GXvtg &dest);
    static bool onGXgsa(const String &sentence, uint8_t fields_count, GXgsa &dest);
    //! @}

    GpsData data;
    Stream &m_serial;
    NMEA0183 m_sentence_parser;
};
