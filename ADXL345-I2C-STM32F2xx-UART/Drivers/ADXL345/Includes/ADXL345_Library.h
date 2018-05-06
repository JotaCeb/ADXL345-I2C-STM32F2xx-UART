/*******************************************************************************
																																				 CEBRI�N
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#ifndef __ADXL345_LIBRARY_H
#define __ADXL345_LIBRARY_H

#include "stm32f2xx_hal.h"
#include "stdint.h"

/*I2C Address---------------------------------------------------------------*/
#define ADXL345_ADDRESS             		((uint16_t)0x0053)    			// Assumes ALT address pin low
#define ADXL345_ADDRESS_SHIFT        		(((uint16_t)0x0053)<<1)    	// Assumes ALT address pin low

/*Registros-----------------------------------------------------------------*/
#define ADXL345_REG_DEVID               ((uint8_t)0x00)    // Device ID
#define ADXL345_REG_THRESH_TAP          ((uint8_t)0x1D)    // Tap threshold
#define ADXL345_REG_OFSX                ((uint8_t)0x1E)    // X-axis offset
#define ADXL345_REG_OFSY                ((uint8_t)0x1F)    // Y-axis offset
#define ADXL345_REG_OFSZ                ((uint8_t)0x20)    // Z-axis offset
#define ADXL345_REG_DUR                 ((uint8_t)0x21)    // Tap duration
#define ADXL345_REG_LATENT              ((uint8_t)0x22)    // Tap latency
#define ADXL345_REG_WINDOW              ((uint8_t)0x23)    // Tap window
#define ADXL345_REG_THRESH_ACT          ((uint8_t)0x24)    // Activity threshold
#define ADXL345_REG_THRESH_INACT        ((uint8_t)0x25)    // Inactivity threshold
#define ADXL345_REG_TIME_INACT          ((uint8_t)0x26)    // Inactivity time
#define ADXL345_REG_ACT_INACT_CTL       ((uint8_t)0x27)    // Axis enable control for activity and inactivity detection
#define ADXL345_REG_THRESH_FF           ((uint8_t)0x28)    // Free-fall threshold
#define ADXL345_REG_TIME_FF             ((uint8_t)0x29)    // Free-fall time
#define ADXL345_REG_TAP_AXES            ((uint8_t)0x2A)    // Axis control for single/double tap
#define ADXL345_REG_ACT_TAP_STATUS      ((uint8_t)0x2B)    // Source for single/double tap
#define ADXL345_REG_BW_RATE             ((uint8_t)0x2C)    // Data rate and power mode control
#define ADXL345_REG_POWER_CTL           ((uint8_t)0x2D)    // Power-saving features control
#define ADXL345_REG_INT_ENABLE          ((uint8_t)0x2E)    // Interrupt enable control
#define ADXL345_REG_INT_MAP             ((uint8_t)0x2F)    // Interrupt mapping control
#define ADXL345_REG_INT_SOURCE          ((uint8_t)0x30)    // Source of interrupts
#define ADXL345_REG_DATA_FORMAT         ((uint8_t)0x31)    // Data format control
#define ADXL345_REG_DATAX0              ((uint8_t)0x32)    // X-axis data 0
#define ADXL345_REG_DATAX1              ((uint8_t)0x33)    // X-axis data 1
#define ADXL345_REG_DATAY0              ((uint8_t)0x34)    // Y-axis data 0
#define ADXL345_REG_DATAY1              ((uint8_t)0x35)    // Y-axis data 1
#define ADXL345_REG_DATAZ0              ((uint8_t)0x36)    // Z-axis data 0
#define ADXL345_REG_DATAZ1              ((uint8_t)0x37)    // Z-axis data 1
#define ADXL345_REG_FIFO_CTL            ((uint8_t)0x38)    // FIFO control
#define ADXL345_REG_FIFO_STATUS         ((uint8_t)0x39)    // FIFO status

/*Used with register 0x2C (ADXL345_REG_BW_RATE) to set bandwidth--------------*/

#define ADXL345_DATARATE_3200_HZ    ((uint8_t)0x0F) 	// 1600Hz Bandwidth   140�A IDD
#define ADXL345_DATARATE_1600_HZ    ((uint8_t)0x0E) 	//  800Hz Bandwidth    90�A IDD
#define ADXL345_DATARATE_800_HZ     ((uint8_t)0x0D)		//  400Hz Bandwidth   140�A IDD
#define ADXL345_DATARATE_400_HZ     ((uint8_t)0x0C) 	//  200Hz Bandwidth   140�A IDD
#define ADXL345_DATARATE_200_HZ     ((uint8_t)0x0B) 	//  100Hz Bandwidth   140�A IDD
#define ADXL345_DATARATE_100_HZ     ((uint8_t)0x0A) 	//   50Hz Bandwidth   140�A IDD
#define ADXL345_DATARATE_50_HZ      ((uint8_t)0x09) 	//   25Hz Bandwidth    90�A IDD
#define ADXL345_DATARATE_25_HZ      ((uint8_t)0x08) 	// 12.5Hz Bandwidth    60�A IDD
#define ADXL345_DATARATE_12_5_HZ    ((uint8_t)0x07) 	// 6.25Hz Bandwidth    50�A IDD
#define ADXL345_DATARATE_6_25HZ     ((uint8_t)0x06) 	// 3.13Hz Bandwidth    45�A IDD
#define ADXL345_DATARATE_3_13_HZ    ((uint8_t)0x05) 	// 1.56Hz Bandwidth    40�A IDD
#define ADXL345_DATARATE_1_56_HZ    ((uint8_t)0x04) 	// 0.78Hz Bandwidth    34�A IDD
#define ADXL345_DATARATE_0_78_HZ    ((uint8_t)0x03) 	// 0.39Hz Bandwidth    23�A IDD
#define ADXL345_DATARATE_0_39_HZ    ((uint8_t)0x02) 	// 0.20Hz Bandwidth    23�A IDD
#define ADXL345_DATARATE_0_20_HZ    ((uint8_t)0x01) 	// 0.10Hz Bandwidth    23�A IDD
#define ADXL345_DATARATE_0_10_HZ    ((uint8_t)0x00)  	// 0.05Hz Bandwidth    23�A IDD (default value)

/*Definici�n de las funciones de la librer�a ADXL345----------------------------*/
void I2C_ReadAxis_ADXL345(void);
void UARTInterface (void);
void InterfaceOption (char y);
void TIMProgrammingOption(char CharIn);
char ReadingOption(char CharIn);
char WritingOption(char CharIn);

#endif 

