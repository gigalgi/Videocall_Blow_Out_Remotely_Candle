//Libreria de control para servos
#include <Servo.h>
//Definir servo
Servo fanOnOff; 
//Variables
const int mic = A0;  
const int candle = 2; 
int sound;
bool flame;
bool setTimer = true; 
bool startTimer = false; 
//Tiempo de duracion del soplido
const int tiempoEspera = 2000;//ms
unsigned long antes = 0;  
unsigned long ahora = 0;
int varTiempo = 0;
int countPeak = 0;
//tiempo necesario en alto al recibir sonido del microfono
//ajusta segun el tiempo del soplido
const int peakHighTime = 1000;
// limite superir para definir el sonido captado por el microfono como alto
//ajustar de arcuerdo a su microfono
const int soundLimit = 400;

void setup() 
{
  fanOnOff.attach(3);
  fanOnOff.write(0);
  pinMode(candle, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  //verificar si la vela esta encendida
  flame = digitalRead(candle);
  while(flame)
  {
    //si la vela esta encendida el microfono comienza a registrar
    sound = analogRead(mic);
    //Serial.println(sound);
    //si el ruido generado por el soplido supera el umbral se incia el conteo segun los configurado en este caso 2000ms = 2seg
    if(sound > soundLimit && setTimer)
    {
      antes = millis();
      ahora = millis();
      setTimer = false;
      startTimer = true;
    }

    varTiempo = ahora - antes;
    //verificamos el tiempo si entre 0s y 2s se registra ruido por encima del umbral y se cuenta el numero de picos generados
    if(varTiempo <= tiempoEspera && startTimer)
    {
      sound = analogRead(mic);
      if(sound > soundLimit) countPeak++;
    }
    
    else
    {
      //se terminan los 2 seg y sse queda ala espera de un nuevo pico y volver a contar
      setTimer = true;
      startTimer = false;
      if(countPeak > 0) Serial.println(countPeak); 
      //si el numero de picos definido se supera significa que hubo un soplido constante por al menos 2 segundos
      if(countPeak > peakHighTime)
      {
        //secuencia para activar el ventilador y apagar las velas
        //esta secuencia puede varia deacuerdo a su ventilador configurela para su propio caso si este no se acopla al suyo
        fanOnOff.write(50);
        delay(300);
        fanOnOff.write(0);
        delay(300);
        fanOnOff.write(50);
        delay(300);
        fanOnOff.write(0);
        delay(300);
        fanOnOff.write(50);
        delay(300);
        fanOnOff.write(0);
        delay(4000);
        fanOnOff.write(50);
        delay(300);
        fanOnOff.write(0); 
        countPeak = 0;  
      }
      else
      {
        countPeak = 0; 
        fanOnOff.write(0); 
      }
    }
    //se toma el tiempo en cada iteracion para medir el intervalo de 2 segundos cuando se detecta un soplido y se verifica si la llama esta aun encendida
    ahora = millis();
    flame = digitalRead(candle);
  }
}
