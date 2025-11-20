# Sistema IoT para Monitoramento de Crises Motoras em Pacientes com Parkinson

![ESP32](https://img.shields.io/badge/ESP32-IoT-blue)
![MQTT](https://img.shields.io/badge/Protocol-MQTT-green)
![Parkinson](https://img.shields.io/badge/Aplicação-Saúde-red)

Sistema de monitoramento em tempo real para detectar tremores e crises motoras em pacientes com doença de Parkinson, utilizando tecnologias IoT.

## 🎯 Objetivo

Desenvolver um dispositivo vestível que monitora continuamente os movimentos do paciente, detecta episódios de tremor e envia alertas para cuidadores através do protocolo MQTT.

## 🛠️ Hardware Utilizado

- **Microcontrolador:** ESP32 DevKit C
- **Sensor de Movimento:** MPU6050 (Acelerômetro + Giroscópio)
- **Atuador:** Buzzer Ativo 5V
- **Conexões:** Protoboard e jumpers
- **Alimentação:** USB ou Bateria LiPo 3.7V

## 📋 Funcionalidades

- ✅ Monitoramento contínuo de movimentos
- ✅ Detecção automática de tremores
- ✅ Alertas sonoros locais (buzzer)
- ✅ Transmissão de dados via MQTT
- ✅ Conexão Wi-Fi
- ✅ Dashboard em tempo real

## 🔧 Instalação e Configuração

### Pré-requisitos

1. **Arduino IDE** (versão 2.0 ou superior)
2. **Placa ESP32** configurada na IDE
3. **Bibliotecas necessárias:**

### Bibliotecas Requeridas

Instale via Library Manager do Arduino IDE:
- `PubSubClient` by Nick O'Leary
- `MPU6050` by Electronic Cats
- `Wire` (já incluída na IDE)

### Configuração do Código

1. Abra o arquivo `parkinson_iot_monitor.ino`
2. Configure suas credenciais Wi-Fi:
```cpp
const char* ssid = "SUA_REDE_WIFI";
const char* password = "SUA_SENHA_WIFI";
```

# 📡 Uso
## Execução Normal
Conecte todos os componentes conforme o esquema

Carregue o código parkinson_iot_monitor.ino no ESP32

Abra o Monitor Serial (115200 baud)

O sistema iniciará automaticamente

Monitoramento
Alertas Locais: Buzzer emite som ao detectar tremor

Alertas Remotos: Mensagens MQTT publicadas no tópico parkinson/alerta

Dados: Armazenados na EEPROM do ESP32 para análise

## 🎮 Simulação
Versão para Wokwi
Para teste sem hardware físico, use o projeto criado no Wokwi: https://wokwi.com/projects/448103182161124353

#### 🔗 Simular no Wokwi
Características da Versão Simulada:

| Recurso        | Versão Real         | Versão Simulada            |
|----------------|----------------------|-----------------------------|
| Sensor MPU6050 | Hardware real        | Geração de dados simulados |
| Buzzer         | Som físico           | LED indicador               |
| Wi-Fi          | Conexão real         | Simulação de rede           |
| MQTT           | Broker real          | Mensagens no Serial         |


#### Como Usar a Simulação:
Acesse Wokwi.com
Execute a simulação
