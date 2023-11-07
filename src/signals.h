// brake pedal pressure = byte 3
#define S2000_BRAKE_PEDAL_RESSURE 0x106

// throttle pedal position = byte 1
#define S2000_THROTTLE_PEDAL_POSITION 0x170

// coolant temperature = byte 1
// ambient temperature = byte 2
// engine RPM = bytes 5/6
#define S2000_ENGINE_STATUS 0x300

// wheel speeds = bytes 6/5
#define S2000_WHEEL_SPEED 0x448

// steering wheel position = bytes 1/2
// lateral acceleration = byte 3
#define S2000_TELEMETRY_MESSGAGE_1 0x198

// engine RPM = bytes 0/1
// throttle position = bytes 4/5 (y = x / 10)
#define HALTECH_MESSAGE_360 0x360

// wheel speeds (two bytes per wheel - y = x/10)
#define HALTECH_MESSAGE_36C 0x36C

// coolant temperature = bytes 0-1 (y = x / 10)
// air temperature = bytes 2-3 (y = x / 10)
#define HALTECH_MESSAGE_3E0 0x3E0

// brakes
#define BRZ_BRAKES 209 
#define BRZ_ACCELERATOR_POSITION 320
#define BRZ_COOLANT_TEMPERATURE 864
#define BRZ_ENGINE 321

