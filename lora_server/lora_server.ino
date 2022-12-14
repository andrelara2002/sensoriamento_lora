#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#include <WiFi.h>
#include <HTTPClient.h>
 
/* Definicoes para comunicação com radio LoRa */
#define SCK_LORA           5
#define MISO_LORA          19
#define MOSI_LORA          27
#define RESET_PIN_LORA     14
#define SS_PIN_LORA        18
#define USE_SERIAL Serial
 
#define HIGH_GAIN_LORA     20  /* dBm */
#define BAND               915E6  /* 915MHz de frequencia */
 
/* Definicoes do OLED */
#define OLED_SDA_PIN    4
#define OLED_SCL_PIN    15
#define SCREEN_WIDTH    128 
#define SCREEN_HEIGHT   64  
#define OLED_ADDR       0x3C 
#define OLED_RESET      16
 
/* Offset de linhas no display OLED */
#define OLED_LINE1     0
#define OLED_LINE2     10
#define OLED_LINE3     20
#define OLED_LINE4     30
#define OLED_LINE5     40
#define OLED_LINE6     50
 
/* Definicoes gerais */
#define DEBUG_SERIAL_BAUDRATE    115200

 
/* Local prototypes */
void displayInit(void);
bool loRaInit(void);
 
/* Variaveis e objetos globais */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

StaticJsonDocument<96> doc;

JsonObject doc_0 = doc.createNestedObject();

static const uint8_t scania_logo[1952] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xfc, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0x83, 0x00, 0x62, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0x7e, 0x80, 0x5d, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xfe, 0x48, 0xbf, 0x4c, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xfe, 0x90, 0x00, 0x02, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xfd, 0x20, 0x08, 0x01, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xfd, 0x40, 0x5d, 0x01, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xfa, 0x40, 0x4d, 0x00, 0xaf, 0xe0, 0x3f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xfa, 0x80, 0x3d, 0x00, 0x9f, 0x80, 0x0e, 0x00, 0x3c, 0x03, 0x01, 0xe0, 0x03, 0x00, 0xff, 0xc0, 
    0xfa, 0x00, 0x42, 0x80, 0x57, 0x00, 0x04, 0x00, 0x18, 0x03, 0x00, 0xe0, 0x03, 0x00, 0x7f, 0xc0, 
    0xf9, 0x81, 0x31, 0x00, 0x66, 0x02, 0x00, 0x00, 0x08, 0x01, 0x00, 0x60, 0x02, 0x00, 0x7f, 0xc0, 
    0xfd, 0x01, 0x00, 0x40, 0x4e, 0x07, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x20, 0x02, 0x00, 0x3f, 0xc0, 
    0xfe, 0xc4, 0xe1, 0x40, 0xde, 0x00, 0x00, 0x1e, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 
    0xff, 0x43, 0x22, 0x41, 0x5e, 0x00, 0x00, 0x3f, 0xf0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 
    0xfe, 0xa0, 0x8b, 0x03, 0x5f, 0x00, 0x00, 0x3f, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x18, 0x1f, 0xc0, 
    0xfe, 0x90, 0x4c, 0x22, 0x5f, 0xc0, 0x00, 0x1e, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1f, 0xc0, 
    0xfe, 0x90, 0x20, 0x84, 0x3c, 0x07, 0x00, 0x0c, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0f, 0xc0, 
    0xfe, 0x88, 0x40, 0x84, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0f, 0xc0, 
    0xff, 0x48, 0x42, 0x0c, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x07, 0xc0, 
    0xff, 0x44, 0x10, 0x99, 0x7f, 0x00, 0x06, 0x00, 0x01, 0xf8, 0x07, 0x80, 0x00, 0x7e, 0x07, 0xc0, 
    0xff, 0xa4, 0x23, 0x10, 0x7f, 0xe0, 0x7f, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0x92, 0x14, 0x12, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xca, 0x08, 0x2d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xe7, 0x00, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xfb, 0x00, 0x67, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xfc, 0xff, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xfe, 0x80, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xfe, 0xd5, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0
};


const char *sensors[10];
const char *variables[10];
 
/* Funcao: inicializa comunicacao com o display OLED
 * Parametros: nenhnum
 * Retorno: nenhnum
*/ 
void displayInit(void)
{
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) 
    {
        Serial.println("[LoRa Receiver] Falha ao inicializar comunicacao com OLED");        
    }
    else
    {
        Serial.println("[LoRa Receiver] Comunicacao com OLED inicializada com sucesso");
     
        /* Limpa display e configura tamanho de fonte */
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
    }
}
 
/* Funcao: inicia comunicação com chip LoRa
 * Parametros: nenhum
 * Retorno: true: comunicacao ok
 *          false: falha na comunicacao
*/
bool loRaInit(void)
{
    bool status_init = false;
    Serial.println("[LoRa Receiver] Tentando iniciar comunicacao com o radio LoRa...");
    SPI.begin(SCK_LORA, MISO_LORA, MOSI_LORA, SS_PIN_LORA);
    LoRa.setPins(SS_PIN_LORA, RESET_PIN_LORA, LORA_DEFAULT_DIO0_PIN);
     
    if (!LoRa.begin(BAND)) 
    {
        Serial.println("[LoRa Receiver] Comunicacao com o radio LoRa falhou. Nova tentativa em 1 segundo...");        
        delay(1000);
        status_init = false;
    }
    else
    {
        /* Configura o ganho do receptor LoRa para 20dBm, o maior ganho possível (visando maior alcance possível) */ 
        LoRa.setTxPower(HIGH_GAIN_LORA); 
        Serial.println("[LoRa Receiver] Comunicacao com o radio LoRa ok");
        status_init = true;
    }
 
    return status_init;
}
 
/* Funcao de setup */
void setup() 
{
    Serial.begin(DEBUG_SERIAL_BAUDRATE);
    while (!Serial);
    
    Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

    Serial.println("Aguardando display...");
    displayInit();
    
    display.clearDisplay();    
    
    display.setCursor(0, OLED_LINE1);
    display.drawBitmap(0, 10, scania_logo, 128, 64, 1);
    display.display();

    // Definição dos endereços e descrição sensores
    sensors[0] = "18ea3b3a-b209-4178-9edd-80f50ae4cf75"; variables[0] = "pressure"; //pressure sensor
    sensors[1] = "ccdf5c2d-cc7a-4cbb-a43d-42e0caca2bab"; variables[1] = "percent"; //oil level sensor

    delay(5000);
    
    // Inicialzando Wifi
    display.clearDisplay();
    display.setCursor(0, OLED_LINE1);
    display.println("WiFi: ...");
    display.display();
    
    WiFi.begin("BLOCO_D_PISO_TERREO_SA", "WiFiSen@i123");

    //WiFi.begin("WF4-2.4Ghz", "paralelepipedo");

    while(WiFi.status() != WL_CONNECTED) {
      display.print(".");
      display.display();
      delay(500);
    }

    display.println("");
    display.println("WiFi: OK");

    display.println("LoRa: ...");
    display.display();
 
    /* Tenta, até obter sucesso, comunicacao com o chip LoRa */
    while(loRaInit() == false);  
    display.println("LoRa: OK");

    display.display();
    delay(2000);
}
 
/* Programa principal */
void loop() 
{
    char byte_recebido, informacao_recebida[10];
    int lora_rssi = 0, packet_size = 0;

    char * ptInformaraoRecebida = NULL;
    
   
    /* Verifica se chegou alguma informação do tamanho esperado */
    packet_size = LoRa.parsePacket();

    lora_rssi = LoRa.packetRssi();
    display.clearDisplay();   
    display.setTextSize(1);
    display.setCursor(0, OLED_LINE1);
    display.print("Sinal LoRa: ");
    display.println(lora_rssi);

    display.print("WiFi: ");
    display.println(WiFi.status()== WL_CONNECTED ? "OK" : "ERROR");

    display.display();
     
    if (packet_size) 
    {
         
        /* Recebe os dados conforme protocolo */

        char received[100];
        String lora_data = LoRa.readString();
        strcpy(received, lora_data.c_str());
        
        int i = 0;

        /*
         Desestrutura os dados recebidos:
            0 - ID do sensor
            1 - Dados recebidos
        */
    
        char *p = strtok (received, "\n");
        char *data[3];

        char id_sensor[3];
    
        while (p)
        {
          data[i++] = p;
          p = strtok (NULL, "-");
        }

        strcpy(id_sensor, data[0]);
        strcpy(received, data[1]);
       
        /* Escreve RSSI de recepção e informação recebida */
        lora_rssi = LoRa.packetRssi();

        display.clearDisplay();   
        display.setTextSize(1);
        display.setCursor(0, OLED_LINE1);
        display.print("Sinal LoRa: ");
        display.println(lora_rssi);
    
        display.print("WiFi: ");
        display.println(WiFi.status()== WL_CONNECTED ? "OK" : "ERROR");
        display.println("---------------------");
        display.setTextSize(2);
        display.print("ID: ");
        display.println(id_sensor);
        
        display.print("DATA: ");
        display.println(received);

        display.display();
        
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
      
        // Your Domain name with URL path or IP address with path
        http.begin("https://api.tago.io/data");

        char token[36];
        char type[20];
        strcpy(token, sensors[atoi(id_sensor)]);
        strcpy(type, variables[atoi(id_sensor)]);
        
   
        // If you need an HTTP request with a content type: application/json, use the following:
        http.addHeader("Content-Type", "application/json");
        http.addHeader("device-token", token);

        /*

        TagoIO upload sample
        {
          "variable": "percent",
          "value": 20,
          "unit": "%",
          "time": "2021-09-16T17:59:43.672Z"
        }

        String httpRequestData = "[\n{\n \"variable\":\"temperature\",\n \"value\":27\n}\n]";  
        
        **/

        String output;
        
        doc_0["variable"] = type;
        doc_0["value"] = atoi(received);
        
        Serial.print("Dado recebido: ");
        Serial.println(received);
        
        serializeJson(doc, output);
        
           
        // Envia os dados para API através de HTTP
        
        int httpResponseCode = http.POST(output);

        Serial.println(output);
       
        Serial.println("HTTP Response code: ");
        Serial.print(httpResponseCode);
          
        // Encerra a requisição HTTP
        http.end();
      }
      else {
        Serial.println("\nWiFi Disconnected");
      }
    }
}
