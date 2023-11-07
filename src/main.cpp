#include "main.h"

// #define CAN_IN_CS 14
// #define CAN_IN_INT 23

// #define CAN_OUT_CS 13
// #define CAN_OUT_INT 24

// MCP2515Class can_in;
// MCP2515Class can_out;

VehicleState state;

unsigned long lastUpdate = 0;
uint16_t REFRESH_PERIOD = 10;

void setup() {
  pinMode(CAN_CHIP_SELECT, OUTPUT);
  // can_in.setPins(CAN_IN_CS, CAN_IN_INT);
  // can_out.setPins(CAN_OUT_CS, CAN_OUT_INT);
  Serial.begin(115200);

  // start the CAN bus at 500 kbps
  // if (!can_in.begin(500E3)) {
  //   while (1) {
  //     delay(1000);
  //     Serial.println("Starting CAN input failed!");
  //   }
  // }

  // if (!can_out.begin(500E3)) {
  //   Serial.println("Starting CAN output failed");
  // }

  if(Canbus.init(CANSPEED_500))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");
}

// Message *message = NULL;
tCAN message;

// void loop() {
//   if (mcp2515_check_message()) 
//   {
//     if (mcp2515_get_message(&message)) 
//     {                
//       Serial.print("ID: ");
//       Serial.print(message.id,HEX);
//       Serial.print(", ");
//       Serial.print("Data: ");
//       Serial.print(message.header.length,DEC);
//       for(int i=0;i<message.header.length;i++) 
//       { 
//         Serial.print(message.data[i],HEX);
//         Serial.print(" ");
//       }
//       Serial.println("");
//     }
//   }
// }

void loop() {
  // try to parse packet
  if (mcp2515_check_message()) {
    if (mcp2515_get_message(&message)) {
      updateVehicleState();
    }
  }

  auto now = millis();
  if (now - lastUpdate >= REFRESH_PERIOD) {
    // send out updates;
    sendVehicleState();
    lastUpdate = now;
  }
}

// void buildPacket(Message *message) {
//   // received a packet
//   message->id = can_in.packetId();
//   message->extended = can_in.packetExtended();
//   message->rtr = can_in.packetRtr();

//   uint8_t index = 0;
//   while (can_in.available()) {
//     message->data[index] = can_in.read();
//     index++;
//   }
// }

bool updateVehicleState() {
  bool updated = false;
  uint8_t *data = message.data;
  // switch (message.id) {
  //   case HALTECH_MESSAGE_360:
  //     state.rpm = data[0];
  //     state.throttlePosition = data[4];
  //     updated = true;
  //     Serial.printf("RPM: %d, Throttle Position: %d\n", state.rpm, state.throttlePosition);
  //     break;
  //   case HALTECH_MESSAGE_36C:
  //     Serial.println("Wheel speed message");
  //     break;
  //   case HALTECH_MESSAGE_3E0:
  //     state.coolantTemperature = data[0];
  //     state.ambientTemperature = data[2];
  //     updated = true;
  //     Serial.printf("Temps - Coolant: %.2f: Ambient: %.2f", state.coolantTemperature / 10.0, state.ambientTemperature / 10.0);
  //     break;
  //   default:
  //     Serial.printf("Ignoring message: %d\n", message.id);
  //     break;
  // }

  switch (message.id) {
    case BRZ_BRAKES:
      state.brakePosition = data[2];
      state.brakePressure = data[2] * 128;
      // Serial.printf("brake pedal: %d\n", state.brakePosition);
      break;
    case BRZ_COOLANT_TEMPERATURE:
      state.coolantTemperature = data[3] - 40;
      // Serial.printf("coolant temp: %d\n", state.coolantTemperature);
      break;
    case BRZ_ENGINE:
      state.rpm = ((data[5] - 0x80) * 256) + data[4];
      // Serial.printf("rpm: %d\n", state.rpm);
      break;
    // default:
    //   Serial.printf("[%d] - unknown message\n", message.id);
  }

  return updated;
}

void sendVehicleState() {
  tCAN send;

  send.id = S2000_ENGINE_STATUS;
  send.header.length = 7;
  send.data[1] = state.coolantTemperature;
  send.data[5] = state.rpm;
  mcp2515_send_message(&send);
}