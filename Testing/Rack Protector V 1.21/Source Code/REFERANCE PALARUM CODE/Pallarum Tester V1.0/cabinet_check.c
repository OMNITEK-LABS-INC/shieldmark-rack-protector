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
#include "cabinet_check.h"
#include "constants.h"
#include "port_defines.h"
//#include "rfal_rf.h"
#include "rfal_nfca.h"
#include "eeprom_constants.h"


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  uint8_t cabinet_check(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  lu8sendCabinetMessage is 1 if the SmartLink message is to 
 *          be sent.
 * DESCRIPTION:  This function checks the cabinet door position.  If the cabinet 
 *          is detected to be open and the b1cabinetWasOpened flag is not set, 
 *          the u8cabinetOpenCount is incremented.  Once it reaches
 *          the CABINET_OPEN_COUNTS value, the open cabinet message is loaded.
 *          If the cabinet was already open and acknowledged, the SLEEP_WAIT
 *          state is set.  If the cabinet is closed, all flags are cleared.
 * *****************************************************************************/
stateMachine cabinet_check(void)
{
    uint8_t lu8dispenserState, i;
    ReturnCode err;

//This is the next state if nothing changes it in this routine    
    lu8dispenserState = IR_READS;
    
    asm("nop");
    asm("nop");

    
//If cabinet input is high, cabinet is open    
    if(RESET_IN == 0)
    {
        u8cabinetClosedCount = 0;
        
        if(!STRUCT_statusFlags.b1cabinetWasOpened)
        {
            for(i = 0, u8cabinetOpenCount = 0; i < CABINET_OPEN_TESTS; i++)
            {
                if(RESET_IN)
                {
                    ++u8cabinetOpenCount;
                }
            }
            
            if(u8cabinetOpenCount >= CABINET_OPEN_COUNTS_VALID)
            {
                //setup_flash_pattern(CAB_OPEN_NORMAL_PATTERN_0, CAB_OPEN_NORMAL_TIME_P0, CAB_OPEN_NORMAL_PATTERN_1, CAB_OPEN_NORMAL_TIME_P1, CAB_OPEN_NORMAL_PATTERN_2, 
                //        CAB_OPEN_NORMAL_TIME_P2, CAB_OPEN_NORMAL_PATTERN_3, CAB_OPEN_NORMAL_TIME_P3, CAB_OPEN_NORMAL_PATTERN_4, CAB_OPEN_NORMAL_TIME_P4, CAB_OPEN_NORMAL_PATTERN_5, 
                //        CAB_OPEN_NORMAL_TIME_P5, CAB_OPEN_REPETITIONS, CAB_OPEN_NORMAL_INTERVALS);   
                
                u8cabinetOpenCount = 0;
                STRUCT_statusFlags.b1cabinetWasOpened = 1;


//When cabinet is open, clear tag status and signify that the cabinet is open                
                UNION_pumphouseStatusFlags.STRUCT_pumphouseFlagBits.b1vocTagStatus = 0;
                UNION_pumphouseStatusFlags.STRUCT_pumphouseFlagBits.b1refillTagStatus = 0;
                UNION_pumphouseStatusFlags.STRUCT_pumphouseFlagBits.b1cabinetStatus = 1;
                
//Increment cabinet lid open counts and save to EEPROM
                //STRUCT_dispenserPacket.unionCabinetCycleCount.u16cabinetCycleCount++;
                //eeprom_write_data(ADDR_CABINET_LID_COUNTS_HIGH, STRUCT_dispenserPacket.unionCabinetCycleCount.structCabinetCycleCountBytes.u8cabinetCycleCountHigh);
                //eeprom_write_data(ADDR_CABINET_LID_COUNTS_LOW, STRUCT_dispenserPacket.unionCabinetCycleCount.structCabinetCycleCountBytes.u8cabinetCycleCountLow);
                
                
//Clear valid tag data                
                //u8refillTagData[16] = 0;
                
//Clear the dispense lockout flag
                STRUCT_statusFlags.b1lockoutDispense = 0;
                
                lu8dispenserState = PREPARE_FOR_SL_TRANSMIT;
            }
        }
        
        else
        {
            lu8dispenserState = PREPARE_FOR_SL_TRANSMIT;
        }
    }
    
    else
    {
        if(STRUCT_statusFlags.b1cabinetWasOpened)
        {
            if(u8cabinetClosedCount++ >= CABINET_CLOSED_COUNTS)
            {
                UNION_pumphouseStatusFlags.STRUCT_pumphouseFlagBits.b1cabinetStatus = 0;                
                //BLUE_LED_IO = 1;
//Stop any running LED pattern
                //GREEN_LED_IO = 0;
                //__delay_ms(500);
                //GREEN_LED_IO = 1;                  
                //stop_led_sequence( );
                //clear_led_pattern_registers( );
                //u8sleepActivateIntervals = 0;
                
                u8cabinetClosedCount = 0;
                STRUCT_statusFlags.b1cabinetWasOpened = 0;
                //ST25_init( );

//                if(STRUCT_statusFlags.b1ST25InitSuccess)    
//                {
//                    STRUCT_statusFlags.b1ST25InitSuccess = 0;
//                    platformDelay(100);
//                    read_all_tag_data( );
                    platformDelay(100);
//                    st25_hibernate( );
//                    platformDelay(10);
                    
                    u8handSensorStartupDelay = HAND_SENSOR_STARTUP_DELAY;
//                }
/*                
                else
                {
                    setup_flash_pattern(ST25_INIT_FAILURE_PATTERN_0, ST25_INIT_FAILURE_TIME_P0, ST25_INIT_FAILURE_PATTERN_1, ST25_INIT_FAILURE_TIME_P1, ST25_INIT_FAILURE_PATTERN_2, 
                                    ST25_INIT_FAILURE_TIME_P2, ST25_INIT_FAILURE_PATTERN_3, ST25_INIT_FAILURE_TIME_P3, 0, 0, 0, 0, ST25_INIT_FAILURE_REPETITIONS, ST25_INIT_FAILURE_INTERVALS);  
                    STRUCT_statusFlags.b1dispenserFailure = 1;
                } 
*/
                  
//Force the supercap voltage reading on the next wakeup interval.  This function will also check all battery voltages
// and select the proper battery source.  The loaded AA voltage flag is also cleared since a new AA may have been inserted.
                STRUCT_statusFlags.b1loadedAAVoltageLow = 0;
                STRUCT_statusFlags.b1checkVppVoltage = 1;   
            }


//Cabinet was opened but debounce was not completed            
            else
            {
                lu8dispenserState = PREPARE_FOR_SL_TRANSMIT;
            }
        }
    }
    
    return lu8dispenserState;
}
