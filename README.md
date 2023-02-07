# STM32F407VG_PCA21125_SPI_RTC
This repository contains the source code and documentation for interfacing the PCA21125 SPI Real-Time Clock (RTC) with the STM32F407 microcontroller.

## Overview
The PCA21125 is a low-power RTC with a SPI interface, providing real-time clock and calendar functions. The STM32F407 is a popular high-performance microcontroller from STMicroelectronics, with a wide range of peripherals and features. This project demonstrates how to interface the PCA21125 RTC with the STM32F407 using the SPI peripheral, to accurately keep track of the current date and time.

## Requirements
* STM32F407 development board
* PCA21125 RTC module
* STM32 development environment (STM32CubeIDE)
* PCA21125 datasheet and STM32F407 datasheet
* The STM32CubeMX software, available [here](https://www.st.com/en/development-tools/stm32cubemx.html).

## Installation
1. Clone the repository:
git clone [here](https://github.com/Mahmoud-Sharabati/STM32F407VG_PCA21125_SPI_RTC.git).
2. Import the project into STM32CubeMX by selecting File > Import > C/C++ > Existing Code as Makefile Project.
3. Configure the project in STM32CubeMX according to your hardware setup.
4. Generate the code and open the project in your preferred development environment.
5. Build and upload the code to your device.

**OR**: you can use this example by modifying the SPI Handler and Chip-Select pin as your connection as follow.

## Usage

### Before using SPI CAN Driver
1. Modify the Hardware Configuration and Pins using CubeMX software
    This driver use SPI1 with the following pins:
    | SPI1_CS       | SPI1_SCK    | SPI1_MISO   |SPI1_MOSI   |
    | ------------- |:-----------:|:-----------:|:----------:|
    | PC5	    | PA5	  | PA6        | PA7	     |

	#### Notes:																	
	1. You need to modify the name of "Chip-Select pin CS" on your project to be "PCA21125_SPI_CS".
	2. You must modify the SPI parameters as shown in image below.  
	#
	![SPI_RTC_Config](https://user-images.githubusercontent.com/16566502/217271789-834afec2-b3bb-41af-882d-fa5258ebfe6b.png)  
	#
	3. Other parameters will be automatically modified when generates the code.

2. This Board has an optional LED and Buzzer with the following pins:
   | LED	| Buzzer  |
   |------------|---------|
   | PB0	| PE2	  |

 ### Use INA229 Driver instruction
1. Define your INA299_Readings variable to be used to store the INA229 readings. The driver project uses variable nemed "INA299_Values"

2. Initialize the INA229 driver using: INA229_Init(SPI_HandleTypeDef * hspi, TIM_HandleTypeDef *htim)

	Parameters:
   | hspi | htim |
   |------------|--------- |
   | Connected SPI_HandleTypeDef | Used TIM_HandleTypeDef for delay |
   
3. Get the INA229 readings by Get_INA299_Readings() function. The diver project uses the "INA299_Values" variable to store reading values into. 

_NOTE: The INA229 readings in this project can be monitored using the STM32 debugger.

## Credits
This driver is based on the generated code by the SysConfig tool for the TI Sensors. It can be found [here](https://www.ti.com/tool/SYSCONFIG).

## License
This driver is licensed under the MIT License. See the LICENSE file for details.
