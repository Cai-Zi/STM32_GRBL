/*
 * TODO: move all STM32 specific LL and HAL calls here
 *
  stm32utilities.c

  Copyright (c) 2018-2019 Thomas Truong

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stm32utilities.h"
#include <stdio.h>
#include <string.h>

//-- Steps and Directions Pin Arrays --------------
#if (defined(STM32F1_3) || defined(STM32F4_3))
const PIN_MASK step_pin_mask[N_AXIS] =
        {STEP_X_Pin, STEP_Y_Pin, STEP_Z_Pin};
const PIN_MASK direction_pin_mask[N_AXIS] =
        {DIR_X_Pin, DIR_Y_Pin, DIR_Z_Pin};
#endif
#if (defined(STM32F1_4) || defined(STM32F4_4))
const PIN_MASK step_pin_mask[N_AXIS] =
{	STEP_X_Pin, STEP_Y_Pin, STEP_Z_Pin, STEP_A_Pin};
const PIN_MASK direction_pin_mask[N_AXIS] =
{	DIR_X_Pin, DIR_Y_Pin, DIR_Z_Pin, DIR_A_Pin};
#endif
#if (defined(STM32F1_5) || defined(STM32F4_5))
const PIN_MASK step_pin_mask[N_AXIS] =
{	STEP_X_Pin, STEP_Y_Pin, STEP_Z_Pin, STEP_A_Pin, STEP_B_Pin};
const PIN_MASK direction_pin_mask[N_AXIS] =
{	DIR_X_Pin, DIR_Y_Pin, DIR_Z_Pin, DIR_A_Pin, DIR_B_Pin};
#endif
#if (defined(STM32F1_6) || defined(STM32F4_6))
const PIN_MASK step_pin_mask[N_AXIS] =
{ STEP_X_Pin, STEP_Y_Pin, STEP_Z_Pin, STEP_A_Pin, STEP_B_Pin, STEP_C_Pin };
const PIN_MASK direction_pin_mask[N_AXIS] =
{ DIR_X_Pin, DIR_Y_Pin, DIR_Z_Pin, DIR_A_Pin, DIR_B_Pin, DIR_C_Pin };
#endif

//-- Limits  Pin Arrays
#ifdef STM32F1_3
const PIN_MASK limit_pin_mask[N_AXIS] =
        {LIM_X_Pin, LIM_Y_Pin, LIM_Z_Pin};
#endif
#ifdef STM32F1_4
const PIN_MASK limit_pin_mask[N_AXIS] =
{	LIM_X_Pin, LIM_Y_Pin, LIM_Z_Pin, LIM_A_Pin};
#endif
#ifdef STM32F1_5
const PIN_MASK limit_pin_mask[N_AXIS] =
{	LIM_X_Pin, LIM_Y_Pin, LIM_Z_Pin, LIM_A_Pin, LIM_B_Pin};
#endif
#ifdef STM32F1_6
const PIN_MASK limit_pin_mask[N_AXIS] =
{	LIM_X_Pin, LIM_Y_Pin, LIM_Z_Pin, LIM_A_Pin, LIM_B_Pin, LIM_C_Pin};
#endif
/*
 * For the F46, Limits are on SPI expandedIO, not MCU pins, we'll organize the pins sequentially
 */
#ifdef STM32F4_3
const PIN_MASK limit_pin_mask[N_AXIS] =
{	0x01, 0x02, 0x04};
#endif
#ifdef STM32F4_4
const PIN_MASK limit_pin_mask[N_AXIS] =
{	0x01, 0x02, 0x04, 0x08};
#endif
#ifdef STM32F4_5
const PIN_MASK limit_pin_mask[N_AXIS] =
{	0x01, 0x02, 0x04, 0x08, 0x10};
#endif
#ifdef STM32F4_6
const PIN_MASK limit_pin_mask[N_AXIS] =
{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20 };
#endif

#ifdef STM32F1
char *pHello = "Hi G32F1\r\n";
#endif
#ifdef  STM32F4
char *pHello = "Hi G32F4\r\n";
#endif

uint32_t uTICKS_PER_MICROSECOND = 0;
float fTICKS_PER_MINUTE = 0.0f;

//-- Debug
char pDbgStr[80];
char pDbgVal8a[9];
char pDbgVal8b[9];
char pDbgVal16[17];

//-------------------------------------------------------------------
void timing_init()
{
    uTICKS_PER_MICROSECOND = SystemCoreClock / 1000000;
    fTICKS_PER_MINUTE = SystemCoreClock * 60.0f;
}

//-- quick debug functions ------------------------------------------
char *ByteToBin(uint8_t ValIn, char *pStrOut)
{
    for (int i = 0; i < 8; i++)
    {
        if (ValIn & 0x1)
            pStrOut[7 - i] = '1';
        else
            pStrOut[7 - i] = '0';
        ValIn = ValIn >> 1;
    }
    return (pStrOut);
}

char *Int16ToBin(uint16_t ValIn, char *pStrOut)
{
    for (int i = 0; i < 16; i++)
    {
        if (ValIn & 0x1)
            pStrOut[15 - i] = '1';
        else
            pStrOut[15 - i] = '0';
        ValIn = ValIn >> 1;
    }
    return (pStrOut);
}

//------------------------------------------------------------------------
void uart_init()
{
    LL_USART_EnableIT_RXNE(USART1);

    /*
     * Debug
     */
    memset(pDbgStr, 0, 80);
    //sprintf(pDbgStr,"\r\nClk:%ld\r\nTicks_us:%ld\r\nTicks_min:%.2f",SystemCoreClock,uTICKS_PER_MICROSECOND,fTICKS_PER_MINUTE);    //-- Clock check for Overclocking case
//    sprintf(pDbgStr, "\r\nSysClk:%ld\r\n", SystemCoreClock); //-- Clock check for Overclocking case
    uart_sendstr(pDbgStr);

}

//------------------------------------------------------------------------
void uart_sendstr(const char *pStr)
{
    while (*pStr)
        uart_sendch(*pStr++);
}

void uart_sendch(uint8_t uC)
{
    LL_USART_TransmitData8(USART1, uC);
    while (!(LL_USART_IsActiveFlag_TXE(USART1))); // sit till empty
}


//------------------------------------------------------------------------
//------------------------------------------------------------------------
void Spindle_Disable()
{
#ifdef VARIABLE_SPINDLE_ENABLE_PIN
    if (settings.spindle_enable_pin_mode == 1)
        ResetSpindleEnablebit();
    else
        SetSpindleEnablebit();
#endif

    LL_TIM_DisableAllOutputs(SPINDLE_TIMER);
}

void Spindle_Enable()
{
#ifdef VARIABLE_SPINDLE_ENABLE_PIN
    if (settings.spindle_enable_pin_mode == 1)
        SetSpindleEnablebit();
    else
        ResetSpindleEnablebit();
#endif

    LL_TIM_EnableAllOutputs(SPINDLE_TIMER);
}


//------------------------------------------------------------------------
//------------------------------------------------------------------------
//-- Pin based calls, need to use HAL since LL pins and HAL pins are incompatible for F1
void GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}
//------------------------------------------------------------------------

#ifdef STM32F46   //-- board specific hardware, SPI driven limits
uint8_t SPIDataC0W[3]; // 		= { 0x40, 0x00, 0x00 };		//-- Chip0, Limits P & N
uint8_t SPIDataC0R[3]; //		= { 0x41, 0x00, 0x00 };
uint8_t SPIDataC1W[3]; // 		= { 0x66, 0x00, 0x00 };		//-- Chip1, Home and ExpansionIO
uint8_t SPIDataC1R[3]; //		= { 0x67, 0x00, 0x00 };
SPI_HandleTypeDef *pHSPI = NULL;
#define SPI_TIMEOUT 2

/*	SPI Addressing : page 16 of MCP23S17 datasheet
 for reading: 2 bytes
 byte0 : Device Opcode
 bit:	| 7 | 6 | 5 | 4 | 3 | 1 | 1 | 0 |
 0   1   0   0   A2  A1  A0  1 --- Read:1 (Write:0)
 :-------- A0 part of device address, upto 3bit, 8 devices
 :------------ A1
 :---------------- A2

 byte1 : Register

 for write: 3 bytes
 byte0 : Device, with bit7=0 for writing
 byte1 : Register
 byte2 : Value to send

 note on Register IOCON (page 21)
 bit:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
                                                                                           :---- bit0 (NOT USED)

                                                                 :-------- bit1 INTPOL: This bit sets the polarity of the INT output pin
                                                                 1 = Active-high
                                                                 0 = Active-low
                                                         :------------ bit2 ODR: Configures the INT pin as an open-drain output
                                                         1 = Open-drain output (overrides the INTPOL bit.)
                                                         0 = Active driver output (INTPOL bit sets the polarity.)
                                                 :---------------- bit3 HAEN: Hardware Address Enable bit (MCP23S17 only) (Note 1)
                                                 1 = Enables the MCP23S17 address pins.
                                                 0 = Disables the MCP23S17 address pins.
                                         :-------------------- bit4 DISSLW: Slew Rate control bit for SDA output
                                         1 = Slew rate disabled
                                         0 = Slew rate enabled
                                 :------------------------ bit5 SEQOP: Sequential Operation mode bit
                                 1 = Sequential operation disabled, address pointer does not increment.
                                 0 = Sequential operation enabled, address pointer increments.
                         :---------------------------- bit6 MIRROR: INT Pins Mirror bit
                         1 = The INT pins are internally connected
                         0 = The INT pins are not connected. INTA is associated with PORTA and INTB is associated with
                 :-------------------------------- bit7 BANK: Controls how the registers are addressed
                 1 = The registers associated with each port are separated into different banks.
                 0 = The registers are in the same bank (addresses are sequential).

 */

void DebugRead(enum IOExpChip IOC, uint8_t Reg)
{
    uint8_t Val = SPIRead(IOC, Reg);
    sprintf(pDbgStr, "\r\nChip:%d|Reg:%d|Val:%d\r\n", IOC, Reg, Val);
    uart_sendstr(pDbgStr);

}
//----------------------------------------------------------------------
void SPIInit(SPI_HandleTypeDef *HSPI)
{
    pHSPI = HSPI;

    SPIDataC0W[0] = 0x40; //-- 0b01000000
    SPIDataC0W[1] = 0x00;
    SPIDataC0W[2] = 0x00;
    SPIDataC0R[0] = 0x41; //-- 0b01000001
    SPIDataC0R[1] = 0x00;
    SPIDataC0R[2] = 0x00;

    SPIDataC1W[0] = 0x42; //-- 0b01000010
    SPIDataC1W[1] = 0x00;
    SPIDataC1W[2] = 0x00;
    SPIDataC1R[0] = 0x43; //-- 0b01000011
    SPIDataC1R[1] = 0x00;
    SPIDataC1R[2] = 0x00;

    memset(pDbgVal8a, 0, 9);
    memset(pDbgVal8b, 0, 9);
    memset(pDbgVal16, 0, 17);
    HAL_GPIO_WritePin(SPI_SEL0_GPIO_Port, SPI_SEL0_Pin, GPIO_PIN_SET);//-- CS High, disable initially

    //-- Chip0 ----------------------------------
    SPIWrite(IOC0, SPI_IOCONA, 0b01111000); //-- BANK0,MIRROR1,SEQOP1,DISSLW1,HAEN1,ODR0,INTPOL0,0
    //DebugRead(IOC0,SPI_IOCONA);
    SPIWrite(IOC0, SPI_IOCONB, 0b01111000); //-- BANK0,MIRROR1,SEQOP1,DISSLW1,HAEN1,ODR0,INTPOL0,0

    //-- C0:A
    SPIWrite(IOC0, SPI_IODIRA, 0xF0);   //-- upper nibble inputs
    SPIWrite(IOC0, SPI_GPPUA, 0xF0);    //-- pull up upper nibble;
    SPIWrite(IOC0, SPI_IOPOLA, 0x00);   //-- normal polarity
    SPIWrite(IOC0, SPI_INTCONA, 0xFF); // Interrupt control (0 = interrupt on change from previous, 1 = interrupt on change from DEFVAL)
    SPIWrite(IOC0, SPI_DEFVALA, 0xFF); // Default comparison for interrupt on change (interrupts on opposite)

//  SPIWrite(IOC0,SPI_GPINTENA,0xF0); //-- enable interrupts for 4 upper bits on A (B,C Pos&Neg pins)
//  SPIRead(IOC0,SPI_INTCAPA);  //-- read to clear capture port

    //-- C0:B
    SPIWrite(IOC0, SPI_IODIRB, 0xFF);   //-- all bits inputs
    SPIWrite(IOC0, SPI_GPPUB, 0xFF);    //-- pull up all bits;
    SPIWrite(IOC0, SPI_IOPOLB, 0x00);   //-- normal polarity
    SPIWrite(IOC0, SPI_INTCONB, 0xFF); // Interrupt control (0 = interrupt on change from previous, 1 = interrupt on change from DEFVAL)
    SPIWrite(IOC0, SPI_DEFVALB, 0xFF); // Default comparison for interrupt on change (interrupts on opposite)

//  SPIWrite(IOC0,SPI_GPINTENB,0xFF); //-- enable interrupts for all bits on B (X,Y,Z,A Pos&Neg pins)
//  SPIRead(IOC0,SPI_INTCAPB);  //-- read to clear capture port

    DisableLimitsINT();	//-- disable initially

    //-- Chip1 ----------------------------------
    SPIWrite(IOC1, SPI_IOCONA, 0b00111000); //-- BANK0,MIRROR0,SEQOP1,DISSLW1,HAEN1,ODR0,INTPOL0,0
    SPIWrite(IOC1, SPI_IOCONB, 0b00111000); //-- BANK0,MIRROR0,SEQOP1,DISSLW1,HAEN1,ODR0,INTPOL0,0
    //-- C1:A
    SPIWrite(IOC1, SPI_IODIRA, 0xFF); //-- all inputs for A
    SPIWrite(IOC1, SPI_GPPUA, 0xFF);    //-- pull up all bits;
    SPIWrite(IOC1, SPI_IOPOLA, 0x00);   //-- normal polarity
    //-- C1:B
    SPIWrite(IOC1, SPI_IODIRB, 0x3F);   //-- first 6 bits as inputs for B
    SPIWrite(IOC1, SPI_GPPUB, 0x3F);    //-- pull up all input bits;
    SPIWrite(IOC1, SPI_IOPOLB, 0x00);   //-- normal polarity
    SPIWrite(IOC1, SPI_INTCONB, 0xFF); // Interrupt control (0 = interrupt on change from previous, 1 = interrupt on change from DEFVAL)
    SPIWrite(IOC1, SPI_DEFVALB, 0xFF); // Default comparison for interrupt on change (interrupts on opposite)
    SPIWrite(IOC1, SPI_GPINTENB, 0x3F); //-- enable interrupts for lower 6 bits on B (6 home pins)

    SPIRead(IOC1, SPI_INTCAPB);  //-- read to clear capture port

}
//----------------------------------------------------------------------
void EnableLimitsINT()		//-- enable limits interrupt
{
    SPIWrite(IOC0, SPI_GPINTENA, 0xF0); //-- enable interrupts for 4 upper bits on A (B,C Pos&Neg pins)
    SPIWrite(IOC0, SPI_GPINTENB, 0xFF); //-- enable interrupts for all bits on B (X,Y,Z,A Pos&Neg pins)

    SPIRead(IOC0, SPI_INTCAPA);  //-- read to clear capture port
    SPIRead(IOC0, SPI_INTCAPB);  //-- read to clear capture port
}
//----------------------------------------------------------------------
void DisableLimitsINT()	//-- disable
{
    SPIWrite(IOC0, SPI_GPINTENA, 0x00);
    SPIWrite(IOC0, SPI_GPINTENB, 0x00);
}

//----------------------------------------------------------------------
uint8_t SPIRead(enum IOExpChip IOC, uint8_t Reg)
{
    HAL_StatusTypeDef eResult;
    uint8_t Result = 0;
    HAL_GPIO_WritePin(SPI_SEL0_GPIO_Port, SPI_SEL0_Pin, GPIO_PIN_RESET);
    if (IOC == IOC0)
    {
        SPIDataC0R[1] = Reg;
        HAL_SPI_Transmit(pHSPI, SPIDataC0R, 2, SPI_TIMEOUT);
        eResult = HAL_SPI_Receive(pHSPI, &Result, 1, SPI_TIMEOUT);
    }
    else //-- IOC1
    {
        SPIDataC1R[1] = Reg;
        HAL_SPI_Transmit(pHSPI, SPIDataC1R, 2, SPI_TIMEOUT);
        eResult = HAL_SPI_Receive(pHSPI, &Result, 1, SPI_TIMEOUT);
    }

    HAL_GPIO_WritePin(SPI_SEL0_GPIO_Port, SPI_SEL0_Pin, GPIO_PIN_SET);

    if (eResult == HAL_OK)
        return (Result);
    else
        return 0;
}
//----------------------------------------------------------------------
HAL_StatusTypeDef SPIWrite(enum IOExpChip IOC, uint8_t Reg, uint8_t Val)
{
    HAL_StatusTypeDef eResult;
    HAL_GPIO_WritePin(SPI_SEL0_GPIO_Port, SPI_SEL0_Pin, GPIO_PIN_RESET);

    if (IOC == IOC0)
    {
        SPIDataC0W[1] = Reg;
        SPIDataC0W[2] = Val;
        eResult = HAL_SPI_Transmit(pHSPI, SPIDataC0W, 3, SPI_TIMEOUT);
    }
    else //-- IOC1
    {
        SPIDataC1W[1] = Reg;
        SPIDataC1W[2] = Val;
        eResult = HAL_SPI_Transmit(pHSPI, SPIDataC1W, 3, SPI_TIMEOUT);
    }

    HAL_GPIO_WritePin(SPI_SEL0_GPIO_Port, SPI_SEL0_Pin, GPIO_PIN_SET);
    return (eResult);
}
//----------------------------------------------------------------------
/*
 * From limit.c:
 // Returns limit state as a bit-wise uint8 variable. Each bit indicates an axis limit, where
 // triggered is 1 and not triggered is 0. Invert mask is applied. Axes are defined by their
 // number in bit position, i.e. Z_AXIS is (1<<2) or bit 2, and Y_AXIS is (1<<1) or bit 1.
 *
 */
uint16_t GetLimitsState()
{
    uint16_t uActiveLimitsMask = 0;
    uint8_t PortA, PortB; //, debugValA,debugValB;
//	DisableLimitsINT();	//-- disable limits interrupt

    SPIRead(IOC0, SPI_INTCAPA);  //-- read to clear capture port
    SPIRead(IOC0, SPI_INTCAPB);  //-- read to clear capture port

    //-- read SPI Chip0 for all limit pins
    PortA = SPIRead(IOC0, SPI_GPIOA); //-- axes BC in high nibble
    PortB = SPIRead(IOC0, SPI_GPIOB); //-- axis XYZA

//	debugValA = SPIRead(IOC0,SPI_DEFVALA);
//	debugValB = SPIRead(IOC0,SPI_DEFVALB);

    //-- go through the axes
    if (((PortB & LIM_XP_Pin) == 0) || ((PortB & LIM_XN_Pin) == 0))
        uActiveLimitsMask += 0x01;
    if (((PortB & LIM_YP_Pin) == 0) || ((PortB & LIM_YN_Pin) == 0))
        uActiveLimitsMask += 0x02;
    if (((PortB & LIM_ZP_Pin) == 0) || ((PortB & LIM_ZN_Pin) == 0))
        uActiveLimitsMask += 0x04;
    if (((PortB & LIM_AP_Pin) == 0) || ((PortB & LIM_AN_Pin) == 0))
        uActiveLimitsMask += 0x08;

    if (((PortA & LIM_BP_Pin) == 0) || ((PortA & LIM_BN_Pin) == 0))
        uActiveLimitsMask += 0x10;
    if (((PortA & LIM_CP_Pin) == 0) || ((PortA & LIM_CN_Pin) == 0))
        uActiveLimitsMask += 0x20;

    /*
     sprintf(pDbgStr,"\r\nLimState:A:%d:%s|B:%d:%s|Mask:%d:%s\r\n",
     PortA,ByteToBin(PortA,pDbgVal8a),
     PortB,ByteToBin(PortB,pDbgVal8b),
     uActiveLimitsMask,Int16ToBin(uActiveLimitsMask,pDbgVal16) );
     uart_sendstr(pDbgStr);
     */

    //EnableLimitsINT();		//-- enable limits interrupt
    return (uActiveLimitsMask);
}

//----------------------------------------------------------------------
/*
 * Input byte from Chip1, for M66
 */
uint8_t ReadInputByte()
{
    uint8_t val = SPIRead(IOC1, SPI_GPIOA);

    //sprintf(pDbgStr,"\r\ninput:%s\r\n",ByteToBin(val,pDbgVal8a));
    //uart_sendstr(pDbgStr);

    return (val);
}

#endif //STM32F46

void spi_limits_init()
{
#ifdef STM32F46 //-- board specific hardware, SPI driven limits
    SPIInit(&hspi3);
#endif
}

