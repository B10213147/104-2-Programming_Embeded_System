// Date:   7.4.2009
// Author: Janez Puhan

#include "pwm.h"

// Reset and configure PWM
// output        ... enabled outputs [pwmenax, where x = 1, 2, 3, 4, 5 or 6]
// prescale      ... maximum prescale counter value
// match         ... array of match values
// match_control ... match control [any combination of 0x00000000, pwmmrxi, pwmmrxr
//                   and pwmmrxs, where x = 0, 1, 2, 3, 4, 5 or 6]
// edge_control  ... double edge controlled channels [pwmselx, where x = 2, 3, 4, 5
//                   or 6]
// Note: if x-th channel is
//    single edge controlled -> set at PWMTC = PWMMR0,     reset at PWMTC = PWMMRx
//    double edge controlled -> set at PWMTC = PWMMR(x-1), reset at PWMTC = PWMMRx
// Note: PWMTC is reset at PWMTC = PWMMR0
void pwm_init(int output, int prescale, int *match, int match_control, int edge_control)
{
	PWMPCR = edge_control;
	PWMTCR = pwm_reset;
	PWMIR = pwmmr0_interrupt | pwmmr1_interrupt | pwmmr2_interrupt |
		pwmmr3_interrupt | pwmmr4_interrupt | pwmmr5_interrupt | pwmmr6_interrupt;
	PWMPR = prescale;
	PWMLER = 0x00000000;
	PWMMR0 = match[0];
	PWMMR1 = match[1];
	PWMMR2 = match[2];
	PWMMR3 = match[3];
	PWMMR4 = match[4];
	PWMMR5 = match[5];
	PWMMR6 = match[6];
	PWMMCR = match_control;
	if(output & pwmena1) PINSEL0 = (PINSEL0 & ~pwm1) | (pinsel0_pwm_mask & pwm1);
	if(output & pwmena2) PINSEL0 = (PINSEL0 & ~pwm2) | (pinsel0_pwm_mask & pwm2);
	if(output & pwmena3) PINSEL0 = (PINSEL0 & ~pwm3) | (pinsel0_pwm_mask & pwm3);
	if(output & pwmena4) PINSEL0 = (PINSEL0 & ~pwm4) | (pinsel0_pwm_mask & pwm4);
	if(output & pwmena5) PINSEL1 = (PINSEL1 & ~pwm5) | (pinsel1_pwm_mask & pwm5);
	if(output & pwmena6) PINSEL0 = (PINSEL0 & ~pwm6) | (pinsel0_pwm_mask & pwm6);
	PWMPCR = PWMPCR | output;
}
