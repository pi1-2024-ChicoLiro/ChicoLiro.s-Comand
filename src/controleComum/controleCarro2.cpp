/*
  Título: SISTEMA CONTROLADOR DO CARRINHO
  Autores: Daniel Rodrigues da Rocha, Leandro de Almeida
  Descrição: Código para controlar um carrinho seguidor de linha utilizando sensores infravermelhos e controle PWM.
  Data de criação: 19/06/2024
*/

/* MAPEAMENTO DE HARDWARE */
// Conexões do motor (Os pinos de Enable EN_A e EN_B devem ser pinos de PWM para podermos controlar a velocidade dos motores)

#include <Arduino.h>

// Motor A - CORRIGIR
#define IN1 12 // Os pinos com o prefixo "IN" são pinos da Ponte H que determinam as entradas nos terminais dos motores, e consequentemente,
#define IN2 14 // a direção de rotação deles
#define EN_A 13 // Os pinos com o prefixo "EN" são pinos da Ponte H que vão determinar quanto de tensão os motores irão receber, assim determi-
                // nando a velocidade com a qual eles vão girar. Esses pinos irão receber um sinal PWM, que é um sinal digital que emula um
                // sinal analógico

// Motor B - CORRIGIR
#define IN3 27
#define IN4 26
#define EN_B 25

// Conexões do infravermelho
#define pinSensorMeio 15
#define pinSensorEsq 2
#define pinSensorDir 4

// Variáveis de controle de velocidade
const int velocidadeBase = 50; // Velocidade base dos motores
const int velocidadeMaxima = 100; // Velocidade máxima dos motores
const int ajusteVelocidade = 30; // Ajuste de velocidade para correções
const int limitePreto = 100; // Valor limite para detectar a linha preta (ajuste conforme necessário)

void setup() {
  // Configuração dos pinos dos motores
  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_B, OUTPUT);

  // Configuração dos pinos dos sensores infravermelhos
  pinMode(pinSensorMeio, INPUT);
  pinMode(pinSensorEsq, INPUT);
  pinMode(pinSensorDir, INPUT);

  // Inicializa todos os pinos dos motores em nível lógico baixo
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

// Esses pinos abaixo determinam a velocidade na ponte H
  analogWrite(EN_A, 0); // Os pinos de saída analógicos do Arduino tem 8 bits de definição, logo variam entre 0 e 255 (2^8 = 256)
  analogWrite(EN_B, 0); // Mas no caso do motor, ela só pode variar de 0 até 100, pois se não, ele QUEIMA. Isso porque o que tá sendo controlado é a tensao.
                        // 255 significa 12v e o motor aguenta até 6v

  Serial.begin(9600);
}

void loop() {
  // Leitura dos sensores infravermelhos
  // bool sensorMeio = analogRead(pinSensorMeio);
  // bool sensorEsq = analogRead(pinSensorEsq);
  // bool sensorDir = analogRead(pinSensorDir);

 if (analogRead(pinSensorMeio) > limitePreto && analogRead(pinSensorEsq) <= limitePreto && analogRead(pinSensorDir) <= limitePreto) {
    // Linha reta - ambos os motores com velocidade base
    moverFrente(velocidadeBase, velocidadeBase);
  }
  else if (analogRead(pinSensorEsq) > limitePreto && analogRead(pinSensorDir) <= limitePreto) {
    // Virar à esquerda - reduzir velocidade do motor esquerdo
    moverFrente(velocidadeBase - ajusteVelocidade, velocidadeBase);
  }
  else if (analogRead(pinSensorDir) > limitePreto && analogRead(pinSensorEsq) <= limitePreto) {
    // Virar à direita - reduzir velocidade do motor direito
    moverFrente(velocidadeBase, velocidadeBase - ajusteVelocidade);
  }
  else if (analogRead(pinSensorMeio) > limitePreto && analogRead(pinSensorEsq) > limitePreto && analogRead(pinSensorDir) > limitePreto) {
    // Fim do percurso - parar o carrinho
    pararMotores();
  }
  else {
    // Não detectado - parar o carrinho
    pararMotores();
  }

  //Serial.println(analogRead(pinSensorMeio));
  //Serial.print(" ");
  //Serial.println(analogRead(pinSensorEsq)); // esse aqui pode dar um valor um pouco diferente deles por conta da sombra que s1 e s3 dão
  //Serial.print(" ");
  //Serial.println(analogRead(pinSensorDir));
  //Serial.println(" ");

  delay(100);
}
/*
A funcao abaixo faz o carrinho girar pra direita ou esquerda. Ela faz isso, girando uma roda pra frente, enquanto gira outra pra traz
A ponte H possui 6 pinos. Enable A, pinA1, pinA2, Enable B, pinB1, pinB2. Os enables controlam a velocidade
e os outros dois definem o sentido pra qual o motor vai girar. Por exemplo:

pinA1 = HIGH
pinA2 = LOW

Assim o motor vai pra FRENTE. Se inverter:

pinA1 = LOW
pinA2 = HIGH

o motor vai girar pra TRÁS. Se o dois estiverem iguais, o motor não funciona.
Nesse código, pinA1, pinA2, pinB1, pinB2, correspondem respectivamente,
aos IN1, IN2, IN3, IN4.

*/

// Função para mover o carrinho para frente com controle de velocidade
void moverFrente(int velocidadeA, int velocidadeB) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, velocidadeA);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_B, velocidadeB);
}

// Função para parar os motores
void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(EN_B, 0);
}