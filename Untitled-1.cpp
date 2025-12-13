#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "time.h"

// ==============================
// CONFIG WIFI
// ==============================
const char* ssid     = "SEU_WIFI";
const char* password = "SUA_SENHA";

// ==============================
// CONFIG NTP
// ==============================
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;      // GMT-3 (Brasil)
const int   daylightOffset_sec = 0;

// ==============================
// PINOS
// ==============================
#define ONE_WIRE_BUS 4
#define PH_PIN 34
#define SD_CS 5

// ==============================
// OBJETOS
// ==============================
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WebServer server(80);

// Calibração do pH
float phOffset = 0.0;
float phSlope = 3.5;

// ==============================
// FUNÇÃO PARA LER PH
// ==============================
float lerPH() {
  int adc = analogRead(PH_PIN);
  float tensao = adc * (3.3 / 4095.0);
  float ph = phSlope * tensao + phOffset;
  return ph;
}

// ==============================
// TIMESTAMP ATUAL
// ==============================
String getTimeStamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "00:00:00";
  
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

// ==============================
// SALVAR NO HISTÓRICO CSV
// ==============================
void salvarHistorico(float tempC, float ph) {
  File file = SD.open("/historico.csv", FILE_APPEND);
  if (!file) return;

  String linha = getTimeStamp() + ";" + String(tempC,2) + ";" + String(ph,2) + "\n";
  file.print(linha);
  file.close();
}

// ==============================
// API /data
// ==============================
void apiDados() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  float ph = lerPH();

  StaticJsonDocument<200> doc;
  doc["temperatura"] = tempC;
  doc["ph"] = ph;
  doc["hora"] = getTimeStamp();

  String resposta;
  serializeJson(doc, resposta);

  server.send(200, "application/json", resposta);

  salvarHistorico(tempC, ph);
}

// ==============================
// API /download - histórico
// ==============================
void baixarCSV() {
  File file = SD.open("/historico.csv");
  if (!file) {
    server.send(404, "text/plain", "Nenhum historico encontrado");
    return;
  }
  
  server.streamFile(file, "text/csv");
  file.close();
}

// ==============================
// API /config - leitura
// ==============================
void apiConfig() {
  StaticJsonDocument<200> doc;
  doc["phOffset"] = phOffset;
  doc["phSlope"] = phSlope;

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

// ==============================
// API /config - gravação
// ==============================
void salvarConfig() {
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "JSON necessário");
    return;
  }

  StaticJsonDocument<300> doc;
  deserializeJson(doc, server.arg("plain"));

  phOffset = doc["phOffset"].as<float>();
  phSlope = doc["phSlope"].as<float>();

  File file = SD.open("/config.json", FILE_WRITE);
  if (file) {
    serializeJson(doc, file);
    file.close();
  }

  server.send(200, "text/plain", "Configuração salva");
}

// ==============================
// SERVIR ARQUIVOS ESTÁTICOS DO SD
// ==============================
void servirArquivo(String caminho) {
  String contentType = "text/plain";

  if (caminho.endsWith(".html")) contentType = "text/html";
  if (caminho.endsWith(".css")) contentType = "text/css";
  if (caminho.endsWith(".js")) contentType = "application/javascript";
  if (caminho.endsWith(".csv")) contentType = "text/csv";

  File file = SD.open(caminho);
  if (!file) {
    server.send(404, "text/plain", "Arquivo não encontrado");
    return;
  }

  server.streamFile(file, contentType);
  file.close();
}

// ==============================
// SETUP
// ==============================
void setup() {
  Serial.begin(115200);

  sensors.begin();

  // SD
  if (!SD.begin(SD_CS)) {
    Serial.println("Falha ao iniciar SD!");
  } else {
    Serial.println("SD OK");
  }

  // WIFI
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());

  // NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // ROTAS
  server.on("/data", apiDados);
  server.on("/download", baixarCSV);
  server.on("/config", HTTP_GET, apiConfig);
  server.on("/config", HTTP_POST, salvarConfig);

  server.onNotFound([]() {
    servirArquivo(server.uri());
  });

  server.begin();
}

// ==============================
// LOOP
// ==============================
void loop() {
  server.handleClient();
}
