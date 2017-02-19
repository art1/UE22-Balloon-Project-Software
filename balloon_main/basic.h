#ifndef basic_h
#define basic_h
#include "Arduino.h"
#include <math.h>
#include <Wire.h>
#include <SparkFunTSL2561.h>
#include <Adafruit_MCP9808.h>
#include <Adafruit_HTU21DF.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SFE_BMP180.h>
#include <MS5611.h>

#define DEBUG_OUTPUT
//#define IMU_ENABLED
//#define IMU_DEBUG_OUTPUT
#define SD_ENABLED
//#define DALLAS_ENABLED
#define LIGHT_ENABLED
//#define MCP_ENABLED
//#define HUMID_ENABLED
//#define BMP_ENABLED
//#define GPS_SYNC_ENABLED
//#define MS_ENABLED

#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi
#define GPS_SYNC_PIN 3
#define ONE_WIRE_BUS 2

struct raw_data{
  int gx;
  int gy;
  int gz;
  int ax;
  int ay;
  int az;
  float mx;
  float my;
  float mz;
};

struct filtered_data{
  float yaw;
  float pitch;
  float roll;
};

struct pressure_data{
  float T;
  float P;
  float alt;
  pressure_data(): T(-1.f), P(-1.f), alt(-1.f) {}
};

struct dataToSD{
  //IMU, processed and raw
  float yaw;
  float pitch;
  float roll;
  int gx, gy, gz, ax, ay, az;
  float mx, my, mz;
  //3x MCP
  float temp0,temp1,temp2;
  //2x TSL2561
  float lum0,lum1;
  //1x HTU21F
  float humid, temp3;
  //1x BMP180
  float press0, temp4, alt0;
  //1x MS5611
  float press1, temp5, alt1;

  dataToSD(): yaw(0),pitch(0),roll(0),gx(0),gy(0),gz(0),ax(0),ay(0),az(0),mx(0),my(0),mz(0),
              temp0(0),temp1(0),temp2(0),temp3(0),temp4(0),temp5(0),press0(0),press1(0),alt0(0),alt1(0),humid(0),lum0(0),lum1(0) {}

/* Functions that copy struct data to SD struct in order
 * to save it to the given file.
 */

  void filterDataToSDStruct(filtered_data f){
    yaw = ToDeg(f.yaw);
    pitch = ToDeg(f.pitch);
    roll = ToDeg(f.roll);
  }

  void rawDataToSDStruct(raw_data r){
    gx = r.gx; gy = r.gy; gz = r.gz;
    ax = r.ax; ay = r.ay; az = r.az;
    mx = r.mx; my = r.my; mz = r.mz;
  }

  //0 for BMP180, 1 for MS5611
  void pressureDataToSDStruct(pressure_data p,bool sensorType){
    if(!sensorType){
      press0 = p.P;
      temp4 = p.T;
      alt0 = p.alt;
    }else{
      press1 = p.P;
      temp5 = p.T;
      alt1 = p.alt;
    }
  }

  String toString(){
    short dec = 3; //digits after comma
    return String(millis()) + "," + String(yaw, dec) + "," + String(pitch, dec) + "," + String(roll, dec) + "," + String(gx) + "," +
    String(gy) + "," + String(gz) + "," + String(ax) + "," + String(ay) + "," + String(az) + "," + String(mx, dec) + "," +
    String(my, dec) + "," + String(mz, dec) + "," + String(temp0, dec) + "," + String(temp1, dec) + "," + String(temp2, dec) + "," +
    String(temp3, dec) + "," + String(temp4, dec) + "," + String(temp5, dec) + "," + String(press0, dec) + "," + String(press1, dec) + "," +
    String(alt0, dec) + "," + String(alt1, dec) + "," + String(humid, dec) + "," + String(lum0, dec) + "," + String(lum1, dec);
  }
};


#endif
