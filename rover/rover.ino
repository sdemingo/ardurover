#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#include "com.h"
#include "motors.h"

state stmsg;
control ctlmsg;
bool check;
int last_cmd_stamp;

/*
  Rover  
 */

void setup() {
  Serial.begin(9600);
  delay(1000);

  init_motors();
  init_rover_radio();

  last_cmd_stamp = millis();
  ctlmsg.steering = STOP;
}

void loop() {

  check = receive_control(&ctlmsg);
  if (!check) {

    // Compruebo si el tiempo entre mensajes sobrepasa el timeout de corte
    if (millis() - last_cmd_stamp > CUTOFF_TIMEOUT) {
      ctlmsg.steering = STOP;  // comando de parada
    }
  } else {
    last_cmd_stamp = millis();
    Serial.println("Recibo mensaje");
    Serial.println(ctlmsg.steering);
  }

  // Proceso el comando
  if (ctlmsg.steering == FORWARD) {
    run_motor(EN_MOTOR1, 1);
    run_motor(EN_MOTOR2, 1);
  }

  if (ctlmsg.steering == BACKWARD) {
    run_motor(EN_MOTOR1, 0);
    run_motor(EN_MOTOR2, 0);
  }

  if (ctlmsg.steering == STOP) {
    stop_motor(EN_MOTOR1);
    stop_motor(EN_MOTOR2);
  }

  if (ctlmsg.steering == TURN_RIGHT) {
    run_motor(EN_MOTOR1, 1);
  }

  if (ctlmsg.steering == TURN_LEFT) {
    run_motor(EN_MOTOR2, 1);
  }
}
