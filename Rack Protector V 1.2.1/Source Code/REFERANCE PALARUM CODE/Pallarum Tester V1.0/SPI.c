
#include <pic18f26q10.h>
#include "typedefs.h"
#include "port_defines.h"
/**
 * \brief Sends and receives a buffer of \c length SPI characters
 *
 * This function will send and receive a buffer of data via the SPI.
 *
 * In master mode the SPI characters will be sent immediately and the
 * received SPI character will  be read as soon as the shifting of the SPI
 * character is complete.
 *
 * In slave mode this function will place the data to be sent into the transmit
 * buffer. It will then block until an SPI master has shifted the complete
 * buffer and the received data is available.
 *
 * \param[in]  module   Pointer to the software instance struct
 * \param[in]  tx_data  Pointer to the buffer to transmit
 * \param[out] rx_data  Pointer to the buffer where received data will be stored
 * \param[in]  length   Number of SPI characters to transfer
 *
 * \return Status of the operation.
 * \retval STATUS_OK               If the operation was completed
 * \retval STATUS_ERR_INVALID_ARG  If invalid argument(s) were provided
 * \retval STATUS_ERR_TIMEOUT      If the operation was not completed within the
 *                                 timeout in slave mode
 * \retval STATUS_ERR_DENIED       If the receiver is not enabled
 * \retval STATUS_ERR_OVERFLOW     If the data is overflown
 */

uint8_t rx_data;
uint8_t rx_data2;

void SPI_Command(uint8_t spi_command)
{
    //uint8_t display_on = 0xAF;
    SS2_OUT = 0;
    SPI_A = 0;
    SSP2BUF = spi_command;
    while(!PIR3bits.SSP2IF);
    PIR3bits.SSP2IF = 0;
    SS2_OUT = 1;
    SPI_A = 1;
}
void SPI_Data(uint8_t spi_data)
{
    SS2_OUT = 0;
    SSP2BUF = spi_data;
    while(!PIR3bits.SSP2IF);
    PIR3bits.SSP2IF = 0;
    SS2_OUT = 1;
}

int spi_transceive_buffer_wait(uint8_t tx_data)
//int spi_transceive_buffer_wait(uint8_t *tx_data,uint8_t *rx_data,uint16_t length)
{
	/* Sanity check arguments */
	//if(length == 0)
    //{
	//	return -1;
	//}

	/* Send and receive buffer */
	//while(length--)
    //{
		/* Write value will be at least 8-bits long */
        //uint8_t ADC_CH1 = 0xC7;
        //uint8_t rx_data;
        uint8_t data1;
        
        SS2_OUT = 0;
        //SPI_A = 0;
        
        PIR3bits.SSP2IF = 0;
        
		/* Write the data to send */
        SSP2BUF = tx_data;
        while(!PIR3bits.SSP2IF);
        PIR3bits.SSP2IF = 0;
        data1 = SSP2BUF;
        SSP2BUF = data1;
        rx_data = data1;
        //data = SSP2BUF;
        while(!PIR3bits.SSP2IF);
        PIR3bits.SSP2IF = 0;

        SPI_A = 0;
        SS2_OUT = 1;
		/* Read value will be at least 8-bits long */
		//rx_data[rx_pos++] = SSP2BUF;
        rx_data2 = SSP2BUF;
        //data = SSP2BUF;
	//}
    
	return 0;
}
