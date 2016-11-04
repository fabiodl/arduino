/*
 * Based on https://github.com/gloob/vusb-for-arduino
 *
 */
#ifndef __VusbKeyboard_h__
#define __VusbKeyboard_h__
#include <stdint.h>

#define REPORT_KEYS_BYTES 12
#define REPORT_FIRST_KEY 0

class VusbKeyboardDevice {
 public:
  VusbKeyboardDevice ();

  void loop();
  uint8_t* getModifierBuffer();
  uint8_t* getKeyBuffer();
  void clearKeys();
  void sendKeys();
private:
  uint8_t    reportBuffer[1+REPORT_KEYS_BYTES];    // buffer for HID reports [ 1 modifier byte + (len-1) key strokes]

};

extern VusbKeyboardDevice UsbKeyboard;


#endif // __UsbKeyboard_h__
