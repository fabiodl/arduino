#include "MegaController.h"
#include <Arduino.h>
typedef struct
{
  uint8_t player;
  uint8_t pin;
  uint16_t lowFlag;
  uint16_t highFlag;
  uint16_t pulse3Flag;
} input;
 


 
// Controller Button Flags

 
// Controller DB9 Pin 7 Mappings
const uint8_t SELECT[] = { 8, 9 };
 
input inputMap[] = {
  { 0,  0,  MegaController::UP,    MegaController::UP,     MegaController::Z    }, // P0 DB9 Pin 1
  { 0,  1,  MegaController::DOWN,  MegaController::DOWN,   MegaController::Y    }, // P0 DB9 Pin 2
  { 0,  3,  MegaController::ON,    MegaController::LEFT,   MegaController::X    }, // P0 DB9 Pin 3
  { 0,  5,  MegaController::ON,    MegaController::RIGHT,  MegaController::MODE }, // P0 DB9 Pin 4
  { 0,  6,  MegaController::A,     MegaController::B,      0                    }, // P0 DB9 Pin 6
  { 0,  7,  MegaController::START, MegaController::C,      0                    }, // P0 DB9 Pin 9
  { 1,  A0, MegaController::UP,    MegaController::UP,     MegaController::Z    }, // P1 DB9 Pin 1
  { 1,  A1, MegaController::DOWN,  MegaController::DOWN,   MegaController::Y    }, // P1 DB9 Pin 2
  { 1,  A2, MegaController::ON,    MegaController::LEFT,   MegaController::X    }, // P1 DB9 Pin 3
  { 1,  A3, MegaController::ON,    MegaController::RIGHT,  MegaController::MODE }, // P1 DB9 Pin 4
  { 1,  A4, MegaController::A,     MegaController::B,                           }, // P1 DB9 Pin 6
  { 1,  A5, MegaController::START, MegaController::C,                           }  // P1 DB9 Pin 9
};
 
 

MegaController::MegaController(){
  for (uint8_t i=0;i<PLAYERS;i++){
    sixButtonMode[i]=0;
    currentState[i]=0;
  }
}

void MegaController::setup(){
  // Setup input pins
  for (uint8_t i = 0; i < sizeof(inputMap) / sizeof(input); i++)
  {
    pinMode(inputMap[i].pin, INPUT);
    digitalWrite(inputMap[i].pin, HIGH);
  }
  
  // Setup select pins
  for (uint8_t i = 0; i < PLAYERS; i++)
  {
    pinMode(SELECT[i], OUTPUT);
    digitalWrite(SELECT[i], HIGH);
  }
  
}





void MegaController::resetState(uint8_t player)
{
  currentState[player] = 0;
}

uint16_t MegaController::readButtons(uint8_t player){
  resetState(player);
  if (sixButtonMode[player])
    {
      read6buttons(player);
    }
  else
    {
      read3buttons(player);
    }
  return currentState[player];
}

void MegaController::read3buttons(uint8_t player)
{
  // Set SELECT LOW and read lowFlag
  digitalWrite(SELECT[player], LOW);
     
  delayMicroseconds(20);
     
  for (int i = 0; i < sizeof(inputMap) / sizeof(input); i++)
  {
    if (inputMap[i].player == player && digitalRead(inputMap[i].pin) == LOW)
    {
      currentState[player] |= inputMap[i].lowFlag;
    }
  }
 
  // Set SELECT HIGH and read highFlag
  digitalWrite(SELECT[player], HIGH);
     
  delayMicroseconds(20);
     
  for (int i = 0; i < sizeof(inputMap) / sizeof(input); i++)
  {
    if (inputMap[i].player == player && digitalRead(inputMap[i].pin) == LOW)
    {
      currentState[player] |= inputMap[i].highFlag;
    }
  }
    
  // When a six-button first connects, it'll spam UP and DOWN,
  // which signals the game to switch to 6-button polling
  if (currentState[player] == (ON | UP | DOWN))
  {
    sixButtonMode[player] = true;
  }
  // When a controller disconnects, revert to three-button polling
  else if ((currentState[player] & ON) == 0)
  {
    sixButtonMode[player] = false;
  }
  
  delayMicroseconds(20);
}


void MegaController::read6buttons(uint8_t player)
{
  // Poll for three-button states twice
  read3buttons(player);
  read3buttons(player);
  
  // After two three-button polls, pulse the SELECT line
  // so the six-button reports the higher button states
  digitalWrite(SELECT[player], LOW);
  delayMicroseconds(20);
  digitalWrite(SELECT[player], HIGH);
  
  for(uint8_t i = 0; i < sizeof(inputMap) / sizeof(input); i++)
  {
    if (inputMap[i].player == player && digitalRead(inputMap[i].pin) == LOW)
    {
      currentState[player] |= inputMap[i].pulse3Flag;
    }
  }
  
  delayMicroseconds(1000);
}
