#include <stdint.h>

struct WheelSpeeds {
  uint16_t frontLeft;
  uint16_t frontRight;
  uint16_t rearLeft;
  uint16_t rearRight;
};

struct VehicleState {
  uint16_t rpm;
  uint16_t speed;
  float throttlePosition;
  int coolantTemperature;
  int ambientTemperature;
  WheelSpeeds wheelSpeeds;
  float steeringWheelPosition;
  uint8_t brakePosition;
  uint16_t brakePressure;
};

struct Message {
  uint16_t id;
  uint8_t dlc;  
  uint8_t* data;
  bool extended;
  bool rtr;

  Message(uint8_t length) : data(new uint8_t[length]) {
    dlc = length;
  }

  ~Message () {
    delete data;
  }
};