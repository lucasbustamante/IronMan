#include <ESP32Servo.h>

// Pinos do botão e servos
const int servo1Pin = 27;
const int servo2Pin = 32;
const int buttonPin = 25;

Servo servo1;
Servo servo2;

bool isUp = false;               // Estado atual dos servos
bool lastButtonState = LOW;      // Último estado do botão

void setup() {
  pinMode(buttonPin, INPUT_PULLDOWN); // Configura o botão como entrada com PULLDOWN
  servo1.attach(servo1Pin);          // Anexa o servo 1
  servo2.attach(servo2Pin);          // Anexa o servo 2

  servo1.write(0);                   // Servo 1 na posição inicial
  servo2.write(180);                 // Servo 2 na posição inicial oposta

  Serial.begin(115200);              // Inicia o monitor serial
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin); // Lê o estado do botão

  if (currentButtonState == HIGH && lastButtonState == LOW) {
    // Alterna o estado dos servos
    isUp = !isUp;

    if (isUp) {
      moveServos(0,120); // Move os servos para cima (Servo1: 0->90, Servo2: 180->90)
      Serial.println("Subindo");
    } else {
      moveServos(120, 0); // Move os servos para baixo (Servo1: 90->0, Servo2: 90->180)
      Serial.println("Descendo");
    }

    delay(1000); // Pequeno atraso para evitar múltiplos acionamentos
  }

  lastButtonState = currentButtonState; // Atualiza o estado do botão
}

// Função para mover os servos de forma sincronizada
void moveServos(int startPos, int endPos) {
  int step = (startPos < endPos) ? 1 : -1; // Define a direção do movimento

  for (int pos = startPos; pos != endPos + step; pos += step) {
    servo1.write(pos);            // Define a posição do Servo 1
    servo2.write(180 - pos);      // Define a posição espelhada do Servo 2
    delay(5);                     // Ajusta a velocidade do movimento
  }
}
