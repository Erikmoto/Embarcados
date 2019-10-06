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
  int x = 0;
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
  
  //clock_MHz = (SystemCoreClock / 1000000);
  //period_ns = 1000 / clock_MHz;
  
  TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT_UP);
  //TimerControlLevel(TIMER0_BASE, TIMER_A, true);
  //TimerPrescaleSet(TIMER0_BASE, TIMER_A, 10);
  //TimerLoadSet(TIMER0_BASE, TIMER_A, SERVO_PERIOD * period_ns);
  //TimerMatchSet(TIMER0_BASE, TIMER_A, pulseTimeWidth);
  TimerIntRegister(TIMER0_BASE, TIMER_BOTH, TIMER0A_Handler);
  TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerEnable(TIMER0_BASE, TIMER_BOTH);
} // TimerInit