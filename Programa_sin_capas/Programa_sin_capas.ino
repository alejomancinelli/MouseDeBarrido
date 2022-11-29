//---------------------------------------------------------------- Pines
/*
  Pin | Port map  | Características   | Función                   | Descripción
  --- | --------- | ----------------- | ------------------------- | --------------------------
  0   | PD3       | Rx, INT3          | USB                       | 
  1   | PD2       | Tx, INT2          | USB                       | 
  2   | PD1       | INT1              | Pulsador user             | Pulsador grande selección
  3   | PD0       | INT0              | Pulsador modos            | Pulsador cambio de modos
  4   | PD4       |                   | Led modo (1)              | Indicación modo (1)
  5   | PC6       | PWM               | Led modo (2)              | Indicación modo (2)
  6   | PD7       |                   | Pulsador Velocidad Luces  | 
  7   | PE6       | INT6              | Pulsador Velocidad Mouse  | 
  8   | PB4       |                   | Bluetooth                 | Rx BT / Tx Arduino
  9   | PB5       | PWM               | Bluetooth                 | Tx BT / Rx Arduino
  10	| PB6       | PWM               | Buzzer                    |  
  16	| PB2       |                   | LEDMATRIX_ROW_1           | Matriz GND
  14	| PB3       |                   | LEDMATRIX_ROW_2           | Matriz GND
  15	| PB1       |                   | LEDMATRIX_ROW_3           | Matriz GND
  A0	| PF7       |                   | LEDMATRIX_COL_1           | Matriz VCC
  A1	| PF6       |                   | LEDMATRIX_COL_2           | Matriz VCC
  A2	| PF5       |                   | LEDMATRIX_COL_3           | Matriz VCC
  A3	| PF4       |                   |                           | 
*/

//---------------------------------------------------------------- Librerías
#include <Mouse.h>
#include <Keyboard.h>
#include <SoftwareSerial.h>

//---------------------------------------------------------------- Declaraciones
#define MAX_VEL_LEDS 3    // (1) 0.5 seg - (2) 1 seg - (3) 2 seg
#define MAX_MODOS 2       // (1)Mouse de 1 capa - (2)Pictograma
#define MAX_VEL_MOUSE 3 
//#define SONIDO_DEL_MAL  

const int PULSADOR_USUARIO = 2,
          PULSADOR_MODO = 3,
          LED_MODO_1 = 4, 
          LED_MODO_2 = 5,
          PULSADOR_VELOCIDAD_LUCES = 6,
          PULSADOR_VELOCIDAD_MOUSE = 7,
          BLUETOOTH_TX = 8,
          BLUETOOTH_RX = 9,
          BUZZER = 10,
          LEDMATRIX_ROW_1 = 16, 
          LEDMATRIX_ROW_2 = 14, 
          LEDMATRIX_ROW_3 = 15, 
          LEDMATRIX_COL_1 = A0, 
          LEDMATRIX_COL_2 = A1, 
          LEDMATRIX_COL_3 = A2; 
SoftwareSerial BT(BLUETOOTH_RX, BLUETOOTH_TX);  //ArudinoRX ArduinoTX, se conectan al revés con el módulo BT 
          
// Matriz leds
const int MATRIZ_LED[2][3] = {{LEDMATRIX_COL_1, LEDMATRIX_COL_2, LEDMATRIX_COL_3}, 
                              {LEDMATRIX_ROW_1, LEDMATRIX_ROW_2, LEDMATRIX_ROW_3}};

const int LEDS_ID[3][3] = { {7, 8, 9}, 
                            {4, 5, 6}, 
                            {1, 2, 3}};
                        
bool arrayLedsEncendidos[3][3] = {{0, 0, 0},
                                  {0, 0, 0},
                                  {0, 0, 0}};

int matrizFila = 0, matrizColumna = 0; 
bool userInput = 0, cambioSecuenciaLed = 0;

// Modos
bool columnaSelecionada = 0;

// Pulsadores
const int TIME_THRESHOLD = 200;
unsigned long startTimePulsadorUsuario = 0, startTimePulsadorModo = 0, startTimePulsadorVelLuces = 0, startTimePulsadorVelMouse = 0;
bool lastPulsVelLucesState = 0, pulsVelLucesState = 0, lastPulsVelMouseState = 0, pulsVelMouseState = 0;
 
const int MOUSE_THRESHOLD = 100;
unsigned long mouseLastReady = 0;

// Buzzer
const int TONOS_MODO_1[3][3] = {  {261, 293, 329},
                                  {349, 392, 440},
                                  {493, 523, 587}};
const int TONOS_MODO_2[2][3] = {  {261, 329, 349},
                                  {293, 349, 440}};

                            
int buzzerCounter = 0, avisoBuzzerActivo = 0, avisoTone;
// BUZZER_TIME_ON en ms, BUZZER_TIME_OFF * 10 para ms
const int BUZZER_TIME_ON = 100, BUZZER_TIME_OFF = 20; // Pausa entre beeps = BUZZER_TIME_OFF - BUZZER_TIME_ON 
const int BUZZER_SECUENCIA_TIME_ON = 250;


// Timers: Calculo OCR = t * (f / PS)
// OCR1A, TCCR1B -> CS10
//const int TIMER1_INTERRUPTS[MAX_VEL_LEDS][2] = {{31250, 0},    // 0.5 seg, PS: 256   OCR: 31250 
//                                                {65000, 0},    // 1 seg,   PS: 256   OCR: 65000 
//                                                {31250, 1}};   // 2 seg,   PS: 1024  OCR: 31250 
const int TIMER1_INTERRUPTS[MAX_VEL_LEDS] = {200, 100, 50};
int timer1InterruptIndex = 1;
int timer1InterruptCounter = 0, timer1InterruptThreshold = 100;

// Mouse`
const int MOV_MOUSE[MAX_VEL_MOUSE] = {5, 10, 15};
int modo = 1, movIndex = 0, mov = MOV_MOUSE[movIndex], comando; //comando almacena datos bluetooth

//---------------------------------------------------------------- Setup
void setup() {
  pinMode(LED_MODO_1, OUTPUT);
  pinMode(LED_MODO_2, OUTPUT);
  pinMode(LEDMATRIX_COL_1, OUTPUT);
  pinMode(LEDMATRIX_COL_2, OUTPUT);
  pinMode(LEDMATRIX_COL_3, OUTPUT);
  pinMode(LEDMATRIX_ROW_1, OUTPUT);
  pinMode(LEDMATRIX_ROW_2, OUTPUT);
  pinMode(LEDMATRIX_ROW_3, OUTPUT);
  pinMode(PULSADOR_VELOCIDAD_LUCES, INPUT);
  pinMode(PULSADOR_VELOCIDAD_MOUSE, INPUT);
  noInterrupts();
  // Timers
  setupTimer1();
  // Interrupciones de pulsador por rising edge
  attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), selectorGeneral, FALLING);
  attachInterrupt(digitalPinToInterrupt(PULSADOR_MODO), selectorModo, RISING);
  interrupts();
  // Valores iniciales
  configuracionModo();
  // USB
  Serial.begin(9600);
  Keyboard.begin();
  // BT
  BT.begin(9600);
}

//---------------------------------------------------------------- Loop
void loop() {  
  if(cambioSecuenciaLed == 1){
    switch(modo) {
      case 1:
        secuenciaLedIndividual();
        break;
      case 2:
        if(!columnaSelecionada)
          secuenciaColumnas();
        else  
          secuenciaLedPorColumna();  
        break;
      default:
        break;
    }
  }
  if (userInput == 1 && modo == 1) 
    mouseControl();
  // pulsadorVelocidadLucesPooling();
  funcionPulsadorAntiRebotePooling(&PULSADOR_VELOCIDAD_LUCES, &pulsVelLucesState, &lastPulsVelLucesState, &startTimePulsadorVelLuces, &cambioVelocidadLuces);
  funcionPulsadorAntiRebotePooling(&PULSADOR_VELOCIDAD_MOUSE, &pulsVelMouseState, &lastPulsVelMouseState, &startTimePulsadorVelMouse, &cambioVelocidadMouse);
  Bluetooth();
  if (avisoBuzzerActivo > 0 && buzzerCounter >= BUZZER_TIME_OFF){
    avisoCambioVelBuzzer();
    Serial.println("BEEP");
  } 
}

//---------------------------------------------------------------- ISR
// Interrupción Timer 1 que realiza la secuencia de encendido de leds
ISR(TIMER1_COMPA_vect) {
  timer1InterruptCounter++;
  if(timer1InterruptCounter == timer1InterruptThreshold){
    cambioSecuenciaLed = 1;
    timer1InterruptCounter = 0;
  }
  if(avisoBuzzerActivo != 0)
    buzzerCounter++;
}

// Interrupción por rising edge del pulsador de usuario
void selectorGeneral(){
  // Debounce del pulsador
  if(millis() - startTimePulsadorUsuario > TIME_THRESHOLD){
    startTimePulsadorUsuario = millis();
    TCNT1 = 0; // Se limpia el contador del timer 1 
    Serial.println("Pulsador de usuario");
    if(!userInput){
      switch(modo){
        case 1:
          TIMSK1 &= ~(1 << OCIE1A);   // Output compare Timer1 A Interrupt Disable
          userInput = !userInput; 
          break;
        
        case 2:
          if(!columnaSelecionada){
            apagarLeds();
            digitalWrite(MATRIZ_LED[1][matrizFila], LOW);
            digitalWrite(MATRIZ_LED[0][matrizColumna], HIGH);
          }
          else{
            userInput = !userInput;
            TIMSK1 &= ~(1 << OCIE1A);   // Output compare Timer1 A Interrupt Disable
          }
          columnaSelecionada = !columnaSelecionada;
          break;
      } 
    }
    else{
      TIMSK1 |= (1 << OCIE1A);    // Output compare Timer1 A Interrupt Enable
      userInput = !userInput;
      if(modo == 2){
        configuracionModo();
      }
    }
  }
}

// Interrupción por rising edge del pulsador selector de modo
void selectorModo(){
  if(millis() - startTimePulsadorModo > TIME_THRESHOLD){
    startTimePulsadorModo = millis();
    modo == MAX_MODOS ? modo = 0 : modo++;
    configuracionModo();
  }
}

//---------------------------------------------------------------- Funciones
void setupTimer1(){
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 20000;
  timer1InterruptIndex = 1;
  TCCR1B |= (1 << WGM12);   //CTC
  TCCR1B |= (1 << CS11);    // Prescaler 256
  TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable
}

void configuracionModo(){
  noTone(BUZZER);
  digitalWrite(LED_MODO_1, LOW);
  digitalWrite(LED_MODO_2, LOW); 
  apagarLeds();
  TIMSK1 &= ~(1 << OCIE1A);   // Output compare Timer1 A Interrupt Disable
  matrizFila = 0;
  matrizColumna = 0;
  userInput = 0;
  columnaSelecionada = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      arrayLedsEncendidos[i][j] = 0;
    }
  }
  switch(modo){
    case 1:
      digitalWrite(LED_MODO_1, HIGH);
      digitalWrite(MATRIZ_LED[1][matrizFila], LOW);
      digitalWrite(MATRIZ_LED[0][matrizColumna], HIGH);
      break; 

    case 2:
      digitalWrite(LED_MODO_2, HIGH);
      for(int fila = 0; fila < 3; fila++){
        digitalWrite(MATRIZ_LED[1][fila], LOW);
      }
      digitalWrite(MATRIZ_LED[0][matrizColumna], HIGH);
      break;

    default:
      // En modo 0 se podría poner en bajo consumo, desactivando las interrupciones del clock
      break; 
  }
  TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable
}

void apagarLeds(){
  for(int columna = 0; columna < 3; columna++){
    digitalWrite(MATRIZ_LED[0][columna], LOW);
  }
  for(int fila = 0; fila < 3; fila++){
    digitalWrite(MATRIZ_LED[1][fila], HIGH);
  }
}

int estado() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (arrayLedsEncendidos[i][j]) {
        return LEDS_ID[i][j];
      }
    }
  }
  return -1;
}

void secuenciaLedIndividual() {
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
  #ifdef SONIDO_DEL_MAL
    if(avisoBuzzerActivo == 0)
      tone(BUZZER, TONOS_MODO_1[matrizFila][matrizColumna], BUZZER_SECUENCIA_TIME_ON);
  #endif
  cambioSecuenciaLed = 0;
}

void secuenciaColumnas(){
  digitalWrite(MATRIZ_LED[0][matrizColumna], LOW);
  matrizColumna == 2 ? matrizColumna = 0 : matrizColumna++;
  digitalWrite(MATRIZ_LED[0][matrizColumna], HIGH);
  #ifdef SONIDO_DEL_MAL
    if(avisoBuzzerActivo == 0)
      tone(BUZZER, TONOS_MODO_3[0][matrizColumna], BUZZER_SECUENCIA_TIME_ON);
  #endif
  cambioSecuenciaLed = 0;
}

void secuenciaLedPorColumna(){
  digitalWrite(MATRIZ_LED[1][matrizFila], HIGH);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 0;
  matrizFila == 2 ? matrizFila = 0 : matrizFila++;
  digitalWrite(MATRIZ_LED[1][matrizFila], LOW);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 1;
  #ifdef SONIDO_DEL_MAL
    if(avisoBuzzerActivo == 0)
      tone(BUZZER, TONOS_MODO_3[1][matrizFila], BUZZER_SECUENCIA_TIME_ON);
  #endif
  cambioSecuenciaLed = 0;
}

void funcionPulsadorAntiRebotePooling(int* pulsador, bool* state, bool* lastState, unsigned long* startTime, void (*funcionAsociada)()){
  int lectura = digitalRead(*pulsador);
  
  if (lectura != *lastState)
    *startTime = millis();
    
  if ((millis() - *startTime) > TIME_THRESHOLD) {
    if (lectura != *state) {
      *state = lectura;
      if(*state == HIGH){
        Serial.println("Pulsador velocidad mouse");
        funcionAsociada();
      }
    }
  }
  *lastState = lectura;
}

void cambioVelocidadLuces(){
  Serial.println("Velocidad de display");
  noInterrupts();
  timer1InterruptIndex == MAX_VEL_LEDS - 1 ? timer1InterruptIndex = 0 : timer1InterruptIndex++;
  avisoBuzzerActivo = timer1InterruptIndex + 1;
  avisoTone = 261;
  Serial.print("Index: ");
  Serial.println(timer1InterruptIndex);
  timer1InterruptThreshold = TIMER1_INTERRUPTS[timer1InterruptIndex];
  Serial.print("Threshold: ");
  Serial.println(timer1InterruptThreshold);
  timer1InterruptCounter = 0;
  interrupts();
}

void cambioVelocidadMouse(){
  movIndex == MAX_VEL_MOUSE - 1 ? movIndex = 0 : movIndex++;
  Serial.print("Velocidad del mouse: ");
  Serial.println(movIndex);
  avisoBuzzerActivo = movIndex + 1;
  mov = MOV_MOUSE[movIndex];
  avisoTone = 523;
}

void avisoCambioVelBuzzer(){
  noTone(BUZZER);
  tone(BUZZER, avisoTone, BUZZER_TIME_ON);
  buzzerCounter = 0;
  avisoBuzzerActivo--;
}

bool mouseReady(){
  return (millis() - mouseLastReady) > MOUSE_THRESHOLD;
}

void returnToSequence(){
  TIMSK1 |= (1 << OCIE1A);    // Output compare Timer1 A Interrupt Enable
  TIMSK0 &= ~(1 << OCIE0A);   // Output compare Timer3 A Interrupt Disable 
  userInput = !userInput;
}

void mouseControl() {
  switch (estado()) {
    case 7:
      Keyboard.press(KEY_ESC); //tecla escape posible necesidad de pequeña espera
      Keyboard.release(KEY_ESC);
      returnToSequence();
      break;
    case 8:
      if(mouseReady()){
        mouseLastReady = millis();
        Serial.println("Mouse arriba"); //cambiar 20 por mov
        Mouse.move(0, (-mov), 0); //flecha arriba    10=range varias experimentalmente
      }
      break;
    case 9:
      Mouse.isPressed(MOUSE_LEFT) ? Mouse.release(MOUSE_LEFT) : Mouse.press(MOUSE_LEFT); 
      returnToSequence();
      break;
    case 4:
    if(mouseReady()){
        mouseLastReady = millis();
        Serial.println("Mouse izquierda");
        Mouse.move(-(mov), 0, 0); //flecha izquierda
      }
      break;
    case 5:
      TIMSK1 |= (1 << OCIE1A);    // Output compare Timer1 A Interrupt Enable
      userInput = !userInput;
      break;
    case 6:
      if(mouseReady()){
        mouseLastReady = millis();
        Serial.println("Mouse derecha");
        Mouse.move((mov), 0, 0); //flecha derecha
      }
      break;
    case 1:
      Mouse.click(MOUSE_LEFT);
      returnToSequence();
      break;
    case 2:
      if(mouseReady()){
        mouseLastReady = millis();
        Serial.println("Mouse abajo");
        Mouse.move(0, (mov), 0); //flecha abajo
      }
      break;
    case 3:
      Mouse.click(MOUSE_RIGHT);   //click derecho
      returnToSequence();
      break;
    default:
      break;
  }
}

void Bluetooth () { // a=arriba, b=below, d=derecha, i=izquierda, y=click izquierdo, z=click derecho
  if (BT.available() > 0) {    // lee el bluetooth y almacena en serial
    comando = BT.read();
    
    switch (comando) {
      case 'a':
        Mouse.move(0, -mov, 0);
        Serial.println("BT Mouse arriba"); 
        break;
      case 'b':
        Serial.println("BT Mouse abajo"); 
        Mouse.move(0, mov, 0);
        break;
      case 'd':
        Serial.println("BT Mouse derecha"); 
        Mouse.move(mov, 0, 0);
        break;
      case 'i':
        Serial.println("BT Mouse izquierda"); 
        Mouse.move(-mov, 0, 0);
        break;
      case 'e':
        Serial.println("BT ESC"); 
        Keyboard.press(KEY_ESC); //tecla escape posible necesidad de pequeña espera
        Keyboard.release(KEY_ESC);
        break;        
      case 'y':
        Serial.println("BT Left click"); 
        Mouse.click(MOUSE_LEFT);
        break;
      case 'z':
        Serial.println("BT Right click"); 
        Mouse.click(MOUSE_RIGHT);
        break;
    
    // MODOS
      case 'u':
        Serial.println("BT Modo 1");
        modo = 1;
        configuracionModo();
        break;
      case 'o':
        Serial.println("BT Modo 2");
        modo = 2;
        configuracionModo();
        break;

      // Velocidad LED
      case 's': //VLed baja
        Serial.println("BT Vel 0");
        noInterrupts();
        timer1InterruptIndex = 0;
        timer1InterruptThreshold = TIMER1_INTERRUPTS[timer1InterruptIndex];
        timer1InterruptCounter = 0;
        avisoTone = 261;
        avisoBuzzerActivo = timer1InterruptIndex + 1;
        interrupts();
        break;
      case 'm': //VLed media
        Serial.println("BT Vel 1");
        noInterrupts();
        timer1InterruptIndex = 1;
        timer1InterruptThreshold = TIMER1_INTERRUPTS[timer1InterruptIndex];
        timer1InterruptCounter = 0;
        avisoTone = 261;
        avisoBuzzerActivo = timer1InterruptIndex + 1;
        interrupts();
        break;
      case 'w': //VLed alta
        Serial.println("BT Vel 2");
        noInterrupts();
        timer1InterruptIndex = 2;
        timer1InterruptThreshold = TIMER1_INTERRUPTS[timer1InterruptIndex];
        timer1InterruptCounter = 0;
        avisoTone = 261;
        avisoBuzzerActivo = timer1InterruptIndex + 1;
        interrupts();
        break;
        
      // Velocidad MOUSE
      case 'k': //VMouse baja
        Serial.println("BT Mouse 0");
        movIndex = 0;
        mov = MOV_MOUSE[movIndex];
        avisoTone = 523;
        avisoBuzzerActivo = movIndex + 1;
        break;
      case 'n': //VMouse medua
        Serial.println("BT Mouse 1");
        movIndex = 1;
        mov = MOV_MOUSE[movIndex];
        avisoTone = 523;
        avisoBuzzerActivo = movIndex + 1;
        break;
      case 'p': //VMouse alta
        Serial.println("BT Mouse 2");
        movIndex = 2;
        mov = MOV_MOUSE[movIndex];
        avisoTone = 523;
        avisoBuzzerActivo = movIndex + 1;
        break;
    }
  }
}
