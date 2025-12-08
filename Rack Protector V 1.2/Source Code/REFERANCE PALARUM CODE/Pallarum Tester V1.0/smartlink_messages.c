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
#include "smartlink_messages.h"
#include "eeprom_constants.h"





/*******************************************************************************
 * FUNCTION:  void smartlink_transmit_message(void) 
 * RETURNED VALUE:  None
 * PASSED PARAMETERS:  None
 * PURPOSE:  Enables the UART transmitter and interrupts.  Interrupt will occur
 *              immediately after the transmitter is enabled
 ******************************************************************************/
void smartlink_message_transmit(void)
{
    STRUCT_uartMsgStatusFlags.b1uartActive = 1;
    
    if(STRUCT_uartMsgStatusFlags.b1breakSignalRequired)
    {
        u8transmitBufferIndex = 1;        
    }
    
    else
    {
        u8transmitBufferIndex = 0;        
    }
    

    PIE3bits.TXIE = 1;
    
 
//Interrupt will occur when TXEN is set since nothing is in the transmit buffer    
    TX1STAbits.TXEN = 1;
}


/*******************************************************************************
 * FUNCTION:  TX to scale, either to Tare or Print 
 ******************************************************************************/
void smartlink_dispenser_event_load(uint8_t u8subtypeIDHigh, uint8_t u8subtypeIDLow)
//void smartlink_dispenser_event_load(uint8_t u8subtypeIDHigh)
{

    STRUCT_dispenseEventPacket.u8dispenseEventSubtypeHighByte = u8subtypeIDHigh;
    STRUCT_dispenseEventPacket.u8dispenseEventSubtypeLowByte = u8subtypeIDLow;
    //STRUCT_dispenseEventPacket.u8dispenseEventTypeHighByte = u8typeIDHigh;
    
    if(pSTRUCT_slXmtTransmitBuffer < MAXIMUM_TRANSMIT_BUFFER_ADDRESS)
    {
        clear_transmit_buffer( );
        pSTRUCT_slXmtTransmitBuffer -> u8xmtBufferValidFlag = BUFFER_LOADED_FLAG;
        pSTRUCT_slXmtTransmitBuffer -> u8xmtByteMessageLength = DE_TOTAL_BYTES_TO_SEND;
    
        pSTRUCT_slXmtTransmitBuffer -> u8transmitBufferByte[0] = u8subtypeIDHigh;
        pSTRUCT_slXmtTransmitBuffer -> u8transmitBufferByte[1] = u8subtypeIDLow;
        //pSTRUCT_slXmtTransmitBuffer -> u8transmitBufferByte[2] = u8typeIDHigh;       
    
        pSTRUCT_slXmtTransmitBuffer++;
    }
}
    

  
/*******************************************************************************
 * FUNCTION:  void clear_buffer(void) 
 * RETURNED VALUE:  8-bit CRC calculation
 * PASSED PARAMETERS:  None
 * PURPOSE:  Clear the transmit buffer
 ******************************************************************************/
void clear_transmit_buffer(void)
{
    uint8_t i;
    
    for(i = 0; i < TRANSMIT_BUFFER_SIZE; i++)
    {
        pSTRUCT_slXmtTransmitBuffer -> u8transmitBufferByte[i] = 0;
    }    
}



/*******************************************************************************
 * FUNCTION:  uint8_t calculate_crc(void) 
 * RETURNED VALUE:  None
 * PASSED PARAMETERS:  None
 * PURPOSE:  Calculate CRC for transmit buffer
 ******************************************************************************/
uint8_t calculate_crc(void)
{
    uint8_t i;
    uint8_t u8crcCalculation;
    
    for(i = 0, u8crcCalculation = 0; i < TRANSMIT_BUFFER_SIZE; i++)
    {
        u8crcCalculation += pSTRUCT_slXmtTransmitBuffer -> u8transmitBufferByte[i];
    }
    
    return u8crcCalculation;
}



/*******************************************************************************
 * FUNCTION:  stateMachine manage_buffer_transmit(void) 
 * RETURNED VALUE:  State machine case
 * PASSED PARAMETERS:  None
 * PURPOSE:  Check for status of buffer data that needs to be transmitted
 ******************************************************************************/
stateMachine manage_buffer_transmit(void)
{
    stateMachine returnedState;
    
    
    //returnedState = CHECK_FOR_UART_XMT_WAITING;
    //if(steps == 1){
    //    returnedState = IR_READS;
    //}
    //else if(steps == 2){
    //    returnedState = ACTIVATE_PUMP;
    //}
    //else if(steps == 3){
    //    returnedState = GET_READY_FOR_SLEEP;
    //}
    if((pSTRUCT_slXmtTransmitBuffer ->u8xmtBufferValidFlag) == BUFFER_LOADED_FLAG)
    {
        if(STRUCT_uartMsgStatusFlags.b1breakSignalRequired && !STRUCT_uartMsgStatusFlags.b1breakSignalInProgress
                &&!STRUCT_uartMsgStatusFlags.b1breakSignalComplete)
        {
            TX1STAbits.SENDB = 1;
            TX1STAbits.TXEN = 1;
            TX1REG = DUMMY_BYTE;
            TX1REG = pSTRUCT_slXmtTransmitBuffer -> u8transmitBufferByte[0];  
            STRUCT_uartMsgStatusFlags.b1breakSignalInProgress = 1;
        }        
        
        else if(STRUCT_uartMsgStatusFlags.b1breakSignalInProgress && !TX1STAbits.SENDB)
        {
//            TX1STAbits.TXEN = 0;
            STRUCT_uartMsgStatusFlags.b1breakSignalInProgress = 0; 
            STRUCT_uartMsgStatusFlags.b1breakSignalComplete = 1;
        }    
        
        else if(STRUCT_uartMsgStatusFlags.b1breakSignalComplete || !STRUCT_uartMsgStatusFlags.b1breakSignalRequired)
        {
            STRUCT_uartMsgStatusFlags.b1breakSignalComplete = 0;
            pSTRUCT_slXmtTransmitBuffer ->u8xmtBufferValidFlag = BUFFER_CLEARED_FLAG;
            smartlink_message_transmit( );
            STRUCT_uartMsgStatusFlags.b1xmtMessageInProgress = 1;                   
        }
    }
    
    
    
    

//If the message was already // sent and the UART is now inactive, check for next message.  
// Buffer pointer is incremented // and the next buffer will be checked on the next 
// iteration of this case.                
    else
    {
        if(!STRUCT_uartMsgStatusFlags.b1uartActive)
        {
            if(STRUCT_uartMsgStatusFlags.b1xmtMessageInProgress)
            {
                STRUCT_uartMsgStatusFlags.b1xmtMessageInProgress = 0;  
                
                if(pSTRUCT_slXmtTransmitBuffer < MAXIMUM_TRANSMIT_BUFFER_ADDRESS)
                {
                    pSTRUCT_slXmtTransmitBuffer++;
                }
                
                else
                {
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    returnedState = CHECK_FOR_RECEIVED_MESSAGES; //was CHECK_FOR_RECEIVED_MESSAGES
                }
            }
                        
            else
            {
                pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                returnedState = CHECK_FOR_RECEIVED_MESSAGES; //was CHECK_FOR_RECEIVED_MESSAGES                                          
            }
        }
    }
              
    
                
    return returnedState;
                   
}




/*******************************************************************************
 * FUNCTION:  void check_for_received_messages(void)
 * RETURNED VALUE:  State Machine value
 * PASSED PARAMETERS:  None
 * PURPOSE:  Check for received SMART LINK messages
 ******************************************************************************/

stateMachine check_for_received_messages(void)
{
    stateMachine returnedState;
    uint8_t lu8ledMaskPattern1, lu8ledMaskPattern2, lu8patternRepetitions;
    uint8_t lu8activationDelayTime;
    uint16_t lu16pattern1Time, lu16pattern2Time;
    
       
    returnedState = CHECK_FOR_RECEIVED_MESSAGES;
    
    if((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferValidFlag) == BUFFER_LOADED_FLAG)
    {
        STATE_messageState = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[0];
        scaleRX[0] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[0];
        scaleRX[1] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1];
        scaleRX[2] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[2];
        scaleRX[3] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[3];
        scaleRX[4] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[4];
        scaleRX[5] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[5];
        scaleRX[6] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[6];
        scaleRX[7] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[7];
        scaleRX[8] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[8];
        scaleRX[9] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[9];
        scaleRX[10] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[10];
        //scaleRX[11] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[11];
        //scaleRX[12] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[12];
        //scaleRX[13] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[13];
        //scaleRX[14] = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[14];
        
        switch(STATE_messageState)
        {
            case ID_GET_PACKET:

//Check if the requested packet is the dispenser statistics                
                if(((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1]) == DS_ID_HIGH_BYTE) 
                        && ((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[2]) == DS_ID_LOW_BYTE))
                {
                    //smartlink_dispense_stats_load( );
                }

//Check if the requested packet is the refill information                           
                else if(((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1]) == DR_ID_HIGH_BYTE) 
                        && ((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[2]) == DR_ID_LOW_BYTE))
                {
                    //smartlink_refill_info_load( );                    
                }

//Check if the requested packet is the voc information                           
                else if(((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1]) == DV_ID_HIGH_BYTE) 
                        && ((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[2]) == DV_ID_LOW_BYTE))
                {
                    //smartlink_voc_info_load( );                    
                }
                
//Check if the requested packet is the dispenser diagnostics information                           
                else if(((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1]) == DD_ID_HIGH_BYTE) 
                        && ((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[2]) == DD_ID_LOW_BYTE))
                {
                    //smartlink_dispense_diagnostics_load( );
                }  

//Check if the requested packet is the dispenser pumphouse information                           
                else if(((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1]) == DP_ID_HIGH_BYTE) 
                        && ((pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[2]) == DP_ID_LOW_BYTE))
                {
                    //smartlink_dispenser_pumphouse_load( );
                }                  
                
                
                
                
            break; 

            
            case ID_SET_LED_PATTERN:
                lu8ledMaskPattern1 = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1];
                lu8ledMaskPattern1 &= PATTERN1_MASK;
                lu8ledMaskPattern1 = lu8ledMaskPattern1 >> 1;
                lu8ledMaskPattern1 = ~lu8ledMaskPattern1;
                
                lu8ledMaskPattern2 = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1];
                lu8ledMaskPattern2 &= PATTERN2_MASK;
                lu8ledMaskPattern2 = lu8ledMaskPattern2 << 3; 
                lu8ledMaskPattern2 = ~lu8ledMaskPattern2;

                
//Timing values are in 16ms intervals.  Since the timer resolution is 2ms, multiply each value by 8 (shift by 3)
                lu16pattern1Time = (uint16_t)(pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[2]) << 3;
                lu16pattern2Time = (uint16_t)(pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[3]) << 3;
                
                lu8patternRepetitions = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[4];

                //setup_flash_pattern(lu8ledMaskPattern1, lu16pattern1Time, lu8ledMaskPattern2, lu16pattern2Time, 0, 0, 0, 0, 
                //        0, 0, 0, 0, lu8patternRepetitions, 0);                    
                
            break;


            
            
            case ID_SET_CONFIGURATION:
                lu8activationDelayTime = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1];
                
                if(lu8activationDelayTime == LOCKOUT_DISPENSER_UNTIL_CAB_OPEN)
                {
                    STRUCT_statusFlags.b1lockoutDispense = 1;
                }
                
                else if(lu8activationDelayTime <= CLEAR_TIME_MAX)
                {
                
//Each count is 1 second, so multiply the value by 4 since the received value is in seconds and the variable used
// in the program counts wakeup intervals
                    u8intervalsBetweenActivations = lu8activationDelayTime;
                }
                
                else
                {
                    u8intervalsBetweenActivations = INTERVALS_BETWEEN_ACTIVATIONS_DEFAULT;                    
                }
                
//Write values to eeprom
                eeprom_write_data(ADDR_INTVLS_BETW_ACTIVATIONS, u8intervalsBetweenActivations);                
                
            break;


            
            
            case ID_DISPENSE_XHAND:
                u8xhandDispenseCommand = pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[1];
                
            break;


            
            
            case ID_SET_BREAK_SIGNAL:
                
                
            break;
            
            
            
            default:

                
            break;
            
                        
        }
        
//Clear the current buffer locations since the message was acted upon                
        clear_receive_buffer_location( );


//Increment buffer location.  This is a circular buffer.
        if(pSTRUCT_slRcvrBufferToCheck < &STRUCT_slRcvrBuffer[3])
        {
            pSTRUCT_slRcvrBufferToCheck++;            
        }
        
        else
        {
            pSTRUCT_slRcvrBufferToCheck = &STRUCT_slRcvrBuffer[0];            
        }
    }
    
    
//Check if the receiver message exceeded the timeout.  If so, reset the receiver
    if(STRUCT_uartMsgStatusFlags.b1messageInProgress || STRUCT_uartMsgStatusFlags.b1messageStarted)
    {
        if((TMR3 - u16slReceiveStartTimer) > SMARTLINK_RECEIVE_TIMEOUT)
        {
            STRUCT_uartMsgStatusFlags.b1rcvMessageInProgress = 0;
            STRUCT_uartMsgStatusFlags.b1messageStarted = 0;
            u8receiveBufferIndex = 0;            
        }
    }    
    
    
    if(!STRUCT_uartMsgStatusFlags.b1messageInProgress && !STRUCT_uartMsgStatusFlags.b1messageStarted)
    {
        returnedState = GET_READY_FOR_SLEEP;          
    }
        
    
    return returnedState; 
     
}





/*******************************************************************************
 * FUNCTION:  void clear_receive_buffer(void) 
 * RETURNED VALUE:  NONE
 * PASSED PARAMETERS:  None
 * PURPOSE:  Clear the receive buffer
 ******************************************************************************/
void clear_receive_buffer_location(void)
{
    uint8_t i;
    
    
    pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferValidFlag = BUFFER_CLEARED_FLAG;
    
    for(i = 0; i < RECEIVE_BUFFER_SIZE; i++)
    {
        pSTRUCT_slRcvrBufferToCheck -> u8rcvBufferByte[i] = 0;
    }    
}




