/*
   Emisora radiofrecuencia

   Sergio de Mingo
   Febrero 2019
*/

#include <LiquidCrystal_I2C.h>
#include "com.h"

state stmsg;
control ctlmsg;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD addr: 0x27

const int pinJoyY = A0;
const int pinJoyX = A1;
byte printed_ctl = 0;

// PROGRAMA PRINCIPAL

void setup(void) {
  Serial.begin(115200);
  delay(1000);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando");
  Serial.println("Emisora iniciada");

  /*
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(boton3, INPUT);
  */

  //zero_state(&st);
  //init_base_radio();
  delay(1000);

  ctlmsg.course = STOP;
  lcd.print("Parado          ");
}

void loop(void) {
  int Xvalue = 0;
  int Yvalue = 0;

  /*
  if (receive_state(&st))
  {
    // hacer algo con estado recibido
  }
  */

  Xvalue = analogRead(pinJoyX);
  delay(100);  //es necesaria una pequeña pausa entre lecturas analógicas
  Yvalue = analogRead(pinJoyY);

  // Reviso coordenada Y del joystick:
  if (Yvalue < 460) {
    ctlmsg.steering = BACKWARD;
  }
  if ((Yvalue >= 460) && (Yvalue <= 560)) {
    ctlmsg.steering = STOP;
  }
  if (Yvalue > 560) {
    ctlmsg.steering = FORWARD;
  }

  // Reviso coordenada X del joystick:
  if (Xvalue < 460) {
    ctlmsg.steering = TURN_LEFT;
  }

  if (Xvalue > 560) {
    ctlmsg.steering = TURN_RIGHT;
  }

  // Imprimo comando en pantalla solo si cambia
  if (printed_ctl != ctlmsg.steering) {
    lcd.setCursor(0, 0);
    if (ctlmsg.steering == FORWARD) {
      lcd.print("Avanzo          ");
    }
    if (ctlmsg.steering == STOP) {
      lcd.print("Parado          ");
    }
    if (ctlmsg.steering == BACKWARD) {
      lcd.print("Retrocedo       ");
    }
    if (ctlmsg.steering == TURN_RIGHT) {
      lcd.print("Giro Derecha    ");
    }
    if (ctlmsg.steering == TURN_LEFT) {
      lcd.print("Giro Izquierda  ");
    }

    printed_ctl == ctlmsg.steering;
  }

  // ---- ENVIAMOS CONTROL DEL ROVER ---

  //Serial.println(ctlmsg.steering);

  //send_control(&ctlmsg);
}
