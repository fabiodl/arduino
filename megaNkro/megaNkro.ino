#include <VusbKeyboard.h>
#include <usbHidConstants.h>


uint8_t* keyBuffer=UsbKeyboard.getKeyBuffer();
int cnt;

void fillKeys(int flags){
  UsbKeyboard.clearKeys();
  if (flags&0x0001){
    keyBuffer[0]=(1<<4);
  }
}


void setup() {
 // Disable timer0 since it can mess with the USB timing. Note that
  // this means some functions such as delay() will no longer work.
  

}

void loop() {
  // put your main code here, to run repeatedly:
  UsbKeyboard.loop();
  int flags=(cnt/1000)%2;
  fillKeys(flags);
  UsbKeyboard.sendKeys();
  cnt++;
}
