///-----------------------------------------------------------------------
///  Copyright (c) ${2025}, Omnitek, LLC
///  All Rights Reserved
///
///  This code is confidential and proprietary of Omnitek, LLC
///  and may only be used under a license from Omnitek, LLC
///
///
///     @brief
///
/// V 1.2.1 Added testing functionality via PUTTY inputs
///
/// V 1.2 Added battery level voltage detection, added functionality at low voltage levels
///
/// V 1.1 Disabled buzzer, fixed range for closer sensing
///
/// V 1.0 Dan Buday Preliminary Version
///
///    Created by Bradley Staron 10/24/2025
///-----------------------------------------------------------------------

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/i2c1_master.h"
#include "mcc_generated_files/i2c1_master_ex.h"
//#include "mcc_generated_files/i2c1_master_example.h"
#include <stdio.h>  // needed for printf/putch

#define I2C_Slave_Address 0x5a//was 0xb4
#define I2C_CTRL1_Reg 0x20
#define I2C_CTRL1_Data 0x10
#define I2C_CTRL1_Data2 0x18 // was 0x14 makes auto time 33ms this is too fast; resetting to 500ms
#define I2C_OneShot_Reg 0x21
#define I2C_OneShot_Data 0x01
#define I2C_DataReady_Reg 0x23
#define I2C_Ambient_Reg 0x28
#define I2C_Ambient_Reg2 0x29
#define I2C_Presence_Reg 0x3A
#define I2C_Presence_Reg2 0x3B
#define I2C_Motion_Reg 0x3C
#define I2C_Motion_Reg2 0x3D
#define I2C_ClearDRDY_Reg 0x25
#define I2C_LPF1_Reg 0x0C
#define I2C_LPF1_Data  0x14
#define I2C_LPF2_Reg 0x0D
#define I2C_TRIM_Reg 0x10
#define I2C_TRIM_Data 0x02
#define I2C_Embeded_Set_Data 0x10
#define I2C_PresThresh_L 0x21
#define I2C_PresThresh_H 0x20
#define I2C_PresThresh_Data_L 0xC8
#define I2C_PresThresh_Data_H 0x00
#define I2C_MotThresh_L 0x23
#define I2C_MotThresh_H 0x22
#define I2C_MotThresh_Data_L 0x2C
#define I2C_MotThresh_Data_H 0x01
#define I2C_PresHyst 0x27
#define I2C_PresHyst_Data 0x14
#define I2C_MotHyst 0x26
#define I2C_MotHyst_Data 0x1E
#define I2C_Algo_Reset 0x2A
#define I2C_Algo_Reset_Data 0x01
#define I2C_LPF2_Data 0x32
#define I2C_FUNCSTAT_Reg 0x25

uint8_t RXvalue;
uint8_t RX_I2C_Ready;
uint8_t RX_I2C_P1;
uint8_t RX_I2C_P2;
uint8_t RX_I2C_A1;
uint8_t RX_I2C_A2;
uint8_t RX_I2C_M1;
uint8_t RX_I2C_M2;
uint8_t RX_I2C_L1;
uint8_t RX_I2C_L2;
uint8_t RX_I2C_FS;
uint16_t Presence;
uint16_t Motion;
uint32_t FilterPresence;

// Global variables
uint8_t RX_I2C_P1, RX_I2C_P2;
uint8_t RX_I2C_M1, RX_I2C_M2;
uint16_t Presence, Motion;

// Manual control flags
uint8_t buzzerOverride = 0;   // if 1 ? buzzer forced ON
uint8_t led1_manual = 0;      // if 1 ? LED1 forced ON
uint8_t led2_manual = 0;      // if 1 ? LED2 forced ON


// -----------------------------------------------------------------------------
// *** ADDED FOR 3-SECOND STRONG-PRESENCE HOLD ***
// We simulate timing based on the loop delay (~500ms per cycle).
// -----------------------------------------------------------------------------
uint32_t presenceHoldStart = 0;
uint8_t presenceHoldActive = 0;   // 1 = currently holding the blink state for 3 seconds
// -----------------------------------------------------------------------------


// * NOTE: * PuTTY should be set to the COM port at 115200 8N1, no flow control.
 
void putch(char data)
{
    while(!EUSART1_is_tx_ready());
    EUSART1_Write((uint8_t)data);
}


float Read_BatteryVoltage(void)
{
    uint16_t adcValue;
    float vBatt;

    // Ensure enable pin is an output
    
    
    // Enable the battery sense circuit
    ANA_V_EN_SetHigh();
    __delay_ms(5);      // allow Q7/Q8 to stabilize
    
    ADPCH = channel_ANB4;

    // Read ADC channel AN9
    adcValue = ADCC_GetSingleConversion(channel_ANB4);

    // Convert ADC value to voltage at ANA_V_READ
    float vRead = ((float)adcValue * 3.3f) / 1024.0f;
    vBatt = vRead * 2.98f;      
    
    return vBatt;
}

void main(void)
{
    // Initialize the device (sets up EUSART1 among others)
    SYSTEM_Initialize();
    
    uint32_t lastBlinkTime = 0;   // Timer for 30-sec blink interval
    
    ANA_V_EN_SetDigitalOutput();
    RC6_SetDigitalMode();
    RC7_SetDigitalMode();
    
    // Print a startup banner to the serial terminal
    printf("Firmware Version 1.2\r\n");
    
    __delay_ms(500); // let part boot up
    I2C1_Write1ByteRegister(I2C_Slave_Address, I2C_TRIM_Reg, I2C_TRIM_Data);
    __delay_ms(20);
    RX_I2C_L1 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_LPF1_Reg); // read LPF1
    RX_I2C_L2 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_LPF2_Reg); // read LPF2
    //I2C1_Write1ByteRegister(I2C_Slave_Address, I2C_OneShot_Reg, I2C_OneShot_Data);
    __delay_ms(20);
    I2C1_Write1ByteRegister(I2C_Slave_Address, I2C_CTRL1_Reg, I2C_CTRL1_Data); // set block reads
    __delay_ms(20);
    I2C1_Write1ByteRegister(I2C_Slave_Address, I2C_LPF1_Reg, I2C_LPF1_Data);

    I2C1_Write1ByteRegister(I2C_Slave_Address, I2C_CTRL1_Reg, I2C_CTRL1_Data2); // was 33ms too fast

    FiveV_Enble_SetLow();  // new
    //LED1_Enble_SetHigh();
    //LED2_Enble_SetHigh();

    while (1)
    {   
        // ---------------- KEYBINDS TO PRINT OUT TO PUTTY ----------------
        float batt = Read_BatteryVoltage();
        
        static uint32_t msCounter = 0;   // Accumulates loop timing

        msCounter += 500;  // because loop delays ~500ms per iteration

        if (EUSART1_is_rx_ready())
        {
            char key = EUSART1_Read();

            switch (key)
            {
                case 'h':   // print a menu with the keybinds listed 
                    printf("\r\nRACK PROTECTOR FW V1.21 help menu - keybinds \r\n"
                            "'v' Prints battery voltage level \r\n"
                            "'b' Turns buzzer on \r\n"
                            "'i' Turns LED Bank 1 on \r\n"
                            "'o' Turns LED Bank 2 on \r\n"
                            "'p' Turns LED Bank 1, 2, and the buzzer off \r\n");
                    break;
                    
                case 'v':   // Print Voltage Measurement
                    printf("ADC raw: %u  Batt: %.2f V\r\n", (unsigned)ADCC_GetConversionResult(), batt);
                    break;

                case 'b':   // ENABLE buzzer manually
                    buzzerOverride = 1;
                    LED_Enble_SetHigh();
                    printf("Buzzer Enabled\r\n");
                    break;

                case 'i':   // manual LED1 ON
                    led1_manual = 1;
                    LED1_Enble_SetHigh();
                    printf("LED BANK 1 ON\r\n");
                    break;

                case 'o':   // manual LED2 ON
                    led2_manual = 1;
                    LED2_Enble_SetHigh();
                    printf("LED BANK 2 ON\r\n");
                    break;
                    
                case 'p':   // manual LED1 LED2 buzzer off
                    buzzerOverride = 0;
                    led1_manual = 0;
                    led2_manual = 0;
                    LED_Enble_SetLow();
                    LED1_Enble_SetLow();
                    LED2_Enble_SetLow();
                    printf("All off\r\n");
                    break;

                default:
                    break;
            }
        }
        
         // ---------------- LOW BATTERY RAPID BLINK EVERY ~30 SECONDS ----------------
        
        if (batt < 7.25f)
        {
            if (msCounter - lastBlinkTime >= 90000)   // Blinks every ~30 Seconds
            {
                // Blink LEDs 15 times
                for (int i = 0; i < 15; i++)
                {
                    LED1_Enble_SetHigh();
                    LED2_Enble_SetHigh();
                    __delay_ms(150);

                    LED1_Enble_SetLow();
                    LED2_Enble_SetLow();
                    __delay_ms(150);
                }

                lastBlinkTime = msCounter;  // reset the 30s timer
            }
        }
        
        // ---------------- READS DATA FROM I2C & Blinks LEDS/BUZZER ----------------
        
        RX_I2C_Ready = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_DataReady_Reg);

        __delay_ms(500);
        RX_I2C_P1 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_Presence_Reg);
        RX_I2C_P2 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_Presence_Reg2);

        __delay_ms(50);
        RX_I2C_M1 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_Motion_Reg);
        RX_I2C_M2 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_Motion_Reg2);

        // Assemble 16-bit values with explicit casting (avoids sign/width warnings)
        Presence = (uint16_t)((uint16_t)RX_I2C_P1 | ((uint16_t)RX_I2C_P2 << 8));
        Motion   = (uint16_t)((uint16_t)RX_I2C_M1 | ((uint16_t)RX_I2C_M2 << 8));

        //if (Presence >= 0x2000){
        //    Presence = 0;
        //}
        //if (Motion >= 0x4000){
        //    Motion = 0;
        //}

        // Handle buzzer override first
        if (buzzerOverride)
        {
            LED_Enble_SetHigh();    // force buzzer ON
        }

        // LED Manual Overrides
        if (led1_manual) LED1_Enble_SetHigh();
        if (led2_manual) LED2_Enble_SetHigh();

        // If BOTH LEDs are manual, skip presence logic entirely
        if (led1_manual && led2_manual)
        {
            continue;
        }
        
        uint8_t inCloseRange = (Presence > 0x0500 && Presence < 0x7D00);
        uint8_t inNearRange  = (Presence >= 0x00C0 && Presence <= 0x0500);

        // -----------------------------------------------------------
        // STRONG RANGE (Blink LEDs, enable buzzer)
        // -----------------------------------------------------------
        if (inCloseRange)
        {
            // *** ADDED: Start the 3-second hold timer only when first entering
            if (!presenceHoldActive)
            {
                presenceHoldActive = 1;
                presenceHoldStart = msCounter;   // Start delay timer
            }

            if (!led1_manual) LED1_Enble_Toggle();
            if (!led2_manual) LED2_Enble_Toggle();
            if (!buzzerOverride) LED_Enble_SetHigh();

            printf("Object Close! Presence: %u\r\n", Presence);
        }

        // -----------------------------------------------------------
        // NEAR RANGE (Solid ON)
        // -----------------------------------------------------------
        else if (inNearRange)
        {
            presenceHoldActive = 0; // Cancel any hold

            if (!led1_manual) LED1_Enble_SetHigh();
            if (!led2_manual) LED2_Enble_SetHigh();
            if (!buzzerOverride) LED_Enble_SetLow();

            printf("Object Approaching. Presence: %u\r\n", Presence);
        }

        // -----------------------------------------------------------
        // OUT OF RANGE
        // -----------------------------------------------------------
        else
        {
            // *** ADDED: Continue blinking for remainder of 3-second hold ***
            if (presenceHoldActive)
            {
                if ((msCounter - presenceHoldStart) < 3000)
                {
                    // keep blinking and buzzer on
                    if (!led1_manual) LED1_Enble_Toggle();
                    if (!led2_manual) LED2_Enble_Toggle();
                    if (!buzzerOverride) LED_Enble_SetHigh();

                    // DO NOT turn LEDs off yet ? remain in this section
                    continue;
                }
                else
                {
                    // *** ADDED: Hold expired ***
                    presenceHoldActive = 0;
                }
            }

            // *** NORMAL OUT-OF-RANGE BEHAVIOR ***
            if (!led1_manual) LED1_Enble_SetLow();
            if (!led2_manual) LED2_Enble_SetLow();
            if (!buzzerOverride) LED_Enble_SetLow();
        }

        // =========================================================================
        // END OF MODIFIED PRESENCE BLOCK
        // =========================================================================
    }
}
/**
 End of File
*/
