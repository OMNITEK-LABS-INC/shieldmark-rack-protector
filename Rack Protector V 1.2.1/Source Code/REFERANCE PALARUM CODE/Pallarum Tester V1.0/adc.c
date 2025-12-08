/* 

 */

#include <xc.h>
#include "constants.h"
#include "port_defines.h"
#include "typedefs.h"
#include <stdint.h>



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  uint16_t adc_read(uint8_t u8adcMuxSelect)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function returns the 10-bit ADC reading of the Vpp voltage
 * ***************************************************************************/

uint16_t adc_read_ir(uint8_t u8adcMuxSelect)
{
    ADPCH = u8adcMuxSelect;
    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO);
   
    return ADRES;
}        



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  uint16_t adc_read_vpp(uint8_t u8adcMuxSelect)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function returns the 10-bit ADC reading of the Vpp voltage
 * ***************************************************************************/

uint16_t adc_read_vpp(uint8_t u8adcMuxSelect)
{
    uint16_t u16sampleCount;

//Select gnd to drain sample/hold capacitor    
    ADPCH = 0b00111100;
    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO);    
    
    
    u16sampleCount = 500;
    
    
    ADPCH = u8adcMuxSelect;
    while(--u16sampleCount != 0);
    
    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO);
   
    return ADRES;
}        



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  uint16_t adc_read_battery(uint8_t u8adcMuxSelect)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function returns the 10-bit ADC reading of the aux or EOR
 *                  batteries
 * ***************************************************************************/

uint16_t adc_read_battery(uint8_t u8adcMuxSelect)
{
    ADPCH = u8adcMuxSelect;
    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO);
   
    return ADRES;
}        


