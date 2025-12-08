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
#include "motor_control.h"
#include "constants.h"
#include "port_defines.h"


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void motor_control(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function compares the encoder capture values and modifies 
 *          the CCP2 duty cycle register on the PWM to achieve speed control.  
 * ***************************************************************************/
void motor_control(void)
{
    uint16_t u16ccpr2NewValue;
    uint16_t u16ccpr2Hold;

    
    STRUCT_statusFlags.b1pulseCaptured = 0;
    u16ccpr2Hold = CCPR2;

//Save the captured values as the previous value since no motor control is 
// performed while pulses are ignored at startup    
    if(!STRUCT_statusFlags.b1activePulseCapture)
    {
        u16encoderCapturePrevious = u16encoderCaptureNew;    
    }

//Perform the motor control    
    else
    {
        u16pulseCountDifference = u16encoderCaptureNew - u16encoderCapturePrevious;
        u16encoderCapturePrevious = u16encoderCaptureNew;
        
        if(u16pulseCountDifference < TARGET_RPM_TIME_BTW_PULSES)
        {
            u32divisionOfPulseWidth = (uint32_t)((u16pulseCountDifference * 100) / TARGET_RPM_TIME_BTW_PULSES);
            u16ccpr2NewValue = (uint16_t)(u16ccpr2Hold - ((u16ccpr2Hold * u32divisionOfPulseWidth / 100) >> PROPORTIONAL_CONSTANT_DECREASE));
            
            if(u16ccpr2Hold < MIN_DUTY_CYCLE)
            {
                CCPR2 = MIN_DUTY_CYCLE;
            }
            
            else
            {
                CCPR2 = u16ccpr2NewValue;
            }
        }
        
        else if(u16pulseCountDifference > TARGET_RPM_TIME_BTW_PULSES)
        {
            u32divisionOfPulseWidth = (uint32_t)((u16pulseCountDifference * 100) / TARGET_RPM_TIME_BTW_PULSES);
            u16ccpr2NewValue = (uint16_t)(u16ccpr2Hold + ((u16ccpr2Hold * u32divisionOfPulseWidth / 100) >> PROPORTIONAL_CONSTANT_INCREASE));
            
            if(u16ccpr2Hold > MAX_DUTY_CYCLE)
            {
                CCPR2 = MAX_DUTY_CYCLE;
            }
            
            else
            {
                CCPR2 = u16ccpr2NewValue;
            }            
        }
    }
}    



