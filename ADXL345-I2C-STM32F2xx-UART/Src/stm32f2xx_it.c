/**
  ******************************************************************************
  * @file    stm32f2xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
																																				 CEBRIáN
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"
#include "stm32f2xx.h"
#include "stm32f2xx_it.h"

/* USER CODE BEGIN 0 */
#include "ADXL345_Library.h"

static char CharIn, flag1, flag2, flagR, flagW, LockingInterface;

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F2xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f2xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles TIM2 global interrupt.
*/
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
  I2C_ReadAxis_ADXL345();	
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	char CharInAux=0; 
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	CharInAux = USART1->DR;
	CharIn = CharInAux;
	if(CharIn == 0x1B || LockingInterface == 0 || (CharIn == 0x53 && LockingInterface == 1)){
		switch(CharIn){
			case 0x31:												//(1)Timer programming
				flag1 = 1; LockingInterface = 1;
				InterfaceOption (1);break;
			case 0x32:												//(2)->(R) or (W)
				flag2 = 1; LockingInterface = 1;
				InterfaceOption (2);break;
			case 0x33:												//(3)Information
				flag1 = flag2 = flagW = flagR = LockingInterface = 0;
				InterfaceOption (3);break;
			case 0x1B:												//(ESC)
				flag1 = flag2 = flagW = flagR = LockingInterface = 0;
				InterfaceOption (4);
		    __HAL_TIM_DISABLE(&htim2);
				UARTInterface();break;
			case 0x49:												//Acquisition Initiation (I)
				flag1 = flag2 = flagW = flagR = LockingInterface = 1;
				__HAL_TIM_ENABLE(&htim2);break;
			case 0x53:												//Acquisition Stop (S)
				flag1 = flag2 = flagW = flagR = LockingInterface = 0;
		    __HAL_TIM_DISABLE(&htim2);
				UARTInterface();break;
		}
	}
	else if(LockingInterface == 1 && flag1 == 1 && CharIn >= 0x31 && CharIn <= 0x35){	//Timer Programming for adquisition frecuency
		TIMProgrammingOption(CharIn);
		flag1 = flag2 = CharIn = flagW = flagR = LockingInterface = 0;
	}
	else if((LockingInterface == 1 && flag2 == 1 && CharIn == 0x52) || flagR == 1){						//Reading
		flagR = 1; flagW = 0; flag2 = 0;
		CharInAux = ReadingOption(CharIn);
		if (CharInAux == 16)	flag1 = flag2 = flagW = flagR = LockingInterface = 0;

	}
	else if((LockingInterface == 1 && flag2 == 1 && CharIn == 0x57) || flagW == 1){						//Writing
		flagW = 1; flagR = 0; flag2 = 0;
		CharInAux = WritingOption(CharIn);	
		if (CharInAux == 16) flag1 = flag2 = flagW = flagR = LockingInterface = 0;

	}
  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
