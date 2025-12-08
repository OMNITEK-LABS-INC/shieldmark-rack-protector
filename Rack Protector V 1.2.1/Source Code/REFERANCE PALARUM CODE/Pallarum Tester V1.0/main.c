/* 

 */



#include <xc.h>
#include <stdint.h>
#include "typedefs.h"
#include "main.h"
#include "constants.h"
#include "port_defines.h"
#include "platform.h"
#include "rfal_analogConfig.h"
#include "rfal_rf.h"
#include "demo.h"
#include "eeprom_constants.h"





int main(int argc, char** argv) 
{   
    uint16_t u16supercapStartupReading;
    uint8_t motor_runs;
    uint8_t motor_cycles;
    uint8_t motor_cycles_const = 15;
    uint8_t start;
    


//Power-up frequency is 1MHZ   
    //DISPLAY_BACKLIGHT = 1;
    system_initialize( );
    
    
//Initialize variables
    //u8greenLedBlinkCounter = GREEN_LED_INTERVAL_COUNTS;
    u16vppCheckCounter = VPP_COUNTER_RELOAD;
    u8handSensorStartupDelay = HAND_SENSOR_STARTUP_DELAY;
    u16chargeTimeout = CHARGE_TIMEOUT_STARTUP;
    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
    pSTRUCT_slRcvrBuffer = &STRUCT_slRcvrBuffer[0];
    pSTRUCT_slRcvrBufferToCheck = &STRUCT_slRcvrBuffer[0];
    u8receiveBufferIndex = 0;  
    u8adcBufferIndex = 0;
    motor_runs = 0;
    motor_cycles = 0;
    X = 0xB0;
    //start = 0;

    
    //STRUCT_uartMsgStatusFlags.b1breakSignalRequired = 1;  //this adds in UART break
 
//Disable the EOR output andtTurn on AUX output (already on at startup, 
// just showing it is on)
    charge_enable( );
    
    STRUCT_statusFlags.b1chargeVppActive = 1;
    STRUCT_statusFlags.b1auxActive = 1;
    
    
  
//Start timer 0
    T0CON0bits.T0EN = 1;
    
//Enable timer 0 interrupt so that it can wake the micro from sleep
    PIE0bits.TMR0IE = 1; 
    
//Initialize state machine to be SLEEP_WAIT
    dispenserState = SLEEP_WAIT;
    
//Turn the motor enable line FET on to disable the motor
    platformDelay(500);
    
//Enable global interrupts    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;  
    
//Enable UART receive interrupt
    PIE3bits.RC1IE = 1;
    
//Set SPI chip select high 
    SS2_OUT = 1;
    SPI_A = 1;

//Get ready to go to sleep    
    dispenserState = GET_READY_FOR_SLEEP;
    
//Program default values if they have not been programmed yet.  Otherwise, read them.
    if(eeprom_read_data(ADDR_EEPROM_INITIALIZED_FLAG) == EEPROM_PROGRAMMED_FLAG) 
        eeprom_read_programmed_values( );
    else
        eeprom_program_default_values( );
    
     
//Turn on global interrupts
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;       
   
    
    extern uint8_t rx_data;
    extern uint8_t rx_data2;
    uint8_t inflate_count = 0;
    uint16_t Pressurize_loops = 0;
    uint8_t deflate_count = 0;
    uint16_t inflate_data[10];
    uint16_t infation_data[100];
    uint8_t tx_data1;
    uint8_t tx_data2;
    uint8_t start_count = 0;
    uint8_t start_count2 = 0;
    uint8_t SD = 0;
           


/*******************************************************************************/
/*******************************************************************************/    
//Main application loop    
    while(1)
    {
        switch(dispenserState)
        {
//Turn on the low power internal timer and go to sleep.  When coming out of
// sleep, disable timer 0 since we don't need a timer0 interrupt.  Then turn on 
// the 4MHZ internal timer, clear interrupt flags, change the state machine case 
// and enable the global interrupts            
            case SLEEP_WAIT:
                
//Disable the FVR and turn off the ADC with the mux set to Vss                
                FVR_disable( );
                ADPCH = ADPCH_LOAD_VSS;
                ADCON0bits.ADON = 0;
                
                CPUDOZE = 0;
                
                T3CONbits.ON = 0;
                BAUD1CONbits.WUE = 1;
                
                asm("nop");
                asm("nop");
                asm("sleep");
                asm("nop");
                asm("nop");
                
                
                FVR_enable( );
                ADCON0bits.ADON = 1;
                
                T3CONbits.ON = 1;
                
                //Check Soap or Sani selector switch

                
                dispenserState = IR_READS; //was CHECK_CABINET
                
                start = START_IN; 
        
            break;





//Perform the hand scan.  The hand_scan( ) routine returns the proper state back
//to this calling routine                
            case IR_READS:
                platformDelay(10);
                if(VISUAL_DEFECT_IN == 0){
                    smartlink_dispenser_event_load(0x56,0x44);  //transmit VD visual defect
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                    dispenserState = CHECK_FOR_RECEIVED_MESSAGES;
                    
                }
                if(RESET_IN == 0){
                    smartlink_dispenser_event_load(0x52,0x45);  //transmit RE reset
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                    dispenserState = PUMP_ENABLE;
                    
                    
                }
                if(start == 0)
                {
                    //YELLOW_LED = 1;
                    //SPI_Data(0xAA);
                    if(start_count == 0){
                        smartlink_dispenser_event_load(0x53,0x42); //SB start button

                        pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                        dispenserState = manage_buffer_transmit(); 
                        
                        //dispenserState = PREPARE_FOR_SL_TRANSMIT;
                        //dispenserState = ACTIVATE_VALVE;
                        start_count++;
                    }
                    else{
                        dispenserState = GET_READY_FOR_SLEEP;
                    }
                    
                    //spi_transceive_buffer_wait(0xA2, SPIreceiveBYTE, 1);
                    platformDelay(50);
                    dispenserState = check_for_received_messages( );
                    if(scaleRX[0] = (0x47)){
                        //YELLOW_LED = 1; 
                        dispenserState = ACTIVATE_VALVE;
                    }
                }
                else
                {
                    //BLUE_LED_IO = 0;
                    dispenserState = GET_READY_FOR_SLEEP;
                
                }
                
                platformDelay(500);
                
                if(EMERGENCY_IN == 0){
                    SD = 2;
                    DIST_VALVE = 0;
                    smartlink_dispenser_event_load(0x45,0x53); //transmit ES emergency stop
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                    dispenserState = PUMP_ENABLE;
                    break;
                }
                
            break;

                
//If a hand is detected, the clock is changed to the 16MHZ internal clock.  A
// Smart Link message is sent to signify that the dispenser is activated. The
// motor is started and the case is changed to monitor the motor functionality.                
            case ACTIVATE_VALVE:
                //platformDelay(100);
                PUMP1 = 0;
                PUMPVALVE = 0;
                DIST_VALVE = 1;
                //turn_on_HFINTOSC4( );
                
                YELLOW_LED = 1;
                while(Pressurize_loops < 290) //290 for 1.5min?
                {

                spi_transceive_buffer_wait(0xC7);
                platformDelay(85);
                inflate_data[inflate_count] = rx_data;
                inflate_count++;
                platformDelay(15);//was 10
                inflate_data[inflate_count] = rx_data2;
                smartlink_dispenser_event_load(inflate_data[inflate_count-1], inflate_data[inflate_count]);
                //smartlink_dispenser_event_load(0x41,0x42);
                inflate_count++;
                
                pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                dispenserState = manage_buffer_transmit(); 
 
                
                spi_transceive_buffer_wait(0xCF);
                platformDelay(85);
                inflate_data[inflate_count] = rx_data;
                inflate_count++;
                platformDelay(15); //was 10
                inflate_data[inflate_count] = rx_data2;
                smartlink_dispenser_event_load(inflate_data[inflate_count-1], inflate_data[inflate_count]); 
                //smartlink_dispenser_event_load(0x43,0x44);
                inflate_count++;
               
                pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                dispenserState = manage_buffer_transmit();
                
                spi_transceive_buffer_wait(0xD7);
                platformDelay(85);
                inflate_data[inflate_count] = rx_data;
                inflate_count++;
                platformDelay(15); //was 10
                inflate_data[inflate_count] = rx_data2;
                smartlink_dispenser_event_load(inflate_data[inflate_count-1], inflate_data[inflate_count]);
                //smartlink_dispenser_event_load(0x45,0x46);
                
                                
                pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                dispenserState = manage_buffer_transmit(); 
                
                inflate_count = 0;
                
                    if(EMERGENCY_IN == 0){
                        SD = 2;
                        DIST_VALVE = 0;
                        dispenserState = PUMP_ENABLE;
                        break;
                    }
                
                    if(RESET_IN == 0){
                        SD = 3;
                        DIST_VALVE = 0;
                        dispenserState = PUMP_ENABLE;
                        break;
                    }    
                
                    if(SENSOR_DEFECT_IN == 0){
                        SD = 1;
                        DIST_VALVE = 0;
                        dispenserState = PUMP_ENABLE;
                        break;
                    }
                
                Pressurize_loops++;
                }
                //platformDelay(10000);
                inflate_count = 0;
                //while(PIR3bits.TXIF != 1){
                //}

                YELLOW_LED = 0;
                if(Pressurize_loops == 290)
                {
                    dispenserState = MONITOR_MOTOR; 
                }
                Pressurize_loops = 0;
                                
             
            break;


//Monitor the run time for the motor and also check if a pulse was captured from
// the encoder wheel.  If a pulse is captured, call the motor_control( )
// function to perform the speed control.When the motor timer expires, change
// the state to stop the motor.
            case MONITOR_MOTOR:
                platformDelay(1000);
                smartlink_dispenser_event_load(0x66,0x66); 
                //platformDelay(1000);
                pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                dispenserState = manage_buffer_transmit(); 
                dispenserState = MOTOR_STOP;

            break;
                
                
//Stop the motor                
            case MOTOR_STOP:
                platformDelay(100);
                //motor_pwm_stop( );
                //YELLOW_LED = 1;
                DIST_VALVE = 0;
                dispenserState = MOTOR_SHUTDOWN;
            break;


//Shut down motor functionality and change the state                
            case MOTOR_SHUTDOWN:
                //YELLOW_LED = 1;
                
                while(Pressurize_loops < 484)//484 for 2.5min
                {

                spi_transceive_buffer_wait(0xC7);
                platformDelay(100);
                inflate_data[inflate_count] = rx_data;
                inflate_count++;
                //platformDelay(15);//was 10
                inflate_data[inflate_count] = rx_data2;
                smartlink_dispenser_event_load(inflate_data[inflate_count-1], inflate_data[inflate_count]);
                //smartlink_dispenser_event_load(0x41,0x42);
                inflate_count++;
                
                pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                dispenserState = manage_buffer_transmit(); 
 
                
                spi_transceive_buffer_wait(0xCF);
                platformDelay(100);
                inflate_data[inflate_count] = rx_data;
                inflate_count++;
                //platformDelay(15); //was 10
                inflate_data[inflate_count] = rx_data2;
                smartlink_dispenser_event_load(inflate_data[inflate_count-1], inflate_data[inflate_count]); 
                //smartlink_dispenser_event_load(0x43,0x44);
                inflate_count++;
               
                pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                dispenserState = manage_buffer_transmit();
                
                spi_transceive_buffer_wait(0xD7);
                platformDelay(100);
                inflate_data[inflate_count] = rx_data;
                inflate_count++;
                //platformDelay(15); //was 10
                inflate_data[inflate_count] = rx_data2;
                smartlink_dispenser_event_load(inflate_data[inflate_count-1], inflate_data[inflate_count]);
                //smartlink_dispenser_event_load(0x45,0x46);
                
                                
                pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                dispenserState = manage_buffer_transmit(); 
                
                inflate_count = 0;
                
                if(EMERGENCY_IN == 0){
                    SD = 2;
                    DIST_VALVE = 0;
                    dispenserState = PUMP_ENABLE;
                    break;
                }
                
                if(RESET_IN == 0){
                    SD = 3;
                    DIST_VALVE = 0;
                    dispenserState = PUMP_ENABLE;
                    break;
                }    
                
                if(SENSOR_DEFECT_IN == 0){
                    //smartlink_dispenser_event_load(0x73,0x64);
                    //pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    //dispenserState = manage_buffer_transmit(); 
                    SD = 1;
                    DIST_VALVE = 0;
                    dispenserState = PUMP_ENABLE;
                    break;
                }
                
                Pressurize_loops++;
                }
                //platformDelay(10000);
                inflate_count = 0;
                //while(PIR3bits.TXIF != 1){
                //}

                //YELLOW_LED = 0;
                if(Pressurize_loops == 484)
                {
                    dispenserState = PUMP_ENABLE; 
                }
                Pressurize_loops = 0;

            break;
                
            case PUMP_ENABLE:
                //YELLOW_LED = 1;
                if(SD == 0){
                    smartlink_dispenser_event_load(0x66,0x66); //normal test ff
                    platformDelay(1000);
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                    //dispenserState = MOTOR_STOP;
                    PUMP1 = 1;
                    PUMPVALVE = 1;
                    platformDelay(1000);
                    platformDelay(1000);
                    platformDelay(1000);
                    PUMP1 = 0;
                    PUMPVALVE = 0;
                    smartlink_dispenser_event_load(0x46,0x54); 
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                }
                else if(SD == 1){
                    smartlink_dispenser_event_load(0x73,0x64); //sensor error sd
                    platformDelay(1000);
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                    //dispenserState = MOTOR_STOP;
                    PUMP1 = 0;
                    PUMPVALVE = 0;
                    platformDelay(1000);
                    platformDelay(1000);
                    smartlink_dispenser_event_load(0x46,0x54); 
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                }
                else if(SD == 2){
                    smartlink_dispenser_event_load(0x65,0x73); //emergency stop es
                    platformDelay(1000);
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                    //dispenserState = MOTOR_STOP;
                    PUMP1 = 0;
                    PUMPVALVE = 0;
                    platformDelay(1000);
                    platformDelay(1000);
                    smartlink_dispenser_event_load(0x46,0x54); 
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                }

                //platformDelay(1000);
                //pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                //dispenserState = manage_buffer_transmit(); 
                //PUMP1 = 0;
                //PUMPVALVE = 0;
                //platformDelay(1000);
                //platformDelay(1000);
                //smartlink_dispenser_event_load(0x46,0x54); 
                //pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                //dispenserState = manage_buffer_transmit(); 
                
                else if(SD == 3){
                    smartlink_dispenser_event_load(0x72,0x73); //reset test re
                    platformDelay(1000);
                    pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                    dispenserState = manage_buffer_transmit(); 
                    dispenserState = SLEEP_WAIT;
                    start = 1;
                    platformDelay(1000);
                    platformDelay(1000);
                    PUMP1 = 0;
                    PUMPVALVE = 0;
                    void clear_transmit_buffer();
                }
                SD = 0;
            break;
//Set pointer to transmit buffers to first buffer
            case PREPARE_FOR_SL_TRANSMIT:
              
                
                //turn_on_HFINTOSC4( );
                //pSTRUCT_slXmtTransmitBuffer = STARTING_TRANSMIT_BUFFER_ADDRESS;
                //dispenserState = CHECK_FOR_UART_XMT_WAITING;
            break;

            
//Wait for the UART to finish and change the state to go to sleep                
            case CHECK_FOR_UART_XMT_WAITING:
                //dispenserState = manage_buffer_transmit( );

            break;
 
            
//Check for received messages
            case CHECK_FOR_RECEIVED_MESSAGES:
                dispenserState = check_for_received_messages( );
                
                if(scaleRX[0] == 0x54)
                {
                    //YELLOW_LED = 1;
                    //dispenserState = MEASUREMENT_PASS_FAIL;
                }
                else if(scaleRX[0] == 0x53)
                {
                    //YELLOW_LED = 1;
                    //dispenserState = MEASUREMENT_PASS_FAIL; 
                }
                else          
                {   
                    break;
                    //dispenserState = GET_READY_FOR_SLEEP;
                }
//Check the dispensed amount and pass or fail
            //case MEASUREMENT_PASS_FAIL:
   
 
                //break;
//Prepare to go to sleep
            case GET_READY_FOR_SLEEP:
                get_ready_for_sleep( );
                dispenserState = SLEEP_WAIT;
                
            break;
                

            
            default:
                break;

                    
        }
    }
}

