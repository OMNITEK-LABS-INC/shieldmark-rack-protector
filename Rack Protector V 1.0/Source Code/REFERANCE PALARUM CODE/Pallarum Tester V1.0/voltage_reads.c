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


#include <xc.h>
#include <stdint.h>
#include "typedefs.h"
#include "constants.h"
#include "port_defines.h"
#include "voltage_reads.h"





/*****************************************************************************
 * FUNCTION:  void FVR_enable(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function turns on the fvr and waits until it is stable
 * ***************************************************************************/
void FVR_enable(void)
{
//Turn on the voltage reference and wait for it to be stable
    FVRCON = FVRCON_ON_LOAD;
    while(!FVRCONbits.RDY);
}


/*****************************************************************************
 * FUNCTION:  void FVR_disable(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function turns on the fvr and waits until it is stable
 * ***************************************************************************/
void FVR_disable(void)
{
//Turn on the voltage reference and wait for it to be stable
    FVRCON = FVRCON_OFF_LOAD;
}



/*****************************************************************************
 * FUNCTION:  void charge_enable(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function enables FVR and turns on the charge out
 * ***************************************************************************/
void charge_enable(void)
{
    u16vppCheckCounter = VPP_COUNTER_RELOAD;
    u16chargeTimeout = CHARGE_TIMEOUT_RUN;    
    STRUCT_statusFlags.b1chargeVppActive = 1;
    STRUCT_statusFlags.b1checkVppVoltage = 0;
    
    if(!STRUCT_statusFlags.b1auxActive) 
    {
        if(adc_read_battery(ADPCH_LOAD_EOR_BAT) < LOW_AA_VOLTAGE)
        {
            STRUCT_statusFlags.b1auxActive = 1;
            //EOR_OUT = 1;
            //asm("nop");
            //AUX_OUT = 0;
        }
    }    
    
    
//Turn on the charge     
    //CHARGE_OUT = 1;
}



/*****************************************************************************
 * FUNCTION:  void charge_disable(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function disables FVR and turns off the charge out. It also
 *                  measures the EOR and AUX battery voltages
 * **************************************************************************/
void charge_disable(void)
{
    STRUCT_statusFlags.b1chargeVppActive = 0;
    //CHARGE_OUT = 0;   
    u16chargeTimeout = CHARGE_TIMEOUT_RUN;
    STRUCT_statusFlags.b1checkVppVoltage = 1;
}
    
    

/*****************************************************************************
 * FUNCTION:  uint8_t hexToBCD(uint8_t u16valueToPack, uint8_t u8multiplier)
 * RECEIVED PARAMETERS:  Number to pack and multiplier value
 * RETURNED VALUES:  Packed BCD number
 * DESCRIPTION:  This function returns the packed BCD number of the value sent
 *                  multiplied by the multiplier
 * **************************************************************************/
uint8_t hexToBCD(uint16_t u16valueToPack, uint8_t u8multiplier)
{
//Not using divide function to conserve code space   
    uint8_t i, j, y;
    uint32_t z;
    
    
    u16valueToPack *= u8multiplier;
   

//Formula is (using base 16 math for simple bit shifts):
// Value = (counts / 1024) x 2.048
// Multiply both sides by 128 
//  Value * 128 = (counts << 10) * 262
//  Value * 128 = (counts * 262) << 10
// We now take this value without dividing by 128
//    then divide by 128 by subtraction
//  Subtract the value by 128 to get the 1's place
//   (successive subtraction until only a remainder remains)
//  Take the remainder and multiply by 10
// do successive subtractions by 128 to get 10's place    
    
    z = ((uint32_t)u16valueToPack * 262) >> 10;
    
    for(i = 0; z >+ 128; i++)
    {
        z -= 128;
    }
    
//i now contains the ones place 
// multiply the remainder by 10
    z *= 10;
    
// Do successive subtractions by 128 to get tenths place
    for(j = 0; z >= 128; j++)
    {
        z -= 128;
    }    
    
//i contains the ones place and j contains the tenths place.  The ones
// place goes in the top nibble and the tenths place goes in the bottom nibble.
    y = (i << 4) + j;
     
//    x = u8valueToPack * u8multiplier;
//    x = x / 2.048;
//    
//    y = (x / 10) << 4;
//    y = y | (x % 10);
    
    return y;
}    
    


    
 
/*****************************************************************************
 * FUNCTION:  void check_for_battery_switch
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  Checks if the unit is running on battery power.  If it is and the
 *                  AA voltage is low, change to AUX.  If it is running on AUX power
 *                  and the AA is not low, change to AA.  No matter the battery voltages,
 *                  the proper battery is selected
 * **************************************************************************/
void check_for_battery_switch(void)
{
    if(!STRUCT_statusFlags.b1auxActive) 
    {
        if((u16eorBatteryVoltage < LOW_AA_VOLTAGE) || (STRUCT_statusFlags.b1loadedAAVoltageLow))
        {
            STRUCT_statusFlags.b1auxActive = 1;
            //EOR_OUT = 1;
            //asm("nop");
            //AUX_OUT = 0;
        }
        
        //else
        //{
            //AUX_OUT = 1;
            //asm("nop");
            //EOR_OUT = 0;
        //}
    }
    
    else
    {
        if(u16eorBatteryVoltage > VOLTAGE_AA_INSERT) 
        {
            STRUCT_statusFlags.b1auxActive = 0;
            //AUX_OUT = 1;
            //asm("nop");
            //EOR_OUT = 0;
        }  
        
        //else
        //{
            //EOR_OUT = 1;
            //asm("nop");
            //AUX_OUT = 0;
        //}        
    }
}
    
    








