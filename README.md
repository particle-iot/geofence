# geofence
Particle library to calculate geofence events asynchronously with callbacks. 

## Usage 

See [usage.cpp](/examples/usage/usage.cpp) for a basic example. In summary:

1. instantiate geofence object
1. setup configuration settings for each geofence point
1. update it's known current coordinates periodically in `loop()`.

Register callbacks to do something meaningful when inside, outside, enter, or exit events occur.
See [tracker-edge](https://github.com/particle-iot/tracker-edge) application where this library is used for real world example usage.

## Develop

This library has a unit test suite that can be run on a standard Linux or OS X computer using `gcc`
The following describes how to build and execute this test suite:

1. Install `gcc` that supports C++ 14 or higher and `cmake`.
1. `git clone` repo and `cd geofence`
1. mkdir `build`
1. `cd build`
1. `cmake ..`
1. `make`; build application code and test suite
1. `./geofence-test`; run test suite

Expect to see the tests pass.

### LICENSE

Unless stated elsewhere, file headers or otherwise, all files herein are licensed under an Apache License, Version 2.0. For more information, please read the LICENSE file.

If you have questions about software licensing, please contact Particle [support](https://support.particle.io/).

---

### LICENSE FAQ

**This firmware is released under Apache License, Version 2.0, what does that mean for you?**

 * You may use this commercially to build applications for your devices!  You **DO NOT** need to distribute your object files or the source code of your application under Apache License.  Your source can be released under a non-Apache license.  Your source code belongs to you when you build an application using this library.

**When am I required to share my code?**

 * You are **NOT required** to share your application firmware binaries, source, or object files when linking against libraries or System Firmware licensed under LGPL.

**Why?**

 * This license allows businesses to confidently build firmware and make devices without risk to their intellectual property, while at the same time helping the community benefit from non-proprietary contributions to the shared reference firmware.

**Questions / Concerns?**

 * Particle intends for this firmware to be commercially useful and safe for our community of makers and enterprises.  Please [Contact Us](https://support.particle.io/) if you have any questions or concerns, or if you require special licensing.

_(Note!  This FAQ isn't meant to be legal advice, if you're unsure, please consult an attorney)_

---

### CONNECT

Having problems or have awesome suggestions? Connect with us [here.](https://community.particle.io/)

---

### Revision History

#### 1.0.0
* Initial version
