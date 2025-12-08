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
#include "motor_pwm_start_stop.h"


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void motor_pwm_start(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function is called when a hand dispense is required.
 *          TMR4 is started, TMR1 is enabled, CCP2 is enabled for the PWM, and
 *          CCP1 is enabled for the encoder captures.  All flags and variables
 *          are also initialized  
 * ***************************************************************************/
void motor_pwm_start(void)
{
//Clear flags and initialize variables
    STRUCT_statusFlags.b1stopMotor = 0;    
    u16encoderCaptureNew = 0;
    STRUCT_statusFlags.b1activePulseCapture = 0;
    STRUCT_statusFlags.b1pulseCaptured = 0;
    u16encoderCaptureIgnore = IGNORE_CAPTURE_PULSES;
    
    //if(u16pumpPulses != 0)
    //{
    //    u16motorWindowCount = u16pumpPulses;            
    //}
    
    //else
    //{
        //u16motorWindowCount = MOTOR_RUN_WINDOW_COUNT_DEFAULT;   
        u16motorWindowCount = Dispense_Amount;
    //}

    u16motorRunTimeout = MOTOR_TIMEOUT;
    
//Start TMR4 to get the 1ms interrupt
    PIR4bits.TMR4IF = 0;
    PIE4bits.TMR4IE = 1;
    T4CONbits.ON = 1;
    
//Start TMR2 and CCP2 (CCP set to start at high duty cycle)
    CCPR2H = CCPR2H_LOAD;
    CCPR2L = CCPR2L_LOAD;    
    CCP2CONbits.CCP2EN = 1;
    T2CONbits.ON = 1;
    
//Start TMR1
    TMR1 = 0;
    T1CONbits.ON = 1;
    
//Turn on opto power
    //OPTO_PWR_OUT = 1;

//Enable CCP1 
    PIR6bits.CCP1IF = 0;    
    PIE6bits.CCP1IE = 1;
    CCP1CONbits.CCP1EN = 1;

    //GREEN_LED_IO = 0;
    
    
//Start the CHARGE to the capacitors and set the flags to indicate the charge is
// active and the voltage needs to be checked.
    charge_enable( );

//Enable the motor
    //MOTOR_EN = 1;

    
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void motor_pwm_stop(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function is called when the motor needs to be shut down.
 *          Timers 1 and 2 are stopped.  CCP1 and 2 are disabled.  
 * ***************************************************************************/
void motor_pwm_stop(void)
{
//Disable the motor
    //MOTOR_EN = 0;
    
//Turn off opto power and green LED
    //OPTO_PWR_OUT = 0;   
    //GREEN_LED_IO = 1;
    
//Disable CCP2 PWM and shut down TMR2    
    CCP2CONbits.CCP2EN = 0;
    T2CONbits.ON = 0;
    
//Stop TMR1 and CCP1
    PIE6bits.CCP1IE = 0;
    CCP1CONbits.CCP1EN = 0;       
    PIR6bits.CCP1IF = 0;
    T1CONbits.ON = 0;
    
}


/*****************************************************************************
 *****************************************************************************
 * FUNCTION:  void motor_shutdown(void)
 * RECEIVED PARAMETERS:  None
 * RETURNED VALUES:  None
 * DESCRIPTION:  This function is called as the final motor shutdown step.
 *          It turns off TMR4 that is still performing some timing functions
 *          during a motor activation.  
 * ***************************************************************************/
void motor_shutdown(void)
{
    STRUCT_statusFlags.b1sensorActivated = 0;
    
//Disable interrupts for TMR4    
    T4CONbits.ON = 0;
    PIE4bits.TMR4IE = 0;
    PIR4bits.TMR4IF = 0;
}


