#include <ESP32Servo.h>

// Pinos do botão, servos e LEDs
const int servo1Pin = 5;
const int servo2Pin = 6;
const int buttonPin = 7;
const int ledPin = 4;     // LED Olhos
const int ledUpPin1 = 1;  // LED adicional 1
const int ledUpPin2 = 20; // LED adicional 2

Servo servo1;
Servo servo2;

bool isUp = false;               // Estado atual dos servos
bool lastButtonState = LOW;      // Último estado do botão

// Parâmetros para controle do movimento
const int movementDuration = 300; 
int startPos1, endPos1;            // Posições inicial e final do Servo 1
int startPos2, endPos2;            // Posições inicial e final do Servo 2
unsigned long movementStartTime;   // Momento do início do movimento
unsigned long ledDelayStartTime;   // Momento de início do atraso do LED
bool moving = false;               // Indicador de movimento em andamento
bool ledDelayed = false;           // Indicador de atraso do LED

void setup() {
  pinMode(buttonPin, INPUT_PULLDOWN); // Configura o botão como entrada com PULLDOWN
  pinMode(ledPin, OUTPUT);           // Configura o LED principal como saída
  pinMode(ledUpPin1, OUTPUT);        // Configura LED adicional 1 como saída
  pinMode(ledUpPin2, OUTPUT);        // Configura LED adicional 2 como saída

  digitalWrite(ledPin, LOW);         // Inicializa o LED principal como desligado
  digitalWrite(ledUpPin1, HIGH);      // Inicializa o LED adicional 1 como desligado
  digitalWrite(ledUpPin2, HIGH);      // Inicializa o LED adicional 2 como desligado

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
      endPos1 = 175;          // quanto maior o número, mais alto o servo (esquerdo)
      endPos2 = 5;            // quanto menor o número, mais alto o servo (direito)
      Serial.println("Aberto");
      digitalWrite(ledPin, HIGH);    // Certifique-se de desligar o LED ao subir
      digitalWrite(ledUpPin1, HIGH); // Liga os LEDs adicionais leds desativado
      digitalWrite(ledUpPin2, HIGH); //leds desativado 
      ledDelayed = false;       // Reseta o atraso do LED
    } else {
      endPos1 = 12;           // quanto menor o número, mais baixo o servo
      endPos2 = 165;          // quanto maior o número, mais baixo o servo
      Serial.println("Fechado");
      digitalWrite(ledUpPin1, HIGH); // Desliga os LEDs adicionais
      digitalWrite(ledUpPin2, HIGH);
      ledDelayStartTime = 0;  // Reseta o início do atraso
    }

    movementStartTime = millis();
    moving = true; // Inicia o movimento
  }

  lastButtonState = currentButtonState; // Atualiza o estado do botão

  // Atualiza os servos enquanto o movimento está em andamento
  if (moving) {
    updateServoPositions();
  }

  // Gerencia o atraso do LED
  if (!isUp && !moving && !ledDelayed) {
    if (ledDelayStartTime == 0) {
      ledDelayStartTime = millis(); // Registra o momento de início do atraso
    } else if (millis() - ledDelayStartTime >= 100) { // Atraso de 1 segundo
      ledPulsingEffect();
      digitalWrite(ledPin, LOW); // Liga o LED após 1 segundo
      ledDelayed = true;         // Marca que o atraso foi processado
    }
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

// Função para criar o efeito de "pulsar" no LED
void ledPulsingEffect() {
  for (int i = 0; i < 5; i++) { // Pisca o LED 5 vezes
    digitalWrite(ledPin, HIGH);
    delay(80); // Liga por 100 ms
    digitalWrite(ledPin, LOW);
    delay(110); // Desliga por 100 ms
  }
  digitalWrite(ledPin, HIGH); // Liga o LED de forma constante após o efeito
}
