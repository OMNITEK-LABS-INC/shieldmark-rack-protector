/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */


#ifndef XC_RFID_CONTROL_H
#define	XC_RFID_CONTROL_H

//PROTOTYPES
bool darwinPollNFCA( void );
//void setup_flash_pattern(uint8_t u8pat0, uint16_t u16pat0time, uint8_t u8pat1, uint16_t u16pat1time, uint8_t u8pat2, uint16_t u16pat2time, 
//        uint8_t u8pat3, uint16_t u16pat3time, uint8_t u8pat4, uint16_t u16pat4time, uint8_t u8pat5, uint16_t u16pat5time, uint8_t u8patRepetitions,
//        uint8_t u8sleepIntervalsRepeat);
void st25_hibernate(void);
void eeprom_write_voc_tag_data(void);
void eeprom_write_refill_tag_data(void);
extern uint8_t st25r95SPISendReceiveByte(uint8_t data);


//VARIABLES
extern uint8_t u8refillTagData[32];
extern uint8_t u8vocTagData[32];
extern pumphouseInfo UNION_pumphouseStatusFlags;
extern uint16_t u16pumpPulses;





#endif	/* XC_RFID_CONTROL_H */

