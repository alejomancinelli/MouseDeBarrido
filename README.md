# Mouse de Barrido 

Este dispositivo permite controlar un mouse de computadora utilizando solamente un pulsador. Mediante una matriz led 3x3 se recorren las distintas funciones del mouse (movientos, click izquierdo, click derecho) asociadas a las distintas celdas. Al presionar el pulsador, se seleccionará la función indicada por la matriz, deteniendo la secuencia. Luego, al volver a presionar el pulsador, se deseleccionará la función, continuando con la secuencia de la matriz. 

Además, se contará con un modo pictograma, permitiendo al usuario expresar distintas necesidades. El film de imágenes se puede reemplazar según sea necesario.

![Referencias matriz led](https://github.com/alejomancinelli/MouseDeBarrido/blob/main/Imagenes/matrizLed.jpg)

## Modos

El Dispositivo cuenta con 9 leds que se encenderan en 3 modos distintos.

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

Mediante la utilización de un buzzer se proporcionada a cada celda de la matriz tiene un sonido único, según el modo de funcionamiento.

## Pulsadores 

- User: pulsador general con el cual el usuario pausa la secuencia de la matriz, seleccionando una acción del mouse o eligiendo un estado en el modo pictograma.
- Modos: permite cambiar el modo con el cual se desea trabajar.
- Velocidad luces: cambia la frecuencia con la que se prenden las luces de la matriz.
- Velocidad mouse: ajusta la velocidad de movimiento del mouse modificando la distancia de movimiento.

Para indicar los cambios de velocidad, tanto en las luces como en el mouse, se cuenta con un buzzer que emitirá una cantidad de sonidos proporcional a la velocidad determinada. 

## Luces e indicadores

Se cuenta con 2 leds para indicar los distintos modos. Al estar ambos apagados, el dispositivo no se encuentra en funcionamiento. 

  - Led de modo 1: indica el modo mouse 1
  - Led de modo 2: indica el modo mouse 2
  - Led de modo 1 y 2: indica el modo 3 pictograma

## Conexiones y Pin Out

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

