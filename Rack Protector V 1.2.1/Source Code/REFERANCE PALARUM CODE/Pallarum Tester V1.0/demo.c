
/**
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/*! \file
 *
 *  \author 
 *
 *  \brief Demo application
 *
 *  This demo shows how to poll for several types of NFC cards/devices and how 
 *  to exchange data with these devices, using the RFAL library.
 *
 *  This demo does not fully implement the activities according to the standards,
 *  it performs the required to communicate with a card/device and retrieve 
 *  its UID. Also blocking methods are used for data exchange which may lead to
 *  long periods of blocking CPU/MCU.
 *  For standard compliant example please refer to the Examples provided
 *  with the RFAL library.
 * 
 */
 
/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "demo.h"
#include "constants.h"
#include "utils.h"
#include "rfal_rf.h"
#include "rfal_nfca.h"
#include <stdio.h>
#include <string.h>

extern const uint8_t *p8slMessagePointer;
extern uint8_t u8bytesToTransmit;
extern statusFlags STRUCT_statusFlags;

/*
******************************************************************************
* GLOBAL DEFINES
******************************************************************************
*/

/* Definition of possible states the demo state machine could have */
#define DEMO_ST_FIELD_OFF             0
#define DEMO_ST_POLL_ACTIVE_TECH      1
#define DEMO_ST_POLL_PASSIV_TECH      2
#define DEMO_ST_WAIT_WAKEUP	          3

#define DEMO_BUF_LEN                  255
#define DEMO_NFCV_BLOCK_LEN           4

/* Macro to cycle through states */
#define	NEXT_STATE()		             {state++; state %= sizeof(stateArray);}

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */

/* State array of all possible states to be executed one after each other */
static uint8_t stateArray[] = { DEMO_ST_FIELD_OFF,
                                DEMO_ST_POLL_ACTIVE_TECH,
                                DEMO_ST_POLL_PASSIV_TECH,
                                DEMO_ST_WAIT_WAKEUP
                              };

/* P2P communication data */
static uint8_t NFCID3[] = {0x01, 0xFE, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
static uint8_t GB[] = {0x46, 0x66, 0x6d, 0x01, 0x01, 0x11, 0x02, 0x02, 0x07, 0x80, 0x03, 0x02, 0x00, 0x03, 0x04, 0x01, 0x32, 0x07, 0x01, 0x03};

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */
  
static uint8_t state = DEMO_ST_FIELD_OFF;    /*!< Actual state, starting with RF field turned off */
  
static char hexStr[40];


/*
******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************
*/

static bool demoPollNFCA( void );
static bool darwinPollNFCA( void );
static char *hex2Str(uint8_t *value, uint8_t len);
void printLog(const uint8_t *addressOfMessage);
bool darwinPollNFCA( void );
ReturnCode DarwinNfcaPollerFullCollisionResolution( rfalComplianceMode compMode, uint8_t devLimit, rfalNfcaListenDevice *nfcaDevList, uint8_t *devCnt );

char *hex2Str(uint8_t *value, uint8_t len)
{
	uint8_t valuePosition;
	char tempStr[4];
	
	if(len > 19)
	   len = 19;
	   
	sprintf(hexStr, "");
	   
	for(valuePosition = 0; valuePosition < len; valuePosition++)
	{
		sprintf(tempStr,"%02X", value[valuePosition]);
		strcat(hexStr, tempStr);
	}
    
	return hexStr;
}

bool demoPollNFCA( void );

/*!
 *****************************************************************************
 * \brief Demo Cycle
 *
 *  This function executes the actual state of the demo state machine. 
 *  Must be called cyclically
 *****************************************************************************
 */
void demoCycle( void )
{
  bool found = false;

  switch( stateArray[state] )
  {
    case DEMO_ST_FIELD_OFF:
		BLUE_LED_OUT = 1;
		RED_LED_OUT = 1;
		GREEN_LED_OUT = 1;
		    
      rfalFieldOff();
      rfalWakeUpModeStop();
      platformDelay(300);

      NEXT_STATE();
      break;


    case DEMO_ST_POLL_ACTIVE_TECH:
        
      platformDelay(40);
      NEXT_STATE();
      break;

    case DEMO_ST_POLL_PASSIV_TECH:
      found |= darwinPollNFCA();
    
      platformDelay(300);
      state = DEMO_ST_FIELD_OFF;
      break;
      
    default:
      break;
  }
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
	uint8_t           devIt = 0, idIndex;
	rfalNfcaSensRes   sensRes;
    
    uint8_t readCmdRxBuf[16];
    uint16_t readCmdRcvLen = 0;
    uint8_t writeCmdTxBuf[4] = {0x00, 0x38, 0x10, 0x0C};
    uint8_t writeCtr, readCtr;
	
	rfalNfcaPollerInitialize();   /* Initialize for NFC-A */
	rfalFieldOnAndStartGT();      /* Turns the Field On if not already and start GT timer */

	err = rfalNfcaPollerTechnologyDetection( RFAL_COMPLIANCE_MODE_NFC, &sensRes );
	if(err == ERR_NONE)
	{
		rfalNfcaListenDevice nfcaDevList[2];
		uint8_t                   devCnt;

// Call rfalNfcaPollerFullCollisionResolution with devLimit = 2 first.  If more than 1 type A 
// NFC tags are present, then the tag with the 2nd-highest UID will be active.  The tag with 
// the highest UID will be halted.  Any other tags present will not be activated. 
		err = rfalNfcaPollerFullCollisionResolution( RFAL_COMPLIANCE_MODE_NFC, 2, nfcaDevList, &devCnt );

		if ( (err == ERR_NONE) && (devCnt > 1) )
		{
// More than 1 tag was found, so we know the 2nd tag is active 
			found = true;
			devIt = 0;
            
            sprintf(hexStr, "Tag count: %u\r\n", devCnt); /* Could > 2 */
			printLog(hexStr);

			GREEN_LED_OUT = 0;
            
            if(nfcaDevList[1].type == RFAL_NFCA_T2T)    /* Ensure it's the correct type */
            {
                printLog("Writing T2T...\r\n");
                for(writeCtr = 4; writeCtr <= 5; writeCtr++)
                {
                    writeCmdTxBuf[0] = writeCtr;
                    writeCmdTxBuf[1] = 0xA2;    // Write A2 to designate the 2nd tag */

                    rfalT2TPollerWrite(writeCtr, writeCmdTxBuf);
                }

                printLog("Reading T2T...\r\n");
                for(readCtr = 4; readCtr <= 5; readCtr++)
                {
                    rfalT2TPollerRead(readCtr, readCmdRxBuf, 4, &readCmdRcvLen);
                }
            }
		}
// Note: at this point, we could do a if ( (err == ERR_NONE) && (devCnt == 1) ) because if there 
// were only 1 tag present, then the first tag would be active.  Since we are about to call 
// rfalNfcaPollerFullCollisionResolution again w/ devLimit = 1, this wouldn't save us anything. 
        
// Now call rfalNfcaPollerFullCollisionResolution with devLimit = 1.  If at least 1 type A NFC 
// tag is present, then the tag with the highest UID will be active.  Any other tags will not be activated. 
        err = rfalNfcaPollerFullCollisionResolution( RFAL_COMPLIANCE_MODE_NFC, 1, nfcaDevList, &devCnt );

        if ( (err == ERR_NONE) && (devCnt > 0) )
		{
            /* At least 1 tag was found, so we know the 1st tag is active */
			found = true;
			devIt = 0;
            
            sprintf(hexStr, "Tag count: %u\r\n", devCnt); /* Could > 1 */
			printLog(hexStr);

			GREEN_LED_OUT = 0;
            
            if(nfcaDevList[1].type == RFAL_NFCA_T2T)    /* Ensure it's the correct type */
            {
                printLog("Writing T2T...\r\n");
                for(writeCtr = 4; writeCtr <= 5; writeCtr++)
                {
                    writeCmdTxBuf[0] = writeCtr;
                    writeCmdTxBuf[1] = 0xA1;    // Write A1 to designate the 1st tag */

                    rfalT2TPollerWrite(writeCtr, writeCmdTxBuf);
                }

                printLog("Reading T2T...\r\n");
                for(readCtr = 4; readCtr <= 5; readCtr++)
                {
                    rfalT2TPollerRead(readCtr, readCmdRxBuf, 4, &readCmdRcvLen);
                }
            }
		}        
	}
	return found;
}

/*******************************************************************************
 * FUNCTION:  void printLog(const uint8_t *addressOfMessage) 
 * RETURNED VALUE:  None
 * PASSED PARAMETERS:  const uint8_t *addressOfMessage
 * PURPOSE:  Enables the UART transmitter and interrupts.  Interrupt will occur
 *              immediately after the transmitter is enabled
 ******************************************************************************/
void printLog(const uint8_t *addressOfMessage)
{
    STRUCT_statusFlags.b1uartActive = 1;
    p8slMessagePointer = addressOfMessage;
    PIR3bits.RCIF = 0;
    PIE3bits.TXIE = 1;
    
//Set number of bytes to transmit
    u8bytesToTransmit = strlen(addressOfMessage);
    
//Interrupt will occur when TXEN is set since nothing is in the transmit buffer    
    TX1STAbits.TXEN = 1;
    
    while(STRUCT_statusFlags.b1uartActive);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
