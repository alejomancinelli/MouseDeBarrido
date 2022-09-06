# Mouse de Barrido 

El Dispositivo cuenta con 9 leds que se encenderan en 3 modos distintos.

- **Modo 1**: Mouse de barrido de una capa
    
    Este Modo enciende Leds en la secuencia 7,8,9,4,6,1,2,3, (Posición en Referencia a Teclado Numerico de PC) 
 
- **Modo 2**: Mouse de barrido de 2 capas
  - Capa 1: Este Modo enciende Leds en la secuencia 8,4,6,8,5 (Posición en Referencia a Teclado Numerico de PC) 
                  Activar el 5 pasa a la otra capa. El resto son las flechas.

  - Capa 2: Este Modo enciende Leds en la secuencia 7,9,1,3,5 (Posición en Referencia a Teclado Numerico de PC)
  
En Ambos modos se tienen las siguientes funciones:

    7 - Caracter de Escape
    8 - Mouse arriba
    9 - Arrastre
    4 - Mouse izquierda
    5 -
    6 - Mouse derecha
    1 - Click izquierdo
    2 - Mouse abajo
    3 - Click derecho

- **Modo 3**: Selección de Pictogramas

    Enciende Columna 1 luego 2 luego 3. En caso de selección enciende led correspondiente a esa columana en las filas 3 luego 2 luego 1.


## Conexiones y Pin Out:

        Pin | Port map  | Características   | Función                   | Descripción
        --- | --------- | ----------------- | ------------------------- | --------------------------
        0	| PD3       | Rx, INT3          | USB                       | 
        1	| PD2       | Tx, INT2          | USB                       | 
        2	| PD1       | INT1              | Pulsador user             | Pulsador grande selección
        3	| PD0       | INT0              | Pulsador modos            | Pulsador cambio de modos
        4	| PD4       |                   | LED MODO 1                | Indicación modo MOUSE
        5	| PC6       | PWM               | LED MODO 2                | Indicación modo PICTO
        6	| PD7       |                   | Pulsador Velocidad Luces  | 
        7	| PE6       | Rx, INT6          | Bluetooth                 | 
        8	| PB4       | Tx                | Bluetooth                 | 
        9	| PB5       | PWM               | Buzzer                    | 
        10	| PB6       | PWM               | Pulsador Velocidad Mouse  |  
        16	| PB2       | LEDMATRIX_ROW_1   |                           | 
        14	| PB3       | LEDMATRIX_ROW_2   |                           | 
        15	| PB1       | LEDMATRIX_ROW_3   |                           | 
        A0	| PF7       | LEDMATRIX_COL_1   |                           | 
        A1	| PF6       | LEDMATRIX_COL_2   |                           | 
        A2	| PF5       | LEDMATRIX_COL_3   |                           | 
        A3	| PF4       |                   |                           | 

