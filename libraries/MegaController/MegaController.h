/* Derived from
 *
 * Sega Controller Reader (Keyboard)
 * Author: Jon Thysell <thysell@gmail.com>
 * Version: 1.1
 * Date: 9/29/2014
 *
 * Reads buttons presses from Sega Genesis 3/6 button controllers
 * and reports their state via keyboard button presses. Handles hot
 * swapping of controllers and auto-switches between 3 and 6 button
 * polling patterns.
 *
 */
 

// Controller DB9 Pin to Button Flag Mappings
// First column is the controller index, second column
// is the Arduino pin that the controller's DB9 pin is
// attached to


 
// Default to three-button mode until six-button connects
#include <stdint.h>

#define  PLAYERS  2

class MegaController{
public:
  enum Buttons{ON = 1<<0,
               UP = 1<<1,
               DOWN = 1<<2,
               LEFT = 1<<3,
               RIGHT = 1<<4,
               START = 1<<5,
               A = 1<<6,
               B = 1<<7,
               C = 1<<8,
               X = 1<<9,
               Y = 1<<10,
               Z = 1<<11,
               MODE = 1<<12};
  void setup();
  int16_t getState();
  MegaController();
private:
  
  void readButtons();
  void resetState(int player);
  void read3buttons(int player);
  void read6buttons(int player);

  bool sixButtonMode[2] = { false, false };
  int currentState[2];
};
 

 


  
