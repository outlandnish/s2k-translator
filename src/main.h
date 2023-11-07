#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <MCP2515.h>
#include "signals.h"
#include "vehicle-state.h"

#define CAN_CHIP_SELECT 9
#define CAN_INTERRUPT 8

bool updateVehicleState();
void sendVehicleState();