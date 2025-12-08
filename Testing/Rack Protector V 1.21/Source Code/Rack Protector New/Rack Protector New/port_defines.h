//#ifndef PORT_DEFINES_H
//#define	PORT_DEFINES_H


/*******************************PORT DEFINITIONS*******************************/
/*******************************PORT DEFINITIONS*******************************/
/******************************************************************************/
//PORTA

#define RIGHT_RX                        PORTAbits.RA0
#define RIGHT_TX                        LATAbits.LATA1
#define MIDDLE_RX                       PORTAbits.RA2
#define MIDDLE_TX                       LATAbits.LATA3
#define LEFT_RX                         PORTAbits.RA4
#define LEFT_TX                         LATAbits.LATA5
//#define TP4                             TESTPOINT
//#define TP2                             TESTPOINT

#define PORTA_LOAD                      0b00000000
#define LATA_LOAD                       0b00000000
#define TRISA_LOAD                      0b00000000     
#define ANSELA_LOAD                     0b00000000
#define WPUA_LOAD                       0b00000000
#define OSCONA_LOAD                     0b00000000
#define SLRCONA_LOAD                    0b00000000
#define INLVLA_LOAD                     0b00000000

/******************************************************************************/

//PORTB

#define RED_LED                         LATBbits.LATB0
#define GREEN_LED                       LATBbits.LATB1
#define BLUE_LED                        LATBbits.LATB2
#define BATT_EN                         LATBbits.LATB3
#define BATT_V                          PORTBbits.RB4
//#define TP3                             TESTPOINT
#define ICSPCLK                         PORTBbits.RB6
#define ICSPDAT                         PORTBbits.RB7

#define PORTB_LOAD                      0b00000000
#define LATB_LOAD                       0b00000000
#define TRISB_LOAD                      0b10000100 //1 is an input 0 is output
#define ANSELB_LOAD                     0b10000000 //1 is for an ADC channel
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

#define LASER_TX                        LATCbits.LATC0
#define LASER_EN                        LATCbits.LATC1
#define LASER_RX                        PORTCbits.RC2
//#define TP5                             TESTPOINT
#define SHDN_NOT                        LATCbits.LATC4
//#define TP6                             TESTPOINT
#define PIC_TX                          LATCbits.LATC6
#define PIC_RX                          LATCbits.LATC7


#define PORTC_LOAD                      0b00000000
#define LATC_LOAD                       0b00000000
#define TRISC_LOAD                      0b00000000
#define ANSELC_LOAD                     0b00000000
#define WPUC_LOAD                       0b00000000
#define OSCONC_LOAD                     0b00000000
#define SLRCONC_LOAD                    0b00000000
#define INLVLC_LOAD                     0b00000000

//CCP2 is output to RC2
//#define RC2PPS_LOAD                     0x06
//#define RC7PPS_LOAD                     0x06  

//TX is RC3
//#define RC3PPS_LOAD                     0x09

//RX
//#define RX1PPS_LOAD                     0x15


/******************************************************************************/

//PORTE

#define WPUE_LOAD                       0b00000000
#define INLVLE_LOAD                     0b00000000


/******************************************************************************/
