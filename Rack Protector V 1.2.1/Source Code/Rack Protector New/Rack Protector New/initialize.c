

#include <xc.h>
#include "typedefs.h"
#include "constants.h"
#include "port_defines.h"
#include "initialize.h"


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void system_initialize(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function calls the peripheral initialization functions.
 *          This function is called by the main( ) function
 * ***************************************************************************/
void system_initialize(void)
{
    //port_init( ); //example
 
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void peripheral_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function enables all modules/peripherals.It is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void peripheral_init(void)
{
//Enable all modules/peripherals
    PMD0 = 0x00;
    PMD1 = 0x00;
    PMD2 = 0x00;
    PMD3 = 0x00;
    PMD4 = 0x00;
    PMD5 = 0x00; 
    
//Clocks can be turned on by other modules
    OSCEN = 0;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void port_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the ports and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void port_init(void)
{
//PORT initialization
    PORTA = PORTA_LOAD;
    LATA = LATA_LOAD;
    TRISA = TRISA_LOAD;
    ANSELA = ANSELA_LOAD;
    WPUA = WPUA_LOAD;
    ODCONA = OSCONA_LOAD;
    SLRCONA = SLRCONA_LOAD;
    INLVLA = INLVLA_LOAD;

    PORTB = PORTB_LOAD;
    LATB = LATB_LOAD;
    TRISB = TRISB_LOAD;
    ANSELB = ANSELB_LOAD;
    WPUB = WPUB_LOAD;
    ODCONB = OSCONB_LOAD;
    SLRCONB = SLRCONB_LOAD;
    INLVLB = INLVLB_LOAD;

    PORTC = PORTC_LOAD;
    LATC = LATC_LOAD;
    TRISC = TRISC_LOAD;
    ANSELC = ANSELC_LOAD;
    WPUC = WPUC_LOAD;
    ODCONC = OSCONC_LOAD;
    SLRCONC = SLRCONC_LOAD;
    INLVLC = INLVLC_LOAD;

    WPUE = WPUE_LOAD;
    INLVLE = INLVLE_LOAD;
    

}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void ADC_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the ports and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void ADC_init(void)
{
//Initialize ADC
    ADCON0 = ADCON0_LOAD;
    ADCON1 = ADCON1_LOAD;
    ADCLK = ADCLK_LOAD_4MHZ;
    ADACQ = ADACQ_LOAD_BATTERY;
    ADCON2 = ADCON2_LOAD;
    ADCON3 = ADCON3_LOAD;
    ADREF = ADREF_LOAD_FVR;
    ADRPT = ADRPT_LOAD;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void TMR0_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the TMR0 and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void TMR0_init(void)
{
//Initialize TMR0
    T0CON0 = T0CON0_LOAD;
    T0CON1 = T0CON1_250MS;   
    TMR0H = TMR0H_LOAD;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void TMR2_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the TMR2 and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void TMR2_init(void)
{
    T2CLKCON = T2CLKCON_LOAD;
    T2CON = T2CON_LOAD;
    T2HLT = T2HLT_LOAD;
    T2PR = T2PR_LOAD;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void TMR4_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the TMR4 and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void TMR4_init(void)
{
    T4CLKCON = T4CLKCON_LOAD;
    T4CON = T4CON_LOAD;
    T4HLT = T4HLT_LOAD;
    T4PR = T4PR_LOAD;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void CCP2_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the CCP2 and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void CCP2_init(void)
{
    CCP2CON = CCP2CON_LOAD_DISABLE;
    CCPR2H = CCPR2H_LOAD;
    CCPR2L = CCPR2L_LOAD;    
    CCPTMRS = CCPTMRS_LOAD;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void TMR1_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the TMR1 and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void TMR1_init(void)
{
    T1CON = T1CON_LOAD;
    TMR1CLK = TMR1CLK_LOAD;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void CCP1_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the CCP1 and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void CCP1_init(void)
{
    CCP1CON = CCP1CON_LOAD;
    CCP1CAP = CCP1CAP_LOAD;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void SSP2_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the SSP2 and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void SSP2_init(void)
{
    SSP2STAT = SSP2STAT_LOAD;
    SSP2CON1 = SSP2CON1_LOAD;
    SSP2CON2 = SSP2CON2_LOAD;
    SSP2CON3 = SSP2CON3_LOAD;
    SSP2ADD = SSP2ADD_LOAD;

    
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void UART_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the UART1 and is called by
 *          the system_initialize( ) function
 * ***************************************************************************/
void UART_init(void)
{
    TX1STA = TX1STA_LOAD;
    RC1STA = RC1STA_LOAD;
    BAUD1CON = BAUD1CON_LOAD;
    SPBRGL = SPBRGL_LOAD_4MHZ;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void TMR3_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes TMR3 with the LFINTOSC clock source
 *                  and a divider of 8
 * ***************************************************************************/
void TMR3_init(void)
{
    TMR3CLK = TMR3CLK_LOAD;
    T3CON = T3CON_LOAD;
}












    
    

