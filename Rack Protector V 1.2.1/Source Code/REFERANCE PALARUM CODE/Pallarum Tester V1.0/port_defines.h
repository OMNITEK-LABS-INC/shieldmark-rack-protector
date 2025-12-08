/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */


#ifndef PORT_DEFINES_H
#define	PORT_DEFINES_H


/*******************************PORT DEFINITIONS*******************************/
/*******************************PORT DEFINITIONS*******************************/
/******************************************************************************/
//PORTA
//#define AUX_ANALOG_IN                   PORTAbits.RA0
//#define MOTOR_PWM_OUT                   PORTAbits.RA0
#define START_IN                        PORTAbits.RA0//was output
#define RESET_IN                        PORTAbits.RA1
#define BIS_IN                          PORTAbits.RA2
#define VISUAL_DEFECT_IN                PORTAbits.RA3
#define SENSOR_DEFECT_IN                PORTAbits.RA4
#define EMERGENCY_IN                    PORTAbits.RA5//was ouput
//#define                                 TRISAbits.TRISA4
//#define                                 TRISAbits.TRISA5
//#define                                 LATAbits.LATA6
//#define                                 PORTAbits.RA7

#define PORTA_LOAD                      0b00000000
#define LATA_LOAD                       0b00000000
#define TRISA_LOAD                      0b00111111      
#define ANSELA_LOAD                     0b00000000
#define WPUA_LOAD                       0b00000000
#define OSCONA_LOAD                     0b00000000
#define SLRCONA_LOAD                    0b00000000
#define INLVLA_LOAD                     0b00000000

/******************************************************************************/

//PORTB
#define SS2_OUT                         LATBbits.LATB0
#define SCK2_OUT                        PORTBbits.RB1
#define SPI_A                           PORTBbits.RB2 //LATBbits.LATB2
#define SDO2_OUT                        PORTBbits.RB3
//#define                                 LATBbits.LATB4
//#define                                 PORTBbits.RB4
//#define                                 LATBbits.LATB5
//#define                                 LATBbits.LATB6    //Also ICSPCLK 
//#define                                 LATBbits.LATB7    //Also ICSPDAT   


#define PORTB_LOAD                      0b00000000
#define LATB_LOAD                       0b00000000
#define TRISB_LOAD                      0b00000100
#define ANSELB_LOAD                     0b00000000
#define WPUB_LOAD                       0b00000000
#define OSCONB_LOAD                     0b00000000
#define SLRCONB_LOAD                    0b00000000
#define INLVLB_LOAD                     0b00010000

//Peripheral Pin Select Settings
#define SSP2DATPPS_LOAD                 0x0A //SDI2 on RB2
#define RB1PPS_LOAD                     0x11 //SCK2 on RB1
#define RB3PPS_LOAD                     0x12 //SDO2 on RB3


/******************************************************/

//PORTC
#define PUMP1                           LATCbits.LATC0
#define PUMPVALVE                       LATCbits.LATC1
#define DIST_VALVE                      LATCbits.LATC2
#define SMARTLINK_TX                    LATCbits.LATC3
#define YELLOW_LED                      LATCbits.LATC4
#define SMARTLINK_RX                    PORTCbits.RC5
//#define                                 PORTCbits.RC6
//#define CHARGE_OUT                      LATCbits.LATC6
//#define EOR_OUT                         LATCbits.LATC7
//#define MOTOR_PWM_OUT                   PORTCbits.RC7

#define PORTC_LOAD                      0b00000000
#define LATC_LOAD                       0b00000000
#define TRISC_LOAD                      0b00100000
#define ANSELC_LOAD                     0b00000000
#define WPUC_LOAD                       0b00000000
#define OSCONC_LOAD                     0b00000000
#define SLRCONC_LOAD                    0b00000000
#define INLVLC_LOAD                     0b00000100

//CCP2 is output to RC2
//#define RC2PPS_LOAD                     0x06
//#define RC7PPS_LOAD                       0x06  

//Smartlink TX is RC3
#define RC3PPS_LOAD                     0x09

//Smartlink RX is RC5
#define RX1PPS_LOAD                     0x15


/******************************************************************************/

//PORTE
//#define XHAND_IN_TEMPORARY              PORTEbits.RE3   ////***REMOVE FOR PRODUCTION***///

#define WPUE_LOAD                       0b00000000
#define INLVLE_LOAD                     0b00000000


/******************************************************************************/
//ADPCH channel select
// Select channel ANC0 for hand RX
#define ADPCH_LOAD_HANDRX               0b00010000

//Select Vss to connect the S&H capacitor to discharge it
#define ADPCH_LOAD_VSS                  0b00111100

// Select ANA7/RA7 for Vpp measurement.  The external resistor divider will
// preset 1/3 Vpp (3V) on the pin)
#define ADPCH_LOAD_VPP                  0b00000111

//Select ANA0 / RA0 for the AUX battery measurement
#define ADPCH_LOAD_AUX_BAT              0b00000000

//Select ANA1 / RA1 for the EOR battery measurement
#define ADPCH_LOAD_EOR_BAT              0b00000001








#endif	/* PORT_DEFINES_H */

