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

#include <stdint.h>
#include "typedefs.h"


/*************************GLOBAL VARIABLES****************************/
uint16_t u16motorWindowCount;
uint16_t u16encoderCaptureNew;
uint16_t u16encoderCapturePrevious;
uint16_t u16encoderCaptureIgnore;
uint16_t u16pulseCountDifference;
//uint8_t u8greenLedBlinkCounter;
//uint8_t start_in;
uint8_t *smartLinkXmtMessagePointer;
uint8_t u8cabinetOpenCount;
uint8_t u8cabinetClosedCount;
uint16_t u16vppCheckCounter;
uint8_t u8handSensorStartupDelay;
uint32_t u32divisionOfPulseWidth;
uint16_t u16motorRunTimeout;
uint16_t u16chargeTimeout;
uint8_t u8installedBottleUUID[7];
uint8_t u8refillTagData[32];
uint8_t u8vocTagData[32];
//uint8_t inflate_data[100];
uint16_t infation_data[100];
uint8_t inflate_count = 0;
//uint8_t inflate_loop = 0;
uint8_t deflate_count = 0;

slXmtTransmitBuffer STRUCT_slXmtTransmitBuffer[4];
slXmtTransmitBuffer *pSTRUCT_slXmtTransmitBuffer;
uint8_t u8transmitBufferIndex;

slRcvrBuffer STRUCT_slRcvrBuffer[4];
slRcvrBuffer *pSTRUCT_slRcvrBuffer;
slRcvrBuffer *pSTRUCT_slRcvrBufferToCheck;
uint8_t u8receiveBufferIndex;
slRcvdMessageId STATE_messageState;
uint8_t scaleRX[20];
uint16_t Dispense_Amount;
uint8_t SPIreceiveBYTE;

uint8_t rx_data;
uint8_t rx_data2;




statusFlags STRUCT_statusFlags;
//ledStatusFlags UNION_ledStatusFlags;
uartMsgStatusFlags STRUCT_uartMsgStatusFlags;

stateMachine dispenserState;

//uint8_t u8ledPatternNumber;

//uint8_t u8ledPattern[10];
//uint16_t u16ledTimingPattern[10];
//uint8_t u8ledPatternHold[10];
//uint16_t u16ledTimingPatternHold[10];

//uint8_t u8ledPatternRepetitions;
//uint8_t u8ledPatternRepetitionsHold;

//uint8_t u8sleepActivateIntervals;
//uint8_t u8sleepActivateIntervalsHold;

dispenserStatisticsPacket STRUCT_dispenserPacket;
dispenserEventPacket STRUCT_dispenseEventPacket;
dispenserDiagnosticsPacket STRUCT_diagnosticsPacket;
uint16_t u16slReceiveStartTimer;

volatile uint8_t u8readRcvBuffer;

uint16_t u16adcBuffer[32];
uint8_t u8adcBufferIndex;

uint8_t u8intervalsBetweenActivations;

uint16_t u16eorBatteryVoltage;
uint16_t u16auxBatteryVoltage;
uint16_t u16vppVoltage;

uint8_t u8dispenseEepromSaveCounter;
uint8_t u8xhandDispenseCommand;

pumphouseInfo UNION_pumphouseStatusFlags;

uint16_t u16pumpPulses;
 







