/*
 * cliente.ino
 * 
 * Sistemas Distribuidos
 *
 * Cristian Alberto Llanos Malca   <cristianllanos@outlook.com>
 * Gian Carlos Huachín Sairitupac  <ghuachins@gmail.com>
 * Jorge de la Cruz Espinoza       <jorgeluisdc.07@gmail.com>
 *
 * Programa que lee datos de temperatura, humedad y proximidad 
 * y los envia a un servidor web.
 *
 */

#include <SPI.h>
#include <Ethernet.h>

// Mac del EthernetShield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Descomentar esta linea si se desea un IP fijo
// IPAddress ip(192, 168, 1, 44);

// Descomentar esta linea para asignar un DNS fijo
//IPAddress myDns(172,17,17,1);

// Inicializa la instancia client
EthernetClient client;

// Direccion del servidor a conectarse
char host[] = "54.174.118.219";

// Variable de tiempo de la ultima conexion en milisegundos
unsigned long ultimaConexion = 0;          
// Estado de la ultima conexion
boolean ultimoEstado = false;
// Intervalo en milisegundos entre conexiones
const unsigned long intervaloConexion = 5000;   

// Pines de los sensores
int temperaturaPin = A0;
int humedadPin = A1;
int proximidadPin = A2;

//para temperatura
float temperaturaEnvio;
//para humedad
int humedadEnvio;
// para proximidad
float proximidadEnvio;



// Datos de envío
String datos = "";

void setup() {
  // Inicializa puerto serial
  Serial.begin(9600);
  Serial.println("#### Sistemas Distribuidos 2014 - I ####");
  Serial.println("Enviando datos a la nube...");
  // Espera 1 segundo para que se inicie la tarjeta Ethernet
  delay(1000);
  // Si deseas usar un ip fijo y un DNS fijo descomentar esta linea y comentar la linea 39
  // Ethernet.begin(mac, ip, myDns);
  // Inicializa la tarjeta ethernet mediante DHCP
  Ethernet.begin(mac);
  // Imprime la direccion IP de la tarjeta
  Serial.print("Direccion IP: ");
  Serial.println(Ethernet.localIP());
}
// Loop principal
void loop() {
  //para temperatura
  temperaturaEnvio = analogRead(temperaturaPin);
  temperaturaEnvio = temperaturaEnvio / (9.31*4);
  //para Humedad
  humedadEnvio = analogRead(humedadPin);
  //Para Distancia
  proximidadEnvio = analogRead(proximidadPin); // lectura del sensor 0
  proximidadEnvio = pow(3027.4 / proximidadEnvio, 1.2134); // conversión a centímetros
  
  // Si hay datos que llegan por la conexion los envia a la puerta serial
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // Si no hay conexion de red y se conecto correctamente la ultima vez
  // detiene el cliente Ehternet
  if (!client.connected() && ultimoEstado) {
    Serial.println();
    Serial.println("Fin del envio");
    client.stop();
  }

  // Si no esta conectado y han pasado X segundos (intervaloConexion) 
  // despues de la ultima conexion envia datos al servidor
  if(!client.connected() && (millis() - ultimaConexion > intervaloConexion)) {
    httpRequest();
  }
  // Actualiza la variable ultimoEstado 
  ultimoEstado = client.connected();
}
// Fin del loop principal

// Realiza la conexion http al servidor
void httpRequest() {
  // Se conecta al servidor en el puerto 80 (web)
  if (client.connect(host, 80)) {
    // Envia el dato al puerto serial 
    Serial.println("Datos: "); 
    Serial.println("Temperatura: " + (String)temperaturaEnvio); 
    Serial.println("Humedad: " + (String)humedadEnvio);
    Serial.println("Proximidad: " + (String)proximidadEnvio);
    // Envia el requerimiento al servidor via GET
    Serial.println("\nEnviando request...");
    datos = "temperatura=" + (String)temperaturaEnvio + "&humedad=" + (String)humedadEnvio + "&proximidad=" + (String)proximidadEnvio;
    client.print("GET /fb/php/get.php?");
    client.print(datos);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(host);
    client.println("User-Agent: Arduino-Ethernet");
    client.println("Connection: close");
    client.println();

    // Actualiza el tiempo en milisegundos de la ultima conexion
    ultimaConexion = millis();
  } 
  else {
    // Si la conexion fallo se desconecta
    Serial.println("Error al conectarse al servidor");
    Serial.println("Desconectando...");
    client.stop();
  }
}
