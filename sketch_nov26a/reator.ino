#define LED_PIN 2  // Substitua pelo pino correto do seu ESP32-C3 Mini

// Variáveis para controle do brilho
int brightness = 160;   // Brilho inicial (mínimo ajustado para 100)
int fadeAmount = 1;     // Incremento/decremento do brilho
const int minBrightness = 160; // Limite mínimo do brilho
const int maxBrightness = 255; // Limite máximo do brilho

void setup() {
  pinMode(LED_PIN, OUTPUT); // Configura o pino como saída
}

void loop() {
  // Calcula o tempo para o LED ficar ligado
  int onTime = map(brightness, minBrightness, maxBrightness, 100, 10000);
  // Calcula o tempo para o LED ficar desligado
  int offTime = 10000 - onTime;

  // Liga o LED por onTime microsegundos
  digitalWrite(LED_PIN, HIGH);
  delayMicroseconds(onTime);

  // Desliga o LED por offTime microsegundos
  digitalWrite(LED_PIN, LOW);
  delayMicroseconds(offTime);

  // Ajusta o brilho para a próxima iteração
  brightness += fadeAmount;

  // Inverte a direção do ajuste ao atingir os limites
  if (brightness <= minBrightness || brightness >= maxBrightness) {
    fadeAmount = -fadeAmount;
  }

  // Aguarda um pouco para suavizar a transição
  delay(5); // Ajuste para controlar a velocidade da oscilação
}
