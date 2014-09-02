
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

// WiFi config
//char ssid[] = "TP-LINK_POCKET_3020_986ECC";     // your network SSID (wifi name) 
//char pass[] = "88635569";                       // your network password
char ssid[] = "JY-G4";
char pass[] = "91294371"; 
int status = WL_IDLE_STATUS;                    // the Wifi radio's status
WiFiUDP Udp;
unsigned int localPort = 8080;                  // local port to listen on
byte packetBuffer[5];                           // buffer to hold incoming packet
long last_sent = 0;
char servo1[13];
String str;

// Servo and ESC config
Servo tamiyaServo;                              // create servo object
Servo tamiyaESC;                                // create esc object
Servo tamiyaPan;                                // create pan object
Servo tamiyaTilt;                               // create tilt object

// sensor switching config
#define sensorPin A5
const int Bat = 8;
const int FC = 4;
int count=0;
int count1=0;

// LED indictor
#define LEDpin 1


void setup ()
{ 
  tamiyaServo.attach(9);
  tamiyaESC.attach(6);
  tamiyaPan.attach(3);
  tamiyaTilt.attach(5);
  //set pins as outputs / inputs


  pinMode(Bat,OUTPUT);
  pinMode(FC,OUTPUT);
  digitalWrite(Bat,LOW); // pin 8 
  digitalWrite(FC,HIGH); //pin 4

  
  // adjust Servo and ESC to it's neutral position
  adjust_neutral();
      
  // Initialize serial port
  Serial.begin(9600);
  
  // Check for presence of WiFi Shield
    if(WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");    
    // dont continue
    while(true);
  }  
  
  //attempt to connect to WiFi network:
  while(status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
    // connect to WPA/WPA2 network
    status = WiFi.begin(ssid,pass); 
    // wait 8 seconds for connection
    delay(1000);
  }
  
  // check wifi status
  Serial.println("Connected to wifi");
  checkWifiStatus();
  Serial.println("\nStarting connection to server...");
  
  // if get a connection, begin to recieve signal
  Udp.begin(localPort);  
  
}


void loop()
{
  digitalWrite(LEDpin,HIGH);
  // if there's data available, read packet
  
  int packetSize = Udp.parsePacket();
  int servo = packetBuffer[0];
  int esc = packetBuffer[1];
  int pan = packetBuffer[2];
  int tilt = packetBuffer[3];
  
  if (packetSize == NULL)
  { //tamiyaServo.detach();
    tamiyaPan.detach();
    tamiyaTilt.detach();

  };
  
  if (packetSize)
  {
    // read the packet into packetBuffer
    int len = Udp.read(packetBuffer,5);//len = maximum size of the buffer
    if (len > 0) packetBuffer[len] = 0;
    //if (packetBuffer[4]==1){
      
 
      //tamiyaServo.attach(9);
        //tamiyaPan.attach(3);
  //tamiyaTilt.attach(5);
      
      //tamiyaServo.write(map(servo,254,0,8,183));
  tamiyaServo.write(map(servo,0,254,180,0));
  tamiyaESC.write(map(esc,0,254,139,46));
  tamiyaPan.write(map(pan,254,0,30,150));
  tamiyaTilt.write(map(tilt,254,0,45,135));

    //}
    
    

    // print Servo and ESC state
    Serial.println("Data: ");
    Serial.print(esc);
    Serial.print("  -  ");
    Serial.print(servo);
    Serial.print("  -  ");
    Serial.print(pan);
    Serial.print("  -  ");
    Serial.print(tilt);  
    Serial.print("  -  ");
    Serial.print(analogRead(A5));

  Udp.beginPacket(Udp.remoteIP(), 8081);
  str=String(servo);
  str = str + pan + tilt + esc;
  str.toCharArray(servo1,13);
  Udp.write(servo1,13);
  Udp.endPacket();
  
      
  }
  
  
  //digitalWrite(switchPin, HIGH);
  //digitalWrite(Fc1,LOW);
  //}

  // set delay to prevent buffer overflow
  //delay(1);

}



// adjust Servo, ESC, Pan and Tilt to neutral state function
void adjust_neutral (void)
{
  tamiyaServo.write(map(127,0,254,180,0));
  tamiyaESC.write(map(127,0,254,46,139));
  tamiyaPan.write(map(127,254,0,30,150));
  tamiyaTilt.write(map(127,254,0,45,135));

}



//checking wifi status function
void checkWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}




