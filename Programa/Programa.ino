  //---------------------------------------------------------------- Pines
// USB(0,1) PULSADOR_USUARIO(2) PULSADOR_MODO(3) LED_MODO_1(4) LED_MODO_2(5) BUZZER(9) LED_CONTROL(10) BT(7,8)
// MATRIZ LEDS(A0, A1, A2, 16, 14, 15) multiplexados

//---------------------------------------------------------------- Librerías
#include <Mouse.h>
#include <Keyboard.h>
#include <SoftwareSerial.h>

//---------------------------------------------------------------- Declaraciones
#define MAX_VEL_LEDS 3
#define MAX_MODOS 3

const int PULSADOR_USUARIO = 2,
          PULSADOR_MODO = 3,
          LED_MODO_1 = 4,
          LED_MODO_2 = 5,
          BLUETOOTH_TX = 7,
          BLUETOOTH_RX = 8,
          BUZZER = 9,
          LEDMATRIX_COL_1 = A0, 
          LEDMATRIX_COL_2 = A1, 
          LEDMATRIX_COL_3 = A2, 
          LEDMATRIX_ROW_1 = 16, 
          LEDMATRIX_ROW_2 = 14, 
          LEDMATRIX_ROW_3 = 15; 
SoftwareSerial BT(BLUETOOTH_RX, BLUETOOTH_TX);  //RX TX 
          
const int MATRIZ_LED[2][3] = {{LEDMATRIX_COL_1, LEDMATRIX_COL_2, LEDMATRIX_COL_3}, 
                              {LEDMATRIX_ROW_1, LEDMATRIX_ROW_2, LEDMATRIX_ROW_3}};

const int LEDS_ID[3][3] = { {7, 8, 9}, 
                            {4, 5, 6}, 
                            {1, 2, 3}};
                        
bool arrayLedsEncendidos[3][3] = {{0, 0, 0},
                                  {0, 0, 0},
                                  {0, 0, 0}};

const int SECUENCIA_MODO_3[2][5][2] = { { {LEDMATRIX_COL_2, LEDMATRIX_ROW_1},
                                          {LEDMATRIX_COL_1, LEDMATRIX_ROW_2},
                                          {LEDMATRIX_COL_3, LEDMATRIX_ROW_2},
                                          {LEDMATRIX_COL_2, LEDMATRIX_ROW_3},
                                          {LEDMATRIX_COL_2, LEDMATRIX_ROW_2}},
                                        { {LEDMATRIX_COL_1, LEDMATRIX_ROW_1},
                                          {LEDMATRIX_COL_3, LEDMATRIX_ROW_1},
                                          {LEDMATRIX_COL_1, LEDMATRIX_ROW_3},
                                          {LEDMATRIX_COL_3, LEDMATRIX_ROW_3},
                                          {LEDMATRIX_COL_2, LEDMATRIX_ROW_2}}};
const int SECUENCIA_LEDS_MODO_3[2][5][2] = {{{1,0},{0,1},{2,1},{1,2},{1,1}},
                                            {{0,0},{2,0},{0,2},{2,2},{1,1}}};
int indexSecuenciaModo3 = 0;
bool capa = 0;

// Por como tratamos a la matriz, las filas se encuentras a GND y las columnas a VCC, pero se podría cambiar
int matrizFila = 0, matrizColumna = 0; 
bool columnaSelecionada = 0;
bool userInput = 0;

const int timeThreshold = 200;
long startTimePulsadorUsuario = 0, startTimePulsadorModo = 0;

// Calculo OCR = t * (f / PS)
// OCR1A, TCCR1B -> CS10
const int TIMER1_INTERRUPTS[MAX_VEL_LEDS][2] = {{31250, 0},    // 0.5 seg, PS: 256   OCR: 31250 
                                                {65000, 0},    // 1 seg,   PS: 256   OCR: 65000 
                                                {31250, 1}};   // 2 seg,   PS: 1024  OCR: 31250 
int timer1InterruptIndex = 1;

bool interruptFlagTimer3 = 0;

int modo = 1, vel_mouse = 1, range = 5, mov = range * vel_mouse, comando; //comando almacena datos bluetooth

//---------------------------------------------------------------- Setup
void setup() {
  Serial.begin(9600);
  // Keyboard.begin();
  BT.begin(9600);
  pinMode(LED_MODO_1, OUTPUT);
  pinMode(LED_MODO_2, OUTPUT);
  pinMode(LEDMATRIX_COL_1, OUTPUT);
  pinMode(LEDMATRIX_COL_2, OUTPUT);
  pinMode(LEDMATRIX_COL_3, OUTPUT);
  pinMode(LEDMATRIX_ROW_1, OUTPUT);
  pinMode(LEDMATRIX_ROW_2, OUTPUT);
  pinMode(LEDMATRIX_ROW_3, OUTPUT);
  noInterrupts();
  //Timers
  setupTimer1();
  setupTimer3();
  // Interrupciones de pulsador por rising edge
  attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), selectorGeneral, RISING);
  attachInterrupt(digitalPinToInterrupt(PULSADOR_MODO), selectorModo, RISING);
  interrupts();
  //USB
  //BT
  //Valores iniciales
  configuracionModo();
}

//---------------------------------------------------------------- Loop
void loop() {
  if (userInput == 1 && (modo == 1 || modo == 2)) {
    mouse_control();
  }
  Bluetooth(); 
}

//---------------------------------------------------------------- ISR
ISR(TIMER1_COMPA_vect) {
  switch(modo) {
    case 1:
      secuenciaLedIndividual();
      break;
    case 2:
      secuenciaLedPorCapas();
      break;
    case 3:
      if(!columnaSelecionada)
        secuenciaColumnas();
      else  
        secuenciaLedPorColumna();  
      break;
    default:
      break;
  }
}

ISR(TIMER3_COMPA_vect) {
  Serial.println("Timer 3 interrupt");
  interruptFlagTimer3 = 1;
}

void selectorGeneral(){
  if(millis() - startTimePulsadorUsuario > timeThreshold){
    startTimePulsadorUsuario = millis();
    TCNT1 = 0; // Se limpia el contador del timer 1 
    Serial.println("Pulsador de usuario");
    if(!userInput){
      switch(modo){
        case 1:
          TIMSK1 &= ~(1 << OCIE1A);   // Output compare Timer1 A Interrupt Disable
          TIMSK3 |= (1 << OCIE3A);    // Output compare Timer3 A Interrupt Enable
          userInput = !userInput; 
          break;
        
        case 2:
          TIMSK1 &= ~(1 << OCIE1A);   // Output compare Timer1 A Interrupt Disable
          TIMSK3 |= (1 << OCIE3A);    // Output compare Timer3 A Interrupt Enable
          userInput = !userInput;        
          break;
        
        case 3:
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
      TIMSK3 &= ~(1 << OCIE3A);   // Output compare Timer3 A Interrupt Disable 
      userInput = !userInput;
      if(modo == 3){
        configuracionModo();
      }
    }
  }
}

void selectorModo(){
  if(millis() - startTimePulsadorModo > timeThreshold){
    startTimePulsadorModo = millis();
    modo == MAX_MODOS ? modo = 0 : modo++;
    configuracionModo();
  }
}

// Este después se borra, se configura por BT
/*void selectorVelMouse(){
  Serial.print("velocidad del mouse: ");
  Serial.println(vel_mouse);
  vel_mouse==3 ? vel_mouse=1 : vel_mouse++;
  mov= vel_mouse*range;
  }*/

// Este después se borra, se configura por BT
// void selectorVelDisplay() {
//   Serial.println("Velocidad de display");
//   noInterrupts();
//   timer1InterruptIndex == 2 ? timer1InterruptIndex = 0 : timer1InterruptIndex++;
//   Serial.print("Index: ");
//   Serial.println(timer1InterruptIndex);
//   OCR1A = TIMER1_INTERRUPTS[timer1InterruptIndex][0];
//   TCCR1B &= ~(1 << CS10);
//   TCCR1B |= (TIMER1_INTERRUPTS[timer1InterruptIndex][3] << CS10);
//   interrupts();
// }

//---------------------------------------------------------------- Funciones
void setupTimer1(){
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 65000;
  TCCR1B |= (1 << WGM12);   //CTC
  TCCR1B |= (1 << CS12);    // Prescaler 256
  TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable
}

void setupTimer3(){
  // Ajustar para el mov del mouse
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;
  OCR3A = 6500;
  TCCR3B |= (1 << WGM32);   //CTC
  TCCR3B |= (1 << CS32);    // Prescaler 256
}

void configuracionModo(){
  digitalWrite(LED_MODO_1, LOW);
  digitalWrite(LED_MODO_2, LOW); 
  apagarLeds();
  TIMSK1 &= ~(1 << OCIE1A);   // Output compare Timer1 A Interrupt Disable
  TIMSK3 &= ~(1 << OCIE3A);   // Output compare Timer3 A Interrupt Disable 
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
      capa = 0;
      digitalWrite(LED_MODO_1, HIGH);
      digitalWrite(LED_MODO_2, HIGH);
      indexSecuenciaModo3 = 0;
      digitalWrite(SECUENCIA_MODO_3[indexSecuenciaModo3][0], HIGH); 
      digitalWrite(SECUENCIA_MODO_3[indexSecuenciaModo3][1], LOW); 
      break;

    case 3:
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
}

void secuenciaColumnas(){
  digitalWrite(MATRIZ_LED[0][matrizColumna], LOW);
  // Uso la primer fila para indicar que toda la columna se encuentra encendida/apagada
  arrayLedsEncendidos[0][matrizColumna] = 0;
  matrizColumna == 2 ? matrizColumna = 0 : matrizColumna++;
  digitalWrite(MATRIZ_LED[0][matrizColumna], HIGH);
  arrayLedsEncendidos[0][matrizColumna] = 1;
}

void secuenciaLedPorColumna(){
  digitalWrite(MATRIZ_LED[1][matrizFila], HIGH);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 0;
  matrizFila == 2 ? matrizFila = 0 : matrizFila++;
  digitalWrite(MATRIZ_LED[1][matrizFila], LOW);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 1;
}

void secuenciaLedPorCapas(){
  digitalWrite(MATRIZ_LED[0][SECUENCIA_LEDS_MODO_3[capa][indexSecuenciaModo3][0]], LOW);
  digitalWrite(MATRIZ_LED[1][SECUENCIA_LEDS_MODO_3[capa][indexSecuenciaModo3][1]], HIGH);
  arrayLedsEncendidos[SECUENCIA_LEDS_MODO_3[capa][indexSecuenciaModo3][1]][SECUENCIA_LEDS_MODO_3[capa][indexSecuenciaModo3][0]] = 0;
  indexSecuenciaModo3 == 4 ? indexSecuenciaModo3 = 0 : indexSecuenciaModo3++;
  digitalWrite(MATRIZ_LED[0][SECUENCIA_LEDS_MODO_3[capa][indexSecuenciaModo3][0]], HIGH);
  digitalWrite(MATRIZ_LED[1][SECUENCIA_LEDS_MODO_3[capa][indexSecuenciaModo3][1]], LOW);
  arrayLedsEncendidos[SECUENCIA_LEDS_MODO_3[capa][indexSecuenciaModo3][1]][SECUENCIA_LEDS_MODO_3[capa][indexSecuenciaModo3][0]] = 1;
}

void mouse_control() {
  switch (estado()) {
    //    case 7:
    //      Keyboard.press(KEY_ESC); //tecla escape posible necesidad de pequeña espera
    //      Keyboard.release(KEY_ESC);
    //      break;
    case 8:
      if(interruptFlagTimer3){
        interruptFlagTimer3 = 0;
        Serial.println("Mouse arriba"); //cambiar 20 por mov
        Mouse.move(0, (-mov), 0); //flecha arriba    10=range varias experimentalmente
      }
      break;
    // case 9:

    // break;
    case 4:
    if(interruptFlagTimer3){
        interruptFlagTimer3 = 0;
        Serial.println("Mouse izquierda");
        Mouse.move(-(mov), 0, 0); //flecha izquierda
      }
      break;
    case 5:
      if(modo == 2)
        capa = !capa;
      TIMSK1 |= (1 << OCIE1A);    // Output compare Timer1 A Interrupt Enable
      TIMSK3 &= ~(1 << OCIE3A);   // Output compare Timer3 A Interrupt Disable 
      userInput = !userInput;
      break;
    case 6:
      if(interruptFlagTimer3){
        interruptFlagTimer3 = 0;
        Serial.println("Mouse derecha");
        Mouse.move((mov), 0, 0); //flecha derecha
      }
      break;
    case 1:
      // Para los click se puede poner las opción que permita pulsar de nuevo capaz? Por el doble click, 
      // salvo que sea otro led eso
      TIMSK1 |= (1 << OCIE1A);    // Output compare Timer1 A Interrupt Enable
      TIMSK3 &= ~(1 << OCIE3A);   // Output compare Timer3 A Interrupt Disable 
      userInput = !userInput;
      Mouse.click(MOUSE_LEFT);
      break;
    case 2:
      if(interruptFlagTimer3){
        interruptFlagTimer3 = 0;
        Serial.println("Mouse abajo");
        Mouse.move(0, (mov), 0); //flecha abajo
      }
      break;
    case 3:
      TIMSK1 |= (1 << OCIE1A);    // Output compare Timer1 A Interrupt Enable
      TIMSK3 &= ~(1 << OCIE3A);   // Output compare Timer3 A Interrupt Disable 
      userInput = !userInput;
      Mouse.click(MOUSE_RIGHT);   //click derecho
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
        break;
      case 'b':
        Mouse.move(0, mov, 0);
        break;
      case 'd':
        Mouse.move(mov, 0, 0);
        break;
      case 'i':
        Mouse.move(-mov, 0, 0);
        break;
      case 'e':
        Keyboard.press(KEY_ESC); //tecla escape posible necesidad de pequeña espera
        Keyboard.release(KEY_ESC);
        break;        

      case 'y':
        Mouse.click(MOUSE_LEFT);
        break;
      case 'z':
        Mouse.click(MOUSE_RIGHT);
        break;

        //agregar funciones de configuración
    }
  }
}
