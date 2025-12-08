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
#include "read_IR.h"
#include "constants.h"
#include "port_defines.h"


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  uint8_t hand_scan(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  dispenser state
 *          Returns ACTIVATE_MOTOR if function determines a soap dispense
 *                  is required
 *          Returns WAIT_FOR_UART_DONE if no dispense is required
 * DESCRIPTION:  This function performs the hand scan to determine if a soap 
 *          dispense is requested.  It first performs a quick burst of ADC 
 *          samples.  It performs up to BURST_ADC_SAMPLES.  If any of the 
 *          samples fall outside of the high and low limits, the routine
 *          exits.  If all samples fall within the limits, a valid hand sense
 *          is detected.
 * ***************************************************************************/

stateMachine hand_scan(void)
{
    stateMachine lu8dispenserState;
    uint8_t i;
    uint16_t u16handSensorReadTxOn, u16handSensorReadTxOff, u16difference, u16rollingAverage;
    
    lu8dispenserState = PREPARE_FOR_SL_TRANSMIT;
    
    
    
    

    //if(!UNION_ledStatusFlags.STRUCT_ledFlagBits.b1ledInitialLoad && !u8handSensorStartupDelay)
    //{
        //setup_flash_pattern(NORMAL_OPERATION_PATTERN_0, NORMAL_OPERATION_TIME_P0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NORMAL_PATTERN_REPETITIONS, NORMAL_OPERATION_INTERVALS);        
    //} 
    
 

//Get the average of the buffered differences of the first IR ON and IR OFF readings
    u16rollingAverage = get_rolling_average( );        
    
    
//Take at least two readings (IR off and on).  Then take the readings up to HIT_TRIES_ACTIVATION.  If
// the difference is still a hit, then activate later.
    i = 0;
    
    do{
//Take Burst ADC readings with the IR transmitter off for this first test.  There are 4 conversions 
// completed and accumulated.  The first reading is with the transmitter off.
        u16handSensorReadTxOff = adc_read_ir(ADPCH_LOAD_HANDRX);
    
//Take Burst ADC readings with the IR transmitter on.  There are 4 conversions completed and accumulated per scan.  
        //IR_XMTR_OUTPUT = 1;
        u16handSensorReadTxOn = adc_read_ir(ADPCH_LOAD_HANDRX);
    
        //IR_XMTR_OUTPUT = 0;
    
//If the difference is greater than zero, hold it.  If not, make it 0.
        if(u16handSensorReadTxOn > u16handSensorReadTxOff)
        {
            u16difference = u16handSensorReadTxOn - u16handSensorReadTxOff;
        }
    
        else
        {
            u16difference = 0;
        }        
    }while((i++ < HIT_TRIES_ACTIVATION) && (u16difference > (u16rollingAverage + MINIMUM_THRESHOLD)));
    
    
//Time the initial delay after power up.  If the delay is complete, check if motor needs to 
// be activated.. Save adc difference readings during the startup delay.    
    if(u8handSensorStartupDelay)
    {
        u8handSensorStartupDelay--;
        save_adc_buffer_read(u16difference); 
    }    
                
    else
    {
        if(u8intervalsBetweenActivations != 0)
        {
            u8intervalsBetweenActivations--;            
        }
        
        if(u16difference > (u16rollingAverage + MINIMUM_THRESHOLD))
        {
            if((!STRUCT_statusFlags.b1handPresent) && (!STRUCT_statusFlags.b1cabinetIsOpen)
                    && (u8intervalsBetweenActivations == 0) && (!STRUCT_statusFlags.b1lockoutDispense))
            {
                STRUCT_statusFlags.b1sensorActivated = 1;
                STRUCT_statusFlags.b1handPresent = 1;                
                lu8dispenserState = ACTIVATE_VALVE;
                u8intervalsBetweenActivations = INTERVALS_BETWEEN_ACTIVATIONS_DEFAULT;
            }
        }
        
        else
        {
            save_adc_buffer_read(u16difference); 
            STRUCT_statusFlags.b1handPresent = 0;
        }
    }
    
    return lu8dispenserState;
    
}



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  uint16_t get_rolling_average(void) 
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  Average of 16 buffer addresses
 * DESCRIPTION:  This function receives the rolling average of the adc reading
 *                  differences from the ADC buffer 
 * ***************************************************************************/
uint16_t get_rolling_average(void)
{
    uint32_t u16bufferAverage;
    uint8_t i;
    
    u16bufferAverage = 0;
    
    for(i = 0; i < BUFFER_SIZE; i++)
    {
        u16bufferAverage += u16adcBuffer[i];
    }
    
    u16bufferAverage = u16bufferAverage >> BUFFER_AVG_SHIFT;
    
    return u16bufferAverage;
}



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void save_adc_buffer_read(uint16_t *u16bufferAddress, uint16_t u16localADCDiff1, 
 *                  uint16_t u16localADCDiff1) 
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  Saves the difference between the on and off IR values at the
 *                  specified *bufferAddress
 * ***************************************************************************/
void save_adc_buffer_read(uint16_t u16localADCDiff1) 
{
    if(u8adcBufferIndex > (BUFFER_SIZE - 1))
    {
        u8adcBufferIndex = 0;
    }
    
    u16adcBuffer[u8adcBufferIndex] = u16localADCDiff1;
     
    u8adcBufferIndex++;
}






