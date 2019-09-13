// -------------------------------------------------------------------------------

// Autores:

// Caio Alessandro Resnauer
// Erik Ryuichi Yamamoto
// Victor Jordão

// -------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

//#include "TM4C1294NCPDT.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "gpio.h"

// -------------------------------------------------------------------------------

void GPIO_Init(void) {
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK));
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ));
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM));
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOH));
  
  
  //GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, BIT7 | BIT6 | BIT5 | BIT4); // LEDs e BCDs [BIT7 | BIT6 | BIT5 | BIT4]
  //GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, BIT5 | BIT4); // Transitores BCDs [BIT5 | BIT4]
  //GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, BIT3 | BIT2 | BIT1 | BIT0); // Motor DC e Motor de Passo Bipolar [BIT3 | BIT2 | BIT1 | BIT0]
  //GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, BIT4 | BIT3 | BIT2 | BIT0); // PWMs dos Motores DC e de Passo Bipolar [BIT3 | BIT2] e LEDs Tiva [BIT4 | BIT0]
  //GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, BIT3 | BIT2 | BIT1 | BIT0); // Motor de Passo Unipolar [BIT3 | BIT2 | BIT1 | BIT0]
  //GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0); // Display LCD
  //GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, BIT3 | BIT2 | BIT1 | BIT0); // Teclado Matricial [BIT3 | BIT2 | BIT1 | BIT0]
  //GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, BIT6 | BIT2 | BIT1 | BIT0); // Transistor LEDs [BIT6] e Controle LCD [BIT2 | BIT1 | BIT0]
  //GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, BIT1 | BIT0); //BIT0 | BIT1
  //GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, BIT3 | BIT2 | BIT1 | BIT0); // LEDs e BCDs [BIT3 | BIT2 | BIT1 | BIT0]
  
  GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, BIT7 | BIT6 | BIT5 | BIT4); // Teclado Matricial [BIT7 | BIT6 | BIT5 | BIT4]
  GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, BIT5 | BIT4); // Conversor A/D - {Sensor BPM [BIT5] / Potenciômetro [BIT4]}
  GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, BIT1 | BIT0);
  
  GPIOPadConfigSet(GPIO_PORTA_BASE, BIT7 | BIT6 | BIT5 | BIT4, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(GPIO_PORTB_BASE, BIT5 | BIT4, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(GPIO_PORTE_BASE, BIT3 | BIT2 | BIT1 | BIT0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(GPIO_PORTF_BASE, BIT4 | BIT3 | BIT2 | BIT0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(GPIO_PORTH_BASE, BIT3 | BIT2 | BIT1 | BIT0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(GPIO_PORTK_BASE, BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(GPIO_PORTM_BASE, BIT6 | BIT2 | BIT1 | BIT0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(GPIO_PORTN_BASE, BIT1 | BIT0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(GPIO_PORTQ_BASE, BIT3 | BIT2 | BIT1 | BIT0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  
  GPIOPadConfigSet(GPIO_PORTC_BASE, BIT7 | BIT6 | BIT5 | BIT4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOPadConfigSet(GPIO_PORTJ_BASE, BIT1 | BIT0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOPadConfigSet(GPIO_PORTL_BASE, BIT3 | BIT2 | BIT1 | BIT0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

// -------------------------------------------------------------------------------

uint32_t PortC_Input(void) {
  return GPIOPinRead(GPIO_PORTC_BASE, BIT7 | BIT6 | BIT5 | BIT4);
}

// -------------------------------------------------------------------------------

uint32_t PortJ_Input(void) {
  return GPIOPinRead(GPIO_PORTJ_BASE, BIT1 | BIT0);
}

// -------------------------------------------------------------------------------

void PortA_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTA_BASE, BIT7 | BIT6 | BIT5 | BIT4, valor);
}

// -------------------------------------------------------------------------------

void PortB_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTB_BASE, BIT5 | BIT4, valor);
}

// -------------------------------------------------------------------------------

void PortE_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTE_BASE, BIT3 | BIT2 | BIT1 | BIT0, valor);
}

// -------------------------------------------------------------------------------

void PortF_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTF_BASE, BIT3 | BIT2, valor);
}

// -------------------------------------------------------------------------------

void PortH_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTH_BASE, BIT3 | BIT2 | BIT1 | BIT0, valor);
}

// -------------------------------------------------------------------------------

void PortK_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTK_BASE, BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0, valor);
}

// -------------------------------------------------------------------------------

void PortL_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTL_BASE, BIT3 | BIT2 | BIT1 | BIT0, valor);
}

// -------------------------------------------------------------------------------

void PortM_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTM_BASE, BIT6 | BIT2 | BIT1 | BIT0, valor);
}

// -------------------------------------------------------------------------------

void PortN_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTN_BASE, BIT1 | BIT0, valor);
}

// -------------------------------------------------------------------------------

void PortQ_Output(uint32_t valor) {
  GPIOPinWrite(GPIO_PORTQ_BASE, BIT3 | BIT2 | BIT1 | BIT0, valor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////