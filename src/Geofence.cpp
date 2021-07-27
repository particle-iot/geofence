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

#include "Geofence.h"
#include <math.h>


constexpr double EARTH_RADIUS = 6371.0; /*!< Earth radius in units of kilometers */

void Geofence::init() {
    //clear out the previous geofence zone boundary states
    for(auto&& iter : GeofenceZoneStates) {
        iter.prev_event = GeofenceEventType::UNKNOWN;
    }
}

void Geofence::loop() {
    if(new_point.test_and_set()) {
        auto zone_index = 0;
        for(auto zone : GeofenceZones) {
            if(zone.enable) {
                double distance;
                auto zone_state = GeofenceZoneStates.at(zone_index);
                CallbackContext context;
                GpsDistance(zone.center_lat, 
                            zone.center_lon, 
                            _geofence_point.lat, 
                            _geofence_point.lon, 
                            distance);
                //distance is outside geofence
                if(distance > zone.radius) {
                    if(zone.event_type == GeofenceEventType::OUTSIDE) {
                        context.event_type = GeofenceEventType::OUTSIDE;
                        context.index = zone_index;
                        for(auto callback : EventCallback) {
                            callback(context);
                        }
                    }
                    else if(zone.event_type == GeofenceEventType::EXIT) {
                        if(zone_state.prev_event == GeofenceEventType::INSIDE) {
                            context.event_type = GeofenceEventType::EXIT;
                            context.index = zone_index;
                            for(auto callback : EventCallback) {
                                callback(context);
                            }
                        }
                    }
                }
                //distance is inside geofence
                else {
                    if(zone.event_type == GeofenceEventType::INSIDE) {
                        context.event_type = GeofenceEventType::INSIDE;
                        context.index = zone_index;
                        for(auto callback : EventCallback) {
                            callback(context);
                        }
                    }
                    else if(zone.event_type == GeofenceEventType::ENTER) {
                        if(zone_state.prev_event == 
                            GeofenceEventType::OUTSIDE) {
                            context.index = zone_index;
                            context.event_type = GeofenceEventType::ENTER;
                            for(auto callback : EventCallback) {
                                callback(context);
                            }
                        }
                    }
                }
                //Store the most recent executed event type for that zone
                zone_state.prev_event = context.event_type;
            }
            zone_index++;
        }
        new_point.clear();
    }
}

int Geofence::RegisterGeofenceCallback(GeofenceEventCallback callback) {
    EventCallback.append(callback);
    return SYSTEM_ERROR_NONE;
}

void Geofence::GpsDistance(double las, 
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
    a = (double)(sin(df * 0.5) * sin(df * 0.5) + sin(dfi * 0.5) * sin(dfi * 0.5) * cos(las) * cos(lae));
    d = (double)(EARTH_RADIUS * 2.0 * atan2(sqrt(a), sqrt(1.0 - a)) * 1000.0);
}







