// Date:   18.9.2013
// Author: Janez Puhan

#ifndef ADC_DAC_H_INCLUDED
#define ADC_DAC_H_INCLUDED

#include "gpio.h"
#include "pll.h"

// Bit masks in PINSEL0 register
#define pinsel0_adc_mask	0xcf333f00
#define adc0_6				0x00000300
#define adc0_7				0x00000c00
#define adc1_0				0x00003000
#define adc1_1				0x00030000
#define adc1_2				0x00300000
#define adc1_3				0x03000000
#define adc1_4				0x0c000000
#define adc1_5				0xc0000000

// Bit masks in PINSEL1 register
#define pinsel1_adc_mask	0x15541800
#define adc0_0				0x00c00000
#define adc0_1				0x03000000
#define adc0_2				0x0c000000
#define adc0_3				0x30000000
#define adc0_4				0x000c0000
#define adc0_5				0x00300000
#define adc1_6				0x00000c00
#define adc1_7				0x00003000
#define dac_pins			0x000c0000
#define dac_value			0x00080000

// Bit masks in AD0CR and AD1CR registers
#define adc_0				0x00000001
#define adc_1				0x00000002
#define adc_2				0x00000004
#define adc_3				0x00000008
#define adc_4				0x00000010
#define adc_5				0x00000020
#define adc_6				0x00000040
#define adc_7				0x00000080
#define burst				0x00010000
#define pdn					0x00200000
#define start_now			0x01000000

// Bit masks in AD0DR and AD1DR registers
#define ls					0x0000ffc0
#define done				0x80000000

// Register addresses
#define AD0CR	(*((volatile int *)0xe0034000)) // A/D Control
#define AD0DR	(*((volatile int *)0xe0034004)) // A/D Data
#define AD1CR	(*((volatile int *)0xe0060000)) // A/D Control
#define AD1DR	(*((volatile int *)0xe0060004)) // A/D Data
#define DACR	(*((volatile int *)0xe006c000)) // D/A Convert

// Function declarations
extern void adc0_init(int adc0_pins0, int adc0_pins1, int mode);
extern void adc1_init(int adc1_pins0, int adc1_pins1, int mode);
extern void dac_init();

#endif
