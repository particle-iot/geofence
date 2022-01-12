/*
 * Copyright (c) 2022 Particle Industries, Inc.
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

constexpr PointData TestPoints[] = {
    { 37.76985894, -122.46612905, 0.0, 0.0, 0 },    // California Academy of Sciences
};

void setup() {
    Geofence test(2);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.SetZoneInfo(1, PoloField);
}

unsigned int loopSeconds = 0;

void loop() {
    if ((System.uptime() - loopSeconds) >= 10) {
    }
}
