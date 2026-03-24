/*
 * L298NDevice.cpp
 *
 *  Created on: 24 мар. 2026 г.
 *      Author: Арсений
 */
#include "L298NDevice.h"

class L298NDevice{
public:
	L298NDevice(TIM_HandleTypeDef& htimx, uint16_t TIM_CHANNEL, GPIO_TypeDef& GPIOx, uint16_t IN3_GPIO_Pin, uint16_t IN4_GPIO_Pin) {
		this->htimx = &htimx;
		this->TIM_CHANNEL=TIM_CHANNEL;
		this->GPIOx = &GPIOx;
		this->IN3_GPIO_Pin=IN3_GPIO_Pin;
		this->IN4_GPIO_Pin=IN4_GPIO_Pin;
		HAL_TIM_PWM_Start(&htimx, TIM_CHANNEL);
	}
	void SetSpeed(uint16_t speed){
		//__HAL_TIM_SET_COMPARE(&htimx,TIM_CHANNEL, speed);
	}
protected:
	TIM_HandleTypeDef* htimx;
	uint16_t TIM_CHANNEL;
	GPIO_TypeDef* GPIOx;
	uint16_t IN3_GPIO_Pin;
	uint16_t IN4_GPIO_Pin;
};
