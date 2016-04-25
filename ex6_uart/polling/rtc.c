// Date:   18.4.2008
// Author: Janez Puhan
// Warning: Real time clock works properly for years from 1901 to 2099.

#include "rtc.h"

char check_time(int *hour, int *min, int *sec)
{
	char changed = 0;
	if(*hour < 0 || *hour > 23)
	{
		*hour = 0;
		changed = 1;
	}
	if(*min < 0 || *min > 59)
	{
		*min = 0;
		changed = 1;
	}
	if(*sec < 0 || *sec > 59)
	{
		*sec = 0;
		changed = 1;
	}
	return changed;
}

void calculate_day_of_week_and_year(int *day_of_week, int *day_of_year, int day,
	int month, int year)
{
	int i;
	char days[11] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};
	for(i = 0, *day_of_year = day; i < month - 1; i = i + 1)
		*day_of_year = *day_of_year + days[i];
	if(month > 2 && !(year % 4)) *day_of_year = *day_of_year + 1;
	*day_of_week = ((year - 1901) * 365 + (year - 1901) / 4 + *day_of_year) % 7;
}

char check_date(int *day_of_week, int *day_of_year, int *day, int *month, int *year)
{
	int calculated_day_of_year, calculated_day_of_week;
	char changed = 0;
	if(*year < 1901 || *year > 2099)
	{
		*year = 1901;
		changed = 1;
	}
	if(*month < 1 || *month > 12)
	{
		*month = 1;
		changed = 1;
	}
	if(*month == 2 && (*year % 4 && *day > 28 || *day > 29) ||
		(*month == 4 || *month == 6 || *month == 9 || *month == 11) && *day > 30)
	{
		*day = 1;
		changed = 1;
	}
	calculate_day_of_week_and_year
		(&calculated_day_of_week, &calculated_day_of_year, *day, *month, *year);
	if(calculated_day_of_year != *day_of_year)
	{
		*day_of_year = calculated_day_of_year;
		changed = 1;
	}
	if(calculated_day_of_week != *day_of_week)
	{
		*day_of_week = calculated_day_of_week;
		changed = 1;
	}
	return changed;
}

void set_checked_date(int day_of_week, int day_of_year, int day, int month,
	int year)
{
	CCR = CCR & ~CLKEN;
	DOW = day_of_week;
	DOM = day;
	DOY = day_of_year;
	MONTH = month;
	YEAR = year;	
	CCR = CCR | CLKEN;
}

// Initialize Real Time Clock
void rtc_init()
{
	int hour, min, sec, day_of_week, day_of_year, day, month, year;
	CIIR = 0x00000000;
	AMR = AMRSEC | AMRMIN | AMRHOUR | AMRDOM | AMRDOW | AMRDOY | AMRMON | AMRYEAR;
	CCR = CLKEN | CLKSRC;
	get_time_and_date
		(&hour, &min, &sec, &day_of_week, &day_of_year, &day, &month, &year);
	if(check_time(&hour, &min, &sec)) set_time(hour, min, sec);
	if(check_date(&day_of_week, &day_of_year, &day, &month, &year))
		set_checked_date(day_of_week, day_of_year, day, month, year);
}

// Set time of Real Time Clock
// hour ... hours [0-23]
// min  ... minutes [0-59]
// sec  ... seconds [0-59]
void set_time(int hour, int min, int sec)
{
	CCR = CCR & ~CLKEN | CTCRST;
	HOUR = hour;
	MIN = min;
	SEC = sec;
	CCR = CCR & ~CTCRST | CLKEN;
}

// Set date of Real Time Clock
// day   ... day of month [1-28,29,30,31]
// month ... month [1-12]
// year  ... year [1901-2099]
void set_date(int day, int month, int year)
{
	int day_of_week, day_of_year;
	calculate_day_of_week_and_year(&day_of_week, &day_of_year, day, month, year);
	set_checked_date(day_of_week, day_of_year, day, month, year);
}

// Get current time and date of Real Time Clock
// hour        ... hours
// min         ... minutes
// sec         ... seconds
// day_of_week ... day of week (0 = monday, 1 = tuesday, ... 6 = sunday)
// day_of_year ... day of year
// day         ... day of month
// month       ... month
// year        ... year
void get_time_and_date(int *hour, int *min, int *sec, int *day_of_week,
	int *day_of_year, int *day, int *month, int *year)
{
	int ctime0, ctime1[2], ctime2;
	do
	{
		ctime1[0] = CTIME1;
		ctime0 = CTIME0;
		ctime2 = CTIME2;
		ctime1[1] = CTIME1;
	} while(ctime1[0] != ctime1[1]);
	*hour = (ctime0 & 0x001f0000) >> 16;
	*min = (ctime0 & 0x00003f00) >> 8;
	*sec = ctime0 & 0x0000003f;
	*day_of_week = (ctime0 & 0x07000000) >> 24;
	*day_of_year = ctime2 & 0x000001ff;
	*day = ctime1[0] & 0x0000001f;
	*month = (ctime1[0] & 0x00000f00) >> 8;
	*year = (ctime1[0] & 0x0fff0000) >> 16;
}
