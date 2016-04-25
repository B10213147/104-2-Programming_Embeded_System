// Date:   18.9.2013
// Author: Janez Puhan

#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

// Bit masks in CCR
#define CLKEN	0x00000001
#define CTCRST	0x00000002
#define CLKSRC	0x00000010

// Bit masks in AMR
#define AMRSEC	0x00000001
#define AMRMIN	0x00000002
#define AMRHOUR	0x00000004
#define AMRDOM	0x00000008
#define AMRDOW	0x00000010
#define AMRDOY	0x00000020
#define AMRMON	0x00000040
#define AMRYEAR	0x00000080

// Register addresses
#define CCR		(*((volatile int *)0xe0024008))	// Clock Control
#define CIIR	(*((volatile int *)0xe002400c))	// Counter Increment Interrupt
#define AMR		(*((volatile int *)0xe0024010))	// Alarm Mask
#define CTIME0	(*((volatile int *)0xe0024014))	// Consolidated Time 0
#define CTIME1	(*((volatile int *)0xe0024018))	// Consolidated Time 1
#define CTIME2	(*((volatile int *)0xe002401c))	// Consolidated Time 2
#define SEC		(*((volatile int *)0xe0024020))	// Second
#define MIN		(*((volatile int *)0xe0024024))	// Minute
#define HOUR	(*((volatile int *)0xe0024028))	// Hour
#define DOM		(*((volatile int *)0xe002402c))	// Day of Month
#define DOW		(*((volatile int *)0xe0024030))	// Day of Week
#define DOY		(*((volatile int *)0xe0024034))	// Day of Year
#define MONTH	(*((volatile int *)0xe0024038))	// Month
#define YEAR	(*((volatile int *)0xe002403c))	// Year

// Function declarations
extern void rtc_init();
extern void set_time(int hour, int min, int sec);
extern void set_date(int day, int month, int year);
extern void get_time_and_date(int *hour, int *min, int *sec, int *day_of_week,
	int *day_of_year, int *day, int *month, int *year);

#endif
