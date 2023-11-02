// PWM a 30 KHz

void setupPWM30khz();
void PWM30khz(char pino, int valor);


void setup() {
  setupPWM30khz();  
}
/*****************************************************************/

void loop() {
  PWM30khz(10, 320); // duty_arduino = (duty(%))*532 -> duty_circuito = 532*(1- duty(%))
                     // o MOSFET está devolvendo a saída complementar 
  
  /*for (int i=0; i<=532; i++){
    PWM30khz(10, i);
    delay(10);  
  }
  for (int i=532; i>=0; i--){
    PWM30khz(10, i);
    delay(10);  
  }*/
}
/*****************************************************************/

void setupPWM30khz(){
  /*  Configura timer 1 para fast PWM a 30KHz
   *   
   *  COM1A1=1, COM1B1=1, COM1A0=0, COM1B0=0:
   *  Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM (non-inverting mode)
   *  
   *  WGM13=1, WGM12=1, WGM11=1, WGM10=0: 
   *  Waveform Generation mode 14: Fast PWM, TOP = ICR1  
   *  
   *  CS12=0, CS11=0, CS10=1:
   *  clkI/O/1 (no prescaling)
   *  
   *  Frequência do PWM = 16000000/(ICR1+1)
   *  ex: ICR1 = 532:
   *  Frequência do PWM = 16000000/(532+1) = 30kHz
   *  
   *  valor min. ICR1 = 3, valor max. ICR1 = 65535
   */
   
    TCCR1A = 0;           // Limpa os registradores de configuração do timer 1
    TCCR1B = 0;           // 
    TCNT1  = 0;           // reseta o timer
    TCCR1A = _BV(COM1A1)  // canal A no modo non-inverting
           | _BV(COM1B1)  // canal B no modo non-inverting
           | _BV(WGM11);  // mode 14: Fast PWM, TOP = ICR1
    TCCR1B = _BV(WGM12)   // 
           | _BV(WGM13)   //
           | _BV(CS10);   // prescaler = 1
    ICR1   = 532;         // TOP = 532

    // configura os pinos do PWM como saída
    pinMode( 9, OUTPUT);
    pinMode(10, OUTPUT); 
}
/*****************************************************************/

void PWM30khz(char pino, int valor){
  switch (pino) {
    case 9:
        OCR1A = valor;
        break;
    case 10:
        OCR1B = valor;
        break;
  }
}
