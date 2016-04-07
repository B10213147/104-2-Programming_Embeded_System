// Date:   15.10.2007
// Author: Janez Puhan

typedef void (* voidfuncptr)();

// Interrupts
#define	wdt			0x00000001
#define	arm_core0	0x00000004
#define	arm_core1	0x00000008
#define	timer0		0x00000010
#define	timer1		0x00000020
#define	uart0		0x00000040
#define	uart1		0x00000080
#define	pwm0		0x00000100
#define	i2c0		0x00000200
#define	spi0		0x00000400
#define	spi1_ssp	0x00000800
#define	pll			0x00001000
#define	rtc			0x00002000
#define	eint0		0x00004000
#define	eint1		0x00008000
#define	eint2		0x00010000
#define	eint3		0x00020000
#define	ad0			0x00040000
#define	i2c1		0x00080000
#define	bod			0x00100000
#define	ad1			0x00200000

// Register addresses
#define VICIntSelect	(*((volatile int *)0xfffff00c))	// Interrupt Select
#define VICIntEnable	(*((volatile int *)0xfffff010))	// Interrupt Enable
#define VICIntEnClear	(*((volatile int *)0xfffff014))	// Interrupt Disable
#define VICVectAddr		(*((volatile int *)0xfffff030))	// Vector Address
#define VICDefVectAddr	(*((volatile int *)0xfffff034))	// Default Vector Address
#define VICVectAddr0	(*((volatile int *)0xfffff100))	// Vector Addresses
#define VICVectAddr1	(*((volatile int *)0xfffff104))
#define VICVectAddr2	(*((volatile int *)0xfffff108))
#define VICVectAddr3	(*((volatile int *)0xfffff10c))
#define VICVectAddr4	(*((volatile int *)0xfffff110))
#define VICVectAddr5	(*((volatile int *)0xfffff114))
#define VICVectAddr6	(*((volatile int *)0xfffff118))
#define VICVectAddr7	(*((volatile int *)0xfffff11c))
#define VICVectAddr8	(*((volatile int *)0xfffff120))
#define VICVectAddr9	(*((volatile int *)0xfffff124))
#define VICVectAddr10	(*((volatile int *)0xfffff128))
#define VICVectAddr11	(*((volatile int *)0xfffff12c))
#define VICVectAddr12	(*((volatile int *)0xfffff130))
#define VICVectAddr13	(*((volatile int *)0xfffff134))
#define VICVectAddr14	(*((volatile int *)0xfffff138))
#define VICVectAddr15	(*((volatile int *)0xfffff13c))
#define VICVectCntl0	(*((volatile int *)0xfffff200))	// Vector Controls
#define VICVectCntl1	(*((volatile int *)0xfffff204))
#define VICVectCntl2	(*((volatile int *)0xfffff208))
#define VICVectCntl3	(*((volatile int *)0xfffff20c))
#define VICVectCntl4	(*((volatile int *)0xfffff210))
#define VICVectCntl5	(*((volatile int *)0xfffff214))
#define VICVectCntl6	(*((volatile int *)0xfffff218))
#define VICVectCntl7	(*((volatile int *)0xfffff21c))
#define VICVectCntl8	(*((volatile int *)0xfffff220))
#define VICVectCntl9	(*((volatile int *)0xfffff224))
#define VICVectCntl10	(*((volatile int *)0xfffff228))
#define VICVectCntl11	(*((volatile int *)0xfffff22c))
#define VICVectCntl12	(*((volatile int *)0xfffff230))
#define VICVectCntl13	(*((volatile int *)0xfffff234))
#define VICVectCntl14	(*((volatile int *)0xfffff238))
#define VICVectCntl15	(*((volatile int *)0xfffff23c))

// Function declaration
extern void vic_init(int fiq, int irq, voidfuncptr *function, int *interrupt,
	voidfuncptr def);
