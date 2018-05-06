/**************************************************************************
****************I2C interface between stm32f2xx and ADXL345****************
***************************************************************************
*****************UART interface between user and stm32f2xx*****************
																																		CEBRI·N
***************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "ADXL345_Library.h"
#include "stm32f2xx_hal.h"
#include "stdlib.h"

uint8_t INTERFACE			[4][100] = {{"\r\n\r\nADXL345 UART Interface. (I) Initiation of acquisition, (S) Stop.\r\n"},
																	{"(1)Acquisition frequency programming(1 to 5 seconds) of axis acceleration.\r\n"},
																	{"(2)Read and write operations.\r\n"},
																	{"(3)Information for the user.\r\n"}};

uint8_t INTERFACE_OPTION			[9][100] = {{"\r\nEnter an integer from 1 to 5: (1), (2), (3), (4), (5)."},
																					{"\r\nRegister reading (R), register writting (W)."},
																					{"\r\nIt is essential that the user knows how the ADXL345 works."},
																					{"\r\nDo not forget to write in uppercase to interact with the interface."},																					
																					{"\r\nNo read and write errors are displayed due to enter incorrect values,"},
																					{"\r\nyou could receive wrong information from the interface if you do not use"},
																					{" correctly.\r\n"},
																					{"\r\nIf you wish to exit one of the options press (ESC)."}};

uint8_t INTERFACE2_OPTION			[3][100] = {{"\r\nWrite register to read: 0x..."},
																					{"\r\nWrite the register to overwrite: 0x..."},
																					{"\r\nEnter the value you wish to overwrite in the register: 0x..."}};

uint8_t ANSWER		[11][100] = {{"\r\nThe entered register has the value of 0x"},
																{"\r\nProgrammed timer\r\n"},
																{"\r\nOverwritten value\r\n"},
																{"X1:0x"}, {"   X2:0x"}, {"   Y1:0x"}, {"   Y2:0x"}, {"   Z1:0x"}, {"   Z2:0x"},{"\r\n"},
																{"\r\nThe value of the entered register is 0:0x"}};
													
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;


char iR, iW;
uint8_t num[2], num2[2]; char num3[3];
uint8_t ValRegRec8, ValRec2Overwrite8; 
uint32_t RegRec2Read32, RegRec2Overwrite32, ValRec2Overwrite32;
																
void I2C_ReadAxis_ADXL345(void){																									//OK
	
	uint8_t i=3, j = 0, AxisData_Matrix[6]; char buff[3]; int desp='a'-'A';
	
	HAL_I2C_Mem_Read(&hi2c1,ADXL345_ADDRESS_SHIFT, ADXL345_REG_DATAX0, I2C_MEMADD_SIZE_8BIT, &AxisData_Matrix[0],6,100);// Lee 6 registros
	
	while(i < 9){
		HAL_UART_Transmit(&huart1, &ANSWER [i][0], sizeof(ANSWER[i]), 10);		//Send message to UART terminal
		sprintf(buff, "%02x", AxisData_Matrix[i-3]);
		for (j=0;buff[j]!='\0';++j)	if(buff[j]>='a'&&buff[j]<='z')buff[j]=buff[j]-desp;	
		HAL_UART_Transmit(&huart1, buff, 2, 10);		//Send message to UART terminal
		i++;
	}
		HAL_UART_Transmit(&huart1, &ANSWER[i][0], sizeof(ANSWER[i]), 10);		//Send message to UART terminal
}

void UARTInterface (void){																												//OK
	uint8_t i = 0;
		while (i < 4){
		HAL_UART_Transmit(&huart1, &INTERFACE[i][0], sizeof(INTERFACE[i]), 100);
		i++;
	}
}
void InterfaceOption(char x){																											//OK
		uint8_t i = 0;
	if(x == 1 || x == 2){														//Show R/W operation OK
		HAL_UART_Transmit(&huart1, &INTERFACE_OPTION[x-1][0], sizeof(INTERFACE[x-1]), 10);
	}
	else if(x == 3){																//Show Information for User OK
		i=2;
		while (i < 8){
			HAL_UART_Transmit(&huart1, &INTERFACE_OPTION[i][0], sizeof(INTERFACE_OPTION[i]), 10);
			i++;
		}
	}
	else{																						//(ESC)
		iR = iW = 0;
	}
}
void TIMProgrammingOption(char CharIn){																						//OK
	char CharInAux = 0;
	CharInAux = atoi(&CharIn);
	htim2.Init.Period = (CharInAux * 1000) + 1;
	TIM_Base_SetConfig(TIM2, &htim2.Init);
	HAL_UART_Transmit(&huart1, &ANSWER[1][0], sizeof(ANSWER[1]), 10);
	iR = iW = 0;
}
char ReadingOption(char CharIn){																									
	uint8_t j; int desp='a'-'A';	char flag_OK = 0;

		if(iR == 0){
			HAL_UART_Transmit(&huart1,&INTERFACE2_OPTION[0][0], sizeof(INTERFACE2_OPTION[0]), 10);
			iR++;
		}
		else if (iR == 1 && ((CharIn >= 0x41 && CharIn <= 0x46) || (CharIn >= 0x30 && CharIn <= 0x39))){
			num[0] = CharIn;
			iR++;
		}
		else if (iR >= 2 && ((CharIn >= 0x41 && CharIn <= 0x46) || (CharIn >= 0x30 && CharIn <= 0x39))){
			num[1] = CharIn;
			sscanf((char*)&num, "%02x", &RegRec2Read32);																		//hex conversion		
			HAL_I2C_Mem_Read(&hi2c1, ADXL345_ADDRESS_SHIFT, (uint8_t)RegRec2Read32, I2C_MEMADD_SIZE_8BIT, &ValRegRec8, 1, 10);	
			HAL_UART_Transmit(&huart1,&ANSWER[0][0], sizeof(ANSWER[0]), 10);
			sprintf(num3, "%02x", ValRegRec8);
			for (j=0;num3[j]!='\0';++j)	if(num3[j]>='a'&&num3[j]<='z')num3[j]=num3[j]-desp;	//Uppercase
			HAL_UART_Transmit(&huart1, num3, 2, 10);
			HAL_UART_Transmit(&huart1, ANSWER[9], sizeof(ANSWER[9]), 10);

			iR = iW = 0;
			flag_OK = 16;
		}
	return flag_OK;
}
char WritingOption(char CharIn){
	char flag_OK = 0;
		if(iW == 0){
			HAL_UART_Transmit(&huart1,&INTERFACE2_OPTION[1][0], sizeof(INTERFACE2_OPTION[1]), 10);
			iW++;
		}
		else if (iW == 1 && ((CharIn >= 0x41 && CharIn <= 0x46) || (CharIn >= 0x30 && CharIn <= 0x39))){
			num[0] = CharIn;
			iW++;
		}
		else if (iW == 2 && ((CharIn >= 0x41 && CharIn <= 0x46) || (CharIn >= 0x30 && CharIn <= 0x39))){
			num[1] = CharIn;			
			HAL_UART_Transmit(&huart1, &INTERFACE2_OPTION[2][0], sizeof(INTERFACE2_OPTION[2]), 10);
			iW++;
		}
		else if (iW == 3 && ((CharIn >= 0x41 && CharIn <= 0x46) || (CharIn >= 0x30 && CharIn <= 0x39))){
			num2[0] = CharIn;			
			iW++;
		}
		else if(iW == 4&& ((CharIn >= 0x41 && CharIn <= 0x46) || (CharIn >= 0x30 && CharIn <= 0x39))){
			num2[1] = CharIn;
			sscanf((char*)&num, "%02x", &RegRec2Overwrite32);						//hex conversion
			sscanf((char*)&num2, "%02x", &ValRec2Overwrite32);					//hex conversion
			ValRec2Overwrite8 = ValRec2Overwrite32;
			HAL_I2C_Mem_Write(&hi2c1, ADXL345_ADDRESS_SHIFT, (uint8_t) RegRec2Overwrite32, I2C_MEMADD_SIZE_8BIT, &ValRec2Overwrite8, 1, 10);
			HAL_UART_Transmit(&huart1,&ANSWER[2][0], sizeof(ANSWER[2]), 10);
			iW = iR = 0;
			flag_OK = 16;
		}
	return flag_OK;
}
