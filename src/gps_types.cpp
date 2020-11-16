#include "gps_types.h"

#include <HardwareSerial.h>
#include <cmath>
#include <sstream>

// -------------------------------------------------------------------------------------------------

void print(const GXgsvSatelliteChannel &g, const String &prefix, const String &indent) {
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%sid        = %d\n", indent.c_str(), g.id);
    Serial.printf("%selevation = %d\n", indent.c_str(), g.elevation);
    Serial.printf("%sazimuth   = %d\n", indent.c_str(), g.azimuth);
    Serial.printf("%sSNR       = %d\n", indent.c_str(), g.snr);
}

// -------------------------------------------------------------------------------------------------

void print(const GXgsv &g, const String &prefix, const String &indent) {
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%s# of messages        = %d\n", indent.c_str(), g.numberOfMessages);
    Serial.printf("%smessage #            = %d\n", indent.c_str(), g.messageNumber);
    Serial.printf("%s# satellites in view = %d\n", indent.c_str(), g.numberOfSatellitesInView);
    print(g.channel_1, "channel 1:", indent + "  ");
    print(g.channel_2, "channel 2:", indent + "  ");
    print(g.channel_3, "channel 3:", indent + "  ");
    print(g.channel_4, "channel 4:", indent + "  ");
}

// -------------------------------------------------------------------------------------------------

void print(const GXgga &g, const String &prefix, const String &indent) {
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

void print(const GXgll &g, const String &prefix, const String &indent) {
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

void print(const GXrmc &g, const String &prefix, const String &indent) {
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

void print(const GXvtg &g, const String &prefix, const String &indent) {
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%scourse [°]            = %f\n", indent.c_str(), g.course_deg);
    Serial.printf("%scourse ref.           = %c\n", indent.c_str(), g.course_deg_reference);
    Serial.printf("%scourse magn. [°]      = %f\n", indent.c_str(), g.course_magnetic_deg);
    Serial.printf("%scourse magn. ref.     = %c\n", indent.c_str(), g.course_magnetic_deg_reference);
    Serial.printf("%sspeed hor. [kts]      = %f\n", indent.c_str(), g.speed_horizontal_kts);
    Serial.printf("%sspeed hor. [kts] unit = %c\n", indent.c_str(), g.speed_horizontal_kts_unit);
    Serial.printf("%sspeed hor. [kmh]      = %f\n", indent.c_str(), g.speed_horizontal_kmh);
    Serial.printf("%sspeed hor. [kmh] unit = %c\n", indent.c_str(), g.speed_horizontal_kmh_unit);
    Serial.printf("%smode                  = %c\n", indent.c_str(), g.mode);
}

// -------------------------------------------------------------------------------------------------

void print(const GXgsa &g, const String &prefix, const String &indent) {
    Serial.printf("%s\n", prefix.c_str());
    Serial.printf("%smode 1              = %c\n", indent.c_str(), g.mode_1);
    Serial.printf("%smode 2              = %d\n", indent.c_str(), g.mode_2);
    Serial.printf("%s# sat used ch 1     = %d\n", indent.c_str(), g.sat_used_ch_1);
    Serial.printf("%s# sat used ch 2     = %d\n", indent.c_str(), g.sat_used_ch_2);
    Serial.printf("%s# sat used ch 3     = %d\n", indent.c_str(), g.sat_used_ch_3);
    Serial.printf("%s# sat used ch 4     = %d\n", indent.c_str(), g.sat_used_ch_4);
    Serial.printf("%s# sat used ch 5     = %d\n", indent.c_str(), g.sat_used_ch_5);
    Serial.printf("%s# sat used ch 6     = %d\n", indent.c_str(), g.sat_used_ch_6);
    Serial.printf("%s# sat used ch 7     = %d\n", indent.c_str(), g.sat_used_ch_7);
    Serial.printf("%s# sat used ch 8     = %d\n", indent.c_str(), g.sat_used_ch_8);
    Serial.printf("%s# sat used ch 9     = %d\n", indent.c_str(), g.sat_used_ch_9);
    Serial.printf("%s# sat used ch 10    = %d\n", indent.c_str(), g.sat_used_ch_10);
    Serial.printf("%s# sat used ch 11    = %d\n", indent.c_str(), g.sat_used_ch_11);
    Serial.printf("%s# sat used ch 12    = %d\n", indent.c_str(), g.sat_used_ch_12);
    Serial.printf("%spos. dilution prec. = %f\n", indent.c_str(), g.pdop);
    Serial.printf("%shor. dilution prec. = %f\n", indent.c_str(), g.hdop);
    Serial.printf("%sver. dilution prec. = %f\n", indent.c_str(), g.vdop);
}

// -------------------------------------------------------------------------------------------------

void print(const GpsData &g, const String &prefix, const String &indent) {
    Serial.printf("%s\n", prefix.c_str());
    ::print(g.gpgsv_1, "GPGSV[0]:", indent);
    ::print(g.gpgsv_2, "GPGSV[1]:", indent);
    ::print(g.gpgsv_3, "GPGSV[2]:", indent);

    ::print(g.gngsv_1, "GNGSV[0]:", indent);
    ::print(g.gngsv_2, "GNGSV[1]:", indent);
    ::print(g.gngsv_3, "GNGSV[2]:", indent);

    ::print(g.glgsv_1, "GLGSV[0]:", indent);
    ::print(g.glgsv_2, "GLGSV[1]:", indent);
    ::print(g.glgsv_3, "GLGSV[2]:", indent);

    ::print(g.gpgga, "GPGGA:", indent);
    ::print(g.gngga, "GNGGA:", indent);
    ::print(g.glgga, "GLGGA:", indent);

    ::print(g.gpgll, "GPGLL:", indent);
    ::print(g.gngll, "GNGLL:", indent);
    ::print(g.glgll, "GLGLL:", indent);

    ::print(g.gprmc, "GPRMC:", indent);
    ::print(g.gnrmc, "GNRMC:", indent);
    ::print(g.glrmc, "GLRMC:", indent);

    ::print(g.gpvtg, "GPVTG:", indent);
    ::print(g.gnvtg, "GNVTG:", indent);
    ::print(g.glvtg, "GLVTG:", indent);

    ::print(g.gpgsa, "GPGSA:", indent);
    ::print(g.gngsa, "GNGSA:", indent);
    ::print(g.glgsa, "GLGSA:", indent);
}

// -------------------------------------------------------------------------------------------------

void printTabular(const GpsData &g, const String &prefix, const String &indent) {
    // ┌─┬─┐
    // │ │ │
    // ├─┼─┤
    // │ │ │
    // └─┴─┘

    Serial.printf("%s\n", prefix.c_str());
    // clang-format off
    Serial.println(
    "┌────────────────────────────┬─────────────────────┬─────────────────────┬─────────────────────┐");
    Serial.println(
    "| Description                | GNSS (GN)           | GPS (GP)            | GLONASS (GL)        |");
    Serial.println(
    "├────────────────────────────┴─────────────────────┴─────────────────────┴─────────────────────┤");

    Serial.printf("| GLL - Geographic Position - Latitude/Longitude                                               |\n");

    auto convert_utc_hour = [](float gps_utc) {
      if (std::isnan(gps_utc)) return static_cast<int8_t>(-1);
      return static_cast<int8_t>((static_cast<uint32>(gps_utc) % 10000000) / 10000);
    };

    auto convert_utc_minute = [](float gps_utc) {
      if (std::isnan(gps_utc)) return static_cast<int8_t>(-1);
      return static_cast<int8_t>((static_cast<uint32>(gps_utc) % 10000) / 1000);
    };

    auto convert_utc_second = [](float gps_utc) {
      if (std::isnan(gps_utc)) return static_cast<int8_t>(-1);
      return static_cast<int8_t>((static_cast<uint32>(gps_utc) % 100) / 1);
    };

    auto convert_utc_decisecond = [](float gps_utc) {
      if (std::isnan(gps_utc)) return static_cast<int8_t>(-1);
      return static_cast<int8_t>((static_cast<uint32>(gps_utc * 100) % 100));
    };

    auto convert_indicator = [](char indicator) {
      const char i{ static_cast<char>(tolower(indicator)) };
      if(i == 'n') return 'N'; else
      if(i == 'e') return 'E'; else
      if(i == 's') return 'S'; else
      if(i == 'w') return 'W'; else
      if(i == ' ') return '-'; else
          return indicator; };

    Serial.printf("%s| lat/lon [ddmm.mmmm]         %-9.4f%c/%-9.4f%c %-9.4f%c/%-9.4f%c %-9.4f%c/%-9.4f%c|\n",
                  indent.c_str(),
                  g.gngll.latitude, convert_indicator(g.gngll.north_south_indicator), g.gngll.longitude, convert_indicator(g.gngll.east_west_indicator),
                  g.gpgll.latitude, convert_indicator(g.gpgll.north_south_indicator), g.gpgll.longitude, convert_indicator(g.gpgll.east_west_indicator),
                  g.glgll.latitude, convert_indicator(g.glgll.north_south_indicator), g.glgll.longitude, convert_indicator(g.glgll.east_west_indicator));

    Serial.printf("%s| UTC [hh:mm:ss.ss]           %02d:%02d:%02d.%02d           %02d:%02d:%02d.%02d           %02d:%02d:%02d.%02d          |\n",
                  indent.c_str(),
                  convert_utc_hour(g.gngll.utc_time_seconds), convert_utc_minute(g.gngll.utc_time_seconds), convert_utc_second(g.gngll.utc_time_seconds), convert_utc_decisecond(g.gngll.utc_time_seconds),
                  convert_utc_hour(g.gpgll.utc_time_seconds), convert_utc_minute(g.gpgll.utc_time_seconds), convert_utc_second(g.gpgll.utc_time_seconds), convert_utc_decisecond(g.gpgll.utc_time_seconds),
                  convert_utc_hour(g.glgll.utc_time_seconds), convert_utc_minute(g.glgll.utc_time_seconds), convert_utc_second(g.glgll.utc_time_seconds), convert_utc_decisecond(g.glgll.utc_time_seconds));

    auto convert_status = [](char status) {
      char s { static_cast<char>(tolower(status)) };
      if (s == 'a') return "valid"; else
      if (s == 'v') return "invalid"; else
      if (s == ' ') return "-"; else
          return String{status}.c_str(); };

    Serial.printf("%s| status                      %-7s               %-7s               %-7s              |\n",
                  indent.c_str(),
                  convert_status(g.gngll.status), convert_status(g.gpgll.status), convert_status(g.glgll.status) );

    auto convert_mode = [](char mode) {
      const char m{ static_cast<char>(tolower(mode)) };
      if(m == 'a') return "autonomous"; else
      if(m == 'd') return "DGPS"; else
      if(m == 'e') return "DR"; else
      if(m == ' ') return "-"; else
          return String{mode}.c_str(); };

    Serial.printf("%s| mode                        %-10s            %-10s            %-10s           |\n",
                  indent.c_str(),
                  convert_mode(g.gngll.mode), convert_mode(g.gpgll.mode), convert_mode(g.glgll.mode));

    Serial.println("├──────────────────────────────────────────────────────────────────────────────────────────────┤");

// VTG - Course Over Ground and Ground Speed
    Serial.printf("| VTG - Course Over Ground and Ground Speed                                                    |\n");

    Serial.printf("%s| course true [deg]           %-6.2f                %-6.2f                %-6.2f               |\n",
                  indent.c_str(),
                  g.gnvtg.course_deg, g.gpvtg.course_deg, g.glvtg.course_deg);

    auto convert_course_reference = [] (char course_reference) {
        const char r{static_cast<char>(tolower(course_reference))};
        if (r == 't') return "true"; else
        if (r == 'm') return "magnetic"; else
        if (r == ' ') return "-"; else
        return String{course_reference}.c_str(); };

    Serial.printf("%s| course true reference       %-4s                  %-4s                  %-4s                 |\n",
                  indent.c_str(),
                  convert_course_reference(g.gnvtg.course_deg_reference),
                  convert_course_reference(g.gpvtg.course_magnetic_deg_reference),
                  convert_course_reference(g.glvtg.course_deg_reference));

    Serial.printf("%s| course magnetic [deg]       %-6.2f                %-6.2f                %-6.2f               |\n",
                  indent.c_str(),
                  g.gnvtg.course_deg, g.gpvtg.course_deg, g.glvtg.course_deg);

    Serial.printf("%s| course magnetic reference   %-8s              %-8s              %-8s             |\n",
                  indent.c_str(),
                  convert_course_reference(g.gnvtg.course_deg_reference),
                  convert_course_reference(g.gpvtg.course_magnetic_deg_reference),
                  convert_course_reference(g.glvtg.course_deg_reference));

    Serial.printf("%s| speed horizontal [kts]      %-6.2f          [%c]   %-6.2f          [%c]   %-6.2f          [%c]  |\n",
                  indent.c_str(),
                  g.gnvtg.speed_horizontal_kts, g.gnvtg.speed_horizontal_kts_unit,
                  g.gpvtg.speed_horizontal_kts, g.gpvtg.speed_horizontal_kts_unit,
                  g.glvtg.speed_horizontal_kts, g.glvtg.speed_horizontal_kts_unit);

    Serial.printf("%s| speed horizontal [kmh]      %-6.2f          [%c]   %-6.2f          [%c]   %-6.2f          [%c]  |\n",
                  indent.c_str(),
                  g.gnvtg.speed_horizontal_kmh, g.gnvtg.speed_horizontal_kmh_unit,
                  g.gpvtg.speed_horizontal_kmh, g.gpvtg.speed_horizontal_kmh_unit,
                  g.glvtg.speed_horizontal_kmh, g.glvtg.speed_horizontal_kmh_unit);

    Serial.printf("%s| mode                        %-10s            %-10s            %-10s           |\n",
                  indent.c_str(),
                  convert_mode(g.gnvtg.mode), convert_mode(g.gpvtg.mode), convert_mode(g.glvtg.mode));

    Serial.println("├──────────────────────────────────────────────────────────────────────────────────────────────┤");

    Serial.printf("| RMC - Recommended Minimum Specific GNSS Data                                                 |\n");

    Serial.printf("%s| UTC [hh:mm:ss.ss]           %02d:%02d:%02d.%02d           %02d:%02d:%02d.%02d           %02d:%02d:%02d.%02d          |\n",
                  indent.c_str(),
                  convert_utc_hour(g.gnrmc.utc_time_seconds), convert_utc_minute(g.gnrmc.utc_time_seconds), convert_utc_second(g.gngga.utc_time_seconds), convert_utc_decisecond(g.gnrmc.utc_time_seconds),
                  convert_utc_hour(g.gprmc.utc_time_seconds), convert_utc_minute(g.gprmc.utc_time_seconds), convert_utc_second(g.gpgga.utc_time_seconds), convert_utc_decisecond(g.gprmc.utc_time_seconds),
                  convert_utc_hour(g.glrmc.utc_time_seconds), convert_utc_minute(g.glrmc.utc_time_seconds), convert_utc_second(g.glgga.utc_time_seconds), convert_utc_decisecond(g.glrmc.utc_time_seconds));

    Serial.printf("%s| lat/lon [ddmm.mmmm]         %-9.4f%c/%-9.4f%c %-9.4f%c/%-9.4f%c %-9.4f%c/%-9.4f%c|\n",
                  indent.c_str(),
                  g.gnrmc.latitude, convert_indicator(g.gnrmc.north_south_indicator), g.gnrmc.longitude, convert_indicator(g.gnrmc.east_west_indicator),
                  g.gprmc.latitude, convert_indicator(g.gprmc.north_south_indicator), g.gprmc.longitude, convert_indicator(g.gprmc.east_west_indicator),
                  g.glrmc.latitude, convert_indicator(g.glrmc.north_south_indicator), g.glrmc.longitude, convert_indicator(g.glrmc.east_west_indicator));

    Serial.printf("%s| speed over gnd. [kts]       %-7.3f               %-7.3f               %-7.3f              |\n",
                  indent.c_str(),
                  g.gnrmc.speed_over_ground_kts, g.gprmc.speed_over_ground_kts, g.glrmc.speed_over_ground_kts);

    Serial.printf("%s| course over gnd. [deg]      %-7.3f               %-7.3f               %-7.3f              |\n",
                  indent.c_str(),
                  g.gnrmc.course_over_ground_deg, g.gprmc.course_over_ground_deg, g.glrmc.course_over_ground_deg);

    auto convert_date_day = [](const String &gps_date) {
      if (gps_date.length() < 6) return static_cast<int8_t>(-1);
      std::istringstream is(gps_date.substring(0, 2).c_str());
      short int day;
      if(is >> day) return static_cast<int8_t>(day);
      else  return static_cast<int8_t>(-1);
    };

    auto convert_date_month = [](const String &gps_date) {
      if (gps_date.length() < 6) return static_cast<int8_t>(-1);
      std::istringstream is(gps_date.substring(2, 4).c_str());
      short int day;
      if(is >> day) return static_cast<int8_t>(day);
      else return static_cast<int8_t>(-1);
    };

    auto convert_date_decade = [](const String &gps_date) {
      if (gps_date.length() < 6) return static_cast<int8_t>(-1);
      std::istringstream is(gps_date.substring(4, 6).c_str());
      short int day;
      if(is >> day) return static_cast<int8_t>(day);
      else return static_cast<int8_t>(-1);
    };

    Serial.printf("%s| date                        %02d.%02d.%02d              %02d.%02d.%02d              %02d.%02d.%02d             |\n",
                  indent.c_str(),
                  convert_date_day(g.gnrmc.date), convert_date_month(g.gnrmc.date), convert_date_decade(g.gnrmc.date),
                  convert_date_day(g.gprmc.date), convert_date_month(g.gprmc.date), convert_date_decade(g.gprmc.date),
                  convert_date_day(g.glrmc.date), convert_date_month(g.glrmc.date), convert_date_decade(g.glrmc.date));

    Serial.printf("%s| magnetic variation          %-7.3f               %-7.3f               %-7.3f              |\n",
                  indent.c_str(),
                  g.gnrmc.magnetic_variation, g.gprmc.magnetic_variation, g.glrmc.magnetic_variation);

    auto convert_north_south_indicator2 = [](char indicator2){
      const char i{static_cast<char>(tolower(indicator2))};
      if (i == 'e') return 'E'; else
      if (i == 'w') return 'W'; else // TODO: either 'E' or '' then '' is 'W'
      if (i == ' ') return '-'; else // TODO: or 'E' or 'W' or '' then '' is "invalid"
      return indicator2; };

    Serial.printf("%s| E/W indicator 2             %c                     %c                     %c                    |\n",
                  indent.c_str(),
                  convert_north_south_indicator2(g.gnrmc.north_south_indicator),
                  convert_north_south_indicator2(g.gprmc.north_south_indicator),
                  convert_north_south_indicator2(g.glrmc.north_south_indicator));

    Serial.printf("%s| mode                        %-10s            %-10s            %-10s           |\n",
                  indent.c_str(),
                  convert_mode(g.gnrmc.mode), convert_mode(g.gprmc.mode), convert_mode(g.glrmc.mode));


    Serial.println("├──────────────────────────────────────────────────────────────────────────────────────────────┤");

    Serial.printf("| GGA - Global Positioning System Fixed Data                                                   |\n");

    Serial.printf("%s| UTC [hh:mm:ss.ss]           %02d:%02d:%02d.%02d           %02d:%02d:%02d.%02d           %02d:%02d:%02d.%02d          |\n",
                  indent.c_str(),
                  convert_utc_hour(g.gngga.utc_time_seconds), convert_utc_minute(g.gngga.utc_time_seconds), convert_utc_second(g.gngga.utc_time_seconds), convert_utc_decisecond(g.gngga.utc_time_seconds),
                  convert_utc_hour(g.gpgga.utc_time_seconds), convert_utc_minute(g.gpgga.utc_time_seconds), convert_utc_second(g.gpgga.utc_time_seconds), convert_utc_decisecond(g.gpgga.utc_time_seconds),
                  convert_utc_hour(g.glgga.utc_time_seconds), convert_utc_minute(g.glgga.utc_time_seconds), convert_utc_second(g.glgga.utc_time_seconds), convert_utc_decisecond(g.glgga.utc_time_seconds));

    Serial.printf("%s| lat/lon [ddmm.mmmm]         %-9.4f%c/%-9.4f%c %-9.4f%c/%-9.4f%c %-9.4f%c/%-9.4f%c|\n",
                  indent.c_str(),
                  g.gngga.latitude, convert_indicator(g.gngga.north_south_indicator), g.gngga.longitude, convert_indicator(g.gngga.east_west_indicator),
                  g.gpgga.latitude, convert_indicator(g.gpgga.north_south_indicator), g.gpgga.longitude, convert_indicator(g.gpgga.east_west_indicator),
                  g.glgga.latitude, convert_indicator(g.glgga.north_south_indicator), g.glgga.longitude, convert_indicator(g.glgga.east_west_indicator));

    auto translate_position_fix = [](int8_t indicator) {
        if(indicator == -1) return "na"; else
        if(indicator == 0) return "no fix"; else
        if(indicator == 1) return "GPS, SPS"; else
        if(indicator == 2) return "diff. GPS, SPS"; else
        if(indicator == 3 || indicator == 4 || indicator == 5) return "not supported"; else
        if(indicator == 6) return "dead reckoning";
        else return "-"; };

    Serial.printf("%s| position fix indicator      %-14s        %-14s        %-14s       |\n",
                  indent.c_str(),
                  translate_position_fix(g.gngga.position_fix_indicator),
                  translate_position_fix(g.gpgga.position_fix_indicator),
                  translate_position_fix(g.glgga.position_fix_indicator));

    Serial.printf("%s| satellites used             %-2d                    %-2d                    %-2d                   |\n",
                  indent.c_str(),
                  g.gngga.satellites_used, g.gpgga.satellites_used, g.glgga.satellites_used);

    Serial.printf("%s| horiz. dilution precision   %-7.3f               %-7.3f               %-7.3f              |\n",
                  indent.c_str(),
                  g.gngga.horizontal_dilution_of_precision,
                  g.gpgga.horizontal_dilution_of_precision,
                  g.glgga.horizontal_dilution_of_precision);

    Serial.printf("%s| MSL altitude                %-7.3f         [%c]   %-7.3f         [%c]   %-7.3f         [%c]  |\n",
                  indent.c_str(),
                  g.gngga.mean_sea_level_altitude, g.gngga.msl_alt_units,
                  g.gpgga.mean_sea_level_altitude, g.gpgga.msl_alt_units,
                  g.glgga.mean_sea_level_altitude, g.glgga.msl_alt_units);

    Serial.printf("%s| geoid separation            %-7.3f         [%c]   %-7.3f         [%c]   %-7.3f         [%c]  |\n",
                  indent.c_str(),
                  g.gngga.geoid_separation, g.gngga.geoid_separation_units,
                  g.gpgga.geoid_separation, g.gpgga.geoid_separation_units,
                  g.glgga.geoid_separation, g.glgga.geoid_separation_units);

    Serial.printf("%s| age of diff correction      %-7.3f         [s]   %-7.3f         [s]   %-7.3f         [s]  |\n",
                  indent.c_str(),
                  g.gngga.age_of_diff_corr_seconds,
                  g.gpgga.age_of_diff_corr_seconds,
                  g.glgga.age_of_diff_corr_seconds);

    Serial.printf("%s| diff. ref. station-id       %04d                  %04d                  %04d                 |\n",
                  indent.c_str(),
                  g.gngga.diff_ref_station_id, g.gpgga.diff_ref_station_id, g.glgga.diff_ref_station_id);


    Serial.println("└──────────────────────────────────────────────────────────────────────────────────────────────┘");





// GSV - GNSS Satellites in View
/*
 *G[NPL]SGV[0-2]:
  # of messages        = 0
  message #            = 0
  # satellites in view = 0
channel 1:
    id        = 0
    elevation = 0
    azimuth   = 0
    SNR       = 0
channel 2:
    id        = 0
    elevation = 0
    azimuth   = 0
    SNR       = 0
channel 3:
    id        = 0
    elevation = 0
    azimuth   = 0
    SNR       = 0
channel 4:
    id        = 0
    elevation = 0
    azimuth   = 0
    SNR       = 0
 */

// GSA - GNSS DOP and Active Satellites
/*
 *  mode 1              =
  mode 2              = 32
  # sat used ch 1     = 0
  # sat used ch 2     = 0
  # sat used ch 3     = 0
  # sat used ch 4     = 0
  # sat used ch 5     = 0
  # sat used ch 6     = 0
  # sat used ch 7     = 0
  # sat used ch 8     = 0
  # sat used ch 9     = 0
  # sat used ch 10    = 0
  # sat used ch 11    = 0
  # sat used ch 12    = 0
  pos. dilution prec. = 0.000000
  hor. dilution prec. = 0.000000
  ver. dilution prec. = 0.000000
 */


    // clang-format on
}