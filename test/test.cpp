#include <atomic>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Geofence.h"

constexpr ZoneInfo GoldenGatePark = { 2700.0 /*m*/, 37.76887, -122.48248, 1, false, GeofenceEventType::UNKNOWN };
constexpr ZoneInfo PoloField = { 100.0 /*m*/, 37.76815, -122.49268, 1, false, GeofenceEventType::UNKNOWN };
constexpr ZoneInfo EquitationField = { 35.0 /*m*/, 37.76637, -122.50125, 1, false, GeofenceEventType::UNKNOWN };
constexpr ZoneInfo StrawberryHill = { 155.0 /*m*/, 37.76867, -122.47535, 1, false, GeofenceEventType::UNKNOWN };

constexpr PointData TestPoints[] = {
    { -37.76887, 122.48248, 0.0, 0.0, 0 },   // Somewhere south of Australia
    { -34.60845, -58.37225, 0.0, 0.0, 0 },   // Buenos Aires, Argentina
    { 20.67622, -103.34693, 0.0, 0.0, 0 },   // Guadalajara, Mexico
    { 37.68821, -122.47201, 0.0, 0.0, 0 },   // Daly City In-N-Out
    { 37.74316, -122.47725, 0.0, 0.0, 0 },   // Mr Bread Bakery on Taraval
    { 37.76298, -122.45638, 0.0, 0.0, 0 },   // UCSF
    { 37.76705, -122.48593, 0.0, 0.0, 0 },   // Elk Glen Picnic Area
};

std::atomic<int> enterCount(0);
std::atomic<int> exitCount(0);
std::atomic<int> outsideCount(0);
std::atomic<int> insideCount(0);
void geofenceCallback(CallbackContext& context) {
    switch(context.event_type) {
        case GeofenceEventType::ENTER:
            enterCount++;
        break;
        case GeofenceEventType::EXIT:
            exitCount++;
        break;
        case GeofenceEventType::OUTSIDE:
            outsideCount++;
        break;
        case GeofenceEventType::INSIDE:
            insideCount++;
        break;
        default:
        break;
    }
}

TEST_CASE("Check Defaults Test") {
    Geofence test(4);
    test.init();

    auto zone0 = test.GetZoneInfo(0);
    REQUIRE(zone0.radius == 0.0);
    REQUIRE(zone0.center_lat == 0.0);
    REQUIRE(zone0.center_lon == 0.0);
    REQUIRE(zone0.confidence_number == 0);
    REQUIRE(zone0.enable == false);
    REQUIRE(zone0.event_type == GeofenceEventType::UNKNOWN);

    auto zone1 = test.GetZoneInfo(1);
    REQUIRE(zone1.radius == 0.0);
    REQUIRE(zone1.center_lat == 0.0);
    REQUIRE(zone1.center_lon == 0.0);
    REQUIRE(zone1.confidence_number == 0);
    REQUIRE(zone1.enable == false);
    REQUIRE(zone1.event_type == GeofenceEventType::UNKNOWN);

    auto zone2 = test.GetZoneInfo(2);
    REQUIRE(zone2.radius == 0.0);
    REQUIRE(zone2.center_lat == 0.0);
    REQUIRE(zone2.center_lon == 0.0);
    REQUIRE(zone2.confidence_number == 0);
    REQUIRE(zone2.enable == false);
    REQUIRE(zone2.event_type == GeofenceEventType::UNKNOWN);

    auto zone3 = test.GetZoneInfo(3);
    REQUIRE(zone3.radius == 0.0);
    REQUIRE(zone3.center_lat == 0.0);
    REQUIRE(zone3.center_lon == 0.0);
    REQUIRE(zone3.confidence_number == 0);
    REQUIRE(zone3.enable == false);
    REQUIRE(zone3.event_type == GeofenceEventType::UNKNOWN);
}

TEST_CASE("Disabled Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.SetZoneInfo(1, PoloField);
    test.SetZoneInfo(2, EquitationField);
    test.SetZoneInfo(3, StrawberryHill);

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[1]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[2]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[3]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[4]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[5]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("Enabled Test, UNKOWN EVENT TYPE") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.SetZoneInfo(1, PoloField);
    test.GetZoneInfo(1).enable = true;
    test.SetZoneInfo(2, EquitationField);
    test.GetZoneInfo(2).enable = true;
    test.SetZoneInfo(3, StrawberryHill);
    test.GetZoneInfo(3).enable = true;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[1]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[2]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[3]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[4]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[5]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("Outside Event Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).event_type = GeofenceEventType::OUTSIDE;
    test.SetZoneInfo(1, PoloField);
    test.GetZoneInfo(1).enable = true;
    test.GetZoneInfo(1).event_type = GeofenceEventType::OUTSIDE;
    test.SetZoneInfo(2, EquitationField);
    test.GetZoneInfo(2).enable = true;
    test.GetZoneInfo(2).event_type = GeofenceEventType::OUTSIDE;
    test.SetZoneInfo(3, StrawberryHill);
    test.GetZoneInfo(3).enable = true;
    test.GetZoneInfo(3).event_type = GeofenceEventType::OUTSIDE;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 4);

    test.UpdateGeofencePoint(TestPoints[1]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 4);

    test.UpdateGeofencePoint(TestPoints[2]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 4);

    test.UpdateGeofencePoint(TestPoints[3]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 4);
}

TEST_CASE("Inside Event Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).event_type = GeofenceEventType::INSIDE;
    test.SetZoneInfo(1, PoloField);
    test.GetZoneInfo(1).enable = true;
    test.GetZoneInfo(1).event_type = GeofenceEventType::INSIDE;
    test.SetZoneInfo(2, EquitationField);
    test.GetZoneInfo(2).enable = true;
    test.GetZoneInfo(2).event_type = GeofenceEventType::INSIDE;
    test.SetZoneInfo(3, StrawberryHill);
    test.GetZoneInfo(3).enable = true;
    test.GetZoneInfo(3).event_type = GeofenceEventType::INSIDE;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[6]);
    test.loop();
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);
}

