/*
 * LCDDriver.cpp
 *
 *  Created on: 22 мар. 2026 г.
 *      Author: Arsenii Shamrin
 */
#include <LCDDevice.h>

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
				if (currentSymbol == 21) {
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
			this->Send(0b10000000,0);
		}
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
		this->Send(0b00110000, 0);
		this->Send(0b00000010, 0);
		this->Send(0b00001100, 0);
	}
protected:
	uint8_t currentLine;
	uint8_t currentSymbol;
};
