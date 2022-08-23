//---------------------------------------------------------------- Pines
// USB(0,1) PULSADOR_USUARIO(2) PULSADOR_VELOCIDAD(3) BUZZER(5) PULSADOR_CONTROL(6) LED_VEL(9) LED_CONTROL(10) BT(7,8)
// MATRIZ LEDS(A0, A1, A2, 16, 14, 15) multiplexados

//---------------------------------------------------------------- Librerías
#include "Mouse.h"
#include <Keyboard.h>

//---------------------------------------------------------------- Declaraciones
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

const int TIMER1_INTERRUPTS[6][4] = {{31250, 1, 0, 0},
                                     {65000, 1, 0, 0},
                                     {31250,  1, 0, 1}};
//                                     {4688,  1, 0, 1},
//                                     {6250,  1, 0, 1},
//                                     {7813,  1, 0, 1},};
int timer1InterruptIndex = 1;

int  modo = 1, vel = 20;

int estado();
void modo1();

//---------------------------------------------------------------- Setup
void setup() {
  Serial.begin(9600);
  // Keyboard.begin();
  pinMode(VCC_LED_1, OUTPUT);
  pinMode(VCC_LED_2, OUTPUT);
  pinMode(VCC_LED_3, OUTPUT);
  pinMode(GND_LED_1, OUTPUT);
  pinMode(GND_LED_2, OUTPUT);
  pinMode(GND_LED_3, OUTPUT);
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 65000;
  TCCR1B |= (1 << WGM12);   //CTC
  TCCR1B |= (1 << CS12);    // Prescaler 256
  TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable

  // Interrupciones de pulsador por rising edge
  attachInterrupt(digitalPinToInterrupt(PULSADOR_USUARIO), selectorGeneral, RISING);
//  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_MOUSE), selectorVelMouse, RISING);
  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_DISPLAY), selectorVelDisplay, RISING);
  interrupts();
  //Entradas salidas
  //USB
  //BT
  //Timers
  //Valores iniciales
  digitalWrite(GND_LED_1, HIGH);
  digitalWrite(GND_LED_2, HIGH);
  digitalWrite(GND_LED_3, HIGH);
}


//---------------------------------------------------------------- Loop
void loop() {
  if(userInput){
    mouse_control();
  }

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
  switch (modo) {
    case 1:
      modo1();
      break;
  /*case 2:
      MODO2();
      break;
    case 3:
      MODO3();
      break;*/
  }
}

//Bandera temporal
void selectorGeneral(){
  Serial.println("Pulsador de usuario");
  if(!userInput){
    TIMSK1 &= ~(1 << OCIE1A);  // Output compare Timer1 A Interrupt Disable 
    userInput = 1;
    estado(); 
  }
  else{
    TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable
    userInput = 0;
  }
}

void selectorVelMouse(){
  
}

void selectorVelDisplay(){
    Serial.println("Velocidad de display");
    noInterrupts();
    timer1InterruptIndex == 2? timer1InterruptIndex = 0 : timer1InterruptIndex++;
    Serial.print("Index: ");
    Serial.println(timer1InterruptIndex);
    OCR1A = TIMER1_INTERRUPTS[timer1InterruptIndex][0];
    TCCR1B &= ~(1 << CS10);
    TCCR1B |= (TIMER1_INTERRUPTS[timer1InterruptIndex][3] << CS10);    
    interrupts();  
}
//---------------------------------------------------------------- Funciones
int estado() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if(arrayLedsEncendidos[i][j]){
        Serial.print("Led pulsado: ");
        Serial.println((i * 3 + j));
        return (i * 3 + j); 
      }
    }
  }
  return -1;
}

void modo1() {
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
 // Test
  Serial.print(arrayLedsEncendidos[0][0]);
  Serial.print(arrayLedsEncendidos[0][1]);
  Serial.print(arrayLedsEncendidos[0][2]);
  Serial.print(arrayLedsEncendidos[1][0]);
  Serial.print(arrayLedsEncendidos[1][1]);
  Serial.print(arrayLedsEncendidos[1][2]);
  Serial.print(arrayLedsEncendidos[2][0]);
  Serial.print(arrayLedsEncendidos[2][1]);
  Serial.print(arrayLedsEncendidos[2][2]);
  Serial.println();
}

void mouse_control() {
 switch (estado()) {
//    case 0:
//      Keyboard.press(KEY_ESC); //tecla escape posible necesidad de pequeña espera
//      Keyboard.release(KEY_ESC);
//      break;
    case 1:
      Serial.println("Mouse arriba");
      Mouse.move(0, (10 * vel), 0); //flecha arriba    10=range varias experimentalmente
      break;
//    case 2:
//      velocidad = velocidad * k;
//      k++;
//      if (k == 3)   //Usar flag para que varíe única vez
//        k = 1;        //velocidad
//      break;
    case 3:
    Serial.println("Mouse izquierda");
      Mouse.move(-(10 * vel), 0, 0); //flecha izquierda
      break;
//    case 4:
//      modo++;
//      break;
   case 5:
   Serial.println("Mouse derecha");
      Mouse.move((10 * vel), 0, 0); //flecha derecha
      break;
    case 6:
      Mouse.click(MOUSE_LEFT);
      break;
    case 7:
    Serial.println("Mouse abajo");
      Mouse.move(0, -(10 * vel), 0); //flecha abajo
      break;
    case 8:
      Mouse.click(MOUSE_RIGHT); //click derecho
      break;
    default:
      break;
  }
}
