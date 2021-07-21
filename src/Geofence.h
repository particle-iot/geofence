/*
 * Copyright (c) 2021 Particle Industries, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "Particle.h"

/**
 * @brief Type definition of geofence event callback signature.
 *
 */
using GeofenceEventCallback = std::function<void(void)>;

enum class GeofenceEventType {
    INSIDE,
    OUTSIDE,
    ENTER,
    EXIT,
    UNKNOWN,
};

enum class ZoneType {
    CIRCULAR,
    POLYGONAL,
};

struct PointData {
    time_t epochTime; /**< Epoch time from device sources */
    double lat; /**< Point latitude in degrees */
    double lon; /**< Point longitude in degrees */
};

struct ZoneInfo {
    ZoneInfo() : radius(0), center_lat(0), center_lon(0), 
            confidence_number(0), enable(false), 
            event_type(GeofenceEventType::UNKNOWN), 
            zone_type(ZoneType::CIRCULAR) {}
    float radius; //radius in meters that define the geofence zone boundary
    double center_lat;                 /**< Center point latitude in degrees */
    double center_lon;                /**< Center point longitude in degrees */
    uint8_t confidence_number; //a number to reduce false positives
    bool enable; //enable or disable the geofence zone
    GeofenceEventType event_type; //type of event to trigger
    ZoneType zone_type;
};

class Geofence {
public:

    Geofence(int num_of_zones) : GeofenceZones(num_of_zones) {
    }

    /**
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     */
    void init();

    /**
     * @brief Called periodically to check for geofence boundary conditions
     *
     * @details This is periodically called to calculate geofence points
     * and their relation to the boundary. Callbacks will be triggered here if
     * the event type conditions are met (outside, inside, enter, exit) 
     */
    void loop();

    /**
     * @brief Sets the zone info for configuration of a zone
     *
     * @details Number of zones are created by the Geofence ctor, then they have
     * to be configured through this function, and placed in the vector using
     * the index passed to it
     *
     * @param[in] index index of vector to store the zone info
     * @param[in] zone_config reference to the zone info you want to set to
     */
    void SetZoneInfo(int index, ZoneInfo& zone_config) {
        auto zone = GeofenceZones.at(index);
        zone = zone_config;
    }

    /**
     * @brief Pass the point data to be used to calculate boundary
     *
     * @details This function is called to pass point information that is
     * used in the tick() function to calculate geofence bounds
     *
     * @param[in] PointData point to be passed for calculation
     */
    void UpdateGeofencePoint(PointData& point) {
        _geofence_point = point;
    }

    /**
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     *
     * @param[in] <name of variable> <description of variable>
     *
     * @return <what does the function return (optional if void)>
     */
    int RegisterOutsideGeofence(GeofenceEventCallback callback);

    /**
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     *
     * @param[in] <name of variable> <description of variable>
     *
     * @return <what does the function return (optional if void)>
     */
    int RegisterInsideGeofence(GeofenceEventCallback callback);

    /**
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     *
     * @param[in] <name of variable> <description of variable>
     *
     * @return <what does the function return (optional if void)>
     */
    int RegisterEnterGeofence(GeofenceEventCallback callback);

    /**
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     *
     * @param[in] <name of variable> <description of variable>
     *
     * @return <what does the function return (optional if void)>
     */
    int RegisterExitGeofence(GeofenceEventCallback callback);

private:
    Vector<ZoneInfo> GeofenceZones;
    Vector<GeofenceEventCallback> OutsideCallback;
    Vector<GeofenceEventCallback> InsideCallback;
    Vector<GeofenceEventCallback> EnterCallback;
    Vector<GeofenceEventCallback> ExitCallback;

    PointData _geofence_point;
};