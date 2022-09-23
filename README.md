# Mouse de Barrido 

El Dispositivo cuenta con 9 leds que se encenderan en 3 modos distintos.

![Referencias matriz led](https://github.com/alejomancinelli/MouseDeBarrido/blob/Alejo_branch/Imagenes/matrizLed.jpg)

- **Modo 1**: Mouse de barrido de una capa
    
    Este Modo enciende Leds en la secuencia 7,8,9,4,6,1,2,3.
 
- **Modo 2**: Mouse de barrido de 2 capas
  - Capa 1: Este Modo enciende Leds en la secuencia 8,4,6,8,5. Al activar el 5 pasa a la otra capa. El resto son las flechas.

  - Capa 2: Este Modo enciende Leds en la secuencia 7,9,1,3,5
  
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

## Pulsadores 

- User: pulsador general con el cual el usuario pausa la secuencia de la matriz, seleccionando una acción del mouse o eligiendo un estado en el modo pictograma.
- Modos: permite cambiar el modo con el cual se desea trabajar.
- Velocidad luces: cambia la frecuencia con la que se prenden las luces de la matriz.
- Velocidad mouse: ajusta la velocidad de movimiento del mouse modificando la distancia de movimiento.


## Luces e indicadores

Se cuenta con 2 leds para indicar los distintos modos. Al estar ambos apagados, el dispositivo no se encuentra en funcionamiento. 

  - Led modo mouse: al brillar al 50% indica el modo 1, al brillar al 100% indica el modo 2
  - Led modo picto: indica el modo 3

Para indicar los cambios de velocidad, tanto en las luces como en el mouse, se cuenta con un buzzer que emitirá una cantidad de sonidos proporcional a la velocidad determinada. Cada pulsador cuenta con un tono particular.


## Conexiones y Pin Out

        Pin | Port map  | Características   | Función                   | Descripción
        --- | --------- | ----------------- | ------------------------- | --------------------------
        0	| PD3       | Rx, INT3          | USB                       | 
        1	| PD2       | Tx, INT2          | USB                       | 
        2	| PD1       | INT1              | Pulsador user             | Pulsador grande selección
        3	| PD0       | INT0              | Pulsador modos            | Pulsador cambio de modos
        4	| PD4       |                   | Led modo picto            | Indicación modo PICTO
        5	| PC6       | PWM               | Led modo mouse            | Indicación modo MOUSE
        6	| PD7       |                   | Pulsador Velocidad Luces  | 
        7	| PE6       | INT6              | Pulsador Velocidad Mouse  | 
        8	| PB4       |                   | Bluetooth                 | Rx BT / Tx Arduino
        9	| PB5       | PWM               | Bluetooth                 | Tx BT / Rx Arduino
        10	| PB6       | PWM               | Buzzer                    |  
        16	| PB2       | LEDMATRIX_ROW_1   |                           | 
        14	| PB3       | LEDMATRIX_ROW_2   |                           | 
        15	| PB1       | LEDMATRIX_ROW_3   |                           | 
        A0	| PF7       | LEDMATRIX_COL_1   |                           | 
        A1	| PF6       | LEDMATRIX_COL_2   |                           | 
        A2	| PF5       | LEDMATRIX_COL_3   |                           | 
        A3	| PF4       |                   |                           | 

