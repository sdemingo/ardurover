#ifndef MOTORS_H
#define MOTORS_H

const int CUTOFF_TIMEOUT = 500;

// Control de avance o de marcha
const int EN_MOTOR1 = 3;
const int MOTOR1_A = 2;
const int MOTOR1_B = 4;

// Control de dirección
const int EN_MOTOR2 = 7;
const int MOTOR2_A = 5;
const int MOTOR2_B =  6;

void init_motors();
void run_motor(int motor, int dir);
void stop_motor(int motor);

#endif
