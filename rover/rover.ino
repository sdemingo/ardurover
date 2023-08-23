#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#include "com.h"
#include "motors.h"

state stmsg;
control ctlmsg;

/*
  Rover  
 */

void setup()
{
  Serial.begin(115200);
  delay(1000);

  init_motors();

  // Iniciamos radio en modo escucha por ahora
  init_rover_radio();
}

void loop()
{
  //ctlmsg.steering = STOP;
  receive_control(&ctlmsg);
  
  /*
    Control de avance
  */
  if (ctlmsg.steering == FORWARD)
  {
    run_motor(EN_MOTOR1, 1);
    //run_motor(EN_MOTOR2, 1);
  }

  if (ctlmsg.steering == BACKWARD)
  {
    run_motor(EN_MOTOR1, 0);
    //run_motor(EN_MOTOR2, 0);
  }

  if (ctlmsg.steering == STOP)
  {
    stop_motor(EN_MOTOR1);
    stop_motor(EN_MOTOR2);
  }


  /*
    Control de dirección
  */

  if (ctlmsg.course == TURN_RIGHT)
  {
    run_motor(EN_MOTOR2, 0);
    
  }

  if (ctlmsg.course == TURN_LEFT)
  {
    run_motor(EN_MOTOR2, 1);
    
  }

  if (ctlmsg.course == STRAIGHT)
  {
    stop_motor(EN_MOTOR2);
  }


}
