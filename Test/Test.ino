//---------------------------------------------------------------- Pines
// USB(0,1) PULSADOR_USUARIO(2) PULSADOR_VELOCIDAD(3) BUZZER(5) PULSADOR_CONTROL(6) LED_VEL(9) LED_CONTROL(10) BT(7,8)
// MATRIZ LEDS(A0, A1, A2, 16, 14, 15) multiplexados

//---------------------------------------------------------------- Librerías

//---------------------------------------------------------------- Declaraciones
#define MAX_VEL_LEDS 3

const int PULSADOR_USUARIO = 2,
          LED = A0; 
bool ledValue = 0;

int veces = 1;
long tiempo = 0;

bool userInput = 0;

// Calculo OCR = t * (f / PS)
// OCR1A, TCCR1B -> CS10
const int TIMER1_INTERRUPTS[MAX_VEL_LEDS][2] = {{31250, 0},    // 0.5 seg, PS: 256   OCR: 31250 
                                                {65000, 0},    // 1 seg,   PS: 256   OCR: 65000 
                                                {31250, 1}};   // 2 seg,   PS: 1024  OCR: 31250 
int timer1InterruptIndex = 1;

void timer1Setup();
void timer3Setup();

//---------------------------------------------------------------- Setup
void setup() {
  Serial.begin(9600);
  //Entradas salidas
  pinMode(LED, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  noInterrupts();
  //Timers
  delay(5000);
  timer1Setup();
  timer3Setup();
  // Interrupciones de pulsador por rising edge
  attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), selectorGeneral, RISING);
  interrupts();
  Serial.println("Setups rdy");
  //USB
  //BT
  //Valores iniciales
}


//---------------------------------------------------------------- Loop
void loop() {

}


//---------------------------------------------------------------- ISR
ISR(TIMER1_COMPA_vect){
    ledValue = !ledValue;
    digitalWrite(LED, ledValue);
}

ISR(TIMER3_COMPA_vect){
  Serial.print("Se habilitan las interrupciones. Tiempo: ");
  Serial.println(millis() - tiempo);
  attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), selectorGeneral, RISING); 
  TIMSK3 &= ~(1 << OCIE3A);  // Output compare Timer3 A Interrupt Disable 
}

//Bandera temporal
void selectorGeneral(){
  detachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO));
  Serial.print("Pulsador de usuario ");
  Serial.println(veces);
  veces++;
  
  tiempo = millis();
  TIMSK3 |= (1 << OCIE3A);  // Output compare Timer1 A Interrupt Enable

  userInput ? 
    TIMSK1 |= (1 << OCIE1A) :   // Output compare Timer1 A Interrupt Enable
    TIMSK1 &= ~(1 << OCIE1A);   // Output compare Timer1 A Interrupt Disable
  userInput = !userInput;
  
}
//---------------------------------------------------------------- Funciones
void timer1Setup(){
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 65000;
  TCCR1B |= (1 << WGM12);   //CTC
  TCCR1B |= (1 << CS12);    // Prescaler 256
  TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable
  Serial.println("Timer 1 Setup ready");
}

void timer3Setup(){
  // Cambiar para t = 1000ms, algo así
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;
  OCR3A = 65000;
  TCCR3B |= (1 << WGM32);   //CTC
  TCCR3B |= (1 << CS32);    // Prescaler 256
  Serial.println("Timer 3 Setup ready");
}
