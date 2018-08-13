/*
 * Algoritmo para control y calibración servomotores de dos brazos robóticos gemelos con 6 grados de libertad usando Arduino,
 * creado por Michael Ceballos Paulino (michcpaulino@gmail.com), Santo Domingo, Rep. Dominicana;
 * Distribuido para uso educativo bajo licencia GNUv3 y Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 * This work is licensed under a GNUv3 and a Creative Commons Attribution-ShareAlike 4.0 International License.
*/

char DATO;
byte LED_Adicional = 9;
byte LED_Adicional_Negativo = 8;

#include <Servo.h>
Servo Hombro1;
Servo Deltoide1;
Servo Codo1;
Servo Muneca1;
Servo Palma1;
Servo Pinza1;
const byte PinHombro1 = 30;
const byte PinDeltoide1 = 31;
const byte PinCodo1 = 32;
const byte PinMuneca1 = 33;
const byte PinPalma1 = 34;
const byte PinPinza1 = 35;
Servo Hombro2;
Servo Deltoide2;
Servo Codo2;
Servo Muneca2;
Servo Palma2;
Servo Pinza2;
const byte PinHombro2 = 40;
const byte PinDeltoide2 = 41;
const byte PinCodo2 = 42;
const byte PinMuneca2 = 43;
const byte PinPalma2 = 44;
const byte PinPinza2 = 45;

static bool Activar1 = true;
static bool Activar2 = true;
static byte Posicion;
static bool Ajustar;
static bool Sentido;
static bool Secundario = false;
const byte Variacion = 5; //Variable para controlar la variación paso por paso hacia arriba o hacia abajo, presente en metodo Micropaso...
int velocidad = 20; //Variable para control de la velocidad en milisegundos
/*Frecuencias ajustadas de operación servo-motores, obtenidas experimentalmente. Si
  se detecta vibración de los mismos estando parados cercano a los ángulos extremos y sin carga o
  en posición de reposo, reajustar los valores del ángulo que da problemas.
  Valores por defecto de la librería Servo min = 544 y max = 2400.
*/
const int Frec_B_MG996 = 560; //Frecuencia Baja para ángulo inferior Servo MG-996
const int Frec_A_MG996 = 2100; //Frecuencia Alta para ángulo superior Servo MG-996
const int Frec_B_SG90 = 750; //Frecuencia Baja para ángulo inferior Servo SG-90
const int Frec_A_SG90 = 2690; //Frecuencia Alta para ángulo superior Servo SG-90
const int VelocidadArranque = 250; //Controla la espera entre el encendido secuencial de los brazos

//Configuración de la Centella
byte Centella1 = A0;
byte Centella1n = A1;
byte Centella2 = A2;
byte Centella2n = A3;
byte Centella3 = A4;
byte Centella3n = A5;
bool CambioCentella = false;

#include <SoftwareSerial.h>
byte BluetoothTx = 11;
byte BluetoothRx = 10;
SoftwareSerial Bluetooth(BluetoothRx, BluetoothTx);

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_Adicional, OUTPUT);
  pinMode(LED_Adicional_Negativo, OUTPUT);
  pinMode(Centella1, OUTPUT);
  pinMode(Centella1n, OUTPUT);
  pinMode(Centella2, OUTPUT);
  pinMode(Centella2n, OUTPUT);
  pinMode(Centella3, OUTPUT);
  pinMode(Centella3n, OUTPUT);
  digitalWrite(Centella1, LOW);
  digitalWrite(Centella1n, LOW);
  digitalWrite(Centella2, LOW);
  digitalWrite(Centella2n, LOW);
  digitalWrite(Centella3, LOW);
  digitalWrite(Centella3n, LOW);
  digitalWrite(LED_Adicional, LOW);
  digitalWrite(LED_Adicional_Negativo, LOW);

  Serial.begin(9600);
  Bluetooth.begin(9600);

 // EncenderBrazoPrimario();
 // EncenderBrazoSecundario();

  //delay(120);
}

void loop() {

  Bluetooth.listen();
  
  String cadena ="";
  
  while(Bluetooth.available()>0)
  {
    char temp = Bluetooth.read();
    if(isdigit(temp)>0)
    {
      cadena += temp;
      Serial.write(temp);
      delay(20);//pausa de prueba para ver si se corrijen errores en entradas rapidas
    }
    else
    {
      DATO = temp;
      Serial.write(" ");
      Serial.write(DATO);
      Serial.write(" ");
    }    
    if(cadena.toInt()<10)
    {
      DATO = temp;
    }
  }
  if(cadena.toInt()>9)
  {
    Posicion = cadena.toInt();
  }
  cadena="";
  
  if(Serial.available())
  {
    DATO = Serial.read();
    Serial.write(DATO);
    Serial.write(" ");
  }
    
  switch(DATO)
  {
    //Valores para ajustar la posición en pasos de 10 grados.
    case '0':
      Posicion = 0;
      break;
    case '1':
      Posicion = 10;
      break;
    case '2':
      Posicion = 20;
      break;
    case '3':
      Posicion = 30;
      break;
    case '4':
      Posicion = 40;
      break;
    case '5':
      Posicion = 50;
      break;
    case '6':
      Posicion = 60;
      break;
    case '7':
      Posicion = 70;
      break;
    case '8':
      Posicion = 80;
      break;
    case '9':
      Posicion = 90;
      break;
    case 'A':
      Posicion = 100;
      break;
    case 'B':
      Posicion = 110;
      break;
    case 'C':
      Posicion = 120;
      break;
    case 'D':
      Posicion = 130;
      break;
    case 'E':
      Posicion = 140;
      break;
    case 'F':
      Posicion = 150;
      break;
    case 'G':
      Posicion = 160;
      break;
    case 'H':
      Posicion = 170;
      break;
    case 'I':
      Posicion = 180;
      break;
    //Llamadas a las articulaciones.
    //La variable Secundario si puesta en "true" manda las órdenes al segundo brazo. Default: primero.
    case 'J':
      MoverHombro(Posicion, Secundario);
      break;
    case 'K':
      MoverDeltoide(Posicion, Secundario);
      break;
    case 'L':
      MoverCodo(Posicion, Secundario);
      break;
    case 'M':
      MoverMuneca(Posicion, Secundario);
      break;
    case 'N':
      MoverPalma(Posicion, Secundario);
      break;
    case 'O':
      MoverPinza(Posicion, Secundario);
      break;
    //Ajustes finos.
    case 'P':
      AjusteAbajo();
      break;
    case 'Q':
      AjusteArriba();
      break;
    case 'R':
      //Ver los ángulos en que se encuentran los servomotores.
      RevisarMotores();
      break;
    case 'S':
      //Parada de emergencia.
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(LED_Adicional, LOW);

      ActivarMotorNegro(Hombro1, PinHombro1, false);
      ActivarMotorNegro(Deltoide1, PinDeltoide1, false);
      ActivarMotorNegro(Codo1, PinCodo1, false);
      ActivarMotorNegro(Muneca1, PinMuneca1, false);
      ActivarMotorAzul(Palma1, PinPalma1, false);
      ActivarMotorAzul(Pinza1, PinPinza1, false);

      ActivarMotorNegro(Hombro2, PinHombro2, false);
      ActivarMotorNegro(Deltoide2, PinDeltoide2, false);
      ActivarMotorNegro(Codo2, PinCodo2, false);
      ActivarMotorNegro(Muneca2, PinMuneca2, false);
      ActivarMotorAzul(Palma2, PinPalma2, false);
      ActivarMotorAzul(Pinza2, PinPinza2, false);

      Serial.println("Todo desactivado.");
      break;
    case 'T':
      //Switch para el LED en la placa.
      volatile bool P_Estado;
      //Lee el estado del pin
      P_Estado = digitalRead(LED_BUILTIN);

      P_Estado = !P_Estado;

      digitalWrite(LED_BUILTIN, P_Estado);
      
      if(P_Estado == HIGH){
        Serial.println("LED Placa encendido");
      }else{
        Serial.println("LED Placa apagado");
      }
      break;
    case 'U':
      digitalWrite(LED_Adicional, HIGH);
      Serial.println("LED Adicional encendido");
      break;
    case 'V':
      Secundario = false;
      Serial.println("Brazo Primario:");
      break;
    case 'W':
      Secundario = true;
      Serial.println("Brazo Secundario:");
      break;
    case 'Y':
      EncenderBrazoPrimario();
      if(Activar1 == false){
      Serial.println("Brazo 1 encendido.");
      }else
      { Serial.println("Brazo 1 apagado."); }
      break;
    case 'Z':
      EncenderBrazoSecundario();
      if(Activar2 == false){
      Serial.println("Brazo 2 encendido.");
      }else
      { Serial.println("Brazo 2 apagado."); }
      break;

    default:
      break;
  }

  Limpiar();
  Centella();

  //delay(120);
  //delay(90);
}

void Limpiar()
{
  DATO = ' ';
}

void ImprimirAngulo(byte Pin, byte valor)
{
  String Extremidad = "OTRO";
  
  if(Pin == PinHombro1 || Pin == PinHombro2)
  {  Extremidad = "Hombro"; }
  
  if(Pin == PinDeltoide1 || Pin == PinDeltoide2)
  {  Extremidad = "Deltoide"; }
  
  if(Pin == PinCodo1 || Pin == PinCodo2)
  {  Extremidad = "Codo"; }
  
  if(Pin == PinMuneca1 || Pin == PinMuneca2)
  {  Extremidad = "Muneca"; }
  
  if(Pin == PinPalma1 || Pin == PinPalma2)
  {  Extremidad = "Palma"; }
  
  if(Pin == PinPinza1 || Pin == PinPinza2)
  {  Extremidad = "Pinza"; }

  Serial.print(Extremidad);
  Serial.print(", ");
  Serial.print("angulo: ");
  Serial.println(valor);
}

void Centella()
{
  CambioCentella = !CambioCentella;
  digitalWrite(Centella1, CambioCentella);
  delay(100);
  digitalWrite(Centella2, CambioCentella);
  delay(100);
  digitalWrite(Centella3, CambioCentella);
  delay(100);
}

void ActivarMotorNegro(Servo Motor, byte Pin, bool op)
{
  if(op == true){
    //PIN, Angulo mínimo en microsegundos y máximo, este ajuste se obtuvo experimentando.
    Motor.attach(Pin, Frec_B_MG996, Frec_A_MG996);
  }else{
    if(Motor.attached())
    {
      Motor.detach();
    }
  }
}

void ActivarMotorAzul(Servo Motor, byte Pin, bool op)
{
  if(op == true){
    //PIN, Angulo mínimo en microsegundos y máximo, este ajuste se obtuvo experimentando.
    Motor.attach(Pin, Frec_B_SG90, Frec_A_SG90);
  }else{
    if(Motor.attached())
    {
      Motor.detach();
    }
  }
}

void EncenderBrazoPrimario()
{
  ActivarMotorNegro(Hombro1, PinHombro1, Activar1);
  delay(VelocidadArranque);
  ActivarMotorNegro(Deltoide1, PinDeltoide1, Activar1);
  delay(VelocidadArranque);
  ActivarMotorNegro(Codo1, PinCodo1, Activar1);
  delay(VelocidadArranque);
  ActivarMotorNegro(Muneca1, PinMuneca1, Activar1);
  delay(VelocidadArranque);
  ActivarMotorAzul(Palma1, PinPalma1, Activar1);
  delay(VelocidadArranque);
  ActivarMotorAzul(Pinza1, PinPinza1, Activar1);
  delay(VelocidadArranque);
  Activar1 = !Activar1;
}

void EncenderBrazoSecundario()
{
  ActivarMotorNegro(Hombro2, PinHombro2, Activar2);
  delay(VelocidadArranque);
  ActivarMotorNegro(Deltoide2, PinDeltoide2, Activar2);
  delay(VelocidadArranque);
  ActivarMotorNegro(Codo2, PinCodo2, Activar2);
  delay(VelocidadArranque);
  ActivarMotorNegro(Muneca2, PinMuneca2, Activar2);
  delay(VelocidadArranque);
  ActivarMotorAzul(Palma2, PinPalma2, Activar2);
  delay(VelocidadArranque);
  ActivarMotorAzul(Pinza2, PinPinza2, Activar2);
  delay(VelocidadArranque);
  Activar2 = !Activar2;
}

// Métodos para micro ajuste paso por paso de la posición, presente en método MicroPaso...
void SoloAngulo()
{
  Ajustar = 0;
  Sentido = 0;
}
void AjusteArriba()
{
  Ajustar = 1;
  Sentido = 1;
}
void AjusteAbajo()
{
  Ajustar = 1;
  Sentido = 0;
}

byte MicroPaso(byte UltimoAngulo)
{
  if(Ajustar == 1 && Sentido == 1)
  {
    Posicion = UltimoAngulo;
    Posicion = Posicion + Variacion;
    SoloAngulo();
  }
  else
  {
    if(Ajustar == 1 && Sentido == 0)
    {
      Posicion = UltimoAngulo;
      Posicion = Posicion - Variacion;
      SoloAngulo();
    }
  }

  return Posicion;
}

void RevisarMotores()
{
  Serial.println("");
  Serial.println("Revisar Motores:");
  Serial.println("Brazo No.1:");
  ImprimirAngulo(PinHombro1, Hombro1.read());
  ImprimirAngulo(PinDeltoide1, Deltoide1.read());
  ImprimirAngulo(PinCodo1, Codo1.read());
  ImprimirAngulo(PinMuneca1, Muneca1.read());
  ImprimirAngulo(PinPalma1, Palma1.read());
  ImprimirAngulo(PinPinza1, Pinza1.read());
  Serial.println("Brazo No.2:");
  ImprimirAngulo(PinHombro2, Hombro2.read());
  ImprimirAngulo(PinDeltoide2, Deltoide2.read());
  ImprimirAngulo(PinCodo2, Codo2.read());
  ImprimirAngulo(PinMuneca2, Muneca2.read());
  ImprimirAngulo(PinPalma2, Palma2.read());
  ImprimirAngulo(PinPinza2, Pinza2.read());
  Serial.println("");
}

//Enviar instrucciones de movimiento a servo motores, dependiendo del tipo, actualmente MG-996 y SG90.
//Los últimos dos parámetros son la restricción de ángulos mínimo y máximo respectivos.
void OperarMotorNegro(Servo Motor, byte Pin, byte Posicion, byte AnguloMin, byte AnguloMax)
{
  Posicion = constrain(Posicion, AnguloMin, AnguloMax);
  
  if(!Motor.attached())
  {
    ActivarMotorNegro(Motor, Pin, true);
  }
  
  volatile byte UltimoAngulo = Motor.read();

  Posicion = MicroPaso(UltimoAngulo);

  if(Posicion != UltimoAngulo)
  {
    if(Posicion < UltimoAngulo)
    {
      for(byte i = UltimoAngulo; i > Posicion; i--)
      {
        Motor.write(i);
        delay(velocidad);
      }
    }
    else
    {
      for(byte i = UltimoAngulo; i < Posicion; i++)
      {
        Motor.write(i);
        delay(velocidad);
      }
    }
  }
  ImprimirAngulo(Pin, Motor.read());
}

void OperarMotorAzul(Servo Motor, byte Pin, byte Posicion, byte AnguloMin, byte AnguloMax)
{
  Posicion = constrain(Posicion, AnguloMin, AnguloMax);
  
  if(!Motor.attached())
  {
    ActivarMotorAzul(Motor, Pin, true);
  }
  
  volatile byte UltimoAngulo = Motor.read();

  Posicion = MicroPaso(UltimoAngulo);

  if(Posicion != UltimoAngulo)
  {
    if(Posicion < UltimoAngulo)
    {
      for(byte i = UltimoAngulo; i > Posicion; i--)
      {
        Motor.write(i);
        delay(velocidad);
      }
    }
    else
    {
      for(byte i = UltimoAngulo; i < Posicion; i++)
      {
        Motor.write(i);
        delay(velocidad);
      }
    }
  }
  ImprimirAngulo(Pin, Motor.read());
}

//Métodos selectores para mover en brazo derecho o izquierdo
//NOTA: por cada brazo y motor los ángulos de operación pueden ser diferentes
// Aquí es donde se deben poner las restricciones de los ángulos

void MoverHombro(byte Posicion, bool Secundario)
{
  //Libertad de operación: 180 grados.
  if(Secundario == true){
    OperarMotorNegro(Hombro2, PinHombro2, Posicion, 0, 180);
  }else{
    OperarMotorNegro(Hombro1, PinHombro1, Posicion, 0, 180);
  }
}

void MoverDeltoide(byte Posicion, bool Secundario)
{
  //Libertad de operación: 150 grados aprox.
  if(Secundario == true){
    OperarMotorNegro(Deltoide2, PinDeltoide2, Posicion, 0, 150);
  }else{
    OperarMotorNegro(Deltoide1, PinDeltoide1, Posicion, 0, 150);
  }
}

void MoverCodo(byte Posicion, bool Secundario)
{
  //Libertad de operación: 120 grados aprox.
  if(Secundario == true){
    OperarMotorNegro(Codo2, PinCodo2, Posicion, 0, 120);
  }else{
    OperarMotorNegro(Codo1, PinCodo1, Posicion, 0, 120);
  }
}

void MoverMuneca(byte Posicion, bool Secundario)
{
  //Libertad de operación: 180 grados.
  if(Secundario == true){
    OperarMotorNegro(Muneca2, PinMuneca2, Posicion, 0, 180);
  }else{
    OperarMotorNegro(Muneca1, PinMuneca1, Posicion, 0, 180);
  }
}

void MoverPalma(byte Posicion, bool Secundario)
{
  //Libertad de operación: 180 grados.
  if(Secundario == true){
    OperarMotorAzul(Palma2, PinPalma2, Posicion, 0, 180);
  }else{
    OperarMotorAzul(Palma1, PinPalma1, Posicion, 0, 180);
  }
}

void MoverPinza(byte Posicion, bool Secundario)
{
  //Libertad de operación: 75 grados aprox.
  if(Secundario == true){
    OperarMotorAzul(Pinza2, PinPinza2, Posicion, 30, 105);
  }else{
    OperarMotorAzul(Pinza1, PinPinza1, Posicion, 30, 105);
  }
}
