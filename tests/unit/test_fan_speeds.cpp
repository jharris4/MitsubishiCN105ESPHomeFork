/// Fan mode mapping for 4-speed (default) and 3-speed (supports.fan_speeds: 3) units.
#include <gtest/gtest.h>

#include "heatpump_profile_protocol.h"

using esphome::cn105::HeatPumpProtocol;
using namespace esphome::climate;

TEST(FanSpeeds, FourSpeedSendsUnchanged) {
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_AUTO, 4), "AUTO");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_ON, 4), "AUTO");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_QUIET, 4), "QUIET");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_DIFFUSE, 4), "QUIET");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_LOW, 4), "1");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_MEDIUM, 4), "2");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_MIDDLE, 4), "3");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_HIGH, 4), "4");
}

TEST(FanSpeeds, FourSpeedReportsUnchanged) {
    EXPECT_EQ(HeatPumpProtocol::fan_mode("AUTO", 4), CLIMATE_FAN_AUTO);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("QUIET", 4), CLIMATE_FAN_QUIET);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("1", 4), CLIMATE_FAN_LOW);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("2", 4), CLIMATE_FAN_MEDIUM);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("3", 4), CLIMATE_FAN_MIDDLE);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("4", 4), CLIMATE_FAN_HIGH);
}

TEST(FanSpeeds, ThreeSpeedSendsHighAsTopSpeed) {
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_AUTO, 3), "AUTO");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_QUIET, 3), "QUIET");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_LOW, 3), "1");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_MEDIUM, 3), "2");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_HIGH, 3), "3");
    EXPECT_STREQ(HeatPumpProtocol::fan_setting(CLIMATE_FAN_MIDDLE, 3), "3");
}

TEST(FanSpeeds, ThreeSpeedReportsThreeAndFourAsHigh) {
    EXPECT_EQ(HeatPumpProtocol::fan_mode("AUTO", 3), CLIMATE_FAN_AUTO);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("QUIET", 3), CLIMATE_FAN_QUIET);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("1", 3), CLIMATE_FAN_LOW);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("2", 3), CLIMATE_FAN_MEDIUM);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("3", 3), CLIMATE_FAN_HIGH);
    EXPECT_EQ(HeatPumpProtocol::fan_mode("4", 3), CLIMATE_FAN_HIGH);
}

TEST(FanSpeeds, UnknownOrNullReportsAuto) {
    for (int speeds : {3, 4}) {
        EXPECT_EQ(HeatPumpProtocol::fan_mode(nullptr, speeds), CLIMATE_FAN_AUTO);
        EXPECT_EQ(HeatPumpProtocol::fan_mode("5", speeds), CLIMATE_FAN_AUTO);
    }
}

TEST(FanSpeeds, SentSettingRoundTripsThroughFanMap) {
    for (int speeds : {3, 4}) {
        for (auto mode : {CLIMATE_FAN_AUTO, CLIMATE_FAN_QUIET, CLIMATE_FAN_LOW, CLIMATE_FAN_MEDIUM, CLIMATE_FAN_HIGH}) {
            const char* setting = HeatPumpProtocol::fan_setting(mode, speeds);
            EXPECT_GE(cn105_protocol::lookup_index(FAN_MAP, 6, setting), 0);
            EXPECT_EQ(HeatPumpProtocol::fan_mode(setting, speeds), mode);
        }
    }
}
