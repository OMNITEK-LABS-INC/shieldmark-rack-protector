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

#include <xc.h>
#include <stdint.h>
#include "typedefs.h"
#include "charge_check.h"
#include "constants.h"
#include "port_defines.h"



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void supercap_charge_check(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function checks if the Vpp charge period has expired and
 *                  will start the charge.  It also checks if the charge is active
 *                  and ends the charge if the max time is reached.
 * *****************************************************************************/

void supercap_charge_check(void)
{
    uint16_t u16adcReadVss;
    
    if(!STRUCT_statusFlags.b1chargeVppActive)
    {
//Determine if the Vpp should be checked.  If it should, set the check voltage flag 
        if((--u16vppCheckCounter == 0) || STRUCT_statusFlags.b1checkVppVoltage)
        {
//Clear the checkVppVoltage flag, reload the timer, then take a voltage read of the supercaps        
            STRUCT_statusFlags.b1checkVppVoltage = 0;
            u16vppCheckCounter = VPP_COUNTER_RELOAD;
        
//Read each battery voltage. Drain the sample/hold capacitor between readings
            u16adcReadVss = adc_read_battery(ADPCH_LOAD_VSS);
            u16eorBatteryVoltage = adc_read_battery(ADPCH_LOAD_EOR_BAT); 
    
            u16adcReadVss = adc_read_battery(ADPCH_LOAD_VSS);
            u16auxBatteryVoltage = adc_read_battery(ADPCH_LOAD_AUX_BAT); 

            u16adcReadVss = adc_read_battery(ADPCH_LOAD_VSS);    
            u16vppVoltage = adc_read_vpp(ADPCH_LOAD_VPP);    
            
            check_for_battery_switch( );


//If the supercap voltage is below the low limit, start the charge        
            if(u16vppVoltage < VPP_LOW_LIMIT)
            {
                charge_enable( );
            } 
        }
    }
    
//Charge is active 
    else
    {
        if(--u16chargeTimeout == 0)
        {
            charge_disable( );
            u16chargeTimeout = CHARGE_TIMEOUT_RUN;
            STRUCT_statusFlags.b1loadedAAVoltageLow = 1;
            
//Change to the AUX_OUT if EOR is active since the battery is depleted
            if(!STRUCT_statusFlags.b1auxActive)
            {
                STRUCT_statusFlags.b1auxActive = 1;
                //EOR_OUT = 1;
                //asm("nop");
                //asm("nop");            
                //AUX_OUT = 0;
                charge_enable( );
            }
        }
            
        else
        {
//Supercaps are charging so check the voltage.  If the voltage is above the upper limit,
// stop the charge
            u16adcReadVss = adc_read_battery(ADPCH_LOAD_VSS);    
            u16vppVoltage = adc_read_vpp(ADPCH_LOAD_VPP);                    

            if(u16vppVoltage > VPP_HIGH_LIMIT)
            {
                charge_disable( );
            }                         
        }
    }
}


