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
#include <atomic>

//forward declaration of struct and enum class
struct CallbackContext;
enum class GeofenceEventType;

/**
 * @brief Type definition of geofence event callback signature.
 *
 */
using GeofenceEventCallback = 
        std::function<void(CallbackContext& context)>;

enum class GeofenceEventType {
    UNKNOWN,
    INSIDE,
    OUTSIDE,
    ENTER,
    EXIT,
};

struct PointData {
    double lat; /**< Point latitude in degrees */
    double lon; /**< Point longitude in degrees */
    double horizontal_accuracy; /**< error value */
    double hdop; /**<horizontal dilution of prescion */
    time_t gps_time;
    bool operator!=(const PointData& other) const {
        if((lat != other.lat) && (lon != other.lon)) {return true;}
        else {return false;}
    }
};

struct ZoneInfo {
    double radius{0.0}; //radius in meters that define the geofence zone boundary
    double center_lat{0.0};                 /**< Center point latitude in degrees */
    double center_lon{0.0};                /**< Center point longitude in degrees */
    uint8_t confidence_number{0}; //a number to reduce false positives
    bool enable{false}; //enable or disable the geofence zone
    GeofenceEventType event_type{GeofenceEventType::UNKNOWN}; //type of event to trigger
};

struct CallbackContext {
    int index; //index of zone (+1 to get the actual zone number)
    GeofenceEventType event_type; //type of event that caused callback
};

struct GeofenceZoneState {
    GeofenceEventType prev_event{GeofenceEventType::UNKNOWN};
};

class Geofence {
public:

    Geofence(int num_of_zones) : GeofenceZones(num_of_zones), 
            GeofenceZoneStates(num_of_zones) {
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
    void SetZoneInfo(int index, const ZoneInfo& zone_config) {
        auto zone = GeofenceZones.at(index);
        zone = zone_config;
    }

    /**
     * @brief Gets the zone info for a given index
     *
     * @details Number of zones are created by the Geofence ctor, and this 
     * function returns references to the zone info
     *
     * @param[in] index index of vector to get the zone info
     * 
     * @return reference to requested zone info
     */
    ZoneInfo& GetZoneInfo(int index) {
        return GeofenceZones.at(index);
    }

    /**
     * @brief Pass the point data to be used to calculate boundary. Only writes
     * if there is a change to the stored _geofence_point
     *
     * @details This function is called to pass point information that is
     * used in the tick() function to calculate geofence bounds
     *
     * @param[in] PointData point to be passed for calculation
     */
    void UpdateGeofencePoint(const PointData& point) {
        if(_geofence_point != point) {
            _geofence_point = point;
            new_point.test_and_set();
        }
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
    int RegisterGeofenceCallback(GeofenceEventCallback callback);

private:
    /**
     * \brief           Calculate distance and bearing between `2` latitude and longitude coordinates
     * \param[in]       las: Latitude start coordinate, in units of degrees
     * \param[in]       los: Longitude start coordinate, in units of degrees
     * \param[in]       lae: Latitude end coordinate, in units of degrees
     * \param[in]       loe: Longitude end coordinate, in units of degrees
     * \param[out]      d: reference to output distance in units of meters
     * \return          `1` on success, `0` otherwise
     */
    inline void GpsDistance(double las, 
                            double los, 
                            double lae, 
                            double loe, 
                            double& d);
    /**
     * @brief Convert value from degrees to radians
     *
     * @details converts values from degrees to radians using a constant
     *
     * @param[in] x value in degrees
     *
     * @return value in radians
     */
    inline double D2R(double x) {return ((x) * (0.01745329251994));}

    Vector<ZoneInfo> GeofenceZones;
    Vector<GeofenceZoneState> GeofenceZoneStates;
    Vector<GeofenceEventCallback> EventCallback;

    PointData _geofence_point;
    std::atomic_flag new_point = ATOMIC_FLAG_INIT;
};