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

#define DEBUG_OUTPUT
//#define IMU_ENABLED
//#define IMU_DEBUG_OUTPUT
//#define SD_ENABLED
//#define DALLAS_ENABLED
#define LIGHT_ENABLED
#define MCP_ENABLED
#define HUMID_ENABLED
#define BMP_ENABLED
//#define GPS_SYNC_ENABLED

#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi
#define GPS_SYNC_PIN A1
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

struct dataToSD{
  float yaw;
  float pitch;
  float roll;
  int gx, gy, gz, ax, ay, az;
  float mx, my, mz;
  float temp0,temp1,temp2,temp3,temp4,temp5;
  float pressure;
  float humid;
  float lum0,lum1,lum2;

  dataToSD(): yaw(0),pitch(0),roll(0),gx(0),gy(0),gz(0),ax(0),ay(0),az(0),mx(0),my(0),mz(0),
              temp0(0),temp1(0),temp2(0),temp3(0),temp4(0),temp5(0),pressure(0),humid(0),lum0(0),lum1(0),lum2(0) {}

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

  String toString(){
    short dec = 3;
    return "" + String(yaw, dec) + "," + String(pitch, dec) + "," + String(roll, dec) + "," + String(gx) + "," +
    String(gy) + "," + String(gz) + "," + String(ax) + "," + String(ay) + "," + String(az) + "," + String(mx, dec) + "," +
    String(my, dec) + "," + String(mz, dec) + "," + String(temp0, dec)
    + "," + String(temp1, dec) + "," + String(temp2, dec) + "," + String(temp3, dec) + "," + String(temp4, dec)
    + "," + String(pressure, dec) + "," + String(humid, dec) + "," + String(lum0, dec) + "," + String(lum1, dec)
    + "," + String(lum2, dec);
  }
};

struct BMP180_data{
  float T;
  float P;
  float alt;
  BMP180_data(): T(-1.f), P(-1.f), alt(-1.f) {}
};
#endif
