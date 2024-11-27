#define LED_PIN 2  // Substitua pelo pino correto do seu ESP32-C3 Mini

// Variáveis para controle do brilho
int brightness = 0;      // Brilho inicial
int fadeAmount = 5;      // Incremento/decremento do brilho

void setup() {
  pinMode(LED_PIN, OUTPUT); // Configura o pino como saída
}

void loop() {
  // Calcula o tempo para o LED ficar ligado
  int onTime = map(brightness, 35, 255, 100, 10000); // Ajuste o máximo se necessário
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
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  // Aguarda um pouco para suavizar a transição
  delay(5); // Ajuste para controlar a velocidade da oscilação
}
