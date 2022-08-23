const int LED = 3,
          PULSADOR_VEL_DISPLAY = 2;
bool encenderLed = 0;

const int TIMER1_INTERRUPTS[6][4] = {{31250, 1, 0, 0},
                                     {65000, 1, 0, 0},
                                     {3125,  1, 0, 1},
                                     {4688,  1, 0, 1},
                                     {6250,  1, 0, 1},
                                     {7813,  1, 0, 1},};
int timer1InterruptIndex = 1;
          
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  
  noInterrupts();
  // Clock de 16MHz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  // Para interrupcion cada 5 seg: OCR1A = 7813, PS = 1024
  // Para interrupcion cada 4 seg: OCR1A = 6250, PS = 1024
  // Para interrupcion cada 3 seg: OCR1A = 4688, PS = 1024
  // Para interrupcion cada 2 seg: OCR1A = 3125, PS = 1024
  // Para interrupcion cada 1 seg: OCR1A = 65000, PS = 256
  // Para interrupcion cada 0.5 seg: OCR1A = 31250, PS = 256
  OCR1A = 65000;
  TCCR1B |= (1 << WGM12);   //CTC
  TCCR1B |= (1 << CS12);    // Prescaler 256
  TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable

  // Interrupciones de pulsador por rising edge
  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_DISPLAY), selectorVelDisplay, RISING);
  interrupts();

  digitalWrite(LED, LOW);
}


//---------------------------------------------------------------- Loop
void loop() {
  
}


//---------------------------------------------------------------- ISR
ISR(TIMER1_COMPA_vect){
  encenderLed = !encenderLed;
  digitalWrite(LED, encenderLed);
}

// Parecería andar, hay que probar con el Pro Micro
void selectorVelDisplay(){
    timer1InterruptIndex == 6 ? timer1InterruptIndex = 0 : timer1InterruptIndex++;
    noInterrupts();
    OCR1A = TIMER1_INTERRUPTS[timer1InterruptIndex][0];
    TCCR1B &= ~(1 << CS10);
    TCCR1B |= (TIMER1_INTERRUPTS[timer1InterruptIndex][3] << CS10);    
    interrupts();
}
//---------------------------------------------------------------- Funciones
