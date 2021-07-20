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

#include "Particle.h"

/**
 * @brief Type definition of geofence event callback signature.
 *
 */
using GeofenceEventCallback = std::function<void(void)>;

Vector<GeofenceEventCallback> OutsideCallback;
Vector<GeofenceEventCallback> InsideCallback;
Vector<GeofenceEventCallback> EnterCallback;
Vector<GeofenceEventCallback> ExitCallback;

class Geofence {
public:

    enum class EventType{
        INSIDE,
        OUTSIDE,
        ENTER,
        EXIT,
    };

    typedef struct {
        time_t epochTime;               /**< Epoch time from device sources */
        float latitude;                 /**< Point latitude in degrees */
        float longitude;                /**< Point longitude in degrees */
    } point_data_t;

    typedef struct {
        float radius; //radius in meters that define the geofence zone boundary
        float center_lat;                 /**< Center point latitude in degrees */
        float center_lon;                /**< Center point longitude in degrees */
        uint8_t confidence_number; //a number to reduce false positives
        bool enable; //enable or disable the geofence zone
        EventType type; //type of event to trigger
    } zone_info_t;

    Geofence& instance() {
        static Geofence instance;
        return instance;
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
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     *
     * @param[in,out] <name of variable> <description of variable>
     */
    void SetZoneInfo(zone_info_t zone_config);

    /**
     * @brief Pass the point data to be used to calculate boundary
     *
     * @details This function is called to pass point information that is
     * used in the tick() function to calculate geofence bounds
     *
     * @param[in] point_data_t point to be passed for calculation
     */
    void geofence(point_data_t& point);

    /**
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     *
     * @param[in] <name of variable> <description of variable>
     *
     * @return <what does the function return (optional if void)>
     */
    int registerOutsideGeofence(GeofenceEventCallback callback);

    /**
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     *
     * @param[in] <name of variable> <description of variable>
     *
     * @return <what does the function return (optional if void)>
     */
    int registerInsideGeofence(GeofenceEventCallback callback);

    /**
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     *
     * @param[in] <name of variable> <description of variable>
     *
     * @return <what does the function return (optional if void)>
     */
    int registerEnterGeofence(GeofenceEventCallback callback);

    /**
     * @brief <enter a brief one sentence description>
     *
     * @details <details of the function>
     *
     * @param[in] <name of variable> <description of variable>
     *
     * @return <what does the function return (optional if void)>
     */
    int registerExitGeofence(GeofenceEventCallback callback);

private:
    Geofence() {}

    //remove copy and assignment operators
    Geofence(Geofence const&)         = delete;
    void operator=(Geofence const&)   = delete;
};