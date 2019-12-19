#include <stdint.h>
#include <stdbool.h>
#include "comandos_elevador.h"
#include "driverlib/uart.h"
#include "inc/hw_memmap.h"

void envia_resposta(char* resposta, uint8_t tamanho) {
  int i;
  
  for(i = 0; i < tamanho; i++) {
    UARTCharPut(UART0_BASE, resposta[i]);
  }
}

void reiniciar_elevador(char elevador) {
  char resposta[3] = {elevador, 'r', 0x0D};
  envia_resposta(resposta, 3);
}

void abrir_elevador(char elevador) {
  char resposta[3] = {elevador, 'a', 0x0D};
  envia_resposta(resposta, 3);
}

void fechar_elevador(char elevador) {
  char resposta[3] = {elevador, 'f', 0x0D};
  envia_resposta(resposta, 3);
}

void subir_elevador(char elevador) {
  char resposta[3] = {elevador, 's', 0x0D};
  envia_resposta(resposta, 3);
}

void descer_elevador(char elevador) {
  char resposta[3] = {elevador, 'd', 0x0D};
  envia_resposta(resposta, 3);
}

void parar_elevador(char elevador) {
  char resposta[3] = {elevador, 'p', 0x0D};
  envia_resposta(resposta, 3);
}

void posicao_elevador(char elevador) {
  char resposta[3] = {elevador, 'x', 0x0D};
  envia_resposta(resposta, 3);
}

void ligar_luz(char elevador, char andar) {
  char resposta[4] = {elevador, 'L', andar, 0x0D};
  envia_resposta(resposta, 4);
}

void desligar_luz(char elevador, char andar) {
  char resposta[4] = {elevador, 'D', andar, 0x0D};
  envia_resposta(resposta, 4);
}