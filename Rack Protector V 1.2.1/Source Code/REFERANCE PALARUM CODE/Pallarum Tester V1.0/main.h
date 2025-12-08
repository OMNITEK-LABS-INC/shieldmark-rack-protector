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

#ifndef MAIN_H
#define	MAIN_H

#define _XTAL_FREQ 4000000


//VARIABLES
//extern uint8_t u8greenLedBlinkCounter;
//extern uint8_t start_in;

extern statusFlags STRUCT_statusFlags;
extern stateMachine dispenserState;

extern const uint8_t SL_CABINET_OPEN[ ]; 
extern const uint8_t SL_REFILL_CHANGE[ ];
extern const uint16_t *p16slMessagePointer;
extern uint16_t u16vppCheckCounter;
extern uint8_t u8handSensorStartupDelay;
extern uint16_t u16chargeTimeout;

//extern ledStatusFlags UNION_ledStatusFlags;
extern uartMsgStatusFlags STRUCT_uartMsgStatusFlags;

extern slXmtTransmitBuffer STRUCT_slXmtTransmitBuffer[4];
extern slXmtTransmitBuffer *pSTRUCT_slXmtTransmitBuffer;

extern slRcvrBuffer STRUCT_slRcvrBuffer[4];
extern slRcvrBuffer *pSTRUCT_slRcvrBuffer;
extern slRcvrBuffer *pSTRUCT_slRcvrBufferToCheck;
extern uint8_t u8receiveBufferIndex;
extern uint8_t u8adcBufferIndex;
extern uint8_t u8refillTagData[32];
extern dispenserStatisticsPacket STRUCT_dispenserPacket;
extern uint8_t u8dispenseEepromSaveCounter;
extern uint8_t u8xhandDispenseCommand;
extern uint16_t Dispense_Amount;
extern uint8_t SPIreceiveBYTE;

extern uint8_t rx_data;
extern uint8_t rx_data2;

uint8_t scaleRX[20];
uint8_t X;
uint8_t Y;
uint8_t Z;
uint8_t inflate_loop;


//PROTOTYPES
void system_initialize(void);
uint16_t IR_Read(uint8_t u8adpchLoadValue);
uint16_t getRunningAverage(uint16_t u16latestADCReading);
void turn_on_HFINTOSC4(void);
//void motor_pwm_start(void);
//void motor_pwm_stop(void);
//void motor_control(void);
void motor_shutdown(void);
stateMachine hand_scan(void);
void smartlink_message_transmit(void);
void get_ready_for_sleep(void);
stateMachine cabinet_check(void);
void turn_on_HFINTOSC32(void);
unsigned char hand_rx(void);
void charge_enable(void);
extern const uint8_t SL_DISPENSE_MESSAGE[ ]; 
void ST25_init(void);
//void supercap_charge_check(void);
void SPI_Command(uint8_t spi_command);
void SPI_Data(uint8_t spi_data);

void read_all_tag_data(void);
//void start_led_timer(void);
//void check_for_continuous_LED_pattern(void);

void smartlink_dispenser_event_load(uint8_t u8subtypeIDHigh, uint8_t u8subtypeIDLow);
//void smartlink_dispenser_event_load(uint8_t u8subtypeIDHigh);
stateMachine manage_buffer_transmit(void);
//void stop_led_sequence(void);

stateMachine check_for_received_messages(void);


void smartlink_rcv_loop_test_set_LED_pattern(uint8_t u8commandCode, uint8_t u8ledPattern, uint8_t u8pattern1Time, 
        uint8_t u8pattern2Time, uint8_t u8cycleCount, uint8_t u8terminatorByte0, uint8_t u8terminatorByte1);
void st25_hibernate(void);
//void setup_flash_pattern(uint8_t u8pat0, uint16_t u16pat0time, uint8_t u8pat1, uint16_t u16pat1time, uint8_t u8pat2, uint16_t u16pat2time, 
//        uint8_t u8pat3, uint16_t u16pat3time, uint8_t u8pat4, uint16_t u16pat4time, uint8_t u8pat5, uint16_t u16pat5time, uint8_t u8patRepetitions,
//        uint8_t u8sleepIntervalsRepeat);
void FVR_enable(void);
void FVR_disable(void);
uint16_t adc_read_vpp(uint8_t u8adcMuxSelect);

void eeprom_read_voc_tag_data(void);
void eeprom_read_refill_tag_data(void);

extern uint16_t u16vppVoltage;

uint8_t eeprom_read_data(uint32_t u32eepromAddress);
void eeprom_program_default_values(void);
void eeprom_read_programmed_values(void);
void eeprom_write_data(uint32_t u32eepromAddress, uint8_t u8dataToWrite);

#endif	/* MAIN_H */

