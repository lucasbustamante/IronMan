#include <ESP32Servo.h>

// Pinos do botão, servos e LED
const int servo1Pin = 5;
const int servo2Pin = 6;
const int buttonPin = 7;
const int ledPin = 4;

Servo servo1;
Servo servo2;

bool isUp = false;               // Estado atual dos servos
bool lastButtonState = LOW;      // Último estado do botão

// Parâmetros para controle do movimento
const int movementDuration = 300; 
int startPos1, endPos1;            // Posições inicial e final do Servo 1
int startPos2, endPos2;            // Posições inicial e final do Servo 2
unsigned long movementStartTime;   // Momento do início do movimento
bool moving = false;               // Indicador de movimento em andamento

void setup() {
  pinMode(buttonPin, INPUT_PULLDOWN); // Configura o botão como entrada com PULLDOWN
  pinMode(ledPin, OUTPUT);           // Configura o LED como saída
  digitalWrite(ledPin, LOW);         // Inicializa o LED como desligado

  servo1.attach(servo1Pin);          // Anexa o servo 1
  servo2.attach(servo2Pin);          // Anexa o servo 2

  servo1.write(25);                  // Posição inicial do Servo 1
  servo2.write(165);                 // Posição inicial do Servo 2

  Serial.begin(115200);              // Inicia o monitor serial
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin); // Lê o estado do botão

  if (currentButtonState == HIGH && lastButtonState == LOW && !moving) {
    // Alterna o estado dos servos e define as posições de destino
    isUp = !isUp;
    startPos1 = servo1.read();
    startPos2 = servo2.read();

    if (isUp) {
      endPos1 = 180;          // quanto maior o número, mais alto o servo
      endPos2 = 0;            // quanto menor o número, mais alto o servo
      Serial.println("Subindo");
      digitalWrite(ledPin, HIGH); // Desliga o LED quando os servos estão levantados
    } else {
      endPos1 = 25;           // quanto menor o número, mais baixo o servo
      endPos2 = 165;          // quanto maior o número, mais baixo o servo
      Serial.println("Descendo");
      digitalWrite(ledPin, LOW); // Liga o LED quando os servos estão abaixados
    }

    movementStartTime = millis();
    moving = true; // Inicia o movimento
  }

  lastButtonState = currentButtonState; // Atualiza o estado do botão

  // Atualiza os servos enquanto o movimento está em andamento
  if (moving) {
    updateServoPositions();
  }
}

// Função para atualizar as posições dos servos de forma sincronizada
void updateServoPositions() {
  unsigned long elapsedTime = millis() - movementStartTime;

  if (elapsedTime >= movementDuration) {
    // Movimento concluído
    servo1.write(endPos1);
    servo2.write(endPos2);
    moving = false;
    return;
  }

  // Calcula a nova posição com base no tempo decorrido
  float progress = (float)elapsedTime / movementDuration;
  int newPos1 = startPos1 + (endPos1 - startPos1) * progress;
  int newPos2 = startPos2 + (endPos2 - startPos2) * progress;

  servo1.write(newPos1);
  servo2.write(newPos2);
}
