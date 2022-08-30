//---------------------------------------------------------------- Pines
// USB(0,1) PULSADOR_USUARIO(2) PULSADOR_VELOCIDAD(3) BUZZER(5) PULSADOR_CONTROL(6) LED_VEL(9) LED_CONTROL(10) BT(7,8)
// MATRIZ LEDS(A0, A1, A2, 16, 14, 15) multiplexados

//---------------------------------------------------------------- Librerías

//---------------------------------------------------------------- Declaraciones
#define MAX_VEL_LEDS 3

const int PULSADOR_USUARIO = 2,
          PULSADOR_VEL_MOUSE = 6,
          PULSADOR_VEL_DISPLAY = 3, 
          VCC_LED_1 = A0, 
          VCC_LED_2 = A1, 
          VCC_LED_3 = A2, 
          GND_LED_1 = 16, 
          GND_LED_2 = 14, 
          GND_LED_3 = 15; 
          
const int MATRIZ_LED[2][3] = {{VCC_LED_1, VCC_LED_2, VCC_LED_3}, 
                              {GND_LED_1, GND_LED_2, GND_LED_3}};

bool arrayLedsEncendidos[3][3] = {{0, 0, 0}, 
                                  {0, 0, 0}, 
                                  {0, 0, 0}};

// Por como tratamos a la matriz, las filas se encuentras a GND y las columnas a VCC, pero se podría cambiar
int matrizFila = 0, matrizColumna = 0; 
bool userInput = 0;

// Calculo OCR = t * (f / PS)
// OCR1A, TCCR1B -> CS10
const int TIMER1_INTERRUPTS[MAX_VEL_LEDS][2] = {{31250, 0},    // 0.5 seg, PS: 256   OCR: 31250 
                                                {65000, 0},    // 1 seg,   PS: 256   OCR: 65000 
                                                {31250, 1}};   // 2 seg,   PS: 1024  OCR: 31250 
int timer1InterruptIndex = 1;

//---------------------------------------------------------------- Setup
void setup() {
  Serial.begin(9600);
  //Entradas salidas
  pinMode(VCC_LED_1, OUTPUT);
  pinMode(VCC_LED_2, OUTPUT);
  pinMode(VCC_LED_3, OUTPUT);
  pinMode(GND_LED_1, OUTPUT);
  pinMode(GND_LED_2, OUTPUT);
  pinMode(GND_LED_3, OUTPUT);
  noInterrupts();
  //Timers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 65000;
  TCCR1B |= (1 << WGM12);   //CTC
  TCCR1B |= (1 << CS12);    // Prescaler 256
  TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable
  // Interrupciones de pulsador por rising edge
  attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), selectorGeneral, RISING);
  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_MOUSE), selectorVelMouse, RISING);
  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_DISPLAY), selectorVelDisplay, RISING);
  interrupts();
  //USB
  //BT
  //Valores iniciales
  digitalWrite(GND_LED_1, HIGH);
  digitalWrite(GND_LED_2, HIGH);
  digitalWrite(GND_LED_3, HIGH);
}


//---------------------------------------------------------------- Loop
void loop() {
//Recepcion BT por RX
//Función Mouse
//Función COntrol
//  switch (Modo){
//    case 1:
//    //Mouse a 1 capas
//      break;
//    case 2:
//    //Mouse 2 capa
//      break;
//    
//    case 3:
//    //PIctogramas por grupo
//      break;
//    
//    case 4:
//    //pictograma por unidad
//      break;
//    case 5:
//    //POSIBLE 5TO MODO DE CONFIGURACIÓN
//      break;
//    case default:
//      break;
//  }

}


//---------------------------------------------------------------- ISR
ISR(TIMER1_COMPA_vect){
  digitalWrite(MATRIZ_LED[1][matrizFila], HIGH);
  digitalWrite(MATRIZ_LED[0][matrizColumna], LOW);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 0;
  matrizColumna++;
  if(matrizColumna > 2){
    matrizColumna = 0;
    matrizFila == 2 ? matrizFila = 0 : matrizFila++;
  }
  digitalWrite(MATRIZ_LED[1][matrizFila], LOW);
  digitalWrite(MATRIZ_LED[0][matrizColumna], HIGH);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 1;
}

//Bandera temporal
void selectorGeneral(){
  Serial.println("Pulsador de usuario");
  if(!userInput){
    TIMSK1 &= ~(1 << OCIE1A);  // Output compare Timer1 A Interrupt Disable 
    userInput = !userInput; 
  }
  else{
    TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable
    userInput = !userInput;
  }
}

void selectorVelMouse(){
  
}

void selectorVelDisplay(){
    Serial.println("Velocidad de display");
    noInterrupts();
    timer1InterruptIndex == MAX_VEL_LEDS - 1 ? timer1InterruptIndex = 0 : timer1InterruptIndex++;
    Serial.print("Index: ");
    Serial.println(timer1InterruptIndex);
    OCR1A = TIMER1_INTERRUPTS[timer1InterruptIndex][0];
    TCCR1B &= ~(1 << CS10);
    TCCR1B |= (TIMER1_INTERRUPTS[timer1InterruptIndex][3] << CS10);    
    interrupts();  
}
//---------------------------------------------------------------- Funciones
