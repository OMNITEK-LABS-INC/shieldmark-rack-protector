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
#include "interrupt_manager.h"
#include "pic18f26q10.h"




/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void __interrupt() isr_routine(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This is the interrupt function.  The interrupt sources are:
 *          TMR4 - General timer interrupt that times motor functionality
 *          CCP1 - Captures TMR1 counts on encoder wheel edges
 *          UART - UART transmit interrupts when the transmit buffer is empty
 * ***************************************************************************/

void __interrupt() isr_routine(void)
{
    uint8_t lu8pollLoopCount;
    uint8_t lu8pollCountValid;

    
//Check for timer0 interrupt. Just clear the flag.
    if(PIE0bits.TMR0IE && PIR0bits.TMR0IF)
    {
        PIR0bits.TMR0IF = 0;       
    }

    

//TMR6 interrupt is running on the 4MHZ clock and times the LED functionality
//    if(PIE4bits.TMR6IE && PIR4bits.TMR6IF)
//    {
//        PIR4bits.TMR6IF = 0;


//        if(--u16ledTimingPattern[u8ledPatternNumber] == 0)
//        {
//Do not turn off green LED if it was turned on by a motor activation
//            if(STRUCT_statusFlags.b1sensorActivated)
//            {
//                TRISA |= MAKE_LED_PINS_HIGH_EXC_GREEN;
//            }
            
//            else
//            {
//                TRISA |= MAKE_LED_PINS_HIGH;
//            }
            
//            if(u16ledTimingPattern[u8ledPatternNumber + 1])
//            {
//                u8ledPatternNumber++;
//                TRISA &= u8ledPattern[u8ledPatternNumber];
//            }
            
//            else
//            {
//                if(--u8ledPatternRepetitions == 0)
//                {
//                    UNION_ledStatusFlags.STRUCT_ledFlagBits.b1ledSequenceActive = 0;
//                    stop_led_sequence( );
//                    T6CONbits.ON = 0;                    
//                }
                
                //else
                //{
                    //reload_pattern( );
                //}
//            }
//        }
//    }

        


    
//TMR4 interrupt is running on the 32MHZ clock and times the motor run time    
    if(PIE4bits.TMR4IE && PIR4bits.TMR4IF)
    {
        PIR4bits.TMR4IF = 0;
        
        if(--u16motorRunTimeout == 0)
        {
            STRUCT_statusFlags.b1stopMotor = 1;
        }
    }


//This interrupt is for the window pulse capture.  The capture value is saved
// to be used in the motor control algorithm.  It also counts the number of
// window pulses to control the total motor travel.    
    if(PIE6bits.CCP1IE && PIR6bits.CCP1IF)
    {
        for(lu8pollLoopCount = POLL_LOOP_COUNT, lu8pollCountValid = 0; lu8pollLoopCount > 0; lu8pollLoopCount--)
        {

        }
        
        if(lu8pollCountValid >= VALID_SIGNAL_COUNT)
        {
            u16encoderCaptureNew = CCPR1;
            STRUCT_statusFlags.b1pulseCaptured = 1;
        
            if(u16encoderCaptureIgnore)
            {
                if(--u16encoderCaptureIgnore == 0)
                {
                    STRUCT_statusFlags.b1activePulseCapture = 1;
                }
            }
        
            if(u16motorWindowCount)
            {
                if(--u16motorWindowCount == 0)
                {
                    STRUCT_statusFlags.b1stopMotor = 1;
                }
            }
        }
        
        PIR6bits.CCP1IF = 0;
    }


    
    
//Interrupt for UART transmit.  If the UART is enabled and transmitting,
// this flag is set when the transmit buffer is empty.    
    if(PIR3bits.TXIF && PIE3bits.TXIE)
    {
        if(pSTRUCT_slXmtTransmitBuffer -> u8xmtByteMessageLength-- == 0)
        {
//Wait for transmit shift register to clear before shutting down the UART            
            while(!TX1STAbits.TRMT);        
            PIE3bits.TXIE = 0;
            TX1STAbits.TXEN = 0;
            STRUCT_uartMsgStatusFlags.b1uartActive = 0;            
        }
        
        else
        {
            TX1REG = pSTRUCT_slXmtTransmitBuffer -> u8transmitBufferByte[u8transmitBufferIndex++];            
        }

    }
    
    

//Interrupt for UART Receive.  A read of the RC1REG clears the interrupt flag.  If the WUE bit is not set,
// read the RC1REG and process the byte.  The WUE bit is automatically cleared on the rising edge after the break  
// signal.    
    if(PIR3bits.RC1IF && PIE3bits.RC1IE)
    //if(PIE3bits.RC1IE)
    {
        u8readRcvBuffer = RC1REG;
        
        if(!STRUCT_uartMsgStatusFlags.b1messageStarted)
        {
            STRUCT_uartMsgStatusFlags.b1messageStarted = 1;  
            u16slReceiveStartTimer = TMR3;
        }
            
        if(!BAUDCON1bits.WUE)
        {
            pSTRUCT_slRcvrBuffer -> u8rcvBufferByte[u8receiveBufferIndex] = u8readRcvBuffer; 
            //STRUCT_uartMsgStatusFlags.b1rcvMessageInProgress = 1;
            
            //if(u8receiveBufferIndex > MINIMUM_READ_BUFFER_DATA_INDEX)
            //{
            //    if((pSTRUCT_slRcvrBuffer -> u8rcvBufferByte[u8receiveBufferIndex] == UART_READ_SECOND_TERMINATOR_BYTE)
            //            && (pSTRUCT_slRcvrBuffer -> u8rcvBufferByte[(u8receiveBufferIndex - 1)] == UART_READ_FIRST_TERMINATOR_BYTE))
            //    {
                    STRUCT_uartMsgStatusFlags.b1rcvMessageInProgress = 0;
                    STRUCT_uartMsgStatusFlags.b1messageStarted = 0;
                    
                    u8receiveBufferIndex = 0;
                    pSTRUCT_slRcvrBuffer -> u8rcvBufferValidFlag = BUFFER_LOADED_FLAG;
                
                    if(pSTRUCT_slRcvrBuffer < &STRUCT_slRcvrBuffer[3])//was 3
                    {
                        pSTRUCT_slRcvrBuffer++;                    
                    }
                
                    else
                    {
                        pSTRUCT_slRcvrBuffer = &STRUCT_slRcvrBuffer[0];                    
                    }
                //}    
            //}
        }

        
        else
        {
            u8receiveBufferIndex = 0;
        }

        
        if(STRUCT_uartMsgStatusFlags.b1rcvMessageInProgress)
        {
            u8receiveBufferIndex++;            
        }
    }
}




