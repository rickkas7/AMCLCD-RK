#include "AMCLCD-RK.h"

SYSTEM_THREAD(ENABLED);
  // SYSTEM_MODE(MANUAL);

SerialLogHandler logHandler;

AMCLCD_Model_AMC1602 lcdModel;
AMCLCD lcd(lcdModel, 0x3c, Wire);


void setup() {
    waitFor(Serial.isConnected, 15000);
    delay(1000);
    
	lcd.begin();

	lcd.println("HELLO WORLD!");
	lcd.printlnf("testing %d", 123);

	Log.info("setup complete");
}

void loop() {
}
