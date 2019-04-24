/*
  Arduino ESCLAVO (Arduino Mega 2560)
  Se encargara de funciones basicas
*/

#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

//PIN del Sensor de humedad y temperatura
int SENSOR = 8;

//Valor de la futura señal del Master
int funcionAux = 0;

//Pines para la pantalla
LiquidCrystal lcd(7,6,5,4,3,2);
//Pines para el sensor de humedad y temperatura
DHT dht(SENSOR, DHT11);

//Variebles normales
int temperatura;
int humedad;
//Controlaremos si la pantalla va a ser usada por el Master
bool pantallaUsada = false;
//Pin del buzzer
int buzzer = 13;
//Pin Lucesd LEDS
int led1 = 11;
bool estadoLed1 = false;
int led2 = 12;
//Pin Motor
int motor = 9;
bool estadoMotor = false;

void setup() {
  // Unimos este dispositivo al bus I2C con dirección 1
  Wire.begin(1);

  // Registramos el evento al recibir datos
  Wire.onReceive(receiveEvent);
  
  //Iniciamos el sensor
  dht.begin();
  //Iniciamos la pantalla
  lcd.begin(16,2);

  // Iniciamos el monitor serie para monitorear la comunicación
  Serial.begin(9600);

  //LEDS
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  //BUZZER
  pinMode(buzzer, OUTPUT);
} //FIN SETUP-------------------------------------

// Función que se ejecuta siempre que se reciben datos del master
// siempre que en el master se ejecute la sentencia endTransmission
// recibirá toda la información que hayamos pasado a través de la sentencia Wire.write
void receiveEvent(int howMany) {
  funcionAux = Wire.read();
  Serial.print("Funcion: ");
  Serial.println(funcionAux);
}

void loop() {

  //Funcion normal de HOLA THINK
  if(funcionAux == 1){
    Serial.println("HOLA THINK---------------------------------------------");
    pantallaUsada = false;
    resetearFuncion();
  }

  //Funcion normal de ENCIENDE LA LUZ
  if(funcionAux == 2){
    Serial.println("ENCIENDE LA LUZ---------------------------------------------");
    funcionLuces();
    resetearFuncion();
  }
  
  //Funcion normal de DONDE ESTAS 
  if(funcionAux == 3){
    Serial.println("DONDE ESTAS---------------------------------------------");
    funcionBuzzer();
    resetearFuncion();
  }

  //Funcion normal de VENTILADOR
  if(funcionAux == 4){
    Serial.println("VENTILADOR---------------------------------------------");
    funcionMotor();
    resetearFuncion();
  }

  //Funcion normal de MUSICA
  if(funcionAux == 5){
    Serial.println("MUSICA---------------------------------------------");
    musicaPantalla();
    pantallaUsada = true;
    resetearFuncion();

    delay(1000);
  }

  if (pantallaUsada == false){
    pantalla();
  }  
  
  delay(300);
} //FIN LOOP------------------------------------------------------



void resetearFuncion(){
  funcionAux = -1;
}

void pantalla() {
  temperatura = dht.readTemperature();
  //Serial.print("temperatura:");
  //Serial.print(temperatura);
  humedad = dht.readHumidity();
  //Serial.print("\nhumedad: ");
  //Serial.print(humedad);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print(" grados");
  lcd.setCursor(0,1);
  lcd.print("Humedad: ");
  lcd.print(humedad);
  lcd.print("%");
}

void musicaPantalla(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reproduciendo");
  lcd.setCursor(0,1);
  lcd.print("musica");
}  

void funcionBuzzer(){
  pantallaUsada = false;
  delay(4000);
  for (int i = 0; i < 5; i++){
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    delay(1000);
  }
}

void funcionMotor(){
  pantallaUsada = false;
  if (estadoMotor == false){
    analogWrite(motor, 255);
    estadoMotor = true;
  }else{
    analogWrite(motor, 0);
    estadoMotor = false;
  }
}

void funcionLuces(){
  pantallaUsada = false;
  if (estadoLed1 == false){
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      estadoLed1 = true;
  }else{
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      estadoLed1 = false;
  }
}
