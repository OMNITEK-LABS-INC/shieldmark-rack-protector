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
#include "eeprom_read_write.h"
#include "eeprom_constants.h"




/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void eeprom_read_voc_tag_data(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function reads the VOC tag data stored in eeprom
 * **************************************************************************/
void eeprom_read_voc_tag_data(void)
{
    uint8_t i;
    
    NVMCON0bits.NVMEN = 1;
    
    NVMADRL = VOC_EEPROM_TAG_LOW_ADDRESS;
    NVMADRH = VOC_EEPROM_TAG_HIGH_ADDRESS;
    NVMADRU = VOC_EEPROM_TAG_UPPER_ADDRESS;
    
    for(i = 0; i < VOC_EEPROM_BYTE_COUNT; i++)
    {
        NVMCON1bits.RD = 1;
        u8vocTagData[i] = NVMDATL;
        NVMADRL++;
    }
    
    NVMCON0bits.NVMEN = 0;
}



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void eeprom_read_refill_tag_data(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function reads the refill tag data stored in eeprom
 * **************************************************************************/
void eeprom_read_refill_tag_data(void)
{
    uint8_t i;
    
    NVMCON0bits.NVMEN = 1;
    
    NVMADRL = REFILL_EEPROM_TAG_LOW_ADDRESS;
    NVMADRH = REFILL_EEPROM_TAG_HIGH_ADDRESS;
    NVMADRU = REFILL_EEPROM_TAG_UPPER_ADDRESS;
    
    for(i = 0; i < REFILL_EEPROM_BYTE_COUNT; i++)
    {
        NVMCON1bits.RD = 1;
        u8refillTagData[i] = NVMDATL;
        NVMADRL++;
    }
    
    NVMCON0bits.NVMEN = 0;
}



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void eeprom_write_voc_tag_data(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function writes the VOC tag data to eeprom
 * **************************************************************************/
void eeprom_write_voc_tag_data(void)
{
    uint8_t i, u8writeError;
    
    NVMADRL = VOC_EEPROM_TAG_LOW_ADDRESS;
    NVMADRH = VOC_EEPROM_TAG_HIGH_ADDRESS;
    NVMADRU = VOC_EEPROM_TAG_UPPER_ADDRESS;
    
    NVMCON0bits.NVMEN = 1;
    
    for(i = 0; i < VOC_EEPROM_BYTE_COUNT; i++)
    {
        NVMDATL = u8vocTagData[i];
        INTCONbits.GIE = 0;
        
        NVMCON2 = 0x55;
        NVMCON2 = 0xAA;
        
        NVMCON1bits.WR = 1;
        while(NVMCON1bits.WR);
        
        INTCONbits.GIE = 1;
        
        NVMADRL++;
    }


//Verify EEPROM write by reading back the EEPROM written values    
    NVMADRL = VOC_EEPROM_TAG_LOW_ADDRESS;
    NVMADRH = VOC_EEPROM_TAG_HIGH_ADDRESS;
    NVMADRU = VOC_EEPROM_TAG_UPPER_ADDRESS;
    
    u8writeError = 0;
    
    for(i = 0; i < VOC_EEPROM_BYTE_COUNT; i++)
    {
        NVMCON1bits.RD = 1;
        
        if(NVMDATL != u8vocTagData[i])
        {
            u8writeError = 1;
        }

        NVMADRL++;
    }
    
    NVMCON0bits.NVMEN = 0;    
    
    if(u8writeError)
    {
        STRUCT_statusFlags.b1eepromWriteError = 1;            
    }
}



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void eeprom_write_refill_tag_data(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function writes the refill tag data to eeprom
 * **************************************************************************/
void eeprom_write_refill_tag_data(void)
{
    uint8_t i, u8writeError;
    
    NVMADRL = REFILL_EEPROM_TAG_LOW_ADDRESS;
    NVMADRH = REFILL_EEPROM_TAG_HIGH_ADDRESS;
    NVMADRU = REFILL_EEPROM_TAG_UPPER_ADDRESS;
    
    NVMCON0bits.NVMEN = 1;
    
    for(i = 0; i < REFILL_EEPROM_BYTE_COUNT; i++)
    {
        NVMDATL = u8refillTagData[i];
        INTCONbits.GIE = 0;
        
        NVMCON2 = 0x55;
        NVMCON2 = 0xAA;
        
        NVMCON1bits.WR = 1;
        while(NVMCON1bits.WR);
        
        INTCONbits.GIE = 1;
        
        NVMADRL++;
    }
    
    
//Verify EEPROM write by reading back the EEPROM written values    
    NVMADRL = REFILL_EEPROM_TAG_LOW_ADDRESS;
    NVMADRH = REFILL_EEPROM_TAG_HIGH_ADDRESS;
    NVMADRU = REFILL_EEPROM_TAG_UPPER_ADDRESS;
    
    u8writeError = 0;
    
    for(i = 0; i < REFILL_EEPROM_BYTE_COUNT; i++)
    {
        NVMCON1bits.RD = 1;
        
        if(NVMDATL != u8refillTagData[i])
        {
            u8writeError = 1;
        }

        NVMADRL++;
    }
    
    NVMCON0bits.NVMEN = 0;    
    
    if(u8writeError)
    {
        STRUCT_statusFlags.b1eepromWriteError = 1;            
    }    
}



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void eeprom_write_data(uint32_t u32eepromAddress, uint8_t u8dataToWrite)
 * RECEIVED PARAMETERS:  32-bit eeprom address to write, 8-bit data
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function writes one byte to the specified eeprom address
 * **************************************************************************/
void eeprom_write_data(uint32_t u32eepromAddress, uint8_t u8dataToWrite)
{
    NVMADRL = (uint8_t)(u32eepromAddress & 0x000000FF);
    NVMADRH = (uint8_t)((u32eepromAddress & 0x0000FF00) >> 8);
    NVMADRU = (uint8_t)((u32eepromAddress & 0x00FF0000) >> 16);
    
    NVMCON0bits.NVMEN = 1;
    
    NVMDATL = u8dataToWrite;
    INTCONbits.GIE = 0;
        
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
        
    NVMCON1bits.WR = 1;
    while(NVMCON1bits.WR);
       
    INTCONbits.GIE = 1;
    
//Verify the eeprom write
    NVMCON1bits.RD = 1;
    
    if(NVMDATL != u8dataToWrite)
    {
        STRUCT_statusFlags.b1eepromWriteError = 1;            
    }        
    
    NVMCON0bits.NVMEN = 0;
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  uint8_t eeprom_read_data(uint32_t u32eepromAddress)
 * RECEIVED PARAMETERS:  32-bit eeprom address to read
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function reads one byte of eeprom at the specified eeprom address
 * **************************************************************************/
uint8_t eeprom_read_data(uint32_t u32eepromAddress)
{
    NVMADRL = (uint8_t)(u32eepromAddress & 0x000000FF);
    NVMADRH = (uint8_t)((u32eepromAddress & 0x0000FF00) >> 8);
    NVMADRU = (uint8_t)((u32eepromAddress & 0x00FF0000) >> 16);
    
    NVMCON0bits.NVMEN = 1;
    NVMCON1bits.RD = 1;
    NVMCON0bits.NVMEN = 0;        
    
    return NVMDATL;
}
    


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void eeprom_program_default_values(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function programs the EEPROM with all default values
 * **************************************************************************/
void eeprom_program_default_values(void)
{
    eeprom_write_data(ADDR_EEPROM_INITIALIZED_FLAG, EEPROM_PROGRAMMED_FLAG);
    eeprom_write_data(ADDR_INTVLS_BETW_ACTIVATIONS, INTERVALS_BETWEEN_ACTIVATIONS_DEFAULT);    
    eeprom_write_data(ADDR_CABINET_LID_COUNTS_HIGH, 0);
    eeprom_write_data(ADDR_CABINET_LID_COUNTS_LOW, 0);  
    eeprom_write_data(ADDR_DISPENSE_COUNTS_HIGH, 0);  
    eeprom_write_data(ADDR_DISPENSE_COUNTS_MID, 0);  
    eeprom_write_data(ADDR_DISPENSE_COUNTS_LOW, 0);  
    
}



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void eeprom_read_programmed_values(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function reads the programmed EEPROM values
 * **************************************************************************/
void eeprom_read_programmed_values(void)
{
    u8intervalsBetweenActivations = eeprom_read_data(ADDR_INTVLS_BETW_ACTIVATIONS);
    STRUCT_dispenserPacket.unionCabinetCycleCount.structCabinetCycleCountBytes.u8cabinetCycleCountHigh
            = eeprom_read_data(ADDR_CABINET_LID_COUNTS_HIGH);
    STRUCT_dispenserPacket.unionCabinetCycleCount.structCabinetCycleCountBytes.u8cabinetCycleCountLow
            = eeprom_read_data(ADDR_CABINET_LID_COUNTS_LOW);    
    STRUCT_dispenserPacket.unionDispenseCount.structDispenseCycleCountBytes.u8dispenseCountHigh 
            = eeprom_read_data(ADDR_DISPENSE_COUNTS_HIGH);
    STRUCT_dispenserPacket.unionDispenseCount.structDispenseCycleCountBytes.u8dispenseCountMid 
            = eeprom_read_data(ADDR_DISPENSE_COUNTS_MID);
    STRUCT_dispenserPacket.unionDispenseCount.structDispenseCycleCountBytes.u8dispenseCountLow 
            = eeprom_read_data(ADDR_DISPENSE_COUNTS_LOW);

}
    
    
    
