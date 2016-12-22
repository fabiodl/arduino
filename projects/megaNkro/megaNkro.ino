#include <MegaController.h>

#include <VusbKeyboard.h>
#include <usbHidConstants.h>




uint8_t* keyBuffer=UsbKeyboard.getKeyBuffer();
uint16_t cnt;
MegaController controller;


uint8_t keys[MegaController::BTNCOUNT]={
  KEY_NONE, //ON 0
  KEY_W,//UP 1
  KEY_S,//DOWN 2
  KEY_A,//LEFT 3
  KEY_D,//RIGHT 4
  KEY_RETURN,//START 5
  KEY_E,//A  6
  KEY_J,//B 7
  KEY_K,//C 8
  KEY_NONE,//X 9
  KEY_NONE,//Y 10
  KEY_NONE,//Z 11
  KEY_NONE//MODE 12
};


void fillKeys(int flags){
  UsbKeyboard.clearKeys();
  if (flags&0x0001){
    keyBuffer[0]=(1<<4);
  }
}


void keysFromController(){
  UsbKeyboard.clearKeys();
  uint16_t js=controller.readButtons(0);
  
  for (uint8_t i=0;i<MegaController::BTNCOUNT;i++){
    uint8_t keyflag=keys[i];
    if (keyflag!=KEY_NONE && (js&(1<<i))){      
      keyBuffer[keyflag>>3] |=(1<<(keyflag&0b111));
    }
  }//for

}





void setup() {
 // Disable timer0 since it can mess with the USB timing. Note that
  // this means some functions such as delay() will no longer work.
  controller.setup();

}

void loop() {
  // put your main code here, to run repeatedly:
  UsbKeyboard.loop();
  int flags=(cnt/1000)%2;
  fillKeys(flags);
  UsbKeyboard.sendKeys();
  cnt++;
}
