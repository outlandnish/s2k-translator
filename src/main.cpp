#include "main.h"

MCP2515 can(CAN_CHIP_SELECT, CAN_INTERRUPT);

CAN_FRAME message;
CAN_FRAME output;

VehicleState state;

unsigned long lastUpdate = 0;
uint16_t REFRESH_PERIOD = 10;

void handleInterrupt() {
  can.intHandler();
}

void setup() {
  // pinMode(CAN_CHIP_SELECT, OUTPUT);
  Serial.begin(115200);

  // start the CAN bus at 500 kbps
  if (!can.begin(500)) {
    while (1) {
      delay(1000);
      Serial.println("Starting CAN input failed!");
    }
  }

  // if (!can_out.begin(500E3)) {
  //   Serial.println("Starting CAN output failed");
  // }

  attachInterrupt(CAN_INTERRUPT, handleInterrupt, FALLING);
  can.InitFilters(true);
}

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
  if (can.GetRXFrame(message))
    updateVehicleState();

  // auto now = millis();
  // if (now - lastUpdate >= REFRESH_PERIOD) {
  //   // send out updates;
  //   sendVehicleState();
  //   lastUpdate = now;
  // }
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
  uint8_t *data = message.data.bytes;
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
    default:
      Serial.printf("[%d] - unknown message\n", message.id);
  }

  return updated;
}

void sendVehicleState() {
  output.id = S2000_ENGINE_STATUS;
  output.length = 8;
  output.extended = true;
  output.data.byte[0] = state.coolantTemperature + 40;
  output.data.byte[1] = state.ambientTemperature + 40;
  output.data.byte[4] = state.rpm;
  can.sendFrame(output);
}