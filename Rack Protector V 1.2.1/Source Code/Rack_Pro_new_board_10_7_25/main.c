///-----------------------------------------------------------------------
///  Copyright (c) 2025, Omnitek, LLC
///  All Rights Reserved
///
///  V 1.2.2 Updated presence detection to only set off buzzer 0.5 seconds
///          if presence is detected (near qualification / debounce)
///
///  V 1.2.1 Added testing functionality via PUTTY inputs, added delay to strong presence
///  V 1.2   Added battery level voltage detection, added functionality at low voltage levels
///  V 1.1   Disabled buzzer, fixed range for closer sensing
///  V 1.0   Dan Buday Preliminary Version
///
///  Created by Bradley Staron 10/24/2025
///-----------------------------------------------------------------------

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/i2c1_master.h"
#include "mcc_generated_files/i2c1_master_ex.h"
#include <stdio.h>

// ======================== Sensor / I2C Defines =========================
#define I2C_Slave_Address 0x5a
#define I2C_CTRL1_Reg     0x20
#define I2C_CTRL1_Data    0x10
#define I2C_CTRL1_Data2   0x18
#define I2C_DataReady_Reg 0x23
#define I2C_Presence_Reg  0x3A
#define I2C_Presence_Reg2 0x3B
#define I2C_Motion_Reg    0x3C
#define I2C_Motion_Reg2   0x3D
#define I2C_LPF1_Reg      0x0C
#define I2C_LPF1_Data     0x14
#define I2C_LPF2_Reg      0x0D
#define I2C_TRIM_Reg      0x10
#define I2C_TRIM_Data     0x02

// ============================= Globals =================================
static uint8_t  RX_I2C_Ready;
static uint8_t  RX_I2C_P1, RX_I2C_P2;
static uint8_t  RX_I2C_M1, RX_I2C_M2;
static uint8_t  RX_I2C_L1, RX_I2C_L2;
static uint16_t Presence;
static uint16_t Motion;

// Manual control flags
static uint8_t buzzerOverride = 0;   // if 1 -> buzzer forced ON
static uint8_t led1_manual    = 0;   // if 1 -> LED bank 1 forced ON
static uint8_t led2_manual    = 0;   // if 1 -> LED bank 2 forced ON

// ========================= Presence State Machine =======================
typedef enum {
    PRESENCE_IDLE = 0,
    PRESENCE_NEAR,
    PRESENCE_STRONG,
    PRESENCE_HOLD
} presence_state_t;

static presence_state_t pState = PRESENCE_IDLE;

// Strong hold timer (uses msCounter)
static uint32_t presenceHoldStart = 0;

// Near qualification: consecutive valid near samples
// NOTE: Your loop sampling is roughly ~550ms (500ms + 50ms delays),
// so 1 sample ~= 0.55s. If you want "exactly" ~0.5s, keep at 1.
// If you want more noise immunity, use 2.
#define PRESENCE_NEAR_QUAL_COUNTS  1
static uint8_t nearCount = 0;

// ============================= UART printf =============================
void putch(char data)
{
    while(!EUSART1_is_tx_ready());
    EUSART1_Write((uint8_t)data);
}

// ========================== Battery Voltage =============================
static float Read_BatteryVoltage(void)
{
    uint16_t adcValue;

    // Enable the battery sense circuit
    ANA_V_EN_SetHigh();
    __delay_ms(5);

    ADPCH = channel_ANB4;
    adcValue = ADCC_GetSingleConversion(channel_ANB4);

    // Convert ADC value to voltage at ANA_V_READ
    float vRead = ((float)adcValue * 3.3f) / 1024.0f;
    float vBatt = vRead * 2.98f;

    // OPTIONAL: disable sense circuit to save power / avoid loading
    ANA_V_EN_SetLow();

    return vBatt;
}

// ===================== Output Apply (single authority) ==================
// desired values are 0=OFF, 1=ON
static void ApplyOutputs(uint8_t led1_on, uint8_t led2_on, uint8_t buzzer_on)
{
    // Manual overrides have priority
    if (led1_manual) {
        LED1_Enble_SetHigh();
    } else {
        if (led1_on) LED1_Enble_SetHigh();
        else         LED1_Enble_SetLow();
    }

    if (led2_manual) {
        LED2_Enble_SetHigh();
    } else {
        if (led2_on) LED2_Enble_SetHigh();
        else         LED2_Enble_SetLow();
    }

    if (buzzerOverride) {
        LED_Enble_SetHigh();      // forced ON
    } else {
        if (buzzer_on) LED_Enble_SetHigh();
        else           LED_Enble_SetLow();
    }
}

// ================================ main ==================================
void main(void)
{
    SYSTEM_Initialize();

    // Pin modes (keep what you had)
    ANA_V_EN_SetDigitalOutput();
    RC6_SetDigitalMode();
    RC7_SetDigitalMode();

    // IMPORTANT: restore your board-specific rail enable behavior
    FiveV_Enble_SetLow();

    // Startup buzzer beeps
    for (uint8_t i = 0; i < 5; i++)
    {
        LED_Enble_SetHigh();
        __delay_ms(100);
        LED_Enble_SetLow();
        __delay_ms(100);
    }

    printf("Firmware Version 1.2.2\r\n");

    // Sensor init
    __delay_ms(500);
    I2C1_Write1ByteRegister(I2C_Slave_Address, I2C_TRIM_Reg, I2C_TRIM_Data);
    __delay_ms(20);
    RX_I2C_L1 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_LPF1_Reg);
    RX_I2C_L2 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_LPF2_Reg);
    __delay_ms(20);
    I2C1_Write1ByteRegister(I2C_Slave_Address, I2C_CTRL1_Reg, I2C_CTRL1_Data);
    __delay_ms(20);
    I2C1_Write1ByteRegister(I2C_Slave_Address, I2C_LPF1_Reg, I2C_LPF1_Data);
    I2C1_Write1ByteRegister(I2C_Slave_Address, I2C_CTRL1_Reg, I2C_CTRL1_Data2);

    uint32_t msCounter = 0;
    uint32_t lastLowBattAlert = 0;

    while (1)
    {
        // --- pseudo timebase: tied to your blocking delays ---
        // NOTE: This tracks *approximate* elapsed time.
        // Your loop includes 500ms + 50ms delays below.
        msCounter += 550;

        // ================= UART KEYBINDS =================
        if (EUSART1_is_rx_ready())
        {
            char key = EUSART1_Read();
            switch (key)
            {
                case 'h':
                    printf("\r\nRACK PROTECTOR FW help menu\r\n"
                           "v - Print battery voltage\r\n"
                           "b - Buzzer ON (manual)\r\n"
                           "i - LED Bank 1 ON (manual)\r\n"
                           "o - LED Bank 2 ON (manual)\r\n"
                           "p - All OFF (return to auto)\r\n");
                    break;

                case 'v':
                {
                    float batt = Read_BatteryVoltage();
                    printf("Batt: %.2f V\r\n", batt);
                    break;
                }

                case 'b':
                    buzzerOverride = 1;
                    printf("Buzzer manual ON\r\n");
                    break;

                case 'i':
                    led1_manual = 1;
                    printf("LED BANK 1 manual ON\r\n");
                    break;

                case 'o':
                    led2_manual = 1;
                    printf("LED BANK 2 manual ON\r\n");
                    break;

                case 'p':
                    buzzerOverride = 0;
                    led1_manual = 0;
                    led2_manual = 0;
                    printf("All outputs back to AUTO (OFF until presence)\r\n");
                    // also clear FSM debounce so it doesn't jump states
                    nearCount = 0;
                    pState = PRESENCE_IDLE;
                    break;

                default:
                    break;
            }
        }

        // ================= LOW BATTERY ALERT =================
        // Keep behavior: periodic alert when batt < 7.25V
        float batt = Read_BatteryVoltage();
        if (batt < 7.25f)
        {
            if ((msCounter - lastLowBattAlert) >= 90000) // ~90s using your pseudo timebase
            {
                // FIXED: This now pulses buzzer instead of latching ON
                for (uint8_t i = 0; i < 15; i++)
                {
                    if (!buzzerOverride) LED_Enble_SetHigh();
                    __delay_ms(150);
                    if (!buzzerOverride) LED_Enble_SetLow();
                    __delay_ms(150);
                }
                lastLowBattAlert = msCounter;
            }
        }

        // ================= SENSOR READ =================
        RX_I2C_Ready = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_DataReady_Reg);

        __delay_ms(500);
        RX_I2C_P1 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_Presence_Reg);
        RX_I2C_P2 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_Presence_Reg2);

        __delay_ms(50);
        RX_I2C_M1 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_Motion_Reg);
        RX_I2C_M2 = I2C1_Read1ByteRegister(I2C_Slave_Address, I2C_Motion_Reg2);

        Presence = (uint16_t)((uint16_t)RX_I2C_P1 | ((uint16_t)RX_I2C_P2 << 8));
        Motion   = (uint16_t)((uint16_t)RX_I2C_M1 | ((uint16_t)RX_I2C_M2 << 8));

        // Presence range flags (kept from your code)
        uint8_t inNearRange   = (Presence >= 0x00C0 && Presence <= 0x0500);
        uint8_t inStrongRange = (Presence >  0x0500 && Presence <  0x7D00);

        // ================= FSM decides desired outputs =================
        uint8_t led1_on = 0;
        uint8_t led2_on = 0;
        uint8_t buz_on  = 0;

        switch (pState)
        {
            case PRESENCE_IDLE:
                // outputs remain off in auto while idle
                led1_on = 0;
                led2_on = 0;
                buz_on  = 0;

                if (inStrongRange)
                {
                    nearCount = 0;
                    pState = PRESENCE_STRONG;
                }
                else if (inNearRange)
                {
                    // 0.5s qualification: require consecutive near readings
                    if (++nearCount >= PRESENCE_NEAR_QUAL_COUNTS)
                    {
                        nearCount = 0;
                        pState = PRESENCE_NEAR;
                    }
                }
                else
                {
                    nearCount = 0;
                }
                break;

            case PRESENCE_NEAR:
                // In NEAR: LEDs ON, buzzer OFF
                led1_on = 1;
                led2_on = 1;
                buz_on  = 1;

                if (inStrongRange)
                {
                    pState = PRESENCE_STRONG;
                }
                else if (!inNearRange)
                {
                    pState = PRESENCE_IDLE;
                }
                break;

            case PRESENCE_STRONG:
                // In STRONG: blink LEDs, buzzer ON
                // Use toggle behavior but expressed as "desired" via reading current outputs
                // (MCC toggle functions are fine too, but this keeps ApplyOutputs single-source.)
                // If you prefer, we can swap back to LEDx_Enble_Toggle().
                LED1_Enble_Toggle();
                LED2_Enble_Toggle();
                led1_on = led1_manual ? 1 : 2; // 2 means "already toggled" (handled below)
                led2_on = led2_manual ? 1 : 2;
                buz_on  = 1;

                if (!inStrongRange)
                {
                    presenceHoldStart = msCounter;
                    pState = PRESENCE_HOLD;
                }
                break;

            case PRESENCE_HOLD:
                // In HOLD: continue STRONG pattern for ~7s
                LED1_Enble_Toggle();
                LED2_Enble_Toggle();
                led1_on = led1_manual ? 1 : 2;
                led2_on = led2_manual ? 1 : 2;
                buz_on  = 1;

                if ((msCounter - presenceHoldStart) >= 7000)
                {
                    pState = PRESENCE_IDLE;
                }
                break;
        }

        // Special handling: if we already toggled hardware pins in STRONG/HOLD,
        // don't overwrite them with ApplyOutputs ON/OFF for auto mode.
        // We only apply buzzer and non-toggled states here.
        if (pState == PRESENCE_STRONG || pState == PRESENCE_HOLD)
        {
            // Only apply buzzer + manual LED overrides
            if (buzzerOverride) LED_Enble_SetHigh();
            else if (buz_on)    LED_Enble_SetHigh();
            else                LED_Enble_SetLow();

            if (led1_manual) LED1_Enble_SetHigh();
            if (led2_manual) LED2_Enble_SetHigh();
        }
        else
        {
            // Normal states: Apply all outputs deterministically
            ApplyOutputs(led1_on, led2_on, buz_on);
        }
    }
}
