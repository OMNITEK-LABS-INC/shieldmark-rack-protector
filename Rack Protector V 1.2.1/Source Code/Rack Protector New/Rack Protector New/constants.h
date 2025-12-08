//Delete what you dont need

#include <xc.h>

#ifndef CONSTANTS_H
#define	CONSTANTS_H



/******************************PERIPHERAL SETUP********************************/
/******************************************************************************/

/*****************************OSCILLATOR CONTROL*******************************/
//LFINTOSC divided by 1
#define OSCCON1_LFINTOSC                0b01010000

//HFINTOSC
#define OSCCON1_HFINTOSC                0b01100000

//OSCFRQ - set HFINTOSC to 16MHZ
#define OSCFRQ_LOAD_16MHZ               0b00000101

//OSCFRQ - set HFINTOSC to 1MHZ
#define OSCFRQ_LOAD_1MHZ                0b00000000

//OSCFRQ - set HFINTOSC to 4MHZ
#define OSCFRQ_LOAD_4MHZ                0b00000010

//OSCFRQ - set HFINTOSC to 32MHZ
#define OSCFRQ_LOAD_32MHZ                0b00000110



/*********************************ADC SETUP************************************/
//Right justified result, clock is Fosc
#define ADCON0_LOAD                     0b00000100

//ADCON1
#define ADCON1_LOAD                     0b00000000

//ADC Clock divider for 4MHZ clock (Fosc/4)
#define ADCLK_LOAD_4MHZ                 0b00000001   //Fosc/4

//ADACQ acquisition clocks (sampling clocks)
#define ADACQ_LOAD_VSS                  100//35//25

//ADACQ acquisition clocks (sampling clocks)
#define ADACQ_LOAD_IR                   100//35//25

//ADACQ acquisition clocks (sampling clocks)
#define ADACQ_LOAD_VPP                  100//100//35//25

//ADACQ acquisition clocks (sampling clocks)
#define ADACQ_LOAD_BATTERY              100//35//25

//ADACQ acquisition clocks (sampling clocks)
#define ADACQ_LOAD_IR                   100//35//25


//ADCON2
// Legacy mode, one sample for conversion
#define ADCON2_LOAD                     0b00000000

//ADCON3
#define ADCON3_LOAD                     0b00000000

//ADREF - adc reference is fixed voltage reference
#define ADREF_LOAD_FVR                  0b00000011

//ADRPT
// Not used in legacy mode
#define ADRPT_LOAD                      0

//FVRCON - Fixed voltage reference set to 2.048V
#define FVRCON_ON_LOAD                  0b10000010
#define FVRCON_OFF_LOAD                 0



/********************************TMR0 SETUP************************************/
// TMR0H is used as the compare register and is set to 242.  With 8-bit counting, 
// 31000/242 = 128 divider for 1 second.  For 250ms the divider is 32.  Prescaler 
// is set to 32 and postscaler is set to 1 for a divider of 32.

//T0CON0 - postscaler set to 1, 8-bit timer mode, will be enabled in code
#define T0CON0_LOAD                     0b00000000

//T0CON1 - prescaler set to 32, LFINTOSC is clock source, asynchronous
#define T0CON1_250MS                    0b10010101  

//TMR0H_LOAD - set to 242 for the comparison with TMR0l for a rollover
#define TMR0H_LOAD                      242

//Intervals per second.  Not used for timer setup, but used for other variable
// interval counting
#define INTERVALS_PER_SECOND            4



/*********************************TMR2 SETUP***********************************/
//Timer2 is used to generate the period timer for the CCP2 PWM that will feed
// the motor.  The clock speed is 32MHZ, so the timer is fed 8MHZ (Fosc/4).
// The bottom two bits are part of the instruction cycle running at 32MHZ.
// With 10-bit PWM resolution, the period needs to be 1024 counts.  Each count is 
// 1/32MHZ (0.03125us).  1024 x 0.03125us = 32us.  We need a period of 64us to
// get a PWM frequency of 15.625KHZ.  

//The timer needs to roll over every 64us.  The timer input clock is Fosc/4.
// The timer counts from 0 to 255 then rolls over.  The frequency at the timer is
// 32MHZ/4 = 8MHZ (0.125us).  The clock needs to be divided by two.

//T2CLKCON - TMR2 clock is Fosc/4
#define T2CLKCON_LOAD                   0b00000001

//T2CON - prescaler is 2, postscaler is 1, timer will be turned on in code
#define T2CON_LOAD                      0b00010000

//T2HLT - software gate, timer ON synchronized to Fosc/4
#define T2HLT_LOAD                      0b00100000

//T2PR2
#define T2PR_LOAD                       255



/***********************************TMR4 SETUP*********************************/
//Timer4 is used to generate a periodic 500us interrupt.  The clock speed is 8MHZ 
// to the timer (Fosc/4) which is a 0.125us period.  If PR4 is set to 250, then
// prescaler and postscaler need to be 0.5ms/(250 * 0.125us) = 16.    

//T4CLKCON - TMR4 clock is Fosc/4
#define T4CLKCON_LOAD                   0b00000001

//T4CON - prescaler is 16, postscaler is 1, timer will be turned on in code
#define T4CON_LOAD                      0b01000000

//T4HLT - free run mode, timer synchronized to Fosc/4
#define T4HLT_LOAD                      0b00100000

//T4PR
#define T4PR_LOAD                       250



/***********************************TMR6 SETUP*********************************/
//This timer is used to time the LED flashes and sequences.  It runs at the 4MHZ clock speed.  
// The frequency at the counter is 1MHZ (Fosc/4 or a 1us period.  The timeout period 
// is 2ms, so the total count is 2000.  The period is 250, so the total divider is 8.    

//T6CLKCON - TMR6 clock is Fosc/4
#define T6CLKCON_LOAD                   0b00000001

//T6CON - prescaler is 8, postscaler is 1, timer will be turned on in code
#define T6CON_LOAD                      0b00110000

//T6HLT - free run mode, timer synchronized to Fosc/4
#define T6HLT_LOAD                      0b00100000

//T4PR
#define T6PR_LOAD                       250





/***********************************CCP1 SETUP*********************************/
//CCP1CON - module is disabled (will be enabled in software), right alignment,
// capture mode every rising edge
#define CCP1CON_LOAD                    0b00000101

//CCPTMRS - CCP1 is working with TMR1 for capture
// Already shown in the CCP2 setup 

//CCP1CAP - CC1 PPS is the input for the CCP1 capture
#define CCP1CAP_LOAD                    0b00000000



/********************************TMR1 SETUP************************************/
//Timer1 is used to capture the rising edge from the CCP1 input.  The clock 
// speed is 8MHZ to the timer (Fosc/4) which is a 0.125us period.  The timer is 
// counting at a 0.125us rate. If the motor is running at 1700 RPM, with an 
// 8-window encoder wheel, the pulses per second is 1700 x 8/60 = 226.67.  
// This equates to a time of 1/226.67 = 4.41ms between pulses.

//T1CON - prescaler is 8, all 16 bits can be read at once, timer will be turned 
//on in code
#define T1CON_LOAD                      0b00110010

//T1CLK - Clock select is Fosc/4
#define TMR1CLK_LOAD                    0b00000001



/**********************************SPI2 CONSTANTS******************************/
//Sample data at middle of clock, transmit at rising edge of clock
#define SSP2STAT_LOAD                   0b01000000

//SPI clock is Fosc/4, idle clock is low, module enabled in firmware
// (SSPEN bit))
#define SSP2CON1_LOAD                   0b00101010

//Registers cleared since everything pertains to I2C mode
#define SSP2CON2_LOAD                   0b00000000
#define SSP2CON3_LOAD                   0b00000000

//Baud rate is 250KHZ for SPI. Calculation is FOSC/(4 * (SSPxADD + 1))
#define SSP2ADD_LOAD                    3//7//79



/*********************************UART CONSTANTS*******************************/
//Enable UART transmit, 8-bit transmission, high baud rate
#define TX1STA_LOAD                     0b00000100

//Continuous receive, UART enabled in code 
#define RC1STA_LOAD                     0b10010000

//High level clock idle, 8-bit baud rate generator
#define BAUD1CON_LOAD                   0b00000000

// Baud rate = Fosc/(16(n+1))
// n = 25 for 9600 baud rate
// Set for 4MHZ operation
#define SPBRGL_LOAD_4MHZ                25

// Set for 32MHZ operation
#define SPBRGL_LOAD_32MHZ               207



/****************************** TMR3 Setup ************************************/
//Timer3 is started at wakeup and stopped before sleeping.  It runs from the 
// LFINTOSC and is just used to provide a contant timer for process end points.
// The timer value is captured at the start of a process and checked to determine 
// if a process should end.

//T3CON - prescaler is 8, all 16 bits can be read at once, timer will be turned 
// on in code
#define T3CON_LOAD                      0b00110010

//TMR3CLK - Clock select is LFINTOSC
#define TMR3CLK_LOAD                    0b00000100




#endif	/* CONSTANTS_H */

