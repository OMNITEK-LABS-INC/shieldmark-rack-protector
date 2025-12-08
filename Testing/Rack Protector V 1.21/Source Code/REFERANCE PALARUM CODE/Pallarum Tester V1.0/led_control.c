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
#include "led_control.h"
#include "constants.h"
#include "port_defines.h"


void setup_flash_pattern(uint8_t u8pat0, uint16_t u16pat0time, uint8_t u8pat1, uint16_t u16pat1time, uint8_t u8pat2, uint16_t u16pat2time, 
        uint8_t u8pat3, uint16_t u16pat3time, uint8_t u8pat4, uint16_t u16pat4time, uint8_t u8pat5, uint16_t u16pat5time, uint8_t u8patRepetitions,
        uint8_t u8sleepIntervalsRepeat)
{
    stop_led_sequence( );

    clear_led_pattern_registers( );
    u8ledPatternNumber = LED_PATTERN0;    
    
    u8ledPattern[0] = u8pat0;
    u8ledPatternHold[0] = u8pat0;
    u16ledTimingPattern[0] = u16pat0time;
    u16ledTimingPatternHold[0] = u16pat0time;    
    
    u8ledPattern[1] = u8pat1;
    u8ledPatternHold[1] = u8pat1;    
    u16ledTimingPattern[1] = u16pat1time;
    u16ledTimingPatternHold[1] = u16pat1time;   
    
    u8ledPattern[2] = u8pat2;
    u8ledPatternHold[2] = u8pat2;
    u16ledTimingPattern[2] = u16pat2time;
    u16ledTimingPatternHold[2] = u16pat2time;    
    
    u8ledPattern[3] = u8pat3;
    u8ledPatternHold[3] = u8pat3;    
    u16ledTimingPattern[3] = u16pat3time;
    u16ledTimingPatternHold[3] = u16pat3time;   
    
    u8ledPattern[4] = u8pat4;
    u8ledPatternHold[4] = u8pat4;
    u16ledTimingPattern[4] = u16pat4time;
    u16ledTimingPatternHold[4] = u16pat4time;    
    
    u8ledPattern[5] = u8pat5;
    u8ledPatternHold[5] = u8pat5;    
    u16ledTimingPattern[5] = u16pat5time;
    u16ledTimingPatternHold[5] = u16pat5time;   
    
    u8ledPatternRepetitions = u8patRepetitions; 
    u8ledPatternRepetitionsHold = u8patRepetitions;
    
    u8sleepActivateIntervals = u8sleepIntervalsRepeat;
    u8sleepActivateIntervalsHold = u8sleepIntervalsRepeat;

    UNION_ledStatusFlags.STRUCT_ledFlagBits.b1ledInitialLoad = 1;
    UNION_ledStatusFlags.STRUCT_ledFlagBits.b1loadPatternSequence = 1; 

}





void clear_led_pattern_registers(void)
{
    uint8_t i;
    
    for(i = 0; i < 10; i++)
    {
        u8ledPattern[i] = 0;
        u16ledTimingPattern[i] = 0;
        u8ledPatternHold[i] = 0;
        u16ledTimingPatternHold[i] = 0;
    }
    
}




void start_led_timer(void)
{
    T6PR = T6PR_LOAD;
    T6CLKCON = T6CLKCON_LOAD;
    T6HLT = T6HLT_LOAD;
    T6CON = T6CON_LOAD;
    T6TMR = 0;

    PIR4bits.TMR6IF = 0;
    PIE4bits.TMR6IE = 1;
    T6CONbits.ON = 1; 
    
    TRISA |= MAKE_LED_PINS_HIGH;
    TRISA &= u8ledPattern[0];     
}



void reload_pattern(void)
{
    uint8_t i;
    
    for(i = 0; i < 9; i++)
    {
        u8ledPattern[i] = u8ledPatternHold[i];
        u16ledTimingPattern[i] = u16ledTimingPatternHold[i];
    }
    
   
    u8ledPatternNumber = LED_PATTERN0;
    
    TRISA |= MAKE_LED_PINS_HIGH;
    TRISA &= u8ledPattern[0];        
    
}


void check_for_continuous_LED_pattern(void)
{
    uint8_t i;
    
    if(u8sleepActivateIntervals)
    {
        if(--u8sleepActivateIntervals == 0)
        {
            u8sleepActivateIntervals = u8sleepActivateIntervalsHold;
            u8ledPatternRepetitions = u8ledPatternRepetitionsHold;
            
            for(i = 0; i < 10; i++)
            {
                u8ledPattern[i] = u8ledPatternHold[i];
                u16ledTimingPattern[i] = u16ledTimingPatternHold[i];
            }
   
            u8ledPatternNumber = LED_PATTERN0;
    
//            LATA |= MAKE_LED_PINS_HIGH;
//            LATA &= u8ledPattern[0]; 

            UNION_ledStatusFlags.STRUCT_ledFlagBits.b1ledSequenceActive = 1;            
            start_led_timer( );

        }
    }   
    
    else
    {
        stop_led_sequence( );
    }
}


void stop_led_sequence(void)
{
    T6CONbits.ON = 0;        
    PIR4bits.TMR6IF = 0;
    PIE4bits.TMR6IE = 0;
    TRISA |= MAKE_LED_PINS_HIGH;
    
    UNION_ledStatusFlags.u32ledStatusFlagsRegister = 0;
}





