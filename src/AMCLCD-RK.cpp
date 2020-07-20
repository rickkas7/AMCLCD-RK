  

#include "AMCLCD-RK.h"



AMCLCD::AMCLCD(AMCLCD_Model &model, uint8_t addr, TwoWire &wire) : model(model), addr(addr), wire(wire) {
	if (addr < 0x4) {
		// Just passed in 0 - 3, add in the 0x3c to make addresses 0x3C - 0x3F
		addr |= 0x3C;
	}
}

AMCLCD::~AMCLCD() {

}


bool AMCLCD::begin() {
	// Initialize the I2C bus in standard master mode.
	wire.begin();

    writeInst(INST_FUNCTION_SET | model.getConfig());
    delayMicroseconds(100);

    writeInst(INST_DISPLAY_ON_OFF | BIT_DISPLAY_ON);
    delayMicroseconds(100);

    writeInst(INST_CLEAR_DISPLAY);
    delay(10);

    writeInst(INST_ENTRY_MODE_SET | BIT_CURSOR_INCREMENT);
    delayMicroseconds(100);

	return true;
}

size_t AMCLCD::write(uint8_t b) {
    // Note: println outputs CR LF to go to the next line
    if (b == '\r') {
        // CR
        Log.info("cr");
        col = 0;
        return 1;
    }
    else
    if (b == '\n') {
        // LF
        Log.info("lf");
        row++;
        return 1;
    }

    if (wrap) {
        if (col >= model.getCols()) {
            col = 0;
            row++;
        }
    }

    if (col < model.getCols() && row < model.getRows()) {
        uint8_t addr = (uint8_t) (model.getRowAddress(row) + col);

        writeInst(INST_SET_DDRAM_ADDR | addr);
        writeData(b);

        // Log.info("writing %c col=%d row=%d addr=%02x", b, col, row, addr);

        col++;
    }
    return 1;
}

void AMCLCD::clearDisplay() {
    writeInst(INST_SET_DDRAM_ADDR);
    writeData(' ');
 
    writeInst(INST_CLEAR_DISPLAY);
    delay(10);
}



bool AMCLCD::writeInst(uint8_t value) {
    // controlByte = 0x00:
    //     C0 = 0 (mask 0x80) = Last Control Byte, remainder is data
    //     A0 = 0 (mask 0x40) = Byte is instruction data
    return writeDevice(0x00, value);
}

bool AMCLCD::writeData(uint8_t data) {
    // controlByte = 0x40:
    //     C0 = 0 (mask 0x80) = Last Control Byte, remainder is data
    //     A0 = 1 (mask 0x40) = Byte is instruction data
    return writeDevice(0x40, data);
}

bool AMCLCD::writeDevice(uint8_t controlByte, uint8_t value) {
	wire.beginTransmission(addr);
    
    wire.write(controlByte);
    wire.write(value);
    
	int stat = wire.endTransmission(true);

/*
    if (stat == 0) {
        Log.trace("write succeeded controlByte=%02x value=%02x", controlByte, value);
    }
    else {
        Log.trace("write failed controlByte=%02x value=%02x stat=%d", controlByte, value, stat);
    }
*/

	return (stat == 0);
}


// The addresses on the 4-line model are weird!
uint8_t AMCLCD_Model_AMC2004::getRowAddress(int row) const {
    switch(row) {
    case 0: 
        return 0x00;
    case 1:
        return 0x40;
    case 2:
        return 0x14;
    default:
        return 0x54;
    }
}
