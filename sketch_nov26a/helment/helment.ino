#include <ESP32Servo.h>

// ===============================
// Pinos do botão, servos e LEDs
// ===============================
const int servo1Pin = 5;
const int servo2Pin = 6;
const int buttonPin = 7;
const int ledPin = 4;     // LED Olhos
const int ledUpPin1 = 1;  // LED adicional 1
const int ledUpPin2 = 20; // LED adicional 2

Servo servo1;
Servo servo2;

// ===============================
// Variáveis de estado
// ===============================
bool isUp = false;               // Estado atual dos servos
bool lastButtonState = LOW;      // Último estado do botão
bool moving = false;             // Indicador de movimento em andamento
bool ledDelayed = false;         // Indicador de atraso do LED

// ===============================
// Controle de movimento
// ===============================
const int movementDuration = 300;  // Tempo total de movimento (ms)
int startPos1, endPos1;            // Posições inicial e final do Servo 1
int startPos2, endPos2;            // Posições inicial e final do Servo 2
unsigned long movementStartTime;   // Momento do início do movimento
unsigned long ledDelayStartTime;   // Momento de início do atraso do LED

// ===============================
// Setup
// ===============================
void setup() {
  pinMode(buttonPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  pinMode(ledUpPin1, OUTPUT);
  pinMode(ledUpPin2, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(ledUpPin1, HIGH);
  digitalWrite(ledUpPin2, HIGH);

  // Inicializa os servos na posição inicial e depois desliga
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(25);
  servo2.write(165);
  delay(300);
  servo1.detach();
  servo2.detach();

  Serial.begin(115200);
}

// ===============================
// Loop principal
// ===============================
void loop() {
  bool currentButtonState = digitalRead(buttonPin);

  // Detecta toque no botão
  if (currentButtonState == HIGH && lastButtonState == LOW && !moving) {
    isUp = !isUp; // alterna estado

    // Reanexa os servos antes de iniciar o movimento
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    startPos1 = servo1.read();
    startPos2 = servo2.read();

    if (isUp) {
      endPos1 = 175; // quanto maior o número, mais alto o servo (esquerdo)
      endPos2 = 5;   // quanto menor o número, mais alto o servo (direito)
      Serial.println("Aberto");
      digitalWrite(ledPin, HIGH);
      digitalWrite(ledUpPin1, HIGH);
      digitalWrite(ledUpPin2, HIGH);
      ledDelayed = false;
    } else {
      endPos1 = 12;
      endPos2 = 165;
      Serial.println("Fechado");
      digitalWrite(ledUpPin1, HIGH);
      digitalWrite(ledUpPin2, HIGH);
      ledDelayStartTime = 0;
    }

    movementStartTime = millis();
    moving = true;
  }

  lastButtonState = currentButtonState;

  // Atualiza os servos se estiver em movimento
  if (moving) updateServoPositions();

  // Gerencia atraso do LED após fechar
  if (!isUp && !moving && !ledDelayed) {
    if (ledDelayStartTime == 0) {
      ledDelayStartTime = millis();
    } else if (millis() - ledDelayStartTime >= 100) {
      ledPulsingEffect();
      digitalWrite(ledPin, LOW);
      ledDelayed = true;
    }
  }
}

// ===============================
// Função para atualizar servos
// ===============================
void updateServoPositions() {
  unsigned long elapsedTime = millis() - movementStartTime;

  if (elapsedTime >= movementDuration) {
    // Movimento concluído
    servo1.write(endPos1);
    servo2.write(endPos2);

    // Pequeno tempo para estabilizar e depois desligar
    delay(100);
    servo1.detach();
    servo2.detach();

    moving = false;
    return;
  }

  // Calcula a posição proporcional ao tempo
  float progress = (float)elapsedTime / movementDuration;
  int newPos1 = startPos1 + (endPos1 - startPos1) * progress;
  int newPos2 = startPos2 + (endPos2 - startPos2) * progress;

  servo1.write(newPos1);
  servo2.write(newPos2);
}

// ===============================
// Efeito de pulsar no LED
// ===============================
void ledPulsingEffect() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);
    delay(80);
    digitalWrite(ledPin, LOW);
    delay(110);
  }
  digitalWrite(ledPin, HIGH);
}
