#include "Bn880qGps.h"
#include <HardwareSerial.h>
#include <NMEAParser.h>
#include <Stream.h>

#include <sstream>

// -------------------------------------------------------------------------------------------------

void print(const GXgsvSatelliteChannel &g, const String &prefix, const String &indent)
{
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%sid        = %d\n", indent.c_str(), g.id);
    Serial.printf("%selevation = %d\n", indent.c_str(), g.elevation);
    Serial.printf("%sazimuth   = %d\n", indent.c_str(), g.azimuth);
    Serial.printf("%sSNR       = %d\n", indent.c_str(), g.snr);
}

// -------------------------------------------------------------------------------------------------

void print(const GXgsv &g, const String &prefix, const String &indent)
{
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%s# of messages        = %d\n", indent.c_str(), g.numberOfMessages);
    Serial.printf("%smessage #            = %d\n", indent.c_str(), g.messageNumber);
    Serial.printf("%s# satellites in view = %d\n", indent.c_str(), g.numberOfSatellitesInView);
    print(g.channel1, "channel1:", indent + "  ");
}

// -------------------------------------------------------------------------------------------------

void print(const GXgga &g, const String &prefix, const String &indent)
{
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%sUTC [s]                    = %f\n", indent.c_str(), g.utc_time_seconds);
    Serial.printf("%slatitude [°]               = %f\n", indent.c_str(), g.latitude);
    Serial.printf("%sN/S indicator              = %c\n", indent.c_str(), g.north_south_indicator);
    Serial.printf("%slongitude [°]              = %f\n", indent.c_str(), g.longitude);
    Serial.printf("%sE/W indicator              = %c\n", indent.c_str(), g.east_west_indicator);
    Serial.printf("%sposition fix indicator     = %d\n", indent.c_str(), g.position_fix_indicator);
    Serial.printf("%ssatellites used            = %d\n", indent.c_str(), g.satellites_used);
    Serial.printf("%shoriz. dilution precision  = %f\n", indent.c_str(), g.horizontal_dilution_of_precision);
    Serial.printf("%sMSL altitude               = %f\n", indent.c_str(), g.mean_sea_level_altitude);
    Serial.printf("%sMSL alttitude units        = %c\n", indent.c_str(), g.msl_alt_units);
    Serial.printf("%sgeoid separation           = %f\n", indent.c_str(), g.geoid_separation);
    Serial.printf("%sage of diff correction [s] = %f\n", indent.c_str(), g.age_of_diff_corr_seconds);
    Serial.printf("%sdiff. ref. station-id      = %d\n", indent.c_str(), g.diff_ref_station_id);
}

// -------------------------------------------------------------------------------------------------

void print(const GXgll &g, const String &prefix, const String &indent)
{
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%slatitude      = %f\n", indent.c_str(), g.latitude);
    Serial.printf("%sN/S indicator = %c\n", indent.c_str(), g.north_south_indicator);
    Serial.printf("%slongitude     = %f\n", indent.c_str(), g.longitude);
    Serial.printf("%sE/W indicator = %c\n", indent.c_str(), g.east_west_indicator);
    Serial.printf("%sutc time [s]  = %f\n", indent.c_str(), g.utc_time_seconds);
    Serial.printf("%sstatus        = %c\n", indent.c_str(), g.status);
    Serial.printf("%smode          = %c\n", indent.c_str(), g.mode);
}

// -------------------------------------------------------------------------------------------------

void print(const GXrmc &g, const String &prefix, const String &indent)
{
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%sutc time [s]          = %f\n", indent.c_str(), g.utc_time_seconds);
    Serial.printf("%sstatus                = %c\n", indent.c_str(), g.status);
    Serial.printf("%slatitude [°]          = %f\n", indent.c_str(), g.latitude);
    Serial.printf("%sN/S indicator         = %c\n", indent.c_str(), g.north_south_indicator);
    Serial.printf("%slongitude [°]         = %f\n", indent.c_str(), g.longitude);
    Serial.printf("%sE/W indicator         = %c\n", indent.c_str(), g.east_west_indicator);
    Serial.printf("%sspeed over gnd. [kts] = %f\n", indent.c_str(), g.speed_over_ground_kts);
    Serial.printf("%scourse over gnd. [°]  = %f\n", indent.c_str(), g.course_over_ground_deg);
    Serial.printf("%sdate                  = %s\n", indent.c_str(), g.date.c_str());
    Serial.printf("%smagn. variation       = %f\n", indent.c_str(), g.magnetic_variation);
    Serial.printf("%sE/W indicator 2       = %c\n", indent.c_str(), g.east_west_indicator_2);
    Serial.printf("%smode                  = %c\n", indent.c_str(), g.mode);
}

// -------------------------------------------------------------------------------------------------

void print(const GXvtg &g, const String &prefix, const String &indent)
{
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%scourse [°]            = %f\n", indent.c_str(), g.course_deg);
    Serial.printf("%scourse ref.           = %c\n", indent.c_str(), g.course_deg_reference);
    Serial.printf("%scourse magn. [°]      = %f\n", indent.c_str(), g.course_magnetic_deg);
    Serial.printf("%scourse magn. ref.     = %c\n", indent.c_str(), g.course_magnetic_deg_reference);
    Serial.printf("%sspeed hor. [kts]      = %f\n", indent.c_str(), g.speed_horizontal_kts);
    Serial.printf("%sspeed hor. [kts] unit = %f\n", indent.c_str(), g.speed_horizontal_kts_unit);
    Serial.printf("%sspeed hor. [kmh]      = %f\n", indent.c_str(), g.speed_horizontal_kmh);
    Serial.printf("%sspeed hor. [kmh] unit = %f\n", indent.c_str(), g.speed_horizontal_kmh_unit);
    Serial.printf("%smode                  = %c\n", indent.c_str(), g.mode);
}

// -------------------------------------------------------------------------------------------------

namespace
{
//!
//! \tparam IntermediateType
//! \tparam ValueType
//! \param str
//! \param field_nr
//! \param value
//! \return
template <typename IntermediateType, typename ValueType>
bool parse_value(const String &str, uint8_t field_nr, ValueType &value)
{
    constexpr char separator{ ',' };
    uint8_t field_begin_index{ 0 };
    while(field_nr > 0)
    {
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

    if(is >> intermediate_type)
    {
        value = static_cast<ValueType>(intermediate_type);
        //Serial.printf("OK: %s (%d,%d) -> ", value_string.c_str(), field_begin_index, field_end_index);
        //Serial.println(value);
        return true;
    }
    else
    {
        //Serial.printf("FAILED: %s -> ", value_string.c_str());
        //Serial.println(value);
        return false;
    }
}

bool parse_value(const String &str, uint8_t field_nr, String &value)
{
    constexpr char separator{ ',' };
    uint8_t field_begin_index{ 0 };
    while(field_nr > 0)
    {
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


} // namespace
// -------------------------------------------------------------------------------------------------

Bn880qGps::Bn880qGps(Stream &serial) : m_serial(serial) {}

// -------------------------------------------------------------------------------------------------

void Bn880qGps::setup()
{
    // Clear the list of messages which are sent.
    // MicroNMEA::sendSentence(gps_serial, "$PORZB");

    // Send only RMC and GGA messages.
    // MicroNMEA::sendSentence(gps_serial, "$PORZB,RMC,1,GGA,1");

    // Disable compatability mode (NV08C-CSM proprietary message) and
    // adjust precision of time and position fields

    // MicroNMEA::sendSentence(gps_serial, "$PNVGNME,2,9,1");
    // MicroNMEA::sendSentence(gps, "$PONME,2,4,1,0");

    sendSentence("PSRF105,1");  // debug on
    //TODO II $GNTXT,01,01,01,PSRF inv format*35

    sendSentence("PSRF106,21"); // set WGS84 datum
    //TODO II $GNTXT,01,01,01,PSRF inv format*35

}
// -------------------------------------------------------------------------------------------------

bool Bn880qGps::process()
{
    bool new_data_available = false;
    while(m_serial.available())
        if(m_sentence_parser.update(m_serial.read()))
            if(parseSentence())
                new_data_available = true;
    return new_data_available;
}

// -------------------------------------------------------------------------------------------------
bool Bn880qGps::parseSentence()
{
    const String sentence{ m_sentence_parser.getSentence() };
    const String sentence_fields{ sentence.substring(6, sentence.length() - 3) };
    const uint8_t fields_count{ m_sentence_parser.getFields() };
    const String sentence_type{ sentence.substring(1, 6) };

    // TODO
    // 1102 GNGSA

    //Serial.printf("IP -%s-\n", sentence_type.c_str());

    if(sentence_type.compareTo("GPGSV") == 0)
        return onGpgsv(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GNGSV") == 0)
        return onGngsv(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GLGSV") == 0)
        return onGlgsv(sentence_fields, fields_count);

    else if(sentence_type.compareTo("GPGGA") == 0)
        return onGpgga(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GNGGA") == 0)
        return onGngga(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GLGGA") == 0)
        return onGlgga(sentence_fields, fields_count);

    else if(sentence_type.compareTo("GPGLL") == 0)
        return onGpgll(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GNGLL") == 0)
        return onGngll(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GLGLL") == 0)
        return onGlgll(sentence_fields, fields_count);

    else if(sentence_type.compareTo("GPRMC") == 0)
        return onGprmc(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GNRMC") == 0)
        return onGnrmc(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GLRMC") == 0)
        return onGlrmc(sentence_fields, fields_count);

    else if(sentence_type.compareTo("GPVTG") == 0)
        return onGpvtg(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GNVTG") == 0)
        return onGnvtg(sentence_fields, fields_count);
    else if(sentence_type.compareTo("GLVTG") == 0)
        return onGlvtg(sentence_fields, fields_count);

    else
      Serial.printf("II %s\n", sentence.c_str());

    return false;
}

// -------------------------------------------------------------------------------------------------

void Bn880qGps::print()
{
    ::print(data.gpsgv, "GPSGV:", "  ");
    ::print(data.gnsgv, "GNSGV:", "  ");
    ::print(data.glsgv, "GLSGV:", "  ");

    ::print(data.gpgga, "GPGGA:", "  ");
    ::print(data.gngga, "GNGGA:", "  ");
    ::print(data.glgga, "GLGGA:", "  ");

    ::print(data.gpgll, "GPGLL:", "  ");
    ::print(data.gngll, "GNGLL:", "  ");
    ::print(data.glgll, "GLGLL:", "  ");

    ::print(data.gprmc, "GPRMC:", "  ");
    ::print(data.gnrmc, "GNRMC:", "  ");
    ::print(data.glrmc, "GLRMC:", "  ");

    ::print(data.gpvtg, "GPVTG:", "  ");
    ::print(data.gnvtg, "GNVTG:", "  ");
    ::print(data.glvtg, "GLVTG:", "  ");
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGpgsv(const String &sentence, uint8_t fields_count)
{
    return onGXgsv(sentence, fields_count, data.gpsgv);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGngsv(const String &sentence, uint8_t fields_count)
{
    return onGXgsv(sentence, fields_count, data.gnsgv);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlgsv(const String &sentence, uint8_t fields_count)
{
    return onGXgsv(sentence, fields_count, data.glsgv);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXgsv(const String &sentence, uint8_t fields_count, GXgsv &dest)
{
    /**
     * GSV - GNSS Satellites in View
     * Name                 Example     Unit    Desc
     * Message ID           $GPGSV              GSV protocol header
     * Number of Messages   2                   Range 1 to 3 (Depending on the number of satellites
     * tracked, multiple messages of GSV data may be required.) Message Number Range 1 to 3
     * Satellites in View   07 Satellite ID         07                  Channel 1 (Range 1 to 32)
     * Elevation 79 degrees Channel 1 (Maximum 90) Azimuth              048         degrees Channel
     * 1 (True, Range 0 to 359) SNR (C/N0)           42          dBHz    Range 0 to 99, null when
     * not tracking
     * ...                                      ...
     * Satellite ID         27                  Channel 4 (Range 1 to 32)
     * Elevation            27          degrees Channel 4 (Maximum 90)
     * Azimuth              138         degrees Channel 4 (True, Range 0 to 359)
     * SNR (C/N0)           42          dBHz    Range 0 to 99, null when not tracking
     * Checksum             *71
     * <CR> <LF>                                End of message termination
     */

    // Serial.printf("IP -%s-\n", sentence.c_str());

    bool parsed = true;

    if(fields_count > 0)
    {
        parsed |= parse_value<int>(sentence, (uint8_t)0, dest.numberOfMessages);
        if(dest.numberOfMessages > 1)
            return parsed;
    }

    if(fields_count > 1)
    {
        parsed |= parse_value<int>(sentence, 1, dest.messageNumber);
    }

    if(fields_count > 2)
    {
        parsed |= parse_value<int>(sentence, 2, dest.numberOfSatellitesInView);
    }


    if(fields_count > 6)
    {

        parsed |= parse_value<int>(sentence, 3, dest.channel1.id);

        parsed |= parse_value<int>(sentence, 4, dest.channel1.elevation);
        parsed |= parse_value<int>(sentence, 5, dest.channel1.azimuth);
        parsed |= parse_value<int>(sentence, 6, dest.channel1.snr);
    }
    // TODO: consider also each satellite in view > 4 that does not fit in 1st message

    return parsed;
};

// -------------------------------------------------------------------------------------------------
bool Bn880qGps::onGpgga(const String &sentence, uint8_t fields_count)
{
    return onGXgga(sentence, fields_count, data.gpgga);
};


// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGngga(const String &sentence, uint8_t fields_count)
{
    return onGXgga(sentence, fields_count, data.gngga);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlgga(const String &sentence, uint8_t fields_count)
{
    return onGXgga(sentence, fields_count, data.glgga);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXgga(const String &sentence, uint8_t fields_count, GXgga &dest)
{
    /**
     * GGA - Global Positioning System Fixed Data
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
     * Age of Diff. Corr.                       sec     Null fields when DGPS is not used
     * Diff. Ref. Station ID    0000
     * Checksum                 *5E
     * <CR> <LF>                                        End of message terminationn
     */

    // Serial.printf("IP -%s-\n", sentence.c_str());

    bool parsed = true;

    if(fields_count > 0)
        parsed |= parse_value<float>(sentence, 0, dest.utc_time_seconds);
    if(fields_count > 1)
        parsed |= parse_value<float>(sentence, 1, dest.latitude);
    if(fields_count > 2)
        parsed |= parse_value<char>(sentence, 2, dest.north_south_indicator);
    if(fields_count > 3)
        parsed |= parse_value<float>(sentence, 3, dest.longitude);
    if(fields_count > 4)
        parsed |= parse_value<char>(sentence, 4, dest.east_west_indicator);
    if(fields_count > 5)
        parsed |= parse_value<int>(sentence, 5, dest.position_fix_indicator);
    if(fields_count > 6)
        parsed |= parse_value<int>(sentence, 6, dest.satellites_used);
    if(fields_count > 7)
        parsed |= parse_value<float>(sentence, 7, dest.horizontal_dilution_of_precision);
    if(fields_count > 8)
        parsed |= parse_value<float>(sentence, 8, dest.geoid_separation);
    if(fields_count > 9)
        parsed |= parse_value<char>(sentence, 9, dest.geoid_separation_units);
    if(fields_count > 10)
        parsed |= parse_value<float>(sentence, 10, dest.age_of_diff_corr_seconds);
    if(fields_count > 11)
        parsed |= parse_value<int>(sentence, 11, dest.diff_ref_station_id);

    return parsed;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGpgll(const String &sentence, uint8_t fields_count)
{
    return onGXgll(sentence, fields_count, data.gpgll);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGngll(const String &sentence, uint8_t fields_count)
{
    return onGXgll(sentence, fields_count, data.gngll);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlgll(const String &sentence, uint8_t fields_count)
{
    return onGXgll(sentence, fields_count, data.glgll);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXgll(const String &sentence, uint8_t fields_count, GXgll &dest)
{
    /**
     * GLL—Geographic Position - Latitude/Longitude
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
    //Serial.printf("IP -%s-\n", sentence.c_str());

    bool parsed = true;

    if(fields_count > 0)
        parsed |= parse_value<float>(sentence, 0, dest.longitude);
    if(fields_count > 1)
        parsed |= parse_value<char>(sentence, 1, dest.north_south_indicator);
    if(fields_count > 2)
        parsed |= parse_value<float>(sentence, 2, dest.latitude);
    if(fields_count > 3)
        parsed |= parse_value<char>(sentence, 3, dest.east_west_indicator);
    if(fields_count > 4)
        parsed |= parse_value<float>(sentence, 4, dest.utc_time_seconds);
    if(fields_count > 5)
        parsed |= parse_value<char>(sentence, 5, dest.status);
    if(fields_count > 6)
        parsed |= parse_value<char>(sentence, 6, dest.mode);

    return parsed;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGprmc(const String &sentence, uint8_t fields_count)
{
    return onGXrmc(sentence, fields_count, data.gnrmc);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGnrmc(const String &sentence, uint8_t fields_count)
{
    return onGXrmc(sentence, fields_count, data.gnrmc);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlrmc(const String &sentence, uint8_t fields_count)
{
    return onGXrmc(sentence, fields_count, data.glrmc);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXrmc(const String &sentence, uint8_t fields_count, GXrmc &dest)
{
    /**
     * RMC—Recommended Minimum Specific GNSS Data
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

    //Serial.printf("IP -%s-\n", sentence.c_str());

    bool parsed = true;

    if(fields_count > 0)
        parsed |= parse_value<float>(sentence, 0, dest.utc_time_seconds);
    if(fields_count > 1)
        parsed |= parse_value<char>(sentence, 1, dest.status);
    if(fields_count > 2)
        parsed |= parse_value<float>(sentence, 2, dest.latitude);
    if(fields_count > 3)
        parsed |= parse_value<char>(sentence, 3, dest.north_south_indicator);
    if(fields_count > 4)
        parsed |= parse_value<float>(sentence, 4, dest.longitude);
    if(fields_count > 5)
        parsed |= parse_value<char>(sentence, 5, dest.east_west_indicator);
    if(fields_count > 6)
        parsed |= parse_value<float>(sentence, 6, dest.speed_over_ground_kts);
    if(fields_count > 7)
        parsed |= parse_value<float>(sentence, 7, dest.course_over_ground_deg);
    if(fields_count > 8)
        parsed |= parse_value(sentence, 8, dest.date);
    if(fields_count > 9)
        parsed |= parse_value<float>(sentence, 9, dest.magnetic_variation);
    if(fields_count > 10)
        parsed |= parse_value<char>(sentence, 10, dest.east_west_indicator_2);
    if(fields_count > 11)
        parsed |= parse_value<char>(sentence, 11, dest.mode);

    // TODO: test

    return parsed;
}

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGpvtg(const String &sentence, uint8_t fields_count)
{
    return onGXvtg(sentence, fields_count, data.gnvtg);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGnvtg(const String &sentence, uint8_t fields_count)
{
    return onGXvtg(sentence, fields_count, data.gnvtg);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGlvtg(const String &sentence, uint8_t fields_count)
{
    return onGXvtg(sentence, fields_count, data.glvtg);
};

// -------------------------------------------------------------------------------------------------

bool Bn880qGps::onGXvtg(const String &sentence, uint8_t fields_count, GXvtg &dest)
{
    /**
     * VTG—Course Over Ground and Gr// TODOound Speed
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

    //Serial.printf("IP -%s-\n", sentence.c_str());

    bool parsed = true;
    if(fields_count > 0)
        parsed |= parse_value<float>(sentence, 0, dest.course_deg);
    if(fields_count > 1)
        parsed |= parse_value<char>(sentence, 1, dest.course_magnetic_deg_reference);
    if(fields_count > 2)
        parsed |= parse_value<float>(sentence, 2, dest.course_magnetic_deg);
    if(fields_count > 3)
        parsed |= parse_value<char>(sentence, 3, dest.course_magnetic_deg_reference);
    if(fields_count > 4)
        parsed |= parse_value<float>(sentence, 4, dest.speed_horizontal_kts);
    if(fields_count > 5)
        parsed |= parse_value<char>(sentence, 5, dest.speed_horizontal_kts_unit);
    if(fields_count > 6)
        parsed |= parse_value<float>(sentence, 6, dest.speed_horizontal_kmh);
    if(fields_count > 7)
        parsed |= parse_value<char>(sentence, 7, dest.speed_horizontal_kmh_unit);
    if(fields_count > 8)
        parsed |= parse_value<char>(sentence, 8, dest.mode);

    // TODO: test

    return parsed;
}

void Bn880qGps::sendSentence(const char *sentence)
{
    if(sentence == nullptr || sentence[0] == 0)
        return;
    uint8_t crc{ computeCrc(sentence) };
    m_serial.printf("$%s*%02X\r\n", sentence, crc);
    Serial.printf("O $%s*%02X\r\n", sentence, crc);
}

// -------------------------------------------------------------------------------------------------

uint8_t Bn880qGps::computeCrc(const char *sentence)
{
    char crc{ sentence[0] };
    for(const char *c = sentence + 1; *c != 0; c++)
        crc = crc ^ *c;
    return crc;
}
