char elevador_aberto(char elevador, char estado, bool *requisicoes, int andar);
void elevador_fechado(char elevador, char estado);
char botao_externo_pressionado(char elevador, char estado, char caractere, uint8_t andar, uint8_t andar_requisitado);
char botao_interno_pressionado(char elevador, char estado, uint8_t botao, uint8_t andar);
char andar_atingido(char elevador, char estado, bool *requisicoes, uint8_t andar);