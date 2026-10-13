/*
Anotações

---  Parte 2

Aqui temos três instruções diferentes:
#define PINO_DHT A2
Define que o pino de dados do DHT11 será o A2. Nesse caso, ele está sendo usado como pino digital, não para leitura analógica.

#define TIPO_DHT DHT11
Informa à biblioteca qual modelo de sensor estamos utilizando. Isso importa porque existem outros sensores da família DHT, como o DHT22.

DHT dht(PINO_DHT, TIPO_DHT);
Cria o objeto dht, por meio do qual o programa poderá solicitar as medições.


--- Parte 5

------- segurancaAtiva
É uma variável booleana, ou seja, pode assumir apenas dois valores:
- false: segurança desativada.
- true: segurança ativada.

Ela permite que o Arduino saiba se deve ou não reagir aos movimentos detectados pelos sensores PIR.

------- estadoAnteriorBotao

Guarda o estado anterior do botão. O programa compara esse valor com a leitura atual para identificar uma nova pressão, em vez de alternar a segurança continuamente enquanto o botão estiver pressionado.
O valor inicial LOW pressupõe a ligação do botão com resistor de pull-down externo.

------- ultimoAcionamento

unsigned long ultimoAcionamento = 0;


Armazena o instante, em milissegundos, da última alteração aceita do botão.
O tipo unsigned long é adequado para armazenar os valores retornados por millis(), que conta o tempo desde que o Arduino começou a executar o programa.

------- intervaloDebounce

const unsigned long intervaloDebounce = 200;


Define um intervalo de 200 milissegundos para ajudar a evitar que os pequenos contatos elétricos do botão sejam interpretados como várias pressões.

------- TEMPERATURA_LIMITE
const float TEMPERATURA_LIMITE = 30.0;


Define a temperatura que será usada como limite para a ventilação. Se a leitura válida ultrapassar 30 °C, o programa solicitará o acionamento do relé.
O uso de uma constante permite alterar o limite em um único lugar, sem procurar o número 30 em várias partes do código.


*/




// Bibliotecas
#include <LiquidCrystal.h> //LCD
#include <DHT.h> // Semsor de temperatura

// 1. CONFIGURACAO DO LCD
// LiquidCrystal(RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

// 2. CONFIGURACAO DO SENSOR DHT11
#define PINO_DHT  A2
#define TIPO_DHT  DHT11

DHT dht(PINO_DHT, TIPO_DHT);

// 3. PINOS DOS COMPONENTES (Aqui seria de forma organizada onde e o que está conectado nos pinos 'PWM' e 'A')

// Sensores
const int PINO_LDR = A5;
const int PIR_ENTRADA = A0;
const int PIR_VARANDA = A3;

// Iluminacao dos ambientes
const int LED_PORTA = 2;
const int LED_ANDAR_1 = A4;

// Indicadores
const int LED_VERDE = 4;
const int LED_AMARELO = 5;
const int LED_VERMELHO = 13;

// Seguranca e ventilacao
const int BOTAO_SEGURANCA = 3;
const int PINO_BUZZER = A1;
const int RELE_VENTILADOR = 12;

// 4. VARIAVEIS DE ESTADO

bool segurancaAtiva = false;

bool estadoAnteriorBotao = LOW;
unsigned long ultimoAcionamento = 0;

const unsigned long intervaloDebounce = 200;

const float TEMPERATURA_LIMITE = 30.0;

// Controle do LCD
unsigned long ultimaAtualizacaoLCD = 0;
const unsigned long intervaloLCD = 1000;

bool mostrarUmidade = false;

// 5. CONFIGURACAO INICIAL

void setup()
{
  Serial.begin(9600);

  lcd.begin(16, 2);
  dht.begin();

  pinMode(LED_PORTA, OUTPUT);
  pinMode(LED_ANDAR_1, OUTPUT);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  pinMode(RELE_VENTILADOR, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  // Botao com resistor externo de pull-down
  pinMode(BOTAO_SEGURANCA, INPUT);

  pinMode(PIR_ENTRADA, INPUT);
  pinMode(PIR_VARANDA, INPUT);

  // Estado inicial
  digitalWrite(LED_PORTA, LOW);
  digitalWrite(LED_ANDAR_1, LOW);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  // Confirme se o seu modulo funciona dessa forma.
  digitalWrite(RELE_VENTILADOR, LOW);

  noTone(PINO_BUZZER);

  // Aqui é só o fluflu
  lcd.setCursor(0, 0);
  lcd.print("Casa Inteligente");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(1500);
  lcd.clear();
}

// 6. LOOP PRINCIPAL

void loop()
{
  // A. ILUMINACAO AUTOMATICA

  int nivelLuz = analogRead(PINO_LDR);

  if (nivelLuz > 500)
  {
    digitalWrite(LED_PORTA, HIGH);
    digitalWrite(LED_ANDAR_1, HIGH);
  }
  else
  {
    digitalWrite(LED_PORTA, LOW);
    digitalWrite(LED_ANDAR_1, LOW);
  }

  // B. LEITURA DE TEMPERATURA E UMIDADE
/*

  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  bool leituraValida =
    !isnan(temperatura) && !isnan(umidade); // isnan(): verifica se um valor não é um número válido.

*/



  // C. VENTILACAO AUTOMATICA
/*

  if (leituraValida && temperatura > TEMPERATURA_LIMITE)
  {
    digitalWrite(RELE_VENTILADOR, HIGH);
    digitalWrite(LED_AMARELO, HIGH);
  }
  else
  {
    digitalWrite(RELE_VENTILADOR, LOW);
    digitalWrite(LED_AMARELO, LOW);
  }

*/


  // D. BOTAO DE ATIVACAO DA SEGURANCA
/*

  bool estadoAtualBotao = digitalRead(BOTAO_SEGURANCA);

  if (estadoAtualBotao == HIGH &&
      estadoAnteriorBotao == LOW &&
      millis() - ultimoAcionamento >= intervaloDebounce)
  {
    segurancaAtiva = !segurancaAtiva;
    ultimoAcionamento = millis();
  }

  estadoAnteriorBotao = estadoAtualBotao;

*/


  // E. SISTEMA DE SEGURANCA

/*

bool movimento = false;

  if (segurancaAtiva)
  {
    movimento =
      digitalRead(PIR_ENTRADA) == HIGH ||
      digitalRead(PIR_VARANDA) == HIGH;

    digitalWrite(LED_VERDE, HIGH);

    if (movimento)
    {
      digitalWrite(LED_VERMELHO, HIGH);
      tone(PINO_BUZZER, 1000);
    }
    else
    {
      digitalWrite(LED_VERMELHO, LOW);
      noTone(PINO_BUZZER);
    }
  }
  else
  {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    noTone(PINO_BUZZER);
  }

*/
  

  // F. ATUALIZACAO DO LCD

/*

if (millis() - ultimaAtualizacaoLCD >= intervaloLCD)
  {
    ultimaAtualizacaoLCD = millis();

    lcd.clear();

    if (segurancaAtiva && movimento)
    {
      lcd.setCursor(0, 0);
      lcd.print("! MOVIMENTO !");

      lcd.setCursor(0, 1);
      lcd.print("Alarme ativado");
    }
    else if (!leituraValida)
    {
      lcd.setCursor(0, 0);
      lcd.print("Erro no DHT11");

      lcd.setCursor(0, 1);
      lcd.print("Verifique sensor");
    }
    else
    {
      lcd.setCursor(0, 0);

      if (segurancaAtiva)
      {
        lcd.print("Seguranca: ON");
      }
      else
      {
        lcd.print("Seguranca: OFF");
      }

      lcd.setCursor(0, 1);

      if (mostrarUmidade)
      {
        lcd.print("Umidade: ");
        lcd.print(umidade, 0);
        lcd.print("%");
      }
      else
      {
        lcd.print("Temp: ");
        lcd.print(temperatura, 1);
        lcd.print((char)223);
        lcd.print("C");
      }

      // Alterna entre temperatura e umidade
      mostrarUmidade = !mostrarUmidade;
    }
  }

*/
  

  // G. MONITOR SERIAL PARA DIAGNOSTICO

/*

Serial.print("Luz: ");
  Serial.print(nivelLuz);

  Serial.print(" | Temperatura: ");

  if (leituraValida)
  {
    Serial.print(temperatura);
    Serial.print(" C | Umidade: ");
    Serial.print(umidade);
    Serial.print("%");
  }
  else
  {
    Serial.print("Falha na leitura do DHT11");
  }

  Serial.print(" | Seguranca: ");
  Serial.println(segurancaAtiva ? "ON" : "OFF");



*/
  
    delay(100);
}
