/* 
 * YOU MAY USE THIS SOFTWARE AND ANY DERIVATIVES EXCLUSIVELY WITH MICROCHIP
 * PRODUCTS
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, ARE PROVIDED BY MICROCHIP OR APPLY TO THIS 
 * SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION 
 * WITH MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN 
 * ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP  
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR THE DAMAGES ARE 
 * FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, YOU WAIVE ANY 
 * RECOURSE AGAINST MICROCHIP WITH RESPECT TO ANY DAMAGES ARISING FROM 
 * CLAIMS IN ANY WAY RELATED TO THE SOFTWARE AND AGREE THAT ANY SUCH CLAIMS 
 * SHALL NEVER EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY 
 * TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */


/*
 * PERIPHERAL DESCRIPTION
 *  
 * TMR0 - runs continuously and wakes the micro from sleep to perform the hand
 *          scan.  Interrupts are turned on during sleep and turned off when the
 *          motor is activated
 * TMR2 - Runs the period for the motor PWM.  No interrupts are enabled.  Only
 *          runs when the motor is activated
 * TMR1 - Capture timer for the encoder wheel pulses
 * TMR4 - Times the motor run time and the brake timing.  Only running when the
 *          motor is activated
 * TMR3 - Started every time a wakeup occurs and stopped before going to sleep.  Used
 *          to provide timeout functionality to the EUSART receiver.
 * CCP2 - contains the duty cycle for the motor PWM. Paired with TMR2.
 * CCP1 - holds the capture value for the encoder wheel and is paired with CCP1
 * TMR6 - controls the LED timing during the 4MHZ run cycle of the dispenser
 * ADC - set up to take 8 measurements for each trigger
 * UART - Used for SMARTLINK transmitter and receiver
 
 
 */


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
#define ADACQ_LOAD                      50//100//35//25

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
// Timer0 is set up to interrupt every 250ms. The frequency at TMR0 is 31KHZ
// (30.303us/count).  Total count required is 8250 to get a 250ms timeout.  
// With 8-bit counting, 31000/256 = 121.09 divider for 1 second.  For 250ms the
// count is 30.27.  Prescaler is set to 2 and postscaler is set to 15 for a 
// divider of 30.


//T0CON0 - postscaler set to 15, 8-bit timer mode, will be enabled in code
#define T0CON0_LOAD                     0b00001110

//T0CON1 - prescaler set to 2, LFINTOSC is clock source, asynchronous
#define T0CON1_250MS                    0b10010001  

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





/************************************CCP2 SETUP********************************/
//CCP2CON - module is disabled (will be enabled in software), right alignment,
// PWM mode
#define CCP2CON_LOAD_DISABLE            0b00001111

//Starting duty cycle is 90% based on count of 1023
#define CCPR2L_LOAD                     0x99
#define CCPR2H_LOAD                     3   

//CCPTMRS0 - CCP2 is working with TMR2 for PWM, CCP1 working with TMR1 for
// capture.  First 4 bits refer to PWM's that are not being used.
#define CCPTMRS_LOAD                    0b01010101



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
#define SSP2STAT_LOAD                   0b01000000 //was 00000000

//SPI clock is Fosc/4, idle clock is low, module enabled in firmware
// (SSPEN bit))
#define SSP2CON1_LOAD                   0b00101010 //was 00111010

//Registers cleared since everything pertains to I2C mode
#define SSP2CON2_LOAD                   0b00000000
#define SSP2CON3_LOAD                   0b00000000

//Baud rate is 250KHZ for SPI. Calculation is FOSC/(4 * (SSPxADD + 1))
#define SSP2ADD_LOAD                    3//7//79

#define SPI_CH1                         0x00
#define SPI_CH2                         0x08
#define SPI_CH3                         0x10


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
#define SPBRGL_LOAD_4MHZ                12  //was 25

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





/*******************************OPERATIONAL CONSTANTS**************************/
/******************************************************************************/
/******************************************************************************/
//This value is the base 2 shifts to divide the buffer total.  There are 16
// buffer values for the moving hand sense difference average
#define BUFFER_AVG_SHIFT                5//4

//Number of successive hits for activation
#define HIT_TRIES_ACTIVATION            3//4

//Number of buffer values saved for ADC difference readings
#define BUFFER_SIZE                     32//16

//Minimum margin that is needed between the running difference average and the
// current scan difference 
#define MINIMUM_THRESHOLD               40//12//18//50//70//50

//Number of intervals to delay at startup.  Each count is 250ms
#define HAND_SENSOR_STARTUP_DELAY       18

//Number of 200ms intervals to wait after a hand activation
#define INTERVALS_BETWEEN_ACTIVATIONS_DEFAULT   5

/****************************** Motor run timer *******************************/

//The motor is set to run 18 revolutions.  With a 16-window wheel, the total
// number of pulses is 20 x 16 = 320.  Set to 321 to complete the last window.
// Foam sani pump needs 20 revolutions:
//#define MOTOR_RUN_WINDOW_COUNT_DEFAULT          321
// Foam soap pump needs 15 revolutions:
#define MOTOR_RUN_WINDOW_COUNT_DEFAULT        321
//Gel 6.625 Revs = 107
//#define MOTOR_RUN_WINDOW_COUNT_DEFAULT        107
//Number of 500us intervals for motor timeout (1.8 second timeout)
#define MOTOR_TIMEOUT                   5000 //was 3600               



/********************Pulse Capture and PI Control******************************/
//Number of capture pulses to ignore at motor startup
#define IGNORE_CAPTURE_PULSES           5

//At 1700 RPM and a 16-window encoder, the number or pulses per minute is
// 1700 x 16 = 27200.  The number of pulses per second is 453.33.  TMR1 clock is
// 1us. The time between pulses at rated speed is 1/453.33pps = 2.206ms. This is
// a TMR1 count of 2206.  

// Foam pump needs 1700 RPM:
#define TARGET_RPM_TIME_BTW_PULSES      2206   
// Gel pump needs 1000 RPM:
//#define TARGET_RPM_TIME_BTW_PULSES      3750

//Proportional constant.  1 = 50%, 2 = 25%, 3 = 12.5%, etc...  Proportional
// change is shifted right by this number
#define PROPORTIONAL_CONSTANT_INCREASE  2 //was 3, 2 works ok with min_duty set to 512
#define PROPORTIONAL_CONSTANT_DECREASE  2


//Here are the maximum and minimum duty cycle values (95% and 50%)
#define MAX_DUTY_CYCLE                  972
#define MIN_DUTY_CYCLE                  614//512//614//716




/*******************************MISC Operational constants*********************/
//Number of IR read intervals to wait until the green LED is blinked.  The 
// interval is 7 seconds.  With 250ms read intervals, we need 28 intervals
#define GREEN_LED_INTERVAL_COUNTS       28

//Number of transmit bytes
#define NUMBER_OF_TRANSMIT_BYTES        14

//Every scan period the cabinet switch is debounced CABINET_OPEN_TESTS counts.
// If the number of open counts is greater than or equal to CABINET_OPEN_COUNT_VALID
// the cabinet is open.  The CABINET_CLOSED_COUNTS is the number of wake intervals
// to signify a closed cabinet.
#define CABINET_OPEN_TESTS              6
#define CABINET_OPEN_COUNTS_VALID       4
#define CABINET_CLOSED_COUNTS           2


//Reading for Vpp must get above this value during charging.  The 2.048V FVR is
// used and the reading must get above 1.1V.  The voltage divider is 2M/1M.  
// This charge is initiated after every motor activation.  Periodically, the voltage 
// is tested to make sure it is above 1.1V.  If not, the charge is turned on even without a motor 
// activation.  
// (1.0V/2.048V) * 1023 = 550  //3V Vdd
// (0.9/2.048V) * 1023 = 500
//8 accumulated samples per conversion
#define VPP_HIGH_LIMIT                  550//550//500
#define VPP_LOW_LIMIT                   520//500//450//490

//The unit wakes up every 250ms.  A Vpp check will occur every 1 minute  
#define VPP_COUNTER_RELOAD              40//240     *******************************FIX************************************

//The number of wake intervals (1/4s) that the CHARGE is allowed to occur at startup
#define CHARGE_TIMEOUT_STARTUP          200

//The number of wake intervals (1/4s) that the CHARGE is allowed to occur during run time
#define CHARGE_TIMEOUT_RUN              40//20


//Valid motor activation voltage set to 0.9V
#define VALID_ACTIVATION_VOLTAGE        450//500//500//450//490     

//Low limit of AA battery voltage is 0.9V
#define LOW_AA_VOLTAGE                  450

//Battery voltage to allow it to be used if AUX is running (1V)
#define VOLTAGE_AA_INSERT               1000


//Encode poll loops for majority voting nd number of valid high signal level
// counts
#define POLL_LOOP_COUNT                 4
#define VALID_SIGNAL_COUNT              3

//Number of dispenses before the count is saved
#define DISP_COUNTS_BEFORE_EEPROM_SAVE  10 // change to 30 for production       

//Multipliers for voltage readings (used with BCD function).  The BCD function 
// already multiplies the value by 10 to get rid of the decimal place
#define EOR_VOLTAGE_MULTIPLIER          1
#define AUX_VOLTAGE_MULTIPLIER          1
#define VPP_VOLTAGE_MULTIPLIER          3

//Command code for XHAND dispense Smartlink message
#define DISPENSE_XHAND_COMMAND_CODE     0x14




/*******************************RFID Operational constants*********************/
#define ST25_INIT_TRIES                 3
#define TAG_BUFFER_SIZE                 32


/************************************* LED Control*****************************/
//Every 2 counts is a full flash for ST25 success. Flash time expressed in ms
#define ST25_SUCCESS_FLASHES            6
#define ST25_SUCCESS_FLASH_TIME         250

//Every 2 counts is a full flash for ST25 success. Flash time expressed in ms
#define ST25_FAILURE_FLASHES            12
#define ST25_FAILURE_FLASH_TIME         100



//Case 0 in state machine means first pass
#define LED_PATTERN0                    0
#define LED_PATTERN1                    1
#define LED_PATTERN2                    2
#define LED_PATTERN3                    3
#define LED_PATTERN4                    4
#define LED_PATTERN5                    5


//The continuous patterns are LED flashes that run in and out of sleep mode.  They are 
// the normal operation, invalid refill, and pump house fault.  Each count is the wakeup
// interval timing (250ms))
#define NORMAL_OPERATION_INTERVALS      28  //7 seconds    
#define INVALID_REFILL_INTERVALS        16  //4 seconds 
#define INVALID_PUMPHOUSE_INTERVALS     16  //4 seconds
#define CAB_OPEN_NORMAL_INTERVALS       40  //5 seconds
#define ST25_INIT_FAILURE_INTERVALS     12  //2 seconds


//Normal operation where the green led is on for 14ms, off for 7s.  This pattern has
// an on time during a full run.  The off time is counted by interrupts occurring every
// 250ms.  28 interrupts is the off time.  When a pattern time is 0, the LED sequence was
// completed and needs to be reloaded.  Complement means LED is on.  
#define NORMAL_OPERATION_PATTERN_0      ~GREEN_LED_MASK 
#define NORMAL_OPERATION_TIME_P0        7
#define NORMAL_PATTERN_REPETITIONS      1





//Two Tags read
#define TWO_TAGS_READ_PATTERN_0         ~(NO_LEDS_ON)
#define TWO_TAGS_READ_TIME_P0           50
#define TWO_TAGS_READ_PATTERN_1         ~( GREEN_LED_MASK | RED_LED_MASK )
#define TWO_TAGS_READ_TIME_P1           500
#define TWO_TAGS_REPETITIONS            1


//One tag Read
#define ONE_TAG_READ_PATTERN_0          ~(NO_LEDS_ON)
#define ONE_TAG_READ_TIME_P0            50
#define ONE_TAG_READ_PATTERN_1          ~BLUE_LED_MASK
#define ONE_TAG_READ_TIME_P1            500
#define ONE_TAG_REPETITIONS             1


//Failure ST25 initialization
#define ST25_INIT_FAILURE_PATTERN_0     ~(BLUE_LED_MASK)
#define ST25_INIT_FAILURE_TIME_P0       7
#define ST25_INIT_FAILURE_PATTERN_1     ~(GREEN_LED_MASK)
#define ST25_INIT_FAILURE_TIME_P1       7     
#define ST25_INIT_FAILURE_PATTERN_2     ~(RED_LED_MASK)
#define ST25_INIT_FAILURE_TIME_P2       7
#define ST25_INIT_FAILURE_PATTERN_3     ~(NO_LEDS_ON)
#define ST25_INIT_FAILURE_TIME_P3       75
#define ST25_INIT_FAILURE_REPETITIONS   2




/*****************************SMART LINK TRANSMITTER********************************/
/***********************************************************************************/

//GENERAL TRANSMITTER CONSTANTS
#define DUMMY_BYTE                          0x00
#define TRANSMIT_BUFFER_SIZE                24
#define BUFFER_LOADED_FLAG                  0xAA
#define BUFFER_CLEARED_FLAG                 0
#define MAXIMUM_TRANSMIT_BUFFER_ADDRESS     &STRUCT_slXmtTransmitBuffer[3]
#define STARTING_TRANSMIT_BUFFER_ADDRESS    &STRUCT_slXmtTransmitBuffer[0]


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~DISPENSER EVENT CONSTANTS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Subtype ID designations

#define TARE                                0x54
#define PRINT                               0x50
#define L_FEED                              0x0A
#define C_RETURN                            0x0D

#define DE_ST_ID_HIGH_BYTE                  0x54
#define DE_ST_ID_LOW_BYTE_RESET             0x00
#define DE_ST_ID_LOW_BYTE_DISPENSE          0x0D
#define DE_ST_ID_LOW_BYTE_ATT_DISPENSE      0x00

//Type ID designations
#define DE_ID_HIGH_BYTE                     0x0A
#define DE_ID_LOW_BYTE                      0x38

//Payload Length
#define DE_PAYLOAD_LENGTH                   0x01

//Subtype 00 system reset event
#define DE_ST_ID_RESET                      0x00

//Subtype 01 dispense event top two bits
#define DE_INITIATOR_XHAND                  0b10000000
#define DE_INITIATOR_SMARTLINK              0b01000000
#define DE_INITIATOR_HAND_SENSOR            0b00000000

//Subtype 01 pump revolutions
#define DE_INITIATOR_18_REVS                0b00010010

//Subtype 02 attempted dispense aborted
#define DE_CLEAR_TIME_NOT_EXPIRED           0x01
#define DE_DISPENSE_DISABLED                0x02
#define DE_CABINET_OPEN                     0x03
#define DE_REFILL_READ_NOT_FINISHED         0x04
#define DE_NO_REFILL_INSTALLED              0x05
#define DE_INCORRECT_REFILL_FORMULA         0x06
#define DE_INCORRECT_REFILL_DIST_CODE       0x07
#define DE_REFILL_VOLUME_EXCEEDED           0x08
#define DE_VOLTAGE_LOW                      0x09
#define DE_PREV_DISP_ACTIVE                 0x0A
#define DE_POWER_UP_DELAY_ACTIVE            0x0B

//Total bytes to send in message 
#define DE_TOTAL_BYTES_TO_SEND              2





//~~~~~~~~~~~~~~~~~~~~~~~~~~DISPENSER STATISTICS CONSTANTS~~~~~~~~~~~~~~~~~~~~~~~~
//Subtype ID designations
#define DS_ST_ID_HIGH_BYTE                  0x30
#define DS_ST_ID_LOW_BYTE                   0x30

//Type ID designations
#define DS_ID_HIGH_BYTE                     0x36
#define DS_ID_LOW_BYTE                      0x32

//Total bytes to send in message 
#define DS_TOTAL_BYTES_TO_SEND              13
#define DS_PAYLOAD_LENGTH                   7



//~~~~~~~~~~~~~~~~~~~~~~~~~~DISPENSER DIAGNOSTICS CONSTANTS~~~~~~~~~~~~~~~~~~~~~~~~
//Subtype ID designations
#define DD_ST_ID_HIGH_BYTE                  0x30
#define DD_ST_ID_LOW_BYTE                   0x30

//Type ID designations
#define DD_ID_HIGH_BYTE                     0x36
#define DD_ID_LOW_BYTE                      0x33

//Total bytes to send in message 
#define DD_TOTAL_BYTES_TO_SEND              10
#define DD_PAYLOAD_LENGTH                   4



//~~~~~~~~~~~~~~~~~~~~~~~~~~DISPENSER PUMPHOUSE CONSTANTS~~~~~~~~~~~~~~~~~~~~~~~~
//Subtype ID designations
#define DP_ST_ID_HIGH_BYTE                  0x30
#define DP_ST_ID_LOW_BYTE                   0x30

//Type ID designations
#define DP_ID_HIGH_BYTE                     0x30
#define DP_ID_LOW_BYTE                      0x36

//Total bytes to send in message 
#define DP_TOTAL_BYTES_TO_SEND              10
#define DP_PAYLOAD_LENGTH                   4

//Pump Type codes
#define FOAM_SOAP_PUMP                      0x02

//Distributor codes
#define PURELL_SOAP                         0x00




//~~~~~~~~~~~~~~~~~~~~~~~~~~DISPENSER REFILL INFORMATION~~~~~~~~~~~~~~~~~~~~~~~~~~
//Total bytes to send in message
#define DR_TOTAL_BYTES_TO_SEND              24
#define DR_PAYLOAD_LENGTH                   18

//Subtype ID designations
#define DR_ST_ID_HIGH_BYTE                  0x30
#define DR_ST_ID_LOW_BYTE                   0x30

//Type ID designations
#define DR_ID_HIGH_BYTE                     0x36
#define DR_ID_LOW_BYTE                      0x30

//TAG Formula
// bits 7 - 5 are formula type
#define FORMULA_FOAM_SANITIZER              0b00100000
#define FORMULA_FOAM_SOAP                   0b01000000
#define FORMULA_GEL_SANITIZER               0b01100000
#define FORMULA_LOTION_SOAP                 0b10000000
#define FORMULA_MASK                        0b11100000

//Refill Size
#define REFILL_80CL                         0b00001000
#define REFILL_120CL                        0b00001100
#define REFILL_SIZE_MASK                    0b00011111

//Tag Types
#define REFILL_TAG_TYPE                     0x10
#define REFILL_TAG_CODE_MASK                0xF0



//~~~~~~~~~~~~~~~~~~~~~~~~~~DISPENSER VOC INFORMATION~~~~~~~~~~~~~~~~~~~~~~~~~~
//Total bytes to send in message
#define DV_TOTAL_BYTES_TO_SEND              14
#define DV_PAYLOAD_LENGTH                   8

//Subtype ID designations
#define DV_ST_ID_HIGH_BYTE                  0x30
#define DV_ST_ID_LOW_BYTE                   0x30

//Type ID designations
#define DV_ID_HIGH_BYTE                     0x36
#define DV_ID_LOW_BYTE                      0x31

//TAG Color
#define COLOR_LIGHT_BLUE                    0x01
#define COLOR_DARK_BLUE                     0x02
#define COLOR_PLACEHOLDER1                  0x03
#define COLOR_LIGHT_GREY                    0x04
#define COLOR_DARK_GREY                     0x05
#define COLOR_PLACEHOLDER2                  0x06


//Tag Types
#define VOC_TAG_TYPE                        0x20






//~~~~~~~~~~~~~~~~~~~~~~~~~~SET CONFIGURATION INFORMATION~~~~~~~~~~~~~~~~~~~~~~~~~~
#define LOCKOUT_DISPENSER_UNTIL_CAB_OPEN    0x80
#define CLEAR_TIME_MAX                      0x7F


/*****************************SMART LINK RECEIVER********************************/
/***********************************************************************************/

//GENERAL RECEIVER CONSTANTS
//Timeout value for received message.  The receive timeout is timed from TMR 3 that
// is running from the 31768HZ internal timer.  The prescaler is set to 8 so TMR3
// counts at 3971HZ (0.251ms).  The baud rate for the UART is 9600, so each byte take
// 0.833ms.  If we set the longest receive packet to be 20 bytes including the break
// signal, the total receive time is 16.67ms.  We will set the timeout to be 20ms.
// 20ms/0.251ms = 80.
#define SMARTLINK_RECEIVE_TIMEOUT           80       

//Minimum buffer size of read buffer
#define MINIMUM_READ_BUFFER_DATA_INDEX      2

//Buffer Size
#define RECEIVE_BUFFER_SIZE                 20

//Terminator bytes
#define UART_READ_FIRST_TERMINATOR_BYTE     0x31  //0x0D
#define UART_READ_SECOND_TERMINATOR_BYTE    0x32  //0x0A

//Set LED Pattern (as bits appear in received packet)
#define PATTERN1_MASK                       0b01110000
#define PATTERN2_MASK                       0b00000111

//Address Locations
//#define MAXIMUM_RECEIVE_BUFFER_ADDRESS      &STRUCT_slRcvrBuffer[3]
//#define STARTING_RECEIVE_BUFFER_ADDRESS     &STRUCT_slRcvrBuffer[0]






#endif	/* CONSTANTS_H */

