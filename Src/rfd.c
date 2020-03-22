/*
 * rfd.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Nikolina
 */
#include "rfd.h"
	uint16_t distance;
	uint8_t validPixels;
	uint16_t confidenceValue;



	uint16_t readRegister16(uint8_t addr){

	uint8_t data=addr;
	uint8_t data1[2];
		HAL_I2C_Master_Transmit(&hi2c2, RFD77402_ADDR, &data, 1, 100);
		HAL_I2C_Master_Receive(&hi2c2, RFD77402_ADDR, data1, 2, 100);

		return((data1[1] << 8) | data1[0]);
	}
	uint8_t readRegister(uint8_t addr){

	uint8_t data=addr;
	uint8_t data1;

		HAL_I2C_Master_Transmit(&hi2c2, RFD77402_ADDR, &data, 1, 100);
		HAL_I2C_Master_Receive(&hi2c2, RFD77402_ADDR, &data1, 1, 100);

		return data1;
	}
	void writeRegister(uint8_t addr, uint8_t val){

		uint8_t data[2];
		data[0]=addr;
		data[1]=val;
		HAL_I2C_Master_Transmit(&hi2c2, RFD77402_ADDR, data, 2, 100);
	}

	void writeRegister16(uint8_t addr, uint16_t val){

		uint8_t data[3];
		data[0]=addr;
		data[1]=(val & 0xFF);
		data[2]=(val >> 8);
		HAL_I2C_Master_Transmit(&hi2c2, RFD77402_ADDR, data, 3, 100);

	}

	void delay(int x){
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}
	bool begin()
	{

		  //Put chip into standby
		  if (goToStandbyMode() == false) return (false); //Chip timed out before going to standby

		  //Drive INT_PAD high
		  uint8_t setting = readRegister(RFD77402_ICSR);
		  setting &= 0b11110000; //clears writable bits
		  setting |= INT_CLR_REG | INT_CLR | INT_PIN_TYPE | INT_LOHI; //change bits to enable interrupt
		  writeRegister(RFD77402_ICSR, setting);
		  setting = readRegister(RFD77402_INTERRUPTS);
		  setting &= 0b00000000; //Clears bits
		  setting |= INTSRC_DATA | INTSRC_M2H | INTSRC_H2M | INTSRC_RST; //Enables interrupt when data is ready
		  writeRegister(RFD77402_INTERRUPTS, setting);

		  //Configure I2C Interface
		  writeRegister(RFD77402_CONFIGURE_I2C, 0x65); //0b.0110.0101 = Address increment, auto increment, host debug, MCPU debug

		  //Set initialization - Magic from datasheet. Write 0x05 to 0x15 location.
		  writeRegister16(RFD77402_CONFIGURE_PMU, 0x0500); //0b.0000.0101.0000.0000 //Patch_code_id_en, Patch_mem_en

		  if (goToOffMode() == false) return (false); //MCPU never turned off

		  //Read Module ID
		  //Skipped

		  //Set initialization - Magic from datasheet. Write 0x06 to 0x15 location.
		  writeRegister16(RFD77402_CONFIGURE_PMU, 0x0600); //MCPU_Init_state, Patch_mem_en

		  if (goToOnMode() == false) return (false); //MCPU never turned on

		  //ToF Configuration

		  writeRegister16(RFD77402_CONFIGURE_A, 0xE100); //0b.1110.0001 = Peak is 0x0E, Threshold is 1.
		  writeRegister16(RFD77402_CONFIGURE_B, 0x10FF); //Set valid pixel. Set MSP430 default config.
		  writeRegister16(RFD77402_CONFIGURE_HW_0, 0x07D0); //Set saturation threshold = 2,000.
		  writeRegister16(RFD77402_CONFIGURE_HW_1, 0x5008); //Frequecy = 5. Low level threshold = 8.
		  writeRegister16(RFD77402_CONFIGURE_HW_2, 0xA041); //Integration time = 10 * (6500-20)/15)+20 = 4.340ms. Plus reserved magic.
		  writeRegister16(RFD77402_CONFIGURE_HW_3, 0x45D4); //Enable harmonic cancellation. Enable auto adjust of integration time. Plus reserved magic.

		  if (goToStandbyMode() == false) return (false); //Error - MCPU never went to standby

		  //Put device into Standby mode
		  if (goToStandbyMode() == false) return (false); //Error - MCPU never went to standby

		  //Now assume user will want sensor in measurement mode

		  //Set initialization - Magic from datasheet. Write 0x05 to 0x15 location.
		  writeRegister16(RFD77402_CONFIGURE_PMU, 0x0500); //Patch_code_id_en, Patch_mem_en

		  if (goToOffMode() == false) return (false); //Error - MCPU never turned off

		  //Set initialization - Magic from datasheet. Write 0x06 to 0x15 location.
		  writeRegister16(RFD77402_CONFIGURE_PMU, 0x0600); //MCPU_Init_state, Patch_mem_en

		  if (goToOnMode() == false) return (false); //Error - MCPU never turned on

		  return (true); //Success! Sensor is ready for measurements
	}

	//Takes a single measurement and sets the global variables with new data
	//Returns zero if reading is good, otherwise return the errorCode from the result register.
	uint8_t takeMeasurement(void)
	{
	  if (goToMeasurementMode() == false) return (CODE_FAILED_TIMEOUT); //Error - Timeout
	  //New data is now available!

	  //Read result
	  uint16_t resultRegister = readRegister16(RFD77402_RESULT);

	  if (resultRegister & 0x7FFF) //Reading is valid
	  {
	    uint8_t errorCode = (resultRegister >> 13) & 0x03;

	    if (errorCode == 0)
	    {
	      distance = (resultRegister >> 2) & 0x07FF; //Distance is good. Read it.
	      //Serial.println("Distance field valid");

	      //Read confidence register
	      uint16_t confidenceRegister = readRegister16(RFD77402_RESULT_CONFIDENCE);
	      validPixels = confidenceRegister & 0x0F;
	      confidenceValue = (confidenceRegister >> 4) & 0x07FF;
	    }

	    return (errorCode);

	  }
	  else
	  {
	    //Reading is not vald
	    return (CODE_FAILED_NOT_NEW); //Error code for reading is not new
	  }

	}

	//Returns the local variable to the caller
	uint16_t getDistance()
	{
	  return (distance);
	}

	//Returns the number of valid pixels found when taking measurement
	uint8_t getValidPixels()
	{
	  return (validPixels);
	}

	//Returns the qualitative value representing how confident the sensor is about its reported distance
	uint16_t getConfidenceValue()
	{
	  return (confidenceValue);
	}

	//Read the command opcode and covert to mode
	uint8_t getMode()
	{
	  return (readRegister(RFD77402_COMMAND) & 0x3F);
	}

	//Tell MCPU to go to standby mode
	//Return true if successful
	bool goToStandbyMode()
	{
	  //Set Low Power Standby
	  writeRegister(RFD77402_COMMAND, 0x90); //0b.1001.0000 = Go to standby mode. Set valid command.

	  //Check MCPU_ON Status
	  for (uint8_t x = 0 ; x < 10 ; x++)
	  {
	    if ( (readRegister16(RFD77402_DEVICE_STATUS) & 0x001F) == 0x0000) return (true); //MCPU is now in standby
	    delay(10); //Suggested timeout for status checks from datasheet
	  }

	  return (false); //Error - MCPU never went to standby
	}

	//Tell MCPU to go to off state
	//Return true if successful
	bool goToOffMode()
	{
	  //Set MCPU_OFF
	  writeRegister(RFD77402_COMMAND, 0x91); //0b.1001.0001 = Go MCPU off state. Set valid command.

	  //Check MCPU_OFF Status
	  for (uint8_t x = 0 ; x < 10 ; x++)
	  {
	    if ( (readRegister16(RFD77402_DEVICE_STATUS) & 0x001F) == 0x0010) return (true); //MCPU is now off
	    delay(10); //Suggested timeout for status checks from datasheet
	  }

	  return (false); //Error - MCPU never turned off
	}

	//Tell MCPU to go to on state
	//Return true if successful
	bool goToOnMode()
	{
	  //Set MCPU_ON
	  writeRegister(RFD77402_COMMAND, 0x92); //0b.1001.0010 = Wake up MCPU to ON mode. Set valid command.

	  //Check MCPU_ON Status
	  for (uint8_t x = 0 ; x < 10 ; x++)
	  {
	    if ( (readRegister16(RFD77402_DEVICE_STATUS) & 0x001F) == 0x0018) return (true); //MCPU is now on
	    delay(10); //Suggested timeout for status checks from datasheet
	  }

	  return (false); //Error - MCPU never turned on
	}

	//Tell MCPU to go to measurement mode
	//Takes a measurement. If measurement data is ready, return true
	bool goToMeasurementMode()
	{
	  //Single measure command
	  writeRegister(RFD77402_COMMAND, 0x81); //0b.1000.0001 = Single measurement. Set valid command.

	  //Read ICSR Register - Check to see if measurement data is ready
	  for (uint8_t x = 0 ; x < 10 ; x++)
	  {
	    if ( (readRegister(RFD77402_ICSR) & (1 << 4)) != 0) return (true); //Data is ready!
	    delay(10); //Suggested timeout for status checks from datasheet
	  }

	  return (false); //Error - Timeout
	}
