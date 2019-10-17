#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"

#include "system_TM4C1294.h" 

void TIMER0A_Handler() {
  TimerLoadSet(TIMER0_BASE, TIMER_A, 0);
  TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
}

void TimerInit(void){
  // Enable the GPIO Peripheral used by the Timer 0.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

  // Enable Timer 0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
  
  GPIOPinConfigure(GPIO_PD0_T0CCP0);
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
  //GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0);
  //GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP));
  
  //TimerControlLevel(TIMER0_BASE, TIMER_A, true);
  //TimerPrescaleSet(TIMER0_BASE, TIMER_A, 10);
  TimerLoadSet(TIMER0_BASE, TIMER_A, 0);
  //TimerMatchSet(TIMER0_BASE, TIMER_A, pulseTimeWidth);
  TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
  
  TimerIntRegister(TIMER0_BASE, TIMER_A, TIMER0A_Handler);
  TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerEnable(TIMER0_BASE, TIMER_A);
} // TimerInit