/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"

#include "IRSM.cpp"
#include <cstring>
#include "LCDDevice.h"
#include "Menu.cpp"

#define LOGUART true

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Custom function to handle logging data in order to give more info about machine's state to user
void Log(bool IsError, std::string Text){
	if (!(LOGUART)) return; // Don't do anything if no logging required
	Text = (IsError ? "[ERROR]: " : "[INFO]: ") + Text;
	if (LOGUART){ // Log data to the UART interface
		Text+="\r\n";
		HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t*>(&Text[0]), Text.length(), 10);
	}
}

LCDDevice LCD{hi2c1, 0x27, Log};

class State1 : public State {
public:
	using State::State;
	int counter{ 0 };
	void OnEnter() override {
		(*ParentStateMachine).Log(false, "Entered State1");
	}
	void Tick() override {
		(*ParentStateMachine).Log(false, "TICK1");
		if (counter == 3) {
			(*ParentStateMachine).Log(false, "BEEP");
			(*ParentStateMachine).Emit("STATE2");
		}
		else {
			counter++;
		}
	}
	void OnExit() override {
		(*ParentStateMachine).Log(false, "Exited State1");
	}
};
bool Work{ true };
class State2 : public State {
public:
	using State::State;
	int counter{ 0 };
	void OnEnter() {
		(*ParentStateMachine).Log(false, "Entered State2");
	}
	void Tick() override {
		(*ParentStateMachine).Log(false, "TICK2");
		if (counter == 6) {
			(*ParentStateMachine).Log(false, "BOOP");
			Work = false;
			(*ParentStateMachine).Emit("STATE3");
		}
		else {
			counter++;
		}
	}
	void OnExit() override {
		(*ParentStateMachine).Log(false, "Exited State2");
	}
};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  Log(false, "Starting up");

  LCD.init();
  Log(false, "Initialized LCD");

  std::list<std::string> MenuItems {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6", "Item 7"};
  Menu TestMenu {4, 1, MenuItems};
  Log(false, "Initialized Menu");

  //Display welcome-screen
  LCD.clear();
  LCD.write("01234567890123456789aaaaaaaaaabbbbbbbbbb01234567890123456789ccccccccccdddddddddd");
  HAL_Delay(1000);
  LCD.write("this is a test message that is supposed to show any errors that may occure there");
  HAL_Delay(1000);
  LCD.clear();
  LCD.write("01234567890123456789aaaaaaaaaabbbbbbbbbb01234567890123456789ccccccccccdddddddddd");
  HAL_Delay(1000);
  LCD.write("this is a test message that is supposed to show any errors that may occure there");
  HAL_Delay(1000);
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.write(" Слайсер Роторный  ");
  LCD.setCursor(0,1);
  LCD.write("   Промышленный    ");
  LCD.setCursor(0,2);
  LCD.write("      Изделие      ");
  LCD.setCursor(0,3);
  LCD.write("Тебенькова-Шамрина ");
  HAL_Delay(1000);
  LCD.clear();

  //uint8_t rx_buff[1]={};

  /*StateMachine MainStateMachine{&Log};
  State1 state1{ MainStateMachine, "STATE1" };
  State2 state2{ MainStateMachine, "STATE2" };
  MainStateMachine.AddState(&state1);
  MainStateMachine.AddState(&state2);*/

  //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

  //HAL_GPIO_WritePin(GPIOA, IN1_Pin, GPIO_PIN_SET);
  //HAL_GPIO_WritePin(GPIOA, IN2_Pin, GPIO_PIN_RESET);
  //int32_t CH1_DC = 0;

  //Log(false, "Initialized L298N Driver");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t cursorLine = 2;
  while (1)
  {
	  for (int i=0;i<6;i++) {
		  LCD.clear();
		  LCD.setCursor(0,0);
		  cursorLine = 2;
		  for (std::string Item : TestMenu.getItems()) {
			  if (cursorLine==1) LCD.write("> ");
			  //else LCD.write("  ");
			  if (cursorLine>0) cursorLine--;
		  	  LCD.write(Item);
		  	  LCD.nextLine();
	  	  }
	  	  TestMenu.moveCursor(1);
	  	  HAL_Delay(1000);
	  }
	  for (int i=0;i<6;i++) {
		  LCD.clear();
		  LCD.setCursor(0,0);
		  cursorLine = 2;
		  for (std::string Item : TestMenu.getItems()) {
			  if (cursorLine==1) LCD.write("> ");
			  //else LCD.write("  ");
			  if (cursorLine>0) cursorLine--;
			  LCD.write(Item);
			  LCD.nextLine();
		  }

		  TestMenu.moveCursor(-1);
		  HAL_Delay(1000);
	  }
    /* USER CODE END WHILE */
	  /*
	HAL_UART_Receive_IT(&huart1, rx_buff, 1);
	if (rx_buff[0]!=0){
		rx_buff[0]=0;
	}
	while(CH1_DC < 65535)
	{
		TIM2->CCR1 = CH1_DC;
		TIM2->CCR2 = 65535 - CH1_DC;
		CH1_DC += 70;
		HAL_Delay(1);
	}
	CH1_DC=65535;
	while(CH1_DC > 0)
	{
		TIM2->CCR1 = CH1_DC;
		TIM2->CCR2 = 65535 - CH1_DC;
		CH1_DC -= 70;
		HAL_Delay(1);
	}
	CH1_DC=0;*/
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
