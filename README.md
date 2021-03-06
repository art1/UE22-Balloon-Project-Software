# UE22 - Balloon Project Software
This repo contains the code for a balloon project at the University Toulouse III - Paul Sabatier for the course M2-TSI. The purpose of this project is to demonstrate the feasibility of a balloon project based on Arduino Hardware within the scope of the course UE22. The balloon will measure and record different atmospheric parameters by using various sensors as, pressure, humidity, temperature. Also, an 9-DOF IMU, a GPS receiver and an appropriate AHRS will be implemented.

Enabling and disabling different components and sensors can be done in the "basic.h"

Arduino IDE version: 1.8.1/1.8.0

SD version: 1.1.1

LSM303 version: 3.0.1

L3G version: 3.0.0

Other libraries used, can be found in sensortest:
Adafruit_MCP9808, OneWire, SparkFun_TSL2561

Balloon-main will be running on Arduino Mega2650

---

GPS Logging is done on an Arduino FLORA, based on the [Adafruit GPS Library](https://github.com/adafruit/Adafruit_GPS).
