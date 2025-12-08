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
#include "clock_config.h"
#include "constants.h"
#include "port_defines.h"


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void turn_on_LFINTOSC(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function changes the system clock to 31KHZ
 *****************************************************************************/
void turn_on_LFINTOSC(void)
{
    OSCCON1 = OSCCON1_LFINTOSC;
    while(!OSCCON3bits.ORDY);
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void turn_on_HFINTOSC16(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function changes the system clock to 16MHZ
 *****************************************************************************/
void turn_on_HFINTOSC16(void)
{
    OSCCON1 = OSCCON1_HFINTOSC;
    OSCFRQ = OSCFRQ_LOAD_16MHZ;
    while(!OSCCON3bits.ORDY);
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void turn_on_HFINTOSC1(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function changes the system clock to 1MHZ
 *****************************************************************************/
void turn_on_HFINTOSC1(void)
{
    OSCCON1 = OSCCON1_HFINTOSC;
    OSCFRQ = OSCFRQ_LOAD_1MHZ;
    while(!OSCCON3bits.ORDY);
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void turn_on_HFINTOSC4(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function changes the system clock to 4MHZ
 *****************************************************************************/
void turn_on_HFINTOSC4(void)
{
    while(STRUCT_uartMsgStatusFlags.b1messageInProgress){ }
    while(STRUCT_uartMsgStatusFlags.b1uartActive) { }
    SPBRGL = SPBRGL_LOAD_4MHZ;
    OSCCON1 = OSCCON1_HFINTOSC;
    OSCFRQ = OSCFRQ_LOAD_4MHZ;
    while(!OSCCON3bits.ORDY);
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void turn_on_HFINTOSC32(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function changes the system clock to 16MHZ
 *****************************************************************************/
void turn_on_HFINTOSC32(void)
{
    while(STRUCT_uartMsgStatusFlags.b1messageInProgress){ }
    while(STRUCT_uartMsgStatusFlags.b1uartActive) { }
    SPBRGL = SPBRGL_LOAD_32MHZ;
    OSCCON1 = OSCCON1_HFINTOSC;
    OSCFRQ = OSCFRQ_LOAD_32MHZ;
    while(!OSCCON3bits.ORDY);
}

