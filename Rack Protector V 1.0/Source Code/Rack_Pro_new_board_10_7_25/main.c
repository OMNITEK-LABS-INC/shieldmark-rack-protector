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

/*
 * Redirect stdio's putch() to EUSART1 so printf() goes out over UART.
 * PuTTY should be set to the COM port at 115200 8N1, no flow control.
 * NOTE: Signature must match stdio.h: void putch(char)
 */
void putch(char data)
{
    while(!EUSART1_is_tx_ready());
    EUSART1_Write((uint8_t)data);
}

/*
                         Main application
 */
void main(void)
{
    // Initialize the device (sets up EUSART1 among others)
    SYSTEM_Initialize();
    
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
        //RXvalue = EUSART1_Read(); // not reading anything to run

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

        if (Presence >= 0x2000){
            Presence = 0;
        }
        if (Motion >= 0x4000){
            Motion = 0;
        }

        // LED_Enble_SetLow is for the buzzer Pin to be set to low
        if ((Presence >= 0x00c0) && (Presence <= 0x0500)) { // was 0x0080
            LED1_Enble_SetHigh();
            LED2_Enble_SetHigh();
            LED_Enble_SetLow();
            printf("Object Approaching. \r\n");
        }
        else if ((Presence > 0x0500) && (Presence < 0x7D00)) // was 400 and 1000 then 500 1000
        {
            LED1_Enble_Toggle();
            LED2_Enble_Toggle();
            LED_Enble_SetLow();     // Rev 1 was Set High
            printf("Object Close! \r\n");
        }
        else {
            LED1_Enble_SetLow();
            LED2_Enble_SetLow();
            LED_Enble_SetLow();
        }
    }
}

/**
 End of File
*/
