#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"

#include "system_TM4C1294.h"

#include "temporizador.h"

bool amostrasAltasFeitas = false;
bool amostrasBaixasFeitas = false;

uint16_t indiceAltos = 0;
uint16_t indiceBaixos = 0;

bool amostragemPronta(void) {
  return(amostrasAltasFeitas && amostrasBaixasFeitas);
}

void resetaAmostragem(void) {
  amostrasAltasFeitas = false;
  amostrasBaixasFeitas = false;
  
  indiceAltos = 0;
  indiceBaixos = 0;
  
  TimerLoadSet(TIMER0_BASE, TIMER_A, 0);
  TimerLoadSet(TIMER1_BASE, TIMER_A, 0);
  
  ativaAmostragem();
}

void TIMER1A_Handler() {
  if(indiceAltos < TAM_VETOR) {
    if(indiceAltos == indiceBaixos) {
      leiturasAltos[indiceAltos] = TimerValueGet(TIMER1_BASE, TIMER_A);
      indiceAltos++;
    }
  }
  
  else {
    amostrasAltasFeitas = true;
  }
  
  TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);
}

void TIMER0A_Handler(void) {
  if(indiceAltos > 0) {
    if(indiceBaixos < TAM_VETOR) {
      if(indiceBaixos == indiceAltos - 1) {
        leiturasBaixos[indiceBaixos] = TimerValueGet(TIMER0_BASE, TIMER_A);
        indiceBaixos++;
      }
    }
    
    else {
      amostrasBaixasFeitas = true;
    }
  }
  
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
  
  // Enable Timer 0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1));
  
  GPIOPinConfigure(GPIO_PD2_T1CCP0);
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_2);
  
  GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
  GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
  
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP));
  TimerConfigure(TIMER1_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP));
  
  TimerPrescaleSet(TIMER0_BASE, TIMER_A, 200);
  TimerPrescaleSet(TIMER1_BASE, TIMER_A, 200);
  
  TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_NEG_EDGE);
  TimerControlEvent(TIMER1_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
  
  TimerIntRegister(TIMER0_BASE, TIMER_A, TIMER0A_Handler);
  TimerIntRegister(TIMER1_BASE, TIMER_A, TIMER1A_Handler);
  
  TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);
  
  TimerEnable(TIMER0_BASE, TIMER_A);
  TimerEnable(TIMER1_BASE, TIMER_A);
} // TimerInit

void ativaAmostragem(void) {
  TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerIntEnable(TIMER1_BASE, TIMER_CAPA_EVENT);
}

void desativaAmostragem(void) {
  TimerIntDisable(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerIntDisable(TIMER1_BASE, TIMER_CAPA_EVENT);
}