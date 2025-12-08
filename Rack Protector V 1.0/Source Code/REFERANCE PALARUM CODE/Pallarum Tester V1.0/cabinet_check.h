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

#ifndef CABINET_CHECK_H
#define	CABINET_CHECK_H


//PROTOTYPES
extern const uint8_t SL_CABINET_OPEN[ ]; 
void smartlink_transmit_message(uint8_t *addressOfXmitBuffer);
void turn_on_HFINTOSC32(void);
void read_all_tag_data(void);
//void stop_led_sequence(void);
//void setup_flash_pattern(uint8_t u8pat0, uint16_t u16pat0time, uint8_t u8pat1, uint16_t u16pat1time, uint8_t u8pat2, uint16_t u16pat2time, 
//        uint8_t u8pat3, uint16_t u16pat3time, uint8_t u8pat4, uint16_t u16pat4time, uint8_t u8pat5, uint16_t u16pat5time, uint8_t u8patRepetitions,
//        uint8_t u8sleepIntervalsRepeat);
//void stop_led_sequence(void);
void st25_hibernate(void);
//void clear_led_pattern_registers(void);
void eeprom_write_data(uint32_t u32eepromAddress, uint8_t u8dataToWrite);
uint8_t eeprom_read_data(uint32_t u32eepromAddress);
void check_for_battery_switch(void);


//VARIABLES
extern statusFlags STRUCT_statusFlags;
extern uint8_t u8cabinetOpenCount;
extern uint8_t u8cabinetClosedCount;
extern uint8_t u8handSensorStartupDelay;
//extern uint8_t u8sleepActivateIntervals;
extern dispenserStatisticsPacket STRUCT_dispenserPacket;
extern pumphouseInfo UNION_pumphouseStatusFlags;


#endif	/* CABINET_CHECK_H */

