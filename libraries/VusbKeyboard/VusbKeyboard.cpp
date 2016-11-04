#include <string.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "VusbKeyboard.h"


#ifdef __cplusplus
extern "C"
{
#endif

#include "utility/usbdrv.h"

#ifdef __cplusplus
}
#endif



VusbKeyboardDevice UsbKeyboard;



PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH]= { /* USB report descriptor */
  0x05, 0x01,                           // Usage Page (Generic Desktop),
  0x09, 0x06,                           // Usage (Keyboard),
  0xA1, 0x01,                           // Collection (Application),
  // bitmap of modifiers
  0x75, 0x01,                           //   Report Size (1),
  0x95, 0x08,                           //   Report Count (8),
  0x05, 0x07,                           //   Usage Page (Key Codes),
  0x19, 0xE0,                           //   Usage Minimum (224),
  0x29, 0xE7,                           //   Usage Maximum (231),
  0x15, 0x00,                           //   Logical Minimum (0),
  0x25, 0x01,                           //   Logical Maximum (1),
  0x81, 0x02,                           //   Input (Data, Variable, Absolute), ;Modifier byte
  // LED output report
  /*  0x95, 0x05,                           //   Report Count (5),
  0x75, 0x01,                           //   Report Size (1),
  0x05, 0x08,                           //   Usage Page (LEDs),
  0x19, 0x01,                           //   Usage Minimum (1),
  0x29, 0x05,                           //   Usage Maximum (5),
  0x91, 0x02,                           //   Output (Data, Variable, Absolute),
  0x95, 0x01,                           //   Report Count (1),
  0x75, 0x03,                           //   Report Size (3),
  0x91, 0x03,                           //   Output (Constant),*/
  // bitmap of keys
  0x95, REPORT_KEYS_BYTES * 8,          //   Report Count (),
  0x75, 0x01,                           //   Report Size (1),
  0x15, 0x00,                           //   Logical Minimum (0),
  0x25, 0x01,                           //   Logical Maximum(1),
  0x05, 0x07,                           //   Usage Page (Key Codes),
  0x19, REPORT_FIRST_KEY,               //   Usage Minimum (0),
  0x29, REPORT_KEYS_BYTES * 8 - 1,      //   Usage Maximum (),
  0x81, 0x02,                           //   Input (Data, Variable, Absolute),
  0xc0                                  // End Collection
};

VusbKeyboardDevice:: VusbKeyboardDevice () {
  // Disable timer0 since it can mess with the USB timing. Note that
  // this means some functions such as delay() will no longer work.
  TIMSK0 &= !(1<TOIE0);
  cli();
  usbDeviceDisconnect();
  _delay_ms(250);
  usbDeviceConnect();
  usbInit();
  sei();
  clearKeys();
  usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
}


void VusbKeyboardDevice::clearKeys(){
  memset(reportBuffer, 0, sizeof(reportBuffer));
}

void VusbKeyboardDevice::loop() {
    usbPoll();
} 


void VusbKeyboardDevice::sendKeys(){
  while (!usbInterruptIsReady()) {
    usbPoll();
  }
  usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
}

uchar* VusbKeyboardDevice::getModifierBuffer(){
  return reportBuffer;
}

uchar* VusbKeyboardDevice::getKeyBuffer(){
  return reportBuffer+1;
}

uint8_t* reportBuffer= UsbKeyboard.getModifierBuffer();

usbMsgLen_t usbFunctionSetup(uchar data[8])
  {
    usbRequest_t    *rq = (usbRequest_t *)((void *)data);

    usbMsgPtr = reportBuffer; //
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){
      /* class request type */

      if(rq->bRequest == USBRQ_HID_GET_REPORT){
	/* wValue: ReportType (highbyte), ReportID (lowbyte) */

	/* we only have one report type, so don't look at wValue */
        // TODO: Ensure it's okay not to return anything here?
	return 0;

      }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
	//            usbMsgPtr = &idleRate;
	//            return 1;
	return 0;
      }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
	//idleRate = rq->wValue.bytes[1];         // in 4 ms units
      }
    }else{
      /* no vendor specific requests implemented */
    }
    return 0;
  }
