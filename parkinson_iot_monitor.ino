/*
 * Sistema IoT para Monitoramento de Parkinson
 * Conecta com Wi-Fi, MQTT e simula detecção de tremores
 * Autor: Tainá de Araujo do Nascimento - RA: 10424847
 * Data: 19/11/2025
 */

// ==================== BIBLIOTECAS ====================
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <MPU6050.h>

// ==================== CONFIGURAÇÕES WI-FI ====================
const char* ssid = "SUA_REDE_WIFI";
const char* password = "SENHA_WIFI";

// ==================== CONFIGURAÇÕES MQTT ====================
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic_tremor = "parkinson/paciente01/tremor";
const char* mqtt_topic_alerta = "parkinson/paciente01/alerta";

// ==================== OBJETOS ====================
WiFiClient espClient;
PubSubClient client(espClient);
MPU6050 mpu;

// ==================== PINOS ====================
const int buzzerPin = 23;

// ==================== VARIÁVEIS ====================
unsigned long lastMsg = 0;
bool tremorDetectado = false;
int intensidadeTremor = 0;

// ==================== FUNÇÃO: CONEXÃO WI-FI ====================
void setup_wifi() {
  Serial.print("Conectando na rede: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// ==================== FUNÇÃO: CALLBACK MQTT ====================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  
  Serial.print("Conteúdo: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// ==================== FUNÇÃO: RECONECTAR MQTT ====================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    
    // Gera um Client ID único
    String clientId = "ESP32-Parkinson-";
    clientId += String(random(0xffff), HEX);
    
    // Tenta conectar
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado ao broker MQTT!");
      // Se quiser assinar tópicos, faça aqui:
      // client.subscribe("parkinson/comandos");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

// ==================== FUNÇÃO: DETECTAR TREMOR (SIMULAÇÃO) ====================
void detectarTremor() {
  // Lê os dados do acelerômetro
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  
  // Calcula a intensidade do movimento
  intensidadeTremor = sqrt(ax*ax + ay*ay + az*az);
  
  // Simulação: detecta tremor se intensidade > limite
  if (intensidadeTremor > 15000) {
    if (!tremorDetectado) {
      tremorDetectado = true;
      Serial.println("TREMOR DETECTADO!");
      
      // Aciona o buzzer
      digitalWrite(buzzerPin, HIGH);
      
      // Publica alerta MQTT
      String mensagem = "{\"tremor\":true,\"intensidade\":" + String(intensidadeTremor) + ",\"timestamp\":\"" + String(millis()) + "\"}";
      client.publish(mqtt_topic_alerta, mensagem.c_str());
      Serial.println("📡 Alerta MQTT enviado!");
    }
  } else {
    if (tremorDetectado) {
      tremorDetectado = false;
      digitalWrite(buzzerPin, LOW); // Desliga buzzer
      Serial.println("Tremor cessou");
    }
  }
}

// ==================== FUNÇÃO: PUBLICAR DADOS NORMAIS ====================
void publicarDadosSensor() {
  String mensagem = "{\"ax\":" + String(random(-1000, 1000)) + 
                    ",\"ay\":" + String(random(-1000, 1000)) + 
                    ",\"az\":" + String(random(-1000, 1000)) + 
                    ",\"tremor\":" + String(tremorDetectado) + 
                    ",\"timestamp\":\"" + String(millis()) + "\"}";
  
  client.publish(mqtt_topic_tremor, mensagem.c_str());
  Serial.println("Dados publicados: " + mensagem);
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  
  // Configura pino do buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  // Inicializa MPU6050
  Wire.begin();
  mpu.initialize();
  
  // Verifica se o MPU6050 está conectado
  if (mpu.testConnection()) {
    Serial.println("MPU6050 conectado com sucesso!");
  } else {
    Serial.println("Falha na conexão com MPU6050!");
  }
  
  // Conecta Wi-Fi e MQTT
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ==================== LOOP ====================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  unsigned long now = millis();
  
  // Publica dados a cada 5 segundos
  if (now - lastMsg > 5000) {
    lastMsg = now;
    publicarDadosSensor();
  }
  
  // Detecta tremores continuamente
  detectarTremor();
  
  delay(100); // Pequena pausa
}