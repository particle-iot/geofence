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

#include "geofence.h"
#include "system_error.h"
#include <math.h>

#define D2R(x) ((x) * (0.01745329251994)) /*!< Degrees to radians */
#define EARTH_RADIUS (6371.0) /*!< Earth radius in units of kilometers */

static void gps_distance(double las, 
                            double los, 
                            double lae, 
                            double loe, 
                            double& d);

void Geofence::init() {

}

void Geofence::loop() {
    for(auto zone : GeofenceZones) {
        if(zone.enable) {
            double distance;
            gps_distance(zone.center_lat, 
                        zone.center_lon, 
                        _geofence_point.lat, 
                        _geofence_point.lon, 
                        distance);
            if(distance > zone.radius) {
                if(zone.event_type == GeofenceEventType::OUTSIDE) {
                    for(auto callback : OutsideCallback) {
                        callback();
                    }
                }
                else if(zone.event_type == GeofenceEventType::INSIDE) {
                    for(auto callback : InsideCallback) {
                        callback();
                    }
                }
                else if(zone.event_type == GeofenceEventType::ENTER) {
                    for(auto callback : EnterCallback) {
                        callback();
                    }
                }
                else if(zone.event_type == GeofenceEventType::EXIT) {
                    for(auto callback : ExitCallback) {
                        callback();
                    }
                }
            }
        }
    }
}

int Geofence::RegisterOutsideGeofence(GeofenceEventCallback callback) {
    OutsideCallback.append(callback);
    return SYSTEM_ERROR_NONE;
}

int Geofence::RegisterInsideGeofence(GeofenceEventCallback callback) {
    InsideCallback.append(callback);
    return SYSTEM_ERROR_NONE;
}

int Geofence::RegisterEnterGeofence(GeofenceEventCallback callback) {
    EnterCallback.append(callback);
    return SYSTEM_ERROR_NONE;
}

int Geofence:: RegisterExitGeofence(GeofenceEventCallback callback) {
    ExitCallback.append(callback);
    return SYSTEM_ERROR_NONE;
}

/**
 * \brief           Calculate distance and bearing between `2` latitude and longitude coordinates
 * \param[in]       las: Latitude start coordinate, in units of degrees
 * \param[in]       los: Longitude start coordinate, in units of degrees
 * \param[in]       lae: Latitude end coordinate, in units of degrees
 * \param[in]       loe: Longitude end coordinate, in units of degrees
 * \param[out]      d: reference to output distance in units of meters
 * \return          `1` on success, `0` otherwise
 */
static void gps_distance(double las, 
                            double los, 
                            double lae, 
                            double loe, 
                            double& d) {
    double df, dfi, a;

    /* Convert degrees to radians */
    df = D2R(lae - las);
    dfi = D2R(loe - los);
    las = D2R(las);
    los = D2R(los);
    lae = D2R(lae);
    loe = D2R(loe);

    /*
    * Calculate distance
    *
    * Calculated distance is absolute value in meters between 2 points on earth.
    *
    *
    * a = sin(df / 2)^2 + cos(las) * cos(lae) * sin(dfi / 2)^2
    * d = RADIUS * 2 * atan(a / (1 - a)) * 1000 (for meters)
    */
    a = (sin(df * 0.5) * sin(df * 0.5) + sin(dfi * 0.5) * sin(dfi * 0.5) * cos(las) * cos(lae));
    d = (EARTH_RADIUS * 2.0 * atan2(sqrt(a), sqrt(1.0 - a)) * 1000.0);
}







