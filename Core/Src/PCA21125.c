/*
 * PCA21125.c
 *
 *  Created on: Aug 31, 2022
 *      Author: Mahmoud A. Sharabati
 */

#include "PCA21125.h"

#define CHECK_SEC_MIN_RANGE(X) 	(X > 59) ? X = 59 : ((X < 0)? X = 0 : X)
#define CHECK_HOURS_RANGE(X) 	(X > 23) ? X = 23 : ((X < 0)? X = 0 : X)
#define CHECK_DAYS_RANGE(X) 	(X > 31) ? X = 31 : ((X < 1)? X = 1 : X)
#define CHECK_MONTHS_RANGE(X) 	(X > 12) ? X = 12 : ((X < 1)? X = 1 : X)
#define CHECK_YEARS_RANGE(X) 	(X > 30) ? X = 30 : ((X < 23)? X = 23 : X)

SPI_HandleTypeDef *PCA21125_SPI;

void PCA21125RTC_Init(SPI_HandleTypeDef *hspi, ITStatus INT_Status) {

	PCA21125_SPI = hspi;

	/* Write control registers */
	uint8_t TxADCBuff[3] = {0};
	if (INT_Status) {
		/* Enable the INT pin on each second */
		TxADCBuff[2] = (uint8_t) (CONTROL_2_REGISTER_VALUE);	// INT on each second (ref. Table 7 Page 7)
	}
	else {
		/* Disable the INT pin */
		TxADCBuff[2] = (uint8_t) (0x00);
	}
	/* Fill the SPI data values */
	TxADCBuff[0] = (uint8_t) (WRITE_CONTROL_1_REGESTER_ID);
	TxADCBuff[1] = (uint8_t) (CONTROL_1_REGISTER_VALUE);

	/* Pull the CS pin to low */
	HAL_GPIO_WritePin(PCA21125_SPI_CS_GPIO_Port, PCA21125_SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

	/* Start Transmission process */
	HAL_GPIO_WritePin(PCA21125_SPI_CS_GPIO_Port, PCA21125_SPI_CS_Pin, GPIO_PIN_SET);
	if (HAL_SPI_Transmit(PCA21125_SPI,TxADCBuff, 3, 100) != HAL_OK) {
		Error_Handler();
	}
	HAL_GPIO_WritePin(PCA21125_SPI_CS_GPIO_Port, PCA21125_SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
}

void PCA21125RTC_WriteValue(RTC_Data* _rtc_data) {
	/* Write Time and Date registers */
	uint8_t TxADCBuff[8] = {0};

	/* Fill the SPI data values */
	/* Start with the seconds register */
	TxADCBuff[0] = (uint8_t) (WRITE_SECONDS_REGESTER_ID);
	TxADCBuff[1] = (uint8_t) (_rtc_data->Seconds);
	TxADCBuff[2] = (uint8_t) (_rtc_data->Minutes);
	TxADCBuff[3] = (uint8_t) (_rtc_data->Hours);
	TxADCBuff[4] = (uint8_t) (_rtc_data->Days);
	TxADCBuff[5] = (uint8_t) (CURRENT_BITS_WEEKDAY);
	TxADCBuff[6] = (uint8_t) (_rtc_data->Month);
	TxADCBuff[7] = (uint16_t) (_rtc_data->Year);

	/* Start Transmission process */
	HAL_GPIO_WritePin(PCA21125_SPI_CS_GPIO_Port, PCA21125_SPI_CS_Pin, GPIO_PIN_SET);
	if (HAL_SPI_Transmit(PCA21125_SPI, TxADCBuff, 8, 100) != HAL_OK) {
		Error_Handler();
	}
	HAL_GPIO_WritePin(PCA21125_SPI_CS_GPIO_Port, PCA21125_SPI_CS_Pin, GPIO_PIN_RESET);
}

void RTC_SetTime(RTC_Data* _rtc_data) {
	RTC_Data _rtc_set_data;

	/* Check the time range */	//OK
	_rtc_data->Seconds = CHECK_SEC_MIN_RANGE(_rtc_data->Seconds);
	_rtc_data->Minutes = CHECK_SEC_MIN_RANGE(_rtc_data->Minutes);
	_rtc_data->Hours   = CHECK_HOURS_RANGE(_rtc_data->Hours);
	_rtc_data->Days    = CHECK_DAYS_RANGE(_rtc_data->Days);
	_rtc_data->Month   = CHECK_MONTHS_RANGE(_rtc_data->Month);
	_rtc_data->Year    = CHECK_YEARS_RANGE(_rtc_data->Year);

	/* Convert time to BCD data */
	_rtc_set_data.Seconds = (((_rtc_data->Seconds/10) << 4) &0xF0) | ((_rtc_data->Seconds%10) & 0x0F);
	_rtc_set_data.Minutes = (((_rtc_data->Minutes/10) << 4) &0xF0) | ((_rtc_data->Minutes%10) & 0x0F);
	_rtc_set_data.Hours   = (((_rtc_data->Hours/10) << 4) &0xF0) | ((_rtc_data->Hours%10) & 0x0F);
	_rtc_set_data.Days	  = (((_rtc_data->Days/10) << 4) &0xF0) | ((_rtc_data->Days%10) & 0x0F);
	_rtc_set_data.Month   = (((_rtc_data->Month/10) << 4) &0xF0) | ((_rtc_data->Month%10) & 0x0F);
	_rtc_set_data.Year    = (((_rtc_data->Year/10) << 4) &0xF0) | ((_rtc_data->Year%10) & 0x0F);
	/* Write RTC data */
	PCA21125RTC_WriteValue(&_rtc_set_data);

}

void PCA21125RTC_GetTime(RTC_Data* _rtc_data) {
	uint8_t RxADCBuff[7]={0};
	uint8_t TxRTCBuff[1];
	TxRTCBuff[0]=READ_SECONDS_REGESTER_ID;

	/* Read Time_Date registers */

	/* Pull the CS high */
	HAL_GPIO_WritePin(PCA21125_SPI_CS_GPIO_Port, PCA21125_SPI_CS_Pin, GPIO_PIN_SET);

	if (HAL_SPI_Transmit(PCA21125_SPI, TxRTCBuff , 1, 100) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_SPI_Receive(PCA21125_SPI,RxADCBuff, 7, 100) != HAL_OK) {
		Error_Handler();
	}
	HAL_GPIO_WritePin(PCA21125_SPI_CS_GPIO_Port, PCA21125_SPI_CS_Pin, GPIO_PIN_RESET);

	_rtc_data->Seconds = (RxADCBuff[0]);
	_rtc_data->Minutes = (RxADCBuff[1]);
	_rtc_data->Hours = (RxADCBuff[2]);
	_rtc_data->Days =  (RxADCBuff[3]);
	_rtc_data->Weekday = (RxADCBuff[4]);
	_rtc_data->Month = (RxADCBuff[5]);
	_rtc_data->Year = (RxADCBuff[6]);

	/* Convert the RTC data from RTC_BCD to Readable time */
	RTC2ValidTime(_rtc_data);
}

uint8_t bcd2bin(uint8_t a) {
	return (uint8_t) (((a & 0xf000) >> 12) * 1000 + ((a & 0xf00) >> 8) * 100 + ((a & 0xf0) >> 4) * 10 + (a & 0xf));
}

void RTC2ValidTime(RTC_Data* _rtc_data) {
	_rtc_data->Seconds = (bcd2bin((_rtc_data->Seconds&0x70) >> 4) *10) + bcd2bin(_rtc_data->Seconds&0x0F);
	_rtc_data->Minutes = (bcd2bin((_rtc_data->Minutes&0x70) >> 4) *10) + bcd2bin(_rtc_data->Minutes&0x0F);
	_rtc_data->Hours   = (bcd2bin((_rtc_data->Hours&0x30) >> 4) *10) + bcd2bin(_rtc_data->Hours&0x0F);
	_rtc_data->Days    = (bcd2bin((_rtc_data->Days&0x30) >> 4) *10) + bcd2bin(_rtc_data->Days&0x0F);
	_rtc_data->Month   = (bcd2bin((_rtc_data->Month&0x10) >> 4) *10) + bcd2bin(_rtc_data->Month&0x0F);
	_rtc_data->Year    = (bcd2bin((_rtc_data->Year&0xF0) >> 4) *10) + bcd2bin(_rtc_data->Year&0x0F);
}
