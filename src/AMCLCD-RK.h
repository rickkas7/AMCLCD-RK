#ifndef __AMCLCD_RK_H
#define __AMCLCD_RK_H

// Repository: https://github.com/rickkas7/AMLCD-RK
// License: MIT

#include "Particle.h"

/**
 * @brief Base class of all models of displays
 * 
 * A number of common displays are included here 
 * 
 * - AMCLCD_Model_AMC1602
 * - AMCLCD_Model_AMC2002 
 * - AMCLCD_Model_AMC2004 
 * - AMCLCD_Model_AMC0802
 * 
 * You can also subclass this in user code outside the library and pass it to the constructor.
 */
class AMCLCD_Model {
public:
    virtual int getCols() const = 0;
    virtual int getRows() const = 0;
    virtual uint8_t getRowAddress(int row) const = 0;
    virtual uint8_t getConfig() const = 0;
};

/**
 * @brief Display settings for the AMC1602AR display (16x2)
 * 
 * Orient Display AMC1602AR-B-B6WTDW-I2C $7.49
 * COB CHAR 16X2 BLUE TRANSM I2C
 * Character LCD Display Module Transmissive 5 x 8 Dots STN - Super-Twisted Nematic LED - White I2C 80.00mm x 36.00mm x 14.00mm
 * https://www.digikey.com/product-detail/en/orient-display/AMC1602AR-B-B6WTDW-I2C/2544-AMC1602AR-B-B6WTDW-I2C-ND/12089223
 * 
 */ 
class AMCLCD_Model_AMC1602 : public AMCLCD_Model {
public:
    virtual int getCols() const { return 16; };
    virtual int getRows() const { return 2; };
    virtual uint8_t getRowAddress(int row) const { return (row == 0) ? 0x00 : 0x40; };
    virtual uint8_t getConfig() const { return 0x38; };
};

/**
 * @brief Display settings for the AMC2002CR display (20x2)
 * 
 * AMC2002CR-B-B6WTDW-I2C $11.70
 * https://www.digikey.com/product-detail/en/orient-display/AMC2002CR-B-B6WTDW-I2C/2544-AMC2002CR-B-B6WTDW-I2C-ND/12089302
 * 
 * Note: The yellow-green backlight model requires 120 mA vs. 30 mA for white. The white backlight is usually a better choice.
 */
class AMCLCD_Model_AMC2002 : public AMCLCD_Model {
public:
    virtual int getCols() const { return 20; };
    virtual int getRows() const { return 2; };
    virtual uint8_t getRowAddress(int row) const { return (row == 0) ? 0x00 : 0x40; };
    virtual uint8_t getConfig() const { return 0x38; };
};

/**
 * @brief Display settings for the AMC2004AR display (20x4)
 * 
 * AMC2004AR-B-B6WTDW-I2C
 * https://www.digikey.com/product-detail/en/orient-display/AMC2004AR-B-B6WTDW-I2C/2544-AMC2004AR-B-B6WTDW-I2C-ND/12089320
 * 
 * Note: The yellow-green backlight model requires 120 mA vs. 30 mA for white. The white backlight is usually a better choice.
 */
class AMCLCD_Model_AMC2004 : public AMCLCD_Model {
public:
    virtual int getCols() const { return 20; };
    virtual int getRows() const { return 4; };
    virtual uint8_t getRowAddress(int row) const;
    virtual uint8_t getConfig() const { return 0x38; };
};

/**
 * @brief Display settings for the AMC0802BR display (8x2)
 * 
 * AMC0802BR-B-B6WTDW-I2C $5.20
 * https://www.digikey.com/product-detail/en/orient-display/AMC0802BR-B-B6WTDW-I2C/2544-AMC0802BR-B-B6WTDW-I2C-ND/12089222
 * 
 * Note: The yellow-green backlight model requires 120 mA vs. 30 mA for white. The white backlight is usually a better choice.
 */
class AMCLCD_Model_AMC0802 : public AMCLCD_Model {
public:
    virtual int getCols() const { return 8; };
    virtual int getRows() const { return 2; };
    virtual uint8_t getRowAddress(int row) const { return (row == 0) ? 0x00 : 0x40; };
    virtual uint8_t getConfig() const { return 0x38; };
};


/**
 * @brief Class for the Orient Display AMC line of character LCD I2C displays
 * 
 * Be sure to call the begin() method from setup(). 
 */
class AMCLCD : public Print {
public:
	/**
	 * @brief Construct the object
	 *
     * @param cols Number of columns (16 for the display above)
     * 
     * @param rows Number of rows (2 for the display above)
     * 
	 * @param addr The address. Can be 0 - 7 based on the address select pin and the normal base of60
	 *
	 * @param wire The I2C interface to use. Normally Wire, the primary I2C interface. Can be a
	 * different one on devices with more than one I2C interface.
	 * 
     */
	AMCLCD(AMCLCD_Model &model, uint8_t addr = 0x3C, TwoWire &wire = Wire);

	/**
	 * @brief Destructor. Not normally used as this is typically a globally instantiated object.
	 */
	virtual ~AMCLCD();


	/**
	 * @brief Set up the I2C device and begin running.
	 *
	 * You cannot do this from STARTUP or global object construction time. It should only be done from setup
	 * or loop (once).
	 *
	 * Make sure you set the LED current using withLEDCurrent() before calling begin if your LED has a
	 * current other than the default of 5 mA.
	 */
	bool begin();

    bool writeInst(uint8_t value);

    bool writeData(uint8_t data);

    void setPosition(int col, int row = 0) { this->col = col; this->row = row; };

    int getCol() const { return col; };

    int getRow() const { return row; };

    void setWrap(bool wrap = true) { this->wrap = wrap; };

    bool getWrap() const { return wrap; };

    virtual size_t write(uint8_t b);

    void clearDisplay();

    static const uint8_t INST_CLEAR_DISPLAY                       = 0x01;     //!< 
    static const uint8_t INST_RETURN_HOME                         = 0x02;     //!< 
    static const uint8_t INST_ENTRY_MODE_SET                      = 0x04;     //!< 
    static const uint8_t  BIT_CURSOR_INCREMENT                    = 0b10;
    static const uint8_t  BIT_DISPLAY_SHIFT                       = 0b01;
    static const uint8_t INST_DISPLAY_ON_OFF                      = 0x08;     //!< 
    static const uint8_t  BIT_DISPLAY_ON                          = 0b100;
    static const uint8_t  BIT_CURSOR_ON                           = 0b010;
    static const uint8_t  BIT_BLINKING_ON                         = 0b001;
    static const uint8_t INST_CURSOR_DISPLAY_SHIFT                = 0x10;     //!< 
    static const uint8_t INST_FUNCTION_SET                        = 0x20;     //!< 
    static const uint8_t INST_SET_CGRAM_ADDR                      = 0x40;     //!< 
    static const uint8_t INST_SET_DDRAM_ADDR                      = 0x80;     //!< 
    

protected:
	/**
	 * @brief Low-level call to write to the device

	 */
	bool writeDevice(uint8_t controlByte, uint8_t dataByte);

protected:
    AMCLCD_Model &model;

	/**
	 * @brief The I2C address (0x00 - 0x7f). Default is 0x3C.
	 *
	 * If you passed in an address 0 - 7 into the constructor, 0x3C - 0x3f is stored here.
	 */
	uint8_t addr;

	/**
	 * @brief The I2C interface to use. Default is Wire. Could be Wire1 or Wire3 on some devices.
	 */
	TwoWire &wire;

    int col = 0;

    int row = 0;

    bool wrap = true;
};


#endif /* __AMCLCD_RK_H */
