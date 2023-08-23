/*
   Libreria de pruebas para enviar telemetría con NRF24L01

   Sergio de Mingo
   Febrero 2019
*/

#include <nRF24L01.h>
#include <RF24.h>
#include "com.h"

#define BASE_SIDE 0
#define ROVER_SIDE 1

#define pinCE 8
#define pinCSN 10

RF24 radio(pinCE, pinCSN);

const uint64_t pipes[2] = {
    0xE8E8F0F0E1LL, // rover-base pipe / state pipe
    0xF0F0F0F0D2LL  // base-rover pipe / control pipe
};
char buf[CONTROL_SZ];
byte side;

int send_errors;
int send_tries;

void init_rover_radio()
{
  radio.begin();
  radio.setRetries(15, 15);

  // open rover-base pipe (status pipe)
  radio.openWritingPipe(pipes[0]);

  //open base-rover pipe (ctl pipe)
  radio.openReadingPipe(1, pipes[1]);

  radio.startListening();

  side = ROVER_SIDE;
  reset_stats();
}

void init_base_radio()
{
  radio.begin();
  radio.setRetries(15, 15);

  // open rover-base pipe (status pipe)
  radio.openReadingPipe(1, pipes[0]);

  //open base-rover pipe (ctl pipe)
  radio.openWritingPipe(pipes[1]);

  radio.startListening();

  side = BASE_SIDE;
  reset_stats();
}

void reset_stats()
{
  send_errors = 0;
  send_tries = 0;
}

int send_stats()
{
  return 99 - ((send_errors * 100) / send_tries);
}

//
// Packing and unpacking
//

int packInt8(char *buf, int num, int off)
{
  buf[off++] = num & 0xFF;
  return off;
}

int packInt16(char *buf, int num, int off)
{
  buf[off++] = (num >> 8) & 0xFF;
  buf[off++] = (num)&0xFF;
  return off;
}

int unpackInt8(char *buf, char *n, int off)
{
  char a = buf[off];
  if ((a & (1 << 7)))
  {
    a = a - (1 << 8);
  }
  *n = a;
  return off + 1;
}

int unpackInt16(char *buf, int *n, int off)
{
  int a = (buf[off] << 8) + (buf[off + 1] & 0xFF);
  if ((a & (1 << 15)))
  {
    a = a - (1 << 16);
  }
  *n = a;
  return off + 2;
}

//
// State messages
//

int packState(char *buf, state *st)
{
  int off = 0;
  off = packInt8(buf, st->batery, off);
  off = packInt8(buf, st->mode, off);
  return off;
}

int unpackState(char *buf, state *st)
{
  int off = 0;
  off = unpackInt8(buf, &st->batery, off);
  off = unpackInt8(buf, &st->mode, off);
  return off;
}

void zero_state(state *st)
{
  st->batery = 0;
  st->mode = 0;
}

bool send_state(state *s)
{
  radio.stopListening();

  if (send_tries >= 100)
  {
    reset_stats();
  }
  memset(buf, 0, sizeof(buf));
  packState(buf, s);
  bool err = radio.write(buf, sizeof(buf));
  if (!err)
  {
    send_errors++;
  }
  send_tries++;

  radio.startListening();
  return err;
}

bool receive_state(state *s)
{

  if (radio.available())
  {
    radio.read(&buf, CONTROL_SZ);
    unpackState(buf, s);
    return true;
  }
  return false;
}





//
// Control messages
//

int pack_control(char *buf, control *c)
{
  int off = 0;
  off = packInt8(buf, c->steering, off);
  off = packInt8(buf, c->course, off);
  return off;
}

int unpack_control(char *buf, control *c)
{
  int off = 0;
  off = unpackInt8(buf, &c->steering, off);
  off = unpackInt8(buf, &c->course, off);
  return off;
}

bool send_control(control *c)
{
  radio.stopListening();

  memset(buf, 0, sizeof(buf));
  pack_control(buf, c);
  bool r = radio.write(buf, sizeof(buf));

  radio.startListening();
  return r;
}

bool receive_control(control *c)
{
  if (radio.available())
  {
    radio.read(&buf, CONTROL_SZ);
    unpack_control(buf, c);
    return true;
  }
  return false;
}
