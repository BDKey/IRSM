/*
 * LCDDriver.cpp
 *
 *  Created on: 22 мар. 2026 г.
 *      Author: Arsenii Shamrin
 */
#include <LCDDevice.h>

const char utf_recode[] { 0x70,0x63,0xbf,0x79,0xe4,0x78,0xe5,0xc0,0xc1,0xe6,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0x41,0xa0,0x42,0xa1,0xe0,0x45,0xa3,0xa4,0xa5,0xa6,0x4b,0xa7,0x4d,0x48,0x4f,0xa8,0x50,0x43,0x54,0xa9,0xaa,0x58,0xe1,0xab,0xac,0xe2,0xad,0xae,0x62,0xaf,0xb0,0xb1,0x61,0xb2,0xb3,0xb4,0xe3,0x65,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0x6f,0xbe};
const int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

class LCDDevice{
public:
	uint16_t I2CAddress;
	I2C_HandleTypeDef* hi2c;
	LCDDevice(I2C_HandleTypeDef& hi2c, uint16_t I2CAddress){
		this->I2CAddress = (I2CAddress << 1);
		this->hi2c = &hi2c;
		currentLine = 1;
		currentSymbol = 0;
	}
	void Send(uint8_t data, uint8_t flags){
		if (flags) {
				currentSymbol++;
				if (currentSymbol > ROWS_AMOUNT) {
					currentSymbol = 0;
					this->SetLine(currentLine+1);
				}
			}
			HAL_StatusTypeDef res;
			 // бесконечный цикл
		        for(;;) {
		                // проверяем, готово ли устройство по адресу lcd_addr для связи
			        res = HAL_I2C_IsDeviceReady(&(*hi2c), I2CAddress, 1, HAL_MAX_DELAY);
			         // если да, то выходим из бесконечного цикла
		                if(res == HAL_OK) break;
			    }
		        // операция И с 1111 0000 приводит к обнулению бит с 0 по 3, остаются биты с 4 по 7
			uint8_t up = data & 0xF0;
		        // то же самое, но data сдвигается на 4 бита влево
		        uint8_t lo = (data << 4) & 0xF0;

			uint8_t data_arr[4];
		         // 4-7 биты содержат информацию, биты 0-3 настраивают работу дисплея
			data_arr[0] = up|flags|BACKLIGHT|PIN_EN;
		         // дублирование сигнала, на выводе Е в этот раз 0
			data_arr[1] = up|flags|BACKLIGHT;
			data_arr[2] = lo|flags|BACKLIGHT|PIN_EN;
			data_arr[3] = lo|flags|BACKLIGHT;

			HAL_I2C_Master_Transmit(&(*hi2c), I2CAddress, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
			HAL_Delay(LCD_DELAY_MS);
	}
	//void SetCursor(uint8_t col, uint8_t row) {
	//	if ( row >= _numlines ) {
	//		row = _numlines-1;    // we count rows starting w/0
	//	}
	//}
	//Outdated
	void SetLine(uint8_t line){
		currentSymbol=0;
		if (line==1){
			currentLine=1;
			this->Send(0b10000000,0);
		} else if (line==2) {
			currentLine=2;
			this->Send(0b11000000,0);
		} else if (line==3) {
			currentLine=3;
			this->Send(0b10010100,0);
		} else if (line==4) {
			currentLine=4;
			this->Send(0b11010100,0);
		} else {
			currentLine=0;
			this->Send(0b10000000,0); // Если не обрабатывается, то переходить на первую строчку
		}
	}
	void NextLine(){
		SetLine(currentLine+1);
	}
	void Write(std::string Text){
		for (char c : Text){
			this->Send((uint8_t)c, 1);
		}
	}
	void WriteDecoded(uint8_t Symbol){
		this->Send(Symbol, 1);
	}
	void Clear(){
		currentLine=1;
		currentSymbol=0;
		this->Send(0b00000001, 0);
	}
	void Init(){
		//20x4
		this->Send(0x30, 0);
		this->Send(0x02, 0);
		this->Send(0x0C, 0);
	}
protected:
	uint8_t currentLine;
	uint8_t currentSymbol;
};
