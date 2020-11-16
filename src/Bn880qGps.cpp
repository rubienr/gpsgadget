#include "Bn880qGps.h"
#include <HardwareSerial.h>
#include <NMEAParser.h>
#include <Stream.h>
#include <sstream>

#include "configuration.h"

// clang-format off
/**
 * Mnemonics
 *
 *   Navigation Satellite System Receivers:
 *   https://www.nmea.org/Assets/NMEA%200183%20Talker%20Identifier%20Mnemonics.pdf
 *   GA ... Galileo Positioning System
 *   GB ... BeiDou System (BDS)
 *   GI ... NavIC (IRNSS)
 *   GL ... GLONASS
 *   GN ... Global Navigation Satellite System (GNSS)
 *   GP ... Global Positioning System GPS
 *   GQ ... QZSS
 *
 *   Serial logging:
 *     I ... input received from GPW receiver
 *       E ... error
 *       P ... parse
 *     O ... output sent to GPS receiver
 */
// clang-format on


// -------------------------------------------------------------------------------------------------

namespace {

// -------------------------------------------------------------------------------------------------

//! \tparam IntermediateType type to which the string chall be converted (for example int)
//! \tparam ValueType type to which the converted value shall be assigned (for example uint8_t aka.
//! unsigned char)
//! \param str comma separated fields (aka sentence)
//! \param field_nr the field to parse (starting with index 0)
//! \param value return value
//! \return true on success, false otherwise
template <typename IntermediateType, typename ValueType>
bool parseValue(const String &str, uint8_t field_nr, ValueType &value) {
    constexpr char separator{ ',' };
    uint8_t field_begin_index{ 0 };
    while(field_nr > 0) {
        field_begin_index = str.indexOf(separator, field_begin_index + 1);
        field_nr--;
    }

    const uint8_t field_end_index = str.indexOf(separator, field_begin_index + 1);
    field_begin_index++;
    const String value_string{ str.substring(field_begin_index, field_end_index).c_str() };

    if(value_string.length() <= 0)
        return false;

    std::istringstream is(value_string.c_str());
    IntermediateType intermediate_type;

    if(is >> intermediate_type) {
        value = static_cast<ValueType>(intermediate_type);
        // Serial.printf("OK: %s (%d,%d) -> ", value_string.c_str(), field_begin_index,
        // field_end_index); Serial.println(value);
        return true;
    } else {
        // Serial.printf("FAILED: %s -> ", value_string.c_str());
        // Serial.println(value);
        return false;
    }
}

// -------------------------------------------------------------------------------------------------

//! Parse the field out of a comma separated string.
//! \param str string of comma separated fields
//! \param field_nr the field number do parts; starting with index 0
//! \param value return value
//! \return true on success, false otherwise
bool parseValue(const String &str, uint8_t field_nr, String &value) {
    constexpr char separator{ ',' };
    uint8_t field_begin_index{ 0 };
    while(field_nr > 0) {
        field_begin_index = str.indexOf(separator, field_begin_index + 1);
        field_nr--;
    }

    const uint8_t field_end_index = str.indexOf(separator, field_begin_index + 1);
    field_begin_index++;
    const String value_string{ str.substring(field_begin_index, field_end_index).c_str() };

    if(value_string.length() <= 0)
        return false;

    value = value_string;
    return true;
}

// -------------------------------------------------------------------------------------------------

uint8_t computeCrc(const char *sentence) {
    uint8_t crc{ static_cast<uint8_t>(sentence[0]) };
    for(const char *c = sentence + 1; *c != 0; c++)
        crc = crc ^ static_cast<uint8_t>(*c);
    return crc;
}

} // namespace
// -------------------------------------------------------------------------------------------------

Bn880qGps::Bn880qGps(Stream &serial) : m_serial(serial) {}

// -------------------------------------------------------------------------------------------------

void Bn880qGps::setup() {
    // Clear the list of messages which are sent.
    // MicroNMEA::sendSentence(gps_serial, "$PORZB");

    // Send only RMC and GGA messages.
    // MicroNMEA::sendSentence(gps_serial, "$PORZB,RMC,1,GGA,1");

    // Disable compatability mode (NV08C-CSM proprietary message) and
    // adjust precision of time and position fields

    // MicroNMEA::sendSentence(gps_serial, "$PNVGNME,2,9,1");
    // MicroNMEA::sendSentence(gps, "$PONME,2,4,1,0");

    sendSentence("PSRF105,1"); // debug on
    // TODO II $GNTXT,01,01,01,PSRF inv format*35

    sendSentence("PSRF106,21"); // set WGS84 datum
    // TODO II $GNTXT,01,01,01,PSRF inv format*35
}
// -------------------------------------------------------------------------------------------------

bool Bn880qGps::process() {
    bool new_data_available = false;
    while(m_serial.available()) {
        if(m_sentence_parser.update(static_cast<char>(m_serial.read()))) {
            if(m_sentence_parser.getState() == NMEA0183_INVALID) {
                Serial.println("IE checksum failure");
            }
            if(parseSentence()) {
                new_data_available = true;
            }
        }
    }
    return new_data_available;
}

// -------------------------------------------------------------------------------------------------

const GpsData &Bn880qGps::getData() { return data; }

// -------------------------------------------------------------------------------------------------
bool Bn880qGps::parseSentence() {
    const String sentence{ m_sentence_parser.getSentence() };
    const String sentence_fields{ sentence.substring(6, sentence.length() - 3) };
    const uint8_t fields_count{ m_sentence_parser.getFields() };
    const String sentence_type{ sentence.substring(1, 6) };

    if(sentence_type == "GPGSV")
        return onGpgsv(sentence_fields, fields_count);
    else if(sentence_type == "GNGSV")
        return onGngsv(sentence_fields, fields_count);
    else if(sentence_type == "GLGSV")
        return onGlgsv(sentence_fields, fields_count);

    else if(sentence_type == "GPGGA")
        return onGpgga(sentence_fields, fields_count);
    else if(sentence_type == "GNGGA")
        return onGngga(sentence_fields, fields_count);
    else if(sentence_type == "GLGGA")
        return onGlgga(sentence_fields, fields_count);

    else if(sentence_type == "GPGLL")
        return onGpgll(sentence_fields, fields_count);
    else if(sentence_type == "GNGLL")
        return onGngll(sentence_fields, fields_count);
    else if(sentence_type == "GLGLL")
        return onGlgll(sentence_fields, fields_count);

    else if(sentence_type == "GPRMC")
        return onGprmc(sentence_fields, fields_count);
    else if(sentence_type == "GNRMC")
        return onGnrmc(sentence_fields, fields_count);
    else if(sentence_type == "GLRMC")
        return onGlrmc(sentence_fields, fields_count);

    else if(sentence_type == "GPVTG")
        return onGpvtg(sentence_fields, fields_count);
    else if(sentence_type == "GNVTG")
        return onGnvtg(sentence_fields, fields_count);
    else if(sentence_type == "GLVTG")
        return onGlvtg(sentence_fields, fields_count);


    else if(sentence_type == "GPGSA")
        return onGpgsa(sentence_fields, fields_count);
    else if(sentence_type == "GNGSA")
        return onGngsa(sentence_fields, fields_count);
    else if(sentence_type == "GLGSA")
        return onGlgsa(sentence_fields, fields_count);

    else
        Serial.printf("IE %s\n", sentence.c_str());

    return false;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGpgsv(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GP");
#endif
    uint8_t message_number{ 0 };

    if(fields_count > 1 && parseValue<int>(sentence, 1, message_number)) {
        if(message_number == 1)
            return onGXgsv(sentence, fields_count, data.gpgsv_1);
        if(message_number == 2)
            return onGXgsv(sentence, fields_count, data.gpgsv_2);
        if(message_number == 3)
            return onGXgsv(sentence, fields_count, data.gpgsv_3);
    }

    return false;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGngsv(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GN");
#endif
    uint8_t message_number{ 0 };

    if(fields_count > 1 && parseValue<int>(sentence, 1, message_number)) {
        if(message_number == 1)
            return onGXgsv(sentence, fields_count, data.gngsv_1);
        if(message_number == 2)
            return onGXgsv(sentence, fields_count, data.gngsv_2);
        if(message_number == 3)
            return onGXgsv(sentence, fields_count, data.gngsv_3);
    }

    return false;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlgsv(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GL");
#endif
    uint8_t message_number{ 0 };

    if(fields_count > 1 && parseValue<int>(sentence, 1, message_number)) {
        if(message_number == 1)
            return onGXgsv(sentence, fields_count, data.glgsv_1);
        if(message_number == 2)
            return onGXgsv(sentence, fields_count, data.glgsv_2);
        if(message_number == 3)
            return onGXgsv(sentence, fields_count, data.glgsv_3);
    }

    return false;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXgsv(const String &sentence, uint8_t fields_count, GXgsv &dest) {
    // clang-format off
    /**
     * GSV - GNSS Satellites in View
     * http://www.nmea.de/nmea0183datensaetze.html#gsv
     *
     * Name                 Example     Unit    Desc
     * Message ID           $GPGSV              GSV protocol header
     * Number of Messages   2                   Range 1 to 3 (Depending on the number of satellites
     *                                          tracked, multiple messages of GSV data may be
     *                                          required.)
     * Message Number       1                   Range 1 to 3
     * Satellites in View   07
     * Satellite ID         07                  Channel 1 (Range 1 to 32)
     * Elevation            79          degrees Channel 1 (Maximum 90)
     * Azimuth              048         degrees Channel 1 (Range 0 to 359)
     * SNR (C/N0)           42          dBHz    Range 0 to 99, null when not tracking
     * ...                                      ...
     * Satellite ID         27                  Channel 4 (Range 1 to 32)
     * Elevation            27          degrees Channel 4 (Maximum 90)
     * Azimuth              138         degrees Channel 4 (True, Range 0 to 359)
     * SNR (C/N0)           42          dBHz    Range 0 to 99, null when not tracking
     * Checksum             *71
     * <CR> <LF>                                End of message termination
     */
    // clang-format on

#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.printf(" GSV %s\n", sentence.c_str());
#endif

    dest = {};
    bool parsed = true;

    if(fields_count > 0) {
        parsed |= parseValue<int>(sentence, (uint8_t)0, dest.numberOfMessages);
    }

    if(fields_count > 1) {
        parsed |= parseValue<int>(sentence, 1, dest.messageNumber);
    }

    if(fields_count > 2) {
        parsed |= parseValue<int>(sentence, 2, dest.numberOfSatellitesInView);
    }


    if(fields_count > 6) {

        parsed |= parseValue<int>(sentence, 3, dest.channel_1.id);

        parsed |= parseValue<int>(sentence, 4, dest.channel_1.elevation);
        parsed |= parseValue<int>(sentence, 5, dest.channel_1.azimuth);
        parsed |= parseValue<int>(sentence, 6, dest.channel_1.snr);
    }

    if(fields_count > 10) {

        parsed |= parseValue<int>(sentence, 7, dest.channel_2.id);

        parsed |= parseValue<int>(sentence, 8, dest.channel_2.elevation);
        parsed |= parseValue<int>(sentence, 9, dest.channel_2.azimuth);
        parsed |= parseValue<int>(sentence, 10, dest.channel_2.snr);
    }

    if(fields_count > 14) {

        parsed |= parseValue<int>(sentence, 11, dest.channel_3.id);

        parsed |= parseValue<int>(sentence, 12, dest.channel_3.elevation);
        parsed |= parseValue<int>(sentence, 13, dest.channel_3.azimuth);
        parsed |= parseValue<int>(sentence, 14, dest.channel_3.snr);
    }

    if(fields_count > 18) {

        parsed |= parseValue<int>(sentence, 15, dest.channel_4.id);

        parsed |= parseValue<int>(sentence, 16, dest.channel_4.elevation);
        parsed |= parseValue<int>(sentence, 17, dest.channel_4.azimuth);
        parsed |= parseValue<int>(sentence, 18, dest.channel_4.snr);
    }

    return parsed;
}

// -------------------------------------------------------------------------------------------------
bool Bn880qGps::onGpgga(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GP");
#endif
    return onGXgga(sentence, fields_count, data.gpgga);
}


// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGngga(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GN");
#endif
    return onGXgga(sentence, fields_count, data.gngga);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlgga(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GL");
#endif
    return onGXgga(sentence, fields_count, data.glgga);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXgga(const String &sentence, uint8_t fields_count, GXgga &dest) {
    // clang-format off
    /**
     * GGA - Global Positioning System Fixed Data
     * http://www.nmea.de/nmea0183datensaetze.html#gga
     *
     * Name                     Example         Unit    Desc
     * UTC Time                 002153.000              hhmmss.sss
     * Latitude                 3342.6618               ddmm.mmmm
     * N/S Indicator            N                       N=north or S=south
     * Longitude                11751.3858              dddmm.mmmm
     * E/W Indicator            W                       E=east or W=west
     * Position Fix Indicator   1                       See Table1-4
     * Satellites Used          10                      Range 0 to 12
     * HDOP                     1.2                     Horizontal Dilution of Precision
     * MSL Altitude             27.0            meters  UnitsMmeters
     * Geoid Separation         -34.2           meters  Geoid-to-ellipsoid separation.
     *                                                  Ellipsoid altitude = MSL Altitude + Geoid-
     *                                                  Separation.
     * Units                    M               meters
     * Age of Diff. Corr.                       sec     Null fields when staticDGPS is not used
     * Diff. Ref. Station ID    0000
     * Checksum                 *5E
     * <CR> <LF>                                        End of message terminationn
     */
    // clang-format on

#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.printf(" GGA %s\n", sentence.c_str());
#endif

    dest = {};
    bool parsed = true;

    if(fields_count > 0)
        parsed |= parseValue<float>(sentence, 0, dest.utc_time_seconds);
    if(fields_count > 1)
        parsed |= parseValue<float>(sentence, 1, dest.latitude);
    if(fields_count > 2)
        parsed |= parseValue<char>(sentence, 2, dest.north_south_indicator);
    if(fields_count > 3)
        parsed |= parseValue<float>(sentence, 3, dest.longitude);
    if(fields_count > 4)
        parsed |= parseValue<char>(sentence, 4, dest.east_west_indicator);
    if(fields_count > 5)
        parsed |= parseValue<int>(sentence, 5, dest.position_fix_indicator);
    if(fields_count > 6)
        parsed |= parseValue<int>(sentence, 6, dest.satellites_used);
    if(fields_count > 7)
        parsed |= parseValue<float>(sentence, 7, dest.horizontal_dilution_of_precision);
    if(fields_count > 8)
        parsed |= parseValue<float>(sentence, 8, dest.geoid_separation);
    if(fields_count > 9)
        parsed |= parseValue<char>(sentence, 9, dest.geoid_separation_units);
    if(fields_count > 10)
        parsed |= parseValue<float>(sentence, 10, dest.age_of_diff_corr_seconds);
    if(fields_count > 11)
        parsed |= parseValue<int>(sentence, 11, dest.diff_ref_station_id);

    return parsed;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGpgll(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GP");
#endif
    return onGXgll(sentence, fields_count, data.gpgll);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGngll(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GN");
#endif
    return onGXgll(sentence, fields_count, data.gngll);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlgll(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GL");
#endif
    return onGXgll(sentence, fields_count, data.glgll);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXgll(const String &sentence, uint8_t fields_count, GXgll &dest) {
    // clang-format off
    /**
     * GLL - Geographic Position - Latitude/Longitude
     * http://www.nmea.de/nmea0183datensaetze.html#gll
     *
     * Name             Example     Unit    Desc
     * Latitude         3723.2475           ddmm.mmmm
     * N/S Indicator    N                   N=north or S=south
     * Longitude        12158.3416          dddmm.mmmm
     * E/W Indicator    W                   E=east or W=west
     * UTC Time         161229.487          hhmmss.sss
     * Status           A                   A=data valid or V=data not valid
     * Mode             A                   A=Autonomous, D=DGPS, E=DR (Only present in NMEA v3.00)
     * Checksum         *41
     * <CR><LF>                             End of message termination
     */
    // clang-format on

#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.printf(" GLL %s\n", sentence.c_str());
#endif

    dest = {};
    bool parsed = true;

    if(fields_count > 0)
        parsed |= parseValue<float>(sentence, 0, dest.longitude);
    if(fields_count > 1)
        parsed |= parseValue<char>(sentence, 1, dest.north_south_indicator);
    if(fields_count > 2)
        parsed |= parseValue<float>(sentence, 2, dest.latitude);
    if(fields_count > 3)
        parsed |= parseValue<char>(sentence, 3, dest.east_west_indicator);
    if(fields_count > 4)
        parsed |= parseValue<float>(sentence, 4, dest.utc_time_seconds);
    if(fields_count > 5)
        parsed |= parseValue<char>(sentence, 5, dest.status);
    if(fields_count > 6)
        parsed |= parseValue<char>(sentence, 6, dest.mode);

    return parsed;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGprmc(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GP");
#endif
    return onGXrmc(sentence, fields_count, data.gnrmc);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGnrmc(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GN");
#endif
    return onGXrmc(sentence, fields_count, data.gnrmc);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlrmc(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GL");
#endif
    return onGXrmc(sentence, fields_count, data.glrmc);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXrmc(const String &sentence, uint8_t fields_count, GXrmc &dest) {
    // clang-format off
    /**
     * RMC - Recommended Minimum Specific GNSS Data
     * http://www.nmea.de/nmea0183datensaetze.html#rmc
     *
     * Name                 Example     Unit    Desc
     * UTC Time             161229.487          hhmmss.sss
     * Status               A                   A=data valid or V=data not valid
     * Latitude             3723.2475           ddmm.mmmm
     * N/S Indicator        N                   N=north or S=south
     * Longitude            12158.3416          dddmm.mmmm
     * E/W Indicator        W                   E=east or W=west
     * Speed Over Ground    0.13        knots
     * Course Over Ground   309.62      degrees True
     * Date                 120598              ddmmyy
     * Magnetic Variation               degrees E=east or W=west
     * East/West Indicator  E                   E=east
     * Mode                 A                   A=Autonomous, D=DGPS, E=DR
     * Checksum             *10
     * <CR> <LF>                                End of message terminatio
     */
    // clang-format on

#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.printf(" RMC %s\n", sentence.c_str());
#endif

    dest = {};
    bool parsed = true;

    if(fields_count > 0)
        parsed |= parseValue<float>(sentence, 0, dest.utc_time_seconds);
    if(fields_count > 1)
        parsed |= parseValue<char>(sentence, 1, dest.status);
    if(fields_count > 2)
        parsed |= parseValue<float>(sentence, 2, dest.latitude);
    if(fields_count > 3)
        parsed |= parseValue<char>(sentence, 3, dest.north_south_indicator);
    if(fields_count > 4)
        parsed |= parseValue<float>(sentence, 4, dest.longitude);
    if(fields_count > 5)
        parsed |= parseValue<char>(sentence, 5, dest.east_west_indicator);
    if(fields_count > 6)
        parsed |= parseValue<float>(sentence, 6, dest.speed_over_ground_kts);
    if(fields_count > 7)
        parsed |= parseValue<float>(sentence, 7, dest.course_over_ground_deg);
    if(fields_count > 8)
        parsed |= parseValue(sentence, 8, dest.date);
    if(fields_count > 9)
        parsed |= parseValue<float>(sentence, 9, dest.magnetic_variation);
    if(fields_count > 10)
        parsed |= parseValue<char>(sentence, 10, dest.east_west_indicator_2);
    if(fields_count > 11)
        parsed |= parseValue<char>(sentence, 11, dest.mode);

    // TODO: test

    return parsed;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGpvtg(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GP");
#endif
    return onGXvtg(sentence, fields_count, data.gnvtg);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGnvtg(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GN");
#endif
    return onGXvtg(sentence, fields_count, data.gnvtg);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlvtg(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GL");
#endif
    return onGXvtg(sentence, fields_count, data.glvtg);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXvtg(const String &sentence, uint8_t fields_count, GXvtg &dest) {
    // clang-format off
    /**
     * VTG - Course Over Ground and Ground Speed
     * http://www.nmea.de/nmea0183datensaetze.html#vtg
     *
     * Name                 Example     Unit    Desc
     * Course               309.62      degrees Measured heading
     * Reference            T                   True
     * Course                           degrees Measured heading
     * Reference            M                   Magnetic
     * Speed                0.13        knots   Measured horizontal speed
     * Units                N                   Knots
     * Speed                0.2km/hr            Measured horizontal speed
     * Units                K                   Kilometers per hour
     * Mode                 A                   A=Autonomous, D=DGPS, E=DR
     * Checksum             *23
     * <CR> <LF>                                End of message terminatio
     */
    // clang-format on

#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.printf(" VTG %s\n", sentence.c_str());
#endif

    dest = {};
    bool parsed = true;

    if(fields_count > 0)
        parsed |= parseValue<float>(sentence, 0, dest.course_deg);
    if(fields_count > 1)
        parsed |= parseValue<char>(sentence, 1, dest.course_magnetic_deg_reference);
    if(fields_count > 2)
        parsed |= parseValue<float>(sentence, 2, dest.course_magnetic_deg);
    if(fields_count > 3)
        parsed |= parseValue<char>(sentence, 3, dest.course_magnetic_deg_reference);
    if(fields_count > 4)
        parsed |= parseValue<float>(sentence, 4, dest.speed_horizontal_kts);
    if(fields_count > 5)
        parsed |= parseValue<char>(sentence, 5, dest.speed_horizontal_kts_unit);
    if(fields_count > 6)
        parsed |= parseValue<float>(sentence, 6, dest.speed_horizontal_kmh);
    if(fields_count > 7)
        parsed |= parseValue<char>(sentence, 7, dest.speed_horizontal_kmh_unit);
    if(fields_count > 8)
        parsed |= parseValue<char>(sentence, 8, dest.mode);

    // TODO: test

    return parsed;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGpgsa(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GP");
#endif
    return onGXgsa(sentence, fields_count, data.gngsa);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGngsa(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GN");
#endif
    return onGXgsa(sentence, fields_count, data.gngsa);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlgsa(const String &sentence, uint8_t fields_count) {
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.print("IP GL");
#endif
    return onGXgsa(sentence, fields_count, data.glgsa);
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXgsa(const String &sentence, uint8_t fields_count, GXgsa &dest) {
    // clang-format off
    /**
     * GSA - GNSS DOP and Active Satellites
     * http://www.nmea.de/nmea0183datensaetze.html#gsa
     *
     * Name                 Example     Unit    Desc
     * Message ID           $GPGSA              GSA protocol header
     * Mode 1               A                   M ...Manual—forced to operate in 2D or 3D mode
     *                                          A ... 2D Automatic—allowed to automatically
     *                                          switch 2D/3D
     * Mode 2               3                   1 ... Fix not available
     *                                          2 ... 2D (<4 SVs used)
     *                                          3 ... 3D (>3 SVs used)
     * Satellite Used       07                  SV on Channel 1
     * Satellite Used       02                  SV on Channel 2
     * ....                                     ....
     * Satellite Used                           SV on Channel 12
     * PDOP                 1.8                 Position Dilution of Precision
     * HDOP                 1.0                 Horizontal Dilution of Precision
     * VDOP                 1.5                 Vertical Dilution of Precision
     * Checksum             *33
     * <CR> <LF>                                End of message termination
     */
    // clang-format on

#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.printf(" GSA %s\n", sentence.c_str());
#endif
    dest = {};
    bool parsed = true;

    if(fields_count > 0)
        parsed |= parseValue<char>(sentence, 0, dest.mode_1);
    if(fields_count > 1)
        parsed |= parseValue<unsigned int>(sentence, 1, dest.mode_2);
    if(fields_count > 2)
        parsed |= parseValue<unsigned int>(sentence, 2, dest.sat_used_ch_1);
    if(fields_count > 3)
        parsed |= parseValue<unsigned int>(sentence, 3, dest.sat_used_ch_2);
    if(fields_count > 4)
        parsed |= parseValue<unsigned int>(sentence, 4, dest.sat_used_ch_3);
    if(fields_count > 5)
        parsed |= parseValue<unsigned int>(sentence, 5, dest.sat_used_ch_4);
    if(fields_count > 6)
        parsed |= parseValue<unsigned int>(sentence, 6, dest.sat_used_ch_5);
    if(fields_count > 7)
        parsed |= parseValue<unsigned int>(sentence, 7, dest.sat_used_ch_6);
    if(fields_count > 8)
        parsed |= parseValue<unsigned int>(sentence, 8, dest.sat_used_ch_7);
    if(fields_count > 9)
        parsed |= parseValue<unsigned int>(sentence, 9, dest.sat_used_ch_8);
    if(fields_count > 10)
        parsed |= parseValue<unsigned int>(sentence, 10, dest.sat_used_ch_9);
    if(fields_count > 11)
        parsed |= parseValue<unsigned int>(sentence, 11, dest.sat_used_ch_10);
    if(fields_count > 12)
        parsed |= parseValue<unsigned int>(sentence, 12, dest.sat_used_ch_11);
    if(fields_count > 13)
        parsed |= parseValue<unsigned int>(sentence, 13, dest.sat_used_ch_12);
    if(fields_count > 14)
        parsed |= parseValue<float>(sentence, 6, dest.pdop);
    if(fields_count > 15)
        parsed |= parseValue<char>(sentence, 7, dest.hdop);
    if(fields_count > 16)
        parsed |= parseValue<char>(sentence, 8, dest.vdop);

    // TODO: test

    return parsed;
}

// -------------------------------------------------------------------------------------------------

void Bn880qGps::sendSentence(const char *sentence) {
    if(sentence == nullptr || sentence[0] == 0)
        return;
    uint8_t crc{ computeCrc(sentence) };
    m_serial.printf("$%s*%02X\r\n", sentence, crc);
#if defined(DEBUG_GPS_COMMUNICATION)
    Serial.printf("O $%s*%02X\r\n", sentence, crc);
#endif
}
