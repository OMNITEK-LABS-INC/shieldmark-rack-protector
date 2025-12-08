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

#ifndef INTERRUPT_MANAGER_H
#define	INTERRUPT_MANAGER_H

//PROTOTYPES
//void reload_pattern(void);
//void stop_led_sequence(void);


//VARIABLES
extern statusFlags STRUCT_statusFlags;
extern uint16_t u16encoderCaptureIgnore;
extern uint16_t u16encoderCaptureNew;
extern uint8_t *smartLinkXmtMessagePointer;
extern statusFlags STRUCT_statusFlags;
extern uint16_t u16motorWindowCount;
extern uint16_t u16motorRunTimeout;

//extern ledStatusFlags UNION_ledStatusFlags;
extern uartMsgStatusFlags STRUCT_uartMsgStatusFlags;

//extern uint8_t u8ledPatternNumber;
//extern uint8_t u8ledPatternRepetitions;
//extern uint8_t u8ledPattern[10];
//extern uint16_t u16ledTimingPattern[10];
//extern uint8_t u8ledPatternRepetitionsHold;

extern uint8_t u8transmitBufferIndex;

extern slXmtTransmitBuffer STRUCT_slXmtTransmitBuffer[4];
extern slXmtTransmitBuffer *pSTRUCT_slXmtTransmitBuffer;

extern slRcvrBuffer STRUCT_slRcvrBuffer[4];
extern slRcvrBuffer *pSTRUCT_slRcvrBuffer;
extern uartMsgStatusFlags STRUCT_uartMsgStatusFlags;
extern uint8_t u8receiveBufferIndex;

extern volatile uint8_t u8readRcvBuffer;

extern uint16_t u16slReceiveStartTimer;



#endif	/* INTERRUPT_MANAGER_H */

