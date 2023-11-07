#pragma once
#include <Arduino.h>
#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>
#include <SPI.h>
#include "signals.h"
#include "vehicle-state.h"

#define CAN_CHIP_SELECT 9

bool updateVehicleState();
void sendVehicleState();