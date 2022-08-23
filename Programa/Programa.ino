//---------------------------------------------------------------- Pines
// USB(0,1) PULSADOR_USUARIO(2) PULSADOR_VELOCIDAD(3) BUZZER(5) PULSADOR_CONTROL(6) LED_VEL(9) LED_CONTROL(10) BT(7,8)
// MATRIZ LEDS(A0, A1, A2, 16, 14, 15) multiplexados
//---------------------------------------------------------------- Declaraciones
//#include "Mouse.h"
//#include <Keyboard.h>
const int PULSADOR_USUARIO = 2,
          PULSADOR_VEL_MOUSE = 3,
          PULSADOR_VEL_DISPLAY = 6,
          VCC_LED_1 = A0,
          VCC_LED_2 = A1,
          VCC_LED_3 = A2,
          GND_LED_1 = 16,
          GND_LED_2 = 14,
          GND_LED_3 = 15;

const int MATRIZ_LED[2][3] = {{VCC_LED_1, VCC_LED_2, VCC_LED_3},
  {GND_LED_1, GND_LED_2, GND_LED_3}
};

bool arrayLedsEncendidos[3][3] = {{0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};


// Por como tratamos a la matriz, las filas se encuentras a GND y las columnas a VCC, pero se podría cambiar
int matrizFila = 0, matrizColumna = 0, ledEncendido = 0, Modo = 1;
bool userInput = 0;

//---------------------------------------------------------------- Setup
void setup() {
  Serial.begin(9600);
//  Keyboard.begin();
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
  attachInterrupt(digitalPinToInterrupt(PULSADOR_VEL_MOUSE), selectorVelMouse, RISING);
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
//  estado(); //ve cuál led está prendido
  //LEER SI CAMBIO VALOR DE MODO VERDADERO-->mouse_control();

  //Recepcion BT por RX
  //Función Mouse

}



//---------------------------------------------------------------- ISR
ISR(TIMER1_COMPA_vect) {
  MODO1();
//   switch (Modo) {
//      case 1:
//        MODO1();
//        break;
//        /*case 2:
//          MODO2();
//          break;
//          case 3:
//          MODO3();
//          break;*/
//    }
}

//Bandera temporal
void selectorGeneral() {
  if (!userInput) {
    TIMSK1 &= ~(1 << OCIE1A);  // Output compare Timer1 A Interrupt Disable
    userInput = 1;
    // Testeo
    estado();
  }
  else {
    TIMSK1 |= (1 << OCIE1A);  // Output compare Timer1 A Interrupt Enable
    userInput = 0;
  }
}

void selectorVelMouse() {

}

void selectorVelDisplay() {

}
//---------------------------------------------------------------- Funciones
int estado() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; i < 3; j++) {
      if (arrayLedsEncendidos[i][j])
        Serial.print("Led pulsado: ");
        Serial.println((i * 3 + j));
        return (i * 3 + j); 
    }
  }
  return -1;
}

//void mouse_control() {
// switch (estado()) {
//    case 0:
//      Keyboard.press(KEY_ESC); //tecla escape posible necesidad de pequeña espera
//      Keyboard.release(KEY_ESC);
//      break;
//    case 1:
//      Mouse.move(0, (10 * vel), 0); //flecha arriba    10=range varias experimentalmente
//      break;
//    case 2:
//      velocidad = velocidad * k;
//      k++;
//      if (k == 3)   //Usar flag para que varíe única vez
//        k = 1;        //velocidad
//      break;
//    case 3:
//      Mouse.move(-(10 * vel), 0, 0); //flecha izquierda
//      break;
//    case 4:
//      Modo++;
//      break;
//    case 5:
//      Mouse.move((10 * vel), 0, 0); //flecha derecha
//      break;
//    case 6:
//      Mouse.click(MOUSE_LEFT);
//      break;
//    case 7:
//      Mouse.move(0, -(10 * vel), 0); //flecha abajo
//      break;
//    case 8:
//      Mouse.click(MOUSE_RIGHT); //click derecho
//      break;
//    case default:
//      break;
//
//  }
//
//
//}

void MODO1() {                        //esto prende las lucecitas de una forma
  digitalWrite(MATRIZ_LED[1][matrizFila], HIGH);
  digitalWrite(MATRIZ_LED[0][matrizColumna], LOW);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 0;
  matrizColumna++;
  if (matrizColumna > 2) {
    matrizColumna = 0;
    matrizFila == 2 ? matrizFila = 0 : matrizFila++;
  }
  digitalWrite(MATRIZ_LED[1][matrizFila], LOW);
  digitalWrite(MATRIZ_LED[0][matrizColumna], HIGH);
  arrayLedsEncendidos[matrizFila][matrizColumna] = 1;
   // Test
  for (int i = 0; i < 3; i++) {
    for (int j = 0; i < 3; j++) {
        Serial.print(arrayLedsEncendidos[i][j]);
    }
  }
  Serial.println();
}
