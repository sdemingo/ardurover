
#include "Arduino.h"
#include "motors.h"

void init_motors()
{
    pinMode(EN_MOTOR1, OUTPUT);
    pinMode(MOTOR1_A, OUTPUT);
    pinMode(MOTOR1_B, OUTPUT);

    pinMode(EN_MOTOR2, OUTPUT);
    pinMode(MOTOR2_A, OUTPUT);
    pinMode(MOTOR2_B, OUTPUT);
}

void run_motor(int motor, int dir)
{
    digitalWrite(motor, HIGH);
    if (motor == EN_MOTOR1){
        if (dir)
        {
            digitalWrite(MOTOR1_A, HIGH);
            digitalWrite(MOTOR1_B, LOW);
        }
        else
        {
            digitalWrite(MOTOR1_A, LOW);
            digitalWrite(MOTOR1_B, HIGH);
        }
    }

    if (motor == EN_MOTOR2){
        if (dir)
        {
            digitalWrite(MOTOR2_A, HIGH);
            digitalWrite(MOTOR2_B, LOW);
        }
        else
        {
            digitalWrite(MOTOR2_A, LOW);
            digitalWrite(MOTOR2_B, HIGH);
        }
    }
}

void stop_motor(int motor)
{
    digitalWrite(motor, LOW);

    if (motor == EN_MOTOR1){
        digitalWrite(MOTOR1_A, LOW);
        digitalWrite(MOTOR1_B, LOW);
    }

    if (motor == EN_MOTOR2){
        digitalWrite(MOTOR2_A, LOW);
        digitalWrite(MOTOR2_B, LOW);
    }

}