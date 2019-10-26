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

#define VALOR_MIN 10
#define VALOR_MAX 65500

bool subida = 0;                 // Armazena a leitura do pino de entrada
uint16_t count = 0;                 //Conta as vezes que a interrupção ocorreu
uint32_t ultimoValorAlto = 0;
uint32_t ultimoValorBaixo = 0;
bool amostrasFeitas = false;

bool amostragemPronta(void) {
  return amostrasFeitas;
}

void resetaAmostragemPronta(void) {
  amostrasFeitas = false;
  ativaAmostragem();
}

void TIMER0A_Handler() {
  uint16_t indice = count / 2;
  
  if(count > 0) {
    if(indice < TAM_VETOR) {
      uint32_t periodo;
      uint32_t valorTimer;
      
      valorTimer = TimerValueGet(TIMER0_BASE, TIMER_A); //Leitura do timer
      
      if(valorTimer < ultimoValorAlto) {
        //periodo = valorTimer + (0xFFFFFFFF - ultimoValorAlto);
        ultimoValorBaixo = valorTimer;
      }
      
      else {
        periodo = valorTimer - ultimoValorAlto;
        ultimoValorBaixo = valorTimer;
        t_baixos[indice] = periodo;
      }
      
      count++;
    }
    
    else {
      count = 0;
      amostrasFeitas = true;
    }
  }
  
  TimerLoadSet(TIMER0_BASE, TIMER_A, 0);
  TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
}

void TIMER1A_Handler(void) {
  uint16_t indice = count / 2;
  
  if(indice < TAM_VETOR) {
    uint32_t valorTimer = TimerValueGet(TIMER1_BASE, TIMER_A); //Leitura do timer
    uint32_t periodo = valorTimer - ultimoValorBaixo;
    
    if(valorTimer < ultimoValorAlto) {
      //periodo = valorTimer + (0xFFFFFFFF - ultimoValorBaixo);
      ultimoValorAlto = valorTimer;
    }
    
    else {
      ultimoValorAlto = valorTimer;
      periodo = valorTimer - ultimoValorBaixo;
      t_altos[indice] = periodo;
    }

    count++;
  }
  
  TimerLoadSet(TIMER1_BASE, TIMER_A, 0);
  TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);
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
  
  TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
  TimerControlEvent(TIMER1_BASE, TIMER_A, TIMER_EVENT_NEG_EDGE);
  
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