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

#ifndef SMARTLINK_MESSAGES_H
#define	SMARTLINK_MESSAGES_H


//PROTOTYPES
void clear_transmit_buffer(void);
uint8_t calculate_crc(void);
//void setup_flash_pattern(uint8_t u8pat0, uint16_t u16pat0time, uint8_t u8pat1, uint16_t u16pat1time, uint8_t u8pat2, uint16_t u16pat2time, 
//        uint8_t u8pat3, uint16_t u16pat3time, uint8_t u8pat4, uint16_t u16pat4time, uint8_t u8pat5, uint16_t u16pat5time, uint8_t u8patRepetitions,
//        uint8_t u8sleepIntervalsRepeat);     
void clear_receive_buffer_location(void);
//void smartlink_dispense_stats_load(void);
void eeprom_write_data(uint32_t u32eepromAddress, uint8_t u8dataToWrite);
void smartlink_refill_info_load(void);
uint8_t hexToBCD(uint16_t u8valueToPack, uint8_t u8multiplier);
//uint8_t scaleRX[20];

//VARIABLES
extern uint8_t *smartLinkXmtMessagePointer;
extern statusFlags STRUCT_statusFlags;
extern slXmtTransmitBuffer STRUCT_slXmtTransmitBuffer[4];
extern slXmtTransmitBuffer *pSTRUCT_slXmtTransmitBuffer;
extern uint8_t u8transmitBufferIndex;
extern uartMsgStatusFlags STRUCT_uartMsgStatusFlags;
extern slRcvdMessageId STATE_messageState;

extern slRcvrBuffer STRUCT_slRcvrBuffer[4];
extern slRcvrBuffer *pSTRUCT_slRcvrBuffer;
extern slRcvrBuffer *pSTRUCT_slRcvrBufferToCheck;
extern dispenserStatisticsPacket STRUCT_dispenserPacket;

extern uint16_t u16slReceiveStartTimer;
extern uint8_t u8receiveBufferIndex;

extern uint8_t u8refillTagData[32];
extern uint8_t u8vocTagData[32];
extern dispenserEventPacket STRUCT_dispenseEventPacket;

extern uint8_t u8intervalsBetweenActivations;

extern dispenserDiagnosticsPacket STRUCT_diagnosticsPacket;
extern uint8_t u8xhandDispenseCommand;

extern pumphouseInfo UNION_pumphouseStatusFlags;

extern uint16_t u16eorBatteryVoltage;
extern uint16_t u16auxBatteryVoltage;
extern uint16_t u16vppVoltage;

extern uint8_t scaleRX[20];

#endif	/* SMARTLINK_MESSAGES_H */

