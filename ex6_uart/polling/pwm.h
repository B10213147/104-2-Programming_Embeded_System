// Date:   18.9.2013
// Author: Janez Puhan

#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

#include "gpio.h"

// Bit masks in PINSEL0 register
#define pinsel0_pwm_mask	0x000a800a
#define pwm1				0x00000003
#define pwm2				0x0000c000
#define pwm3				0x0000000c
#define pwm4				0x00030000
#define pwm6				0x000c0000

// Bit masks in PINSEL1 register
#define pinsel1_pwm_mask	0x00000400
#define pwm5				0x00000c00

// Bit masks in PWMIR register
#define pwmmr0_interrupt	0x00000001
#define pwmmr1_interrupt	0x00000002
#define pwmmr2_interrupt	0x00000004
#define pwmmr3_interrupt	0x00000008
#define pwmmr4_interrupt	0x00000010
#define pwmmr5_interrupt	0x00000020
#define pwmmr6_interrupt	0x00000040

// Bit masks in PWMTCR register
#define pwm_reset			0x00000002
#define pwm_enable			0x00000009

// Bit masks in PWMMCR register
#define pwmmr0i				0x00000001
#define pwmmr0r				0x00000002
#define pwmmr0s				0x00000004
#define pwmmr1i				0x00000008
#define pwmmr1r				0x00000010
#define pwmmr1s				0x00000020
#define pwmmr2i				0x00000040
#define pwmmr2r				0x00000080
#define pwmmr2s				0x00000100
#define pwmmr3i				0x00000200
#define pwmmr3r				0x00000400
#define pwmmr3s				0x00000800
#define pwmmr4i				0x00001000
#define pwmmr4r				0x00002000
#define pwmmr4s				0x00004000
#define pwmmr5i				0x00008000
#define pwmmr5r				0x00010000
#define pwmmr5s				0x00020000
#define pwmmr6i				0x00040000
#define pwmmr6r				0x00080000
#define pwmmr6s				0x00100000

// Bit masks in PWMPCR register
#define pwmsel2				0x00000004
#define pwmsel3				0x00000008
#define pwmsel4				0x00000010
#define pwmsel5				0x00000020
#define pwmsel6				0x00000040
#define pwmena1				0x00000200
#define pwmena2				0x00000400
#define pwmena3				0x00000800
#define pwmena4				0x00001000
#define pwmena5				0x00002000
#define pwmena6				0x00004000

// Register addresses
#define PWMIR	(*((volatile int *)0xe0014000)) // PWM Interrupt
#define PWMTCR	(*((volatile int *)0xe0014004)) // PWM Timer Control
#define PWMTC	(*((volatile int *)0xe0014008))	// PWM Counter
#define PWMPR	(*((volatile int *)0xe001400c))	// PWM Prescale
#define PWMMCR	(*((volatile int *)0xe0014014))	// PWM Match Control
#define PWMMR0	(*((volatile int *)0xe0014018))	// PWM Matches
#define PWMMR1	(*((volatile int *)0xe001401c))
#define PWMMR2	(*((volatile int *)0xe0014020))
#define PWMMR3	(*((volatile int *)0xe0014024))
#define PWMMR4	(*((volatile int *)0xe0014040))
#define PWMMR5	(*((volatile int *)0xe0014044))
#define PWMMR6	(*((volatile int *)0xe0014048))
#define PWMPCR	(*((volatile int *)0xe001404c))	// PWM Control
#define PWMLER	(*((volatile int *)0xe0014050))	// PWM Latch Enable

// Function declarations
extern void pwm_init(int output, int prescale, int *match, int match_control,
	int edge_control);

#endif
