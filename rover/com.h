/*
   Libreria de pruebas para enviar datos con NRF24L01

   Sergio de Mingo
   Enero 2020
*/

#ifndef COM_H
#define COM_H

#define CONTROL_SZ 2

// Rover -> Base messages
struct state
{
  byte batery; // V. Bateria x 10
  byte mode;   // maniobra o modo actual
};
typedef struct state state;

void zero_state(state *st);
bool send_state(state *s);
bool receive_state(state *s);

// Base -> Rover messages
struct control
{
  byte steering;
  byte course;
};
typedef struct control control;

bool send_control(control *c);
bool receive_control(control *c);

// Rover commands
const byte STOP = 1;
const byte FORWARD = 2;
const byte BACKWARD = 3;

const byte STRAIGHT = 4;
const byte TURN_RIGHT = 5;
const byte TURN_LEFT = 6;

void init_rover_radio();
void init_base_radio();
void reset_stats();

#endif
