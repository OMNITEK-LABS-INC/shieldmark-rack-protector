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

#include<stdint.h>

#ifndef TYPEDEFS_H
#define	TYPEDEFS_H

//typedef enum {SLEEP_WAIT, LED_SEQUENCE_CHECK, CHARGE_CHECK, CHECK_CABINET, IR_READS, ACTIVATE_PUMP, MONITOR_MOTOR,
                //MOTOR_STOP, MOTOR_SHUTDOWN, PREPARE_FOR_SL_TRANSMIT, CHECK_FOR_UART_XMT_WAITING, CHECK_FOR_RECEIVED_MESSAGES,
                //MEASUREMENT_PASS_FAIL, GET_READY_FOR_SLEEP} stateMachine;

//typedef enum {SLEEP_WAIT, IR_READS, ACTIVATE_VALVE, MONITOR_MOTOR,
                //MOTOR_STOP, MOTOR_SHUTDOWN, PREPARE_FOR_SL_TRANSMIT, CHECK_FOR_UART_XMT_WAITING, CHECK_FOR_RECEIVED_MESSAGES,
                //MEASUREMENT_PASS_FAIL, GET_READY_FOR_SLEEP} stateMachine;                
typedef enum {SLEEP_WAIT, IR_READS, ACTIVATE_VALVE, MONITOR_MOTOR,
                MOTOR_STOP, MOTOR_SHUTDOWN, PUMP_ENABLE, PREPARE_FOR_SL_TRANSMIT, CHECK_FOR_UART_XMT_WAITING, CHECK_FOR_RECEIVED_MESSAGES,
                GET_READY_FOR_SLEEP} stateMachine; 
                
typedef struct 
{
    uint8_t b1sensorActivated:          1;
    uint8_t b1PIDCalc:                  1;
    uint8_t b1stopMotor:                1;
    uint8_t b1handPresent:              1;
    uint8_t b1activePulseCapture:       1;
    uint8_t b1pulseCaptured:            1;
    uint8_t b1cabinetWasOpened:         1;
    uint8_t b1cabinetIsOpen:            1;
    uint8_t b1checkVppVoltage:          1;
    uint8_t b1chargeVppActive:          1;
    uint8_t b1ST25InitSuccess:          1;
    uint8_t b1dispenserFailure:         1;
    uint8_t b1auxActive:                1;
    uint8_t b1lockoutDispense:          1;
    uint8_t b1eepromWriteError:         1;    
    uint8_t b1loadedAAVoltageLow:        1;
}statusFlags;



typedef struct
{
    uint8_t b1xmtMessageInProgress:     1;
    uint8_t b1breakSignalRequired:      1;
    uint8_t b1breakSignalInProgress:    1;
    uint8_t b1breakSignalComplete:      1;    
    uint8_t b1uartActive:               1;
    uint8_t b1messageStarted:          1;
    uint8_t b1messageInProgress:        1;
    uint8_t b1rcvMessageInProgress:     1;
}uartMsgStatusFlags;





typedef union
{
    uint32_t u32ledStatusFlagsRegister;
    
    struct{
        uint8_t b1ledSequenceActive:        1;
        uint8_t b1loadPatternSequence:      1;
        uint8_t b1ledInitialLoad:           1;
        uint8_t b1invalidRefill:            1;
        uint8_t b1handSenseInvalidRefill:   1;
        uint8_t b1covClosedInvalidRefill:   1;
        uint8_t b1pumphouseFault:           1;
        uint8_t b1covOpenedLowAuxBat:       1;
        uint8_t b1whilCovOpenLowAuxBat:     1;
        uint8_t b1noRefillLoaded:           1;
        uint8_t b1normalOperation:          1;
    }STRUCT_ledFlagBits;
}ledStatusFlags;




typedef struct
{
    uint8_t u8xmtBufferValidFlag;
    uint8_t u8xmtByteMessageLength;
    uint8_t u8transmitBufferByte[32];
}slXmtTransmitBuffer;



typedef struct
{
    uint8_t u8rcvBufferValidFlag;
    uint8_t u8rcvByteMessageLength;
    uint8_t u8rcvBufferByte[32];
}slRcvrBuffer;


typedef union
{
    uint8_t u8pumphouseStatusByte;
    
    struct
    {
        uint8_t b1cabinetStatus:            1;
        uint8_t b1refillTagStatus:          1;
        uint8_t b1vocTagStatus:             1;
        uint8_t b1userSensorStatus:         1;
        uint8_t b1auxBatteryStatus:         2;
        uint8_t b1motorDriveFault:          1;
        uint8_t b1nfcReaderFault:           1;
    }STRUCT_pumphouseFlagBits;
}pumphouseInfo;



typedef enum {ID_GET_PACKET = 0x11, ID_SET_LED_PATTERN, ID_SET_CONFIGURATION, ID_DISPENSE_XHAND, ID_SET_BREAK_SIGNAL} slRcvdMessageId;



/**********************************************************************************************
SMART LINK Packets
***********************************************************************************************/

//Dispenser Statistics
typedef struct
{
    union
    {
        uint16_t u16refillCount;
        struct
        {
            uint8_t u8refillCountLow;
            uint8_t u8refillCountHigh;
        }structRefillCountBytes;
    }unionrefillCount;

    union
    {
        uint16_t u16cabinetCycleCount;
        struct
        {
            uint8_t u8cabinetCycleCountLow;
            uint8_t u8cabinetCycleCountHigh;            
        }structCabinetCycleCountBytes;
    }unionCabinetCycleCount;
    
    union
    {
        uint32_t u32dispenseCount;
        struct
        {
            uint8_t u8dispenseCountLow;
            uint8_t u8dispenseCountMid;    
            uint8_t u8dispenseCountHigh;
            uint8_t u8dispenseCountMsb;
        }structDispenseCycleCountBytes;
    }unionDispenseCount;
}dispenserStatisticsPacket;



//Dispenser Diagnostics
typedef struct
{
    union
    {
        uint8_t u8extendedSystemStatus;
        struct
        {
            uint8_t bivoltageBooster:           1;
            uint8_t b1auxBattery:               1;
            uint8_t b1eorBattery:               1;
            uint8_t b1multipleTagError:         1;
            uint8_t b1refillFormulaMismatch:    1;
            uint8_t b1refillDistMismatch:       1;
            uint8_t b1vocFormulaMismatch:       1;
            uint8_t b1statusReserved:           1;
        }structExtendedSystemStatusBytes;
    }unionExtendedSystemStatus;

    uint8_t u8eorVoltage;
    uint8_t u8auxVoltage;
    uint8_t u8systemVoltage;
}dispenserDiagnosticsPacket;
    
    
    


//Dispenser Event
typedef struct
{
    uint8_t u8dispenseEventSubtypeHighByte;
    uint8_t u8dispenseEventSubtypeLowByte;
    uint8_t u8dispenseEventTypeHighByte;
    uint8_t u8dispenseEventTypeLowByte;
    uint8_t u8dispenseEventPayloadLength;
    uint8_t u8dispenseEventInitAbort;
}dispenserEventPacket;




#endif	/* TYPEDEFS_H */

