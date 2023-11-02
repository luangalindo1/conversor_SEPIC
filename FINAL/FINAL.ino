/* Configuração do controlador PWM
* Hélder Matheus Rufino Guimarães
* Luan Fábio Marinho Galindo
* Matheus Petrúcio de Almeida Gomes
*/

#define MAX_AMOSTRAS 10 // quantidade de amostras da tensão
                        // de saída que o controlador irá armazenar
                        
// Resolução do conversor AD 10 bits
#define REFERENCIA 466  // Tensão de referência na saída do
                        // divisor de tensão
                        //  escala: 1023 - 5V -> Vref ~ 2,28V
                        
#define DUTY_MINIMO 266 // 50% (devido a lógica de saída invertida no MOSFET)

#define DUTY_MAXIMO 426 // 20%

int duty = DUTY_MINIMO;
int media = 0;
int cnt = 0;
float amostra_atual = 0, amostras;

void setupPWM30khz(); // função para "converter" a frequência
void PWM30khz(char pino, int valor_duty); // Função para acionar o PWM 


void setup() {
  setupPWM30khz();
  pinMode(A0, INPUT);
}

void loop() {

  amostra_atual = analogRead(A0);

  amostras = amostras + amostra_atual;
  cnt++;

  if (cnt >= MAX_AMOSTRAS)
  {
    // Calculo da média dos valores de entrada
    cnt = 0;
    media = amostras / MAX_AMOSTRAS;
    amostras = 0;
    
    // Controle do Duty Cycle - Lógica invertida
    if (media < REFERENCIA){
      duty = duty - 1;}

    else if (media > REFERENCIA){
      duty = duty + 1;}
    
    // Limitação do Duty Cycle
    if (duty >= DUTY_MAXIMO) duty = DUTY_MAXIMO;
    if (duty <= DUTY_MINIMO) duty = DUTY_MINIMO;

  }
  
  PWM30khz(10, duty); // duty_arduino = (duty(%))*532 -> duty_circuito = 532*(1- duty(%))
                      // o MOSFET está devolvendo a saída complementar
}

void setupPWM30khz() {
  /*  Configura timer 1 para fast PWM a 30KHz

      COM1A1=1, COM1B1=1, COM1A0=0, COM1B0=0:
      Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM (non-inverting mode)

      WGM13=1, WGM12=1, WGM11=1, WGM10=0:
      Waveform Generation mode 14: Fast PWM, TOP = ICR1

      CS12=0, CS11=0, CS10=1:
      clkI/O/1 (no prescaling)

      Frequência do PWM = 16000000/(ICR1+1)
      Neste caso, ICR1 = 532. Então:
      Frequência do PWM = 16000000/(532+1) = 30kHz

      valor min. ICR1 = 3, valor max. ICR1 = 65535
  */

  TCCR1A = 0;             // Limpa os registradores de configuração do timer 1
  TCCR1B = 0;             //
  TCNT1  = 0;             // reseta o timer
  TCCR1A = _BV(COM1A1)    // canal A no modo non-inverting
           | _BV(COM1B1)  // canal B no modo non-inverting
           | _BV(WGM11);  // mode 14: Fast PWM, TOP = ICR1
  TCCR1B = _BV(WGM12)     //
           | _BV(WGM13)   //
           | _BV(CS10);   // prescaler = 1
  ICR1   = 532;           // 30kHz

  // configura os pinos do PWM como saídas
  pinMode( 9, OUTPUT);
  pinMode(10, OUTPUT);
}

void PWM30khz(char pino, int valor_duty){
  switch (pino) {
    case 9:
      OCR1A = valor_duty; // registrador deste pino
      break;
    case 10:
      OCR1B = valor_duty;
      break;
  }
}
