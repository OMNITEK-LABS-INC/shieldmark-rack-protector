


#include <xc.h>
#include "typedefs.h"
#include "main.h"
#include "constants.h"
#include "port_defines.h"









int main(int argc, char** argv) 
{   

//Power-up frequency is 1MHZ    
    //system_initialize( );
    system_initialize( );

    
//Initialize variables
    

//Start timers if any
    T0CON0bits.T0EN = 1;
//Enable interrupts if any
    PIE0bits.TMR0IE = 1;
    INTCONbits.PEIE = 1;     
    INTCONbits.GIE = 1;
    
//Get ready to go to sleep    
    //State = GET_READY_FOR_SLEEP;
    
    
    
    
            
    
/*******************************************************************************/
/*******************************************************************************/    
//Main application loop    
    while(1)
    {
        if(INTCONbits.TMR0IF_bit){
            RED_LED ^= 1;
            TMR0H = 0X0B;
            TMR0L = 0XDC;
            INTCONbits.TMR0IF_bit = 0;
            
        }
         
            
        
        
        /*switch(State)
        {
           
            case SLEEP_WAIT:
                //what you are doing while in sleep mode

        
            break;

               
            case Monitor_Battery:
                //out of sleep mode, check battery
                BATT_V;
                BATT_EN;
                BLUE_LED = 1;
                
                
            break;

                
                
            case IR_TRANSMIT:
               //if the battery is good send IR pulses
               RIGHT_TX;
               MIDDLE_TX;
               LEFT_TX;        
             
            break;


            case IR_READS:
               //see if there was an IR signal return
               if (RIGHT_RX || MIDDLE_RX || LEFT_RX = 1) {
                   OBJECT_DETECTED = 1
               }
               else (RIGHT_RX || MIDDLE_RX || LEFT_RX = 0) {
                   GET_READY_FOR_SLEEP
               }  
                
                 
               
               RIGHT_RX;
               MIDDLE_RX;
               LEFT_RX;
               
            break;
                
                
               
            case RANGE_FINDER:
               //if there is something use the range finder to find the distance
                if (OBJECT_DETECTED = 1) {
                    
                }
                    
               LASER_TX;
               LASER_EN;
               LASER_RX;
               
            break;
               

              
            case OBJECT_DETECTED:
               //if something is detected turn on different color LEDs depending on distance
                if ()
               RED_LED = 1;
               GREEN_LED = 1;
            break;
                

//Prepare to go to sleep
            case GET_READY_FOR_SLEEP:
                //prepare for sleep mode
                
            break;
                

            
            default:
            break;

                    
        }*/
    }
}
      









