/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F26Q10
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/i2c1_master.h"
#include "mcc_generated_files/i2c1_master_ex.h"
//#include "mcc_generated_files/i2c1_master_example.h"
#define I2C_Slave_Address 0x5a//was 0xb4
#define I2C_CTRL1_Reg 0x20
#define I2C_CTRL1_Data 0x10
#define I2C_CTRL1_Data2 0x18 // was 0x14 makes auto time 33ms this is to fast resetting to 500mS
#define I2C_OneShot_Reg 0x21
#define I2C_OneShot_Data 0x01
#define I2C_DataReady_Reg 0x23
#define I2C_Ambient_Reg 0x28
#define I2C_Ambient_Reg2 0x29
#define I2C_Presence_Reg 0x3A    //was a
#define I2C_Presence_Reg2 0x3B    //was 3b
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
#define I2C_LPF1_Reg 0x0C
#define I2C_LPF2_Reg 0x0D
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
uint16_t convertedValue;
uint32_t FilterPresence;
adc_result_t convertedValue;
/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

 
    __delay_ms(500); //let part boot up
    I2C1_Write1ByteRegister(I2C_Slave_Address,I2C_TRIM_Reg,I2C_TRIM_Data);
    __delay_ms(20);
    RX_I2C_L1 = I2C1_Read1ByteRegister(I2C_Slave_Address,I2C_LPF1_Reg); //ser reg LPF1
    RX_I2C_L2 = I2C1_Read1ByteRegister(I2C_Slave_Address,I2C_LPF2_Reg); //set reg LPF2
        //I2C1_Write1ByteRegister(I2C_Slave_Address,I2C_OneShot_Reg,I2C_OneShot_Data);
    __delay_ms(20);
    I2C1_Write1ByteRegister(I2C_Slave_Address,I2C_CTRL1_Reg,I2C_CTRL1_Data); //set block reads
    __delay_ms(20);
    I2C1_Write1ByteRegister(I2C_Slave_Address,I2C_LPF1_Reg,I2C_LPF1_Data);
      
    I2C1_Write1ByteRegister(I2C_Slave_Address,I2C_CTRL1_Reg,I2C_CTRL1_Data2); //was 33ms to fast
    
    FiveV_Enble_SetLow();  //new
    //LED1_Enble_SetHigh();
    //LED2_Enble_SetHigh();
    
    while (1)
    {
        //RXvalue = EUSART1_Read(); //not reading anything to run
        BATT_Enble_SetHigh();
        convertedValue = ADCC_GetSingleConversion(channel_ANB4);
        
        RX_I2C_Ready = I2C1_Read1ByteRegister(I2C_Slave_Address,I2C_DataReady_Reg);

        __delay_ms(500); //was 500
        RX_I2C_P1 = I2C1_Read1ByteRegister(I2C_Slave_Address,I2C_Presence_Reg);
        RX_I2C_P2 = I2C1_Read1ByteRegister(I2C_Slave_Address,I2C_Presence_Reg2);
        

            __delay_ms(50);
        RX_I2C_M1 = I2C1_Read1ByteRegister(I2C_Slave_Address,I2C_Motion_Reg);
        RX_I2C_M2 = I2C1_Read1ByteRegister(I2C_Slave_Address,I2C_Motion_Reg2);

        Presence = (RX_I2C_P1 + (RX_I2C_P2 << 8));
        Motion = (RX_I2C_M1 + (RX_I2C_M2 << 8));
        if(Presence >= 0x1000){
            Presence = 0;
        }
        if(Motion >= 0x7D00){
            Motion = 0;
        }

        if((Presence >= 0x00c0)&&(Presence <= 0x0500)){ //was 0x0080
            
            LED1_Enble_SetHigh();
            LED2_Enble_SetHigh();
            //BUZZ_Enble_SetLow();
        }
        else if((Presence > 0x0500)&&(Presence <0x7D00)) //ws 400 and 1000 then 500 1000 then 500 2000
        {
            
            LED1_Enble_Toggle();
            LED2_Enble_Toggle();
            //BUZZ_Enble_SetHigh();
        }
        else{
            
            LED1_Enble_SetLow();
            LED2_Enble_SetLow();
            //BUZZ_Enble_SetLow();
        }
        
    }
}
/**
 End of File
*/