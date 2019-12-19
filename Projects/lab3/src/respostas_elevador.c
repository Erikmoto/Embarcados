#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "comandos_elevador.h"

char elevador_aberto(char elevador, char estado, bool *requisicoes, uint8_t andar) {
  int i;
  
  requisicoes[andar] = false;
  desligar_luz(elevador, andar + 'a');
  
  if(estado == 's') {
    estado = 'p';
    
    for(i = andar + 1; i < 16; i++) {
      if(requisicoes[i] == true) {
        estado = 's';
        fechar_elevador(elevador);
        return estado;
      }
    }
    
    for(i = andar - 1; i > -1; i--) {
      if(requisicoes[i] == true) {
        estado = 'd';
        fechar_elevador(elevador);
        return estado;
      }
    }
  }
  
  else if(estado == 'd') {
    estado = 'p';
    
    for(i = andar - 1; i > -1; i--) {
      if(requisicoes[i] == true) {
        estado = 'd';
        fechar_elevador(elevador);
        return estado;
      }
    }
    
    for(i = andar + 1; i < 16; i++) {
      if(requisicoes[i] == true) {
        estado = 's';
        fechar_elevador(elevador);
        return estado;
      }
    }
  }
  
  return estado;
}

void elevador_fechado(char elevador, char estado) {
  if(estado == 's') {
    subir_elevador(elevador);
  }
  
  else if(estado == 'd') {
    descer_elevador(elevador);
  }
  
  else if(estado == 'p') {
    parar_elevador(elevador);
  }
}

char botao_externo_pressionado(char elevador, char estado, char caractere, uint8_t andar, uint8_t andar_requisitado) {
  ligar_luz(elevador, andar_requisitado + 'a');
  
  if(estado == 'p') {
    if(andar_requisitado == andar) {
      desligar_luz(elevador, andar_requisitado + 'a');
    }
    
    else {
      if(andar_requisitado < andar) {
        estado = 'd';
      }
      
      else if(andar_requisitado > andar) {
        estado = 's';
      }
    }
  }
  
  return estado;
}

char botao_interno_pressionado(char elevador, char estado, uint8_t botao, uint8_t andar) {
  if(botao > andar) {
    if(estado != 'd') {
      estado = 's';
    }
  }
  
  else if(botao < andar) {
    if(estado != 's') {
      estado = 'd';
    }
  }
  
  return estado;
}

char andar_atingido(char elevador, char estado, bool *requisicoes, uint8_t andar) {
  int i;
  
  if(requisicoes[andar] == true) {
    parar_elevador(elevador);
    
    if(estado == 's') {
      for(i = 0; i < 1; i++) {
        subir_elevador(elevador);
        parar_elevador(elevador);
        abrir_elevador(elevador);
      }
      
      estado = 'p';
      
      for(i = andar; i < 16; i++) {
        if(requisicoes[i] == true) {
          estado = 's';
          break;
        }
      }
    }
    
    else if(estado == 'd') {
      for(i = 0; i < 1; i++) {
        descer_elevador(elevador);
        parar_elevador(elevador);
        abrir_elevador(elevador);
      }
      
      estado = 'p';
      
      for(i = andar; i > -1; i--) {
        if(requisicoes[i] == true) {
          estado = 'd';
          break;
        }
      }
    }
  }
  
  return estado;
}