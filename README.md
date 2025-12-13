# ESP32-Water-Monitor-pH-Temperatura
### Projeto completo de monitoramento da água em tempo real, utilizando ESP32, sensor de pH PH4502C, sensor de temperatura DS18B20 e cartão MicroSD, com dashboard web acessível via navegador (PC ou celular).
_____________________________________________________________________________
## 📌 Visão Geral

### O sistema realiza:

* #### Leitura contínua de temperatura da água
* #### Leitura contínua de pH da água
* #### Exibição em tempo real via navegador
* #### Gravação de histórico em cartão MicroSD (CSV)
* #### Sincronização de horário via NTP
* #### Dashboard web moderno, com gráfico em tempo real
* #### API REST para integração
#### Tudo roda localmente no ESP32, sem depender de servidores externos.
_______________________________________________________________________________
## 🧠 Arquitetura do Sistema

### Sensores → ESP32 → Wi‑Fi → Navegador / Celular

* #### O ESP32 atua como servidor web
* #### Os arquivos do front‑end são carregados diretamente do cartão SD
* #### O navegador consome os dados via JSON (AJAX)
_________________________________________________________________________________
## 🧩 Componentes Utilizados
### Hardware

* #### ESP32 Dev Module
* #### Sensor de temperatura DS18B20
* #### Módulo de pH PH4502C (0–14)
* #### Módulo MicroSD (TF Card)
* #### Resistor 4.7kΩ (DS18B20)
* #### Fonte 5V / USB
_________________________________________________________________________________
## Software
* #### Arduino IDE
* #### Core ESP32
* #### Bibliotecas:
  * #### OneWire
  * #### DallasTemperature
  * #### ArduinoJson
  * __________________________________________________________________________________
## 📁 Estrutura do Cartão SD
### /SDCARD
#### ├── index.html
#### ├── style.css
#### ├── script.js
#### ├── chart.min.js
#### ├── historico.csv
#### └── config.json
_____________________________________________________________________________________
## 🌐 Endpoints Disponíveis
### Rota	Função
#### /	Dashboard Web
#### /data	Dados em tempo real (JSON)
#### /download	Download do histórico CSV
#### /config	Leitura e gravação da calibração
___________________________________________________________________________________
## 📊 Dashboard Web

* #### Atualização automática a cada 1 segundo

* #### Gráfico em tempo real (Chart.js)

* #### Compatível com celular e desktop
### Acesso pelo navegador:
#### http://IP_DO_ESP32
## 💾 Histórico de Dados

### Os dados são gravados automaticamente no arquivo:

#### historico.csv

### Formato:

#### DATA_HORA ; TEMPERATURA ; PH
## 🧪 Calibração de pH

* #### Ajustável remotamente via navegador

* #### Parâmetros:
  * #### Slope
  * #### Offset
* #### Persistência no arquivo config.json
*  ### Recomendado usar soluções tampão pH 4 e pH 7.

## 🚀 Como Executar

1. #### Instale as bibliotecas necessárias
2. #### Compile e envie o main.ino para o ESP32
3. #### Copie os arquivos web para o cartão SD
4. #### Ligue o ESP32
5. #### Acesse o IP exibido no Serial Monitor

## 📈 Consumo de Recursos

* #### RAM utilizada: < 25%

* #### Flash utilizada: < 30%

* #### Totalmente estável para operação contínua

## 🔒 Expansões Futuras

* #### Autenticação (login/senha)

* #### MQTT / Home Assistant

* #### Envio para nuvem

* #### OTA (atualização via navegador)

* #### Alarmes por pH ou temperatura

## 📄 Licença

### Projeto open‑source para fins educacionais e profissionais.

## 👨‍💻 Autor

### Projeto desenvolvido por ## Marcoa Roa #### com foco em monitoramento ambiental, aquicultura e automação embarcada com ESP32.
