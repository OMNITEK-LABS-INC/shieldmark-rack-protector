# 1 "global_variables.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 285 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Users/Steven/.mchp_packs/Microchip/PIC18F-Q_DFP/1.5.124/xc8\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "global_variables.c" 2
# 26 "global_variables.c"
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/musl_xc8.h" 1 3
# 5 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 2 3
# 26 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 1 3
# 133 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned __int24 uintptr_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef __int24 intptr_t;
# 164 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 2 3
# 27 "global_variables.c" 2
# 1 "./typedefs.h" 1
# 38 "./typedefs.h"
typedef enum {SLEEP_WAIT, IR_READS, ACTIVATE_VALVE, MONITOR_MOTOR,
                MOTOR_STOP, MOTOR_SHUTDOWN, PUMP_ENABLE, PREPARE_FOR_SL_TRANSMIT, CHECK_FOR_UART_XMT_WAITING, CHECK_FOR_RECEIVED_MESSAGES,
                GET_READY_FOR_SLEEP} stateMachine;

typedef struct
{
    uint8_t b1sensorActivated: 1;
    uint8_t b1PIDCalc: 1;
    uint8_t b1stopMotor: 1;
    uint8_t b1handPresent: 1;
    uint8_t b1activePulseCapture: 1;
    uint8_t b1pulseCaptured: 1;
    uint8_t b1cabinetWasOpened: 1;
    uint8_t b1cabinetIsOpen: 1;
    uint8_t b1checkVppVoltage: 1;
    uint8_t b1chargeVppActive: 1;
    uint8_t b1ST25InitSuccess: 1;
    uint8_t b1dispenserFailure: 1;
    uint8_t b1auxActive: 1;
    uint8_t b1lockoutDispense: 1;
    uint8_t b1eepromWriteError: 1;
    uint8_t b1loadedAAVoltageLow: 1;
}statusFlags;



typedef struct
{
    uint8_t b1xmtMessageInProgress: 1;
    uint8_t b1breakSignalRequired: 1;
    uint8_t b1breakSignalInProgress: 1;
    uint8_t b1breakSignalComplete: 1;
    uint8_t b1uartActive: 1;
    uint8_t b1messageStarted: 1;
    uint8_t b1messageInProgress: 1;
    uint8_t b1rcvMessageInProgress: 1;
}uartMsgStatusFlags;





typedef union
{
    uint32_t u32ledStatusFlagsRegister;

    struct{
        uint8_t b1ledSequenceActive: 1;
        uint8_t b1loadPatternSequence: 1;
        uint8_t b1ledInitialLoad: 1;
        uint8_t b1invalidRefill: 1;
        uint8_t b1handSenseInvalidRefill: 1;
        uint8_t b1covClosedInvalidRefill: 1;
        uint8_t b1pumphouseFault: 1;
        uint8_t b1covOpenedLowAuxBat: 1;
        uint8_t b1whilCovOpenLowAuxBat: 1;
        uint8_t b1noRefillLoaded: 1;
        uint8_t b1normalOperation: 1;
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
        uint8_t b1cabinetStatus: 1;
        uint8_t b1refillTagStatus: 1;
        uint8_t b1vocTagStatus: 1;
        uint8_t b1userSensorStatus: 1;
        uint8_t b1auxBatteryStatus: 2;
        uint8_t b1motorDriveFault: 1;
        uint8_t b1nfcReaderFault: 1;
    }STRUCT_pumphouseFlagBits;
}pumphouseInfo;



typedef enum {ID_GET_PACKET = 0x11, ID_SET_LED_PATTERN, ID_SET_CONFIGURATION, ID_DISPENSE_XHAND, ID_SET_BREAK_SIGNAL} slRcvdMessageId;
# 146 "./typedefs.h"
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




typedef struct
{
    union
    {
        uint8_t u8extendedSystemStatus;
        struct
        {
            uint8_t bivoltageBooster: 1;
            uint8_t b1auxBattery: 1;
            uint8_t b1eorBattery: 1;
            uint8_t b1multipleTagError: 1;
            uint8_t b1refillFormulaMismatch: 1;
            uint8_t b1refillDistMismatch: 1;
            uint8_t b1vocFormulaMismatch: 1;
            uint8_t b1statusReserved: 1;
        }structExtendedSystemStatusBytes;
    }unionExtendedSystemStatus;

    uint8_t u8eorVoltage;
    uint8_t u8auxVoltage;
    uint8_t u8systemVoltage;
}dispenserDiagnosticsPacket;






typedef struct
{
    uint8_t u8dispenseEventSubtypeHighByte;
    uint8_t u8dispenseEventSubtypeLowByte;
    uint8_t u8dispenseEventTypeHighByte;
    uint8_t u8dispenseEventTypeLowByte;
    uint8_t u8dispenseEventPayloadLength;
    uint8_t u8dispenseEventInitAbort;
}dispenserEventPacket;
# 28 "global_variables.c" 2



uint16_t u16motorWindowCount;
uint16_t u16encoderCaptureNew;
uint16_t u16encoderCapturePrevious;
uint16_t u16encoderCaptureIgnore;
uint16_t u16pulseCountDifference;


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

uint16_t infation_data[100];
uint8_t inflate_count = 0;

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

uartMsgStatusFlags STRUCT_uartMsgStatusFlags;

stateMachine dispenserState;
# 93 "global_variables.c"
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
