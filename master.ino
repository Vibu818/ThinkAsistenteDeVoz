/*
  Arduino MASTER (Arduino UNO)
  Se encargara del microfono y altavoz
*/
//Para conectar dos Arduino
#include <Wire.h>

//Lector de SDs
#include <SD.h>

#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>

//PIN de la SD
#define SD_ChipSelectPin 4

//Creamos Objeto del reproductor
TMRpcm tmrpcm;

//Lo que recibe por el microfono
byte conseguir = 0;
//Numero que usaremos para la musica
int randomNum;
String cancion = "";
//Control de estados
bool estadoLuz = false;
bool estadoVentilador = false;
bool estadoMusica = false;
//LeddeComprobacion
int ledComprobacion = 6;

void setup() {
  //Led de comprobacion
  pinMode(ledComprobacion, OUTPUT);
  
  // Unimos este dispositivo al bus I2C
  Wire.begin();
  //Inicaimos la pantalla
  Serial.begin(9600);
  delay(2000);
  //Preparamos el microfono, ponemos el Common Mode
  Serial.write(0xAA);
  Serial.write(0x37);
  delay(1000);
  //Importar grupo
  Serial.write(0xAA);
  Serial.write(0x21);

  //Respuesta Altavoz
  tmrpcm.speakerPin = 9;
  tmrpcm.setVolume(4);

  //Preparamos SD
  pinMode(SD_ChipSelectPin, OUTPUT);
  digitalWrite(SD_ChipSelectPin, HIGH);
  if (!SD.begin(SD_ChipSelectPin)) {
    return;
  }

  //Seed del Random
  randomSeed(analogRead(0));
}


void loop() {

  conseguir = Serial.read();
  
  if (conseguir == 0x11){  //---------------------HOLA THINK
     tmrpcm.play("holath.wav");
     digitalWrite(ledComprobacion, HIGH);
     
     // Comenzamos la transmisión al dispositivo 1
    Wire.beginTransmission(1);

    // Enviamos un byte, será el pin a encender
    Wire.write(1);

    // Paramos la transmisión
    Serial.write("enviando 1...\n");
    Wire.endTransmission();

    // Esperamos 1 segundo
    delay(1000);
    digitalWrite(ledComprobacion, LOW);
  }
  
  if (conseguir == 0x12) {  //-------------------- LUZ
     digitalWrite(ledComprobacion, HIGH);
     if (estadoLuz == false){
      tmrpcm.play("luz.wav");
      estadoLuz = true;
     }else{
      tmrpcm.play("nluz.wav"); //Apagar luz
      estadoLuz = false;
     }
     
     // Comenzamos la transmisión al dispositivo 1
     Wire.beginTransmission(1);

     // Enviamos un byte, será el pin a encender
     Wire.write(2);

     // Paramos la transmisión
     Serial.write("enviando 2...\n");
     Wire.endTransmission();

     // Esperamos 1 segundo
     delay(1000);
     digitalWrite(ledComprobacion, LOW);
  }
  
  if(conseguir == 0x13) {   //-------------------- DONDE ESTAS
     digitalWrite(ledComprobacion, HIGH);
     tmrpcm.play("aqui.wav");
     // Comenzamos la transmisión al dispositivo 1
     Wire.beginTransmission(1);

     // Enviamos un byte, será el pin a encender
     Wire.write(3);

     // Paramos la transmisión
     Serial.write("enviando 3...\n");
     Wire.endTransmission();

     // Esperamos 1 segundo
     delay(1000);
     digitalWrite(ledComprobacion, LOW);
  }
  
  if(conseguir == 0x14) {   //-------------------- VENTILADOR
     digitalWrite(ledComprobacion, HIGH);
     if (estadoVentilador == false){
      tmrpcm.play("ven.wav");
      estadoVentilador = true;
     }else{
      tmrpcm.play("nven.wav"); //Apagar ventilador
      estadoVentilador = false;
     }
     
     // Comenzamos la transmisión al dispositivo 1
     Wire.beginTransmission(1);

     // Enviamos un byte, será el pin a encender
     Wire.write(4);

     // Paramos la transmisión
     Serial.write("enviando 4...\n");
     Wire.endTransmission();

     // Esperamos 1 segundo
     delay(1000);
     digitalWrite(ledComprobacion, LOW);
  }
  
  if(conseguir == 0x15) {   //-------------------- MUSICA
     randomNum = random(1, 5);
     
     digitalWrite(ledComprobacion, HIGH);
     if (estadoMusica == false){
      tmrpcm.play("rmus.wav");
      estadoMusica = true;
     }else{
      tmrpcm.play("scan.wav"); //Siguiente cancion
     }
     delay(2500);

     switch (randomNum){
        case 1:
            tmrpcm.play("1.wav");
            break;
        case 2:
            tmrpcm.play("2.wav");
            break;
        case 3:
            tmrpcm.play("3.wav");
            break;
        case 4:
            tmrpcm.play("4.wav");
            break;
     }
     

     // Comenzamos la transmisión al dispositivo 1
     Wire.beginTransmission(1);

     // Enviamos un byte, será el pin a encender
     Wire.write(5);

     // Paramos la transmisión
     Serial.write("enviando 5...\n");
     Wire.endTransmission();

     // Esperamos 1 segundo
     delay(1000);
     digitalWrite(ledComprobacion, LOW);
  }
    
}
