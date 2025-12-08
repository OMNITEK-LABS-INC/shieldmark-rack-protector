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


#include <xc.h>
#include <stdint.h>
#include "typedefs.h"
#include "main.h"
#include "constants.h"
#include "port_defines.h"
//#include "platform.h"
#include "rfal_analogConfig.h"
//#include "rfal_rf.h"
#include "rfid_control.h"
//#include "st_errno.h"
#include "rfal_nfca.h"
#include "st25r95_com.h"


//static uint8_t u8hibernate[] = {ST25R95_COMMAND_IDLE, 0x0E, 0x08, 0x04, 0x00, 0x04, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t u8hibernate[] = {ST25R95_COMMAND_IDLE, 0x0E, 0x08, 0x04, 0x00, 0x04, 0x00, 0x18, 0x00, 0x00, 0x60, 0x60, 0x64, 0x74, 0x3F, 0x10};


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void ST25_init(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function initializes the ST25.  It will try three times.
 *                  If successful, the b1ST25InitSuccess flag is set.
 * *****************************************************************************/
void ST25_init(void)
{
    uint8_t i, j;
    
    STRUCT_statusFlags.b1ST25InitSuccess = 0;
    
    platformSpiSelect();
    
    STRUCT_statusFlags.b1ST25InitSuccess = 0;
    
    for(i = ST25_INIT_TRIES; i > 0; i--)
    {
        rfalAnalogConfigInitialize();
        if( rfalInitialize() == ERR_NONE ) 
        {
            STRUCT_statusFlags.b1ST25InitSuccess = 1;
            break;
        }
    } 

    
   platformSpiDeselect();
    
//Turn all LED's off
        //BLUE_LED_IO = 1;
        //RED_LED_IO = 1;
        //GREEN_LED_IO = 1;    
}


/*******************************************************************************/
/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void st25_hibernate(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function puts the ST25 in hibernate mode
 * *****************************************************************************/
/*******************************************************************************/
void st25_hibernate(void)
{
    platformSpiSelect();
    st25r95SPISendReceiveByte(ST25R95_CONTROL_SEND);
    st25r95SPIRxTx(u8hibernate, NULL, u8hibernate[ST25R95_CMD_LENGTH_OFFSET] + 2);
    platformSpiDeselect();
}



/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void read_all_tag_data(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function reads tag data from up to 2 tags and stores all
 *                  values in RAM
 * *****************************************************************************/
void read_all_tag_data(void)
{
    bool booltagFound;
    
    booltagFound = false;
    
//Turn off the RF field    
    rfalFieldOff();
    
//Get out of wakeup mode if in it    
    rfalWakeUpModeStop();    
    
//Drives the RF transceiver operation    
    rfalWorker();
    
//Read the tags and store all tag data
    booltagFound |= darwinPollNFCA();    
    
    
    
}


/*!
 *****************************************************************************
 * \brief Poll NFC-A
 *
 * Configures the RFAL to NFC-A (ISO14443A) communication and polls for a nearby 
 * NFC-A device. 
 * If a device is found turns On a LED and logs its UID.
 *
 * Additionally, if the Device supports NFC-DEP protocol (P2P) it will activate 
 * the device and try to send an URI record.
 * If the device supports ISO-DEP protocol (ISO144443-4) it will 
 * activate the device and try exchange some APDUs with PICC.
 * 
 * 
 *  \return true    : NFC-A device found
 *  \return false   : No device found
 * 
 *****************************************************************************
 */
bool darwinPollNFCA( void )
{
	ReturnCode        err;
	bool              found = false;
	uint8_t           devIt = 0, idIndex, i, j, k;
	rfalNfcaSensRes   sensRes;
    
    uint8_t readCmdRxBuf[32];
    uint16_t readCmdRcvLen = 0;
    uint8_t writeCmdTxBuf[4] = {0x00, 0x38, 0x10, 0x0C};
    uint8_t writeCtr, readCtr;
    uint8_t devCnt;
    
    bool twoTagsFound = false;
    

//clear all tag data    
    for(i = 0; i < TAG_BUFFER_SIZE; i++)
    {
        readCmdRxBuf[i] = 0;
        u8refillTagData[i] = 0;
        u8vocTagData[i] = 0;
    }


    devCnt = 0;
	rfalNfcaPollerInitialize();   /* Initialize for NFC-A */
	rfalFieldOnAndStartGT();      /* Turns the Field On if not already and start GT timer */

	err = rfalNfcaPollerTechnologyDetection( RFAL_COMPLIANCE_MODE_NFC, &sensRes );
	if(err == ERR_NONE)
	{
		rfalNfcaListenDevice nfcaDevList[2];
		uint8_t                   devCnt;

// Call rfalNfcaPollerFullCollisionResolution with devLimit = 2 first.  If more than 1 type A 
// NFC tags are present, then the tag with the 2nd-highest UID will be active.  The tag with 
// the highest UID will be halted.  Any other tags present will not be activated.  We are sending in
// a device number of two to scan for up to two tags.  The devCnt value will reflect how many devices
// were found.
        
		err = rfalNfcaPollerFullCollisionResolution( RFAL_COMPLIANCE_MODE_NFC, 2, nfcaDevList, &devCnt );
        
		if ( (err == ERR_NONE) && devCnt > 1)
		{
            
            err = rfalT2TPollerRead(0, &readCmdRxBuf[0], 32, &readCmdRcvLen);

// More than 1 tag was found, so we know the 2nd tag is active 
			found = true;
			devIt = 0;
            
            if(nfcaDevList[1].type == RFAL_NFCA_T2T)    /* Ensure it's the correct type */
            {

                
//Read 4 pages and 4 bytes per page starting at page 0.  Each read API reads 4 pages starting at a given page (first parameter). 
// Save the data into the proper tag array.  The buffer array size is 32.  readCmdLen is a value that is written
// but the API and has a value of 16 to show that 16 bytes were read.  Read starting at page 0 for the first read, then starting
// at page 4 for the second read.
                err = rfalT2TPollerRead(0, &readCmdRxBuf[0], 32, &readCmdRcvLen);

//Read the next 4 pages
                err = rfalT2TPollerRead(4, &readCmdRxBuf[16], 32, &readCmdRcvLen);     
                
//Determine the tag type (refill or VOC) so that the data is stored in the proper array
                if((readCmdRxBuf[16] & REFILL_TAG_CODE_MASK) == REFILL_TAG_TYPE)
                {
                    UNION_pumphouseStatusFlags.STRUCT_pumphouseFlagBits.b1refillTagStatus = 1;
                    
                    for(i = 0; i < 32; i++)
                    {
                        u8refillTagData[i] = readCmdRxBuf[i];
                    }
                }
                
                else
                {
                    UNION_pumphouseStatusFlags.STRUCT_pumphouseFlagBits.b1vocTagStatus = 1;
                    
                    for(i = 0; i < 32; i++)
                    {
                        u8vocTagData[i] = readCmdRxBuf[i];
                    }
                }

//Output LED flash that two tags were found
                twoTagsFound = true;
                //setup_flash_pattern(TWO_TAGS_READ_PATTERN_0, TWO_TAGS_READ_TIME_P0, TWO_TAGS_READ_PATTERN_1, TWO_TAGS_READ_TIME_P1, 0, 
                //        0, 0, 0, 0, 0, 0, 0,TWO_TAGS_REPETITIONS, 0);   
            }
		}
//If the devCnt is greater than 0, read the tag.  If there are two tags, the first one was read and during that
// read, the tag with the highest UUID was halted.  This fullCollision call with a device limit of 1 will make the
// tag with the highest UUID active.  If there is only one tag, the first fullCollision call will not save anything
// since that call looks for more than 1 tag.  This call will also read the one tag if only one is present.
        
//Now call rfalNfcaPollerFullCollisionResolution with devLimit = 1 if the devCnt from the previous call was greater 
// than 1.  
        if(devCnt > 0)
        {
            err = rfalNfcaPollerFullCollisionResolution( RFAL_COMPLIANCE_MODE_NFC, 1, nfcaDevList, &devCnt );

            if ( err == ERR_NONE )
    		{
    			found = true;
    			devIt = 0;

                if(nfcaDevList[1].type == RFAL_NFCA_T2T)    /* Ensure it's the correct type */
                {
//Now read the tag just as we did previously
                    err = rfalT2TPollerRead(0, &readCmdRxBuf[0], 32, &readCmdRcvLen);

//Read the next 4 pages
                    err = rfalT2TPollerRead(4, &readCmdRxBuf[16], 32, &readCmdRcvLen);     
                
//Determine the tag type (refill or VOC) so that the data is stored in the proper array
                    if((readCmdRxBuf[16] & REFILL_TAG_CODE_MASK) == REFILL_TAG_TYPE)
                    {
                        UNION_pumphouseStatusFlags.STRUCT_pumphouseFlagBits.b1refillTagStatus = 1;
                        
                        for(i = 0; i < 32; i++)
                        {
                            u8refillTagData[i] = readCmdRxBuf[i];
                        }
                    }
                
                    else
                    {
                        UNION_pumphouseStatusFlags.STRUCT_pumphouseFlagBits.b1vocTagStatus = 1;
                        
                        for(i = 0; i < 32; i++)
                        {
                            u8vocTagData[i] = readCmdRxBuf[i];
                        }
                    }   
                    
                    
//If only one tag was found, output the appropriate LED sequence
                    if(twoTagsFound == false)
                    {
                        //setup_flash_pattern(ONE_TAG_READ_PATTERN_0, ONE_TAG_READ_TIME_P0, ONE_TAG_READ_PATTERN_1, ONE_TAG_READ_TIME_P1, 0, 
                        //        0, 0, 0, 0, 0, 0, 0,ONE_TAG_REPETITIONS, 0); 
                    }
                }

  
//Write the values 1, 0xAB, 0xCD, and 0xEF to page 4 of the tag (locations 16, 17, 18, 19))
//                    writeCmdTxBuf[0] = 0x11;
//                    writeCmdTxBuf[1] = 0xAB;    // Write A1 to designate the 1st tag */
//                    writeCmdTxBuf[2] = 0xCD;
//                    writeCmdTxBuf[3] = 0xEF;
//
//                    rfalT2TPollerWrite(4, writeCmdTxBuf);
                
//Write the values 0 and 225 to page 5 of the tag (locations 20,21,22,23))
//                    writeCmdTxBuf[0] = 0x0;
//                    writeCmdTxBuf[1] = 129;    // Write A1 to designate the 1st tag */
//                    writeCmdTxBuf[2] = 0;
//                    writeCmdTxBuf[3] = 0;
//
//                    rfalT2TPollerWrite(5, writeCmdTxBuf);                

            }
		}        
	}
    
//save data to eeprom
    eeprom_write_voc_tag_data( );
    eeprom_write_refill_tag_data( );  
    
//retrieve the pump pulses
    u16pumpPulses = (u8vocTagData[20] << 8) | u8vocTagData[21];
    
	return found;
}




