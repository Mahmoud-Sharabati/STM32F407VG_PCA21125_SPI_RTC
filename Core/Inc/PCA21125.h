/*
 * PCA21125.h
 *
 *  Created on: Aug 31, 2022
 *      Author: Mahmoud A. Sharabati
 */

#ifndef PCA21125_H_
#define PCA21125_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private macro -------------------------------------------------------------*/
/*************************_Control Register IDs_ *************************/
#define WRITE_CONTROL_1_REGESTER_ID		(0b00010000)
#define READ_CONTROL_1_REGESTER_ID		(0b10010000)
#define WRITE_CONTROL_2_REGESTER_ID		(0b00010001)
#define READ_CONTROL_2_REGESTER_ID		(0b10010001)

/********************_Control Register Values_ *******************/
#define CONTROL_1_REGISTER_VALUE		(0b00001000)
#define CONTROL_2_REGISTER_VALUE		(0b01010000)	// This value enable the INT pin on each second

/**********************_Time and Date Registers_ **********************/
#define READ_SECONDS_REGESTER_ID		(0b10010010)
#define WRITE_SECONDS_REGESTER_ID		(0b00010010)
#define MINUTES_REGESTER_ID				(0x03)
#define HOURS_REGESTER_ID				(0x04)
#define DAYS_REGESTER_ID				(0x05)
#define WEEKDAY_REGESTER_ID				(0x06)
#define MONTHS_REGESTER_ID				(0x07)
#define YEARS_REGESTER_ID				(0x08)

/*************************_Alarm Registers_ **************************/
#define MINUTES_ALARM_REGESTER_ID		(0x09)
#define HOUR_ALARM_REGESTER_ID			(0x0A)
#define DAY_ALARM_REGESTER_ID			(0x0B)
#define WEEKDAY_ALARM_REGESTER_ID		(0x0C)

/*********************_CLKOUT Control Registers_ *********************/
#define CLKOUT_CONTROL_REGESTER_ID		(0x0D)

/*************************_Timer Registers_ *************************/
#define TIMER_CONTROL_REGESTER_ID		(0x0E)
#define COUNTDOWN_DOWN_REGESTER_ID		(0x0F)

/********************_RTC Initial/Testing Current Values_ *******************/
#define CURRENT_BCD_SECONDS				(0b10010000)			//bit 7 Clock Integrity "1 default", bits 6:4 Tens"0:5", bits 3:0 Unit"0:9"
#define CURRENT_BCD_MINUTES				(0b01010000)			//bit 7 Unused "0", bits 6:4 Tens"0:5", bits 3:0 Unit"0:9"
#define CURRENT_BCD_HOURS				(0b00010111)			//bits 7:6 Unused "00", bits 5:4 Tens"0:2", bits 3:0 Unit"0:9" (Note: 24 Mode is considered)
#define CURRENT_BCD_DAYS				(0b00000001)			//bits 7:6 Unused "00", bits 5:4 Tens"0:3", bits 3:0 Unit"0:9"
#define CURRENT_BITS_WEEKDAY			(0b00000000)			//bits 7:3 Unused "00000", bits 2:0 Weekday"0:6" start by Sunday(0) and end by Saturday(6)
#define CURRENT_BCD_MONTHS				(0b00001000)			//bits 7:5 Unused "000", bit 4 Tens"0:1", bits 3:0 Unit"0:9"
#define CURRENT_BCD_YEARS				(0b00100010)			//bits 7:4 Tens"0:9", bits 3:0 Unit"0:9"

/* Private defines -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private TypeDef -----------------------------------------------------------*/
typedef struct {
		uint8_t Seconds;
		uint8_t Minutes;
		uint8_t Hours;
		uint8_t Days;
		uint8_t Weekday;
		uint8_t Month;
		uint8_t Year;
} RTC_Data;

/* Private function prototypes -----------------------------------------------*/
void PCA21125RTC_Init(SPI_HandleTypeDef *hspi, ITStatus INT_Status);
void PCA21125RTC_WriteValue(RTC_Data* _rtc_data);
void PCA21125RTC_SetTime();
void PCA21125RTC_GetTime(RTC_Data* _rtc_data);
uint8_t bcd2bin(uint8_t a);
void RTC2ValidTime(RTC_Data*);
void RTC_SetTime(RTC_Data* _rtc_data);

#endif /* PCA21125_H_ */
