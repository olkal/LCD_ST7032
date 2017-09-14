//-------------------------------------------------------------------------------------------
// LCD_ST7032.h - Arduino i2c master library for LCD ST7032
// Olav Kallhovd sept2017
//
// Tested Module    : ERC1602-4, EASTRISING TECHNOLOGY CO,.LTD.
// Drive IC         : ST7032
// INTERFACE        : I2C
// VDD              : 2.7V-5.5V
// Tested with MCU	: Arduino Uno, Attiny85@1mhz(5.0V), Attiny85@8mhz(5.0V) and ESP8266(3.3V)
//-------------------------------------------------------------------------------------------

#include <Arduino.h>
#include <LCD_ST7032.h>
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) \
    || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) \
    || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) \
    || defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny4313__) \
    || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__) \
    || defined( __AVR_ATtiny261__ ) || defined( __AVR_ATtiny461__ ) \
    || defined( __AVR_ATtiny861__ ) || defined(__AVR_ATtiny1634__)
#include <USIWire.h>
#else
#include <Wire.h>
#endif

LCD_ST7032::LCD_ST7032() 
{ 
} 

void LCD_ST7032::begin() 
{
	Wire.begin();
	delay(100);
    Write_Instruction(FUNCTION_SET | FUNCTION_SET_DL | FUNCTION_SET_N | FUNCTION_SET_IS);
    Write_Instruction(INTERNAL_OSC_FREQ | INTERNAL_OSC_FREQ_BS | INTERNAL_OSC_FREQ_F2);
    Write_Instruction(POWER_ICON_BOST_CONTR | POWER_ICON_BOST_CONTR_Ion);
    setcontrast(contrast);
    Write_Instruction(FOLLOWER_CONTROL | FOLLOWER_CONTROL_Fon | FOLLOWER_CONTROL_Rab2);
    delay(300);
    Write_Instruction(displayOnOffSetting);
	Write_Instruction(ENTRY_MODE_SET | ENTRY_MODE_SET_ID); 
    this->clear(); 
	this->home();
}	

void LCD_ST7032::Write_Instruction(uint8_t cmd)
{
	Wire.beginTransmission(Write_Address);
	Wire.write(CNTRBIT_CO);  
	Wire.write(cmd);
	Wire.endTransmission();
	delayMicroseconds(WRITE_DELAY_MS);
}

void LCD_ST7032::Write_Data(uint8_t data)
{
	Wire.beginTransmission(Write_Address);
	Wire.write(CNTRBIT_RS); 
	Wire.write(data);
	Wire.endTransmission();
	delayMicroseconds(WRITE_DELAY_MS);
}

size_t LCD_ST7032::write(uint8_t chr) 
{
	this->Write_Data(chr);
	return 1;
}

void LCD_ST7032::clear() { //clear display
	this->Write_Instruction(CLEAR_DISPLAY);
	delayMicroseconds(HOME_CLEAR_DELAY_MS);
}

void LCD_ST7032::home() { //return to first line address 0
	this->Write_Instruction(RETURN_HOME);
	delayMicroseconds(HOME_CLEAR_DELAY_MS);
}

void LCD_ST7032::setCursor(uint8_t line, uint8_t pos) 
{
	uint8_t p;
	if(pos > 15) pos = 0;
	if(line == 0) p = LINE_1_ADR + pos;
	else p = LINE_2_ADR + pos;
	Write_Instruction(SET_DDRAM_ADDRESS | p);
}

void LCD_ST7032::display() //turn on display
{ 
	displayOnOffSetting |= DISPLAY_ON_OFF_D;
	Write_Instruction(displayOnOffSetting);
}

void LCD_ST7032::noDisplay() //turn off display
{ 
	displayOnOffSetting &= ~DISPLAY_ON_OFF_D;
	Write_Instruction(displayOnOffSetting);
}

void LCD_ST7032::cursor() //display underline cursor
{ 
	displayOnOffSetting |= DISPLAY_ON_OFF_C;
	Write_Instruction(displayOnOffSetting);
}

void LCD_ST7032::noCursor() //stop display underline cursor
{ 
	displayOnOffSetting &= ~DISPLAY_ON_OFF_C;
	Write_Instruction(displayOnOffSetting);
}

void LCD_ST7032::blink() //cursor block blink
{ 
	displayOnOffSetting |= DISPLAY_ON_OFF_B;
	Write_Instruction(displayOnOffSetting);
}

void LCD_ST7032::noBlink() //stop cursor block blink
{ 
	displayOnOffSetting &= ~DISPLAY_ON_OFF_B;
	Write_Instruction(displayOnOffSetting);
}

void LCD_ST7032::setcontrast(int val) 
{
	if (val > CONTRAST_MAX) val = CONTRAST_MIN;
	else if (val < CONTRAST_MIN) val = CONTRAST_MAX;
	Write_Instruction(CONTRAST_SET | (val & B00001111));
	Write_Instruction((val >> 4) | POWER_ICON_BOST_CONTR | POWER_ICON_BOST_CONTR_Bon);
	contrast = val;
}

void LCD_ST7032::adjcontrast(int val) 
{
	setcontrast(val + contrast);
}

uint8_t LCD_ST7032::getcontrast() 
{
	return contrast;
}
