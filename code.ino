/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char* ssid     = "Free Wifi";
const char* password = "reddy1980";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output2State = "off";
String output4State = "off";


// Assign output variables to GPIO pins
const int output2 = 2;
const int output4 = 4;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
//WiFiServer server(301); //just pick any port number you like

const long utcOffsetInSeconds = 20000;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
 int x = 0 ;
 int m = 0;
void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output2, OUTPUT);
  pinMode(output4, OUTPUT);

  // Set outputs to LOW
  digitalWrite(output2, LOW);
  digitalWrite(output4, LOW);


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  int x = 0 ;
  int m = 0;
}

void loop(){
 timeClient.update();
 int hours = timeClient.getHours();
 int minutes = timeClient.getMinutes();
 String autostate ;
  Serial.print(hours);
  Serial.print("   ");
  Serial.print(minutes);
    Serial.print("   ");


Serial.print(x);
  Serial.print("   ");
//**********************************************************************************************************************************************************************
  if(x == 1 && hours == 10 ){
    if(minutes < 5){
        digitalWrite(output2, HIGH);
   Serial.println("in");
      }else{
        x = 2;
        }
  }



  
  if(x == 2 ){
   digitalWrite(output2, LOW);
    Serial.println("out");
  }
  
  if(hours != 10 ){
   digitalWrite(output2, LOW);
    
  }

//**********************************************************************************************************************************************************************
  delay(1000);
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
//******************************************************************************************************************************************************************************************************************************      
       
            // turns the GPIOs on and off
            if (header.indexOf("GET /2/on/auto") >= 0) {
              Serial.println("GPIO 2 on");
              output2State = "ON";
              
//                digitalWrite(output2, HIGH);
                x = 1;
                Serial.println(x);
               
              
             
            } else if (header.indexOf("GET /2/off/auto") >= 0) {
              Serial.println("GPIO 2 off");
              output2State = "OFF";
//              digitalWrite(output2, LOW);
                x = 2;
                Serial.println(x);
                
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }
         



            
          
            // Display the HTML web page
            client.println("<!DOCTYPE html> <!-- saved from url=(0020)http://192.168.1.24/ --> <html nighteye=\"disabled\" class=\"gr__192_168_1_24\"> <head> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1252\" /> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /> <link rel=\"icon\" href=\"data:,\" /> <style> html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; } h1 { color: white; } .button { background-color: #222; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 10px; cursor: pointer; border-radius: 20px; } .buttonon { background-color: #222; border: none; color: rgb(37, 214, 31); padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 10px; cursor: pointer; border-radius: 20px; } .button2 { background-color: #77878a; } .onoff { margin-top: 10vh; } body { margin-top: 5vh; background-color: #000; } .auto { margin-top: 13vh; color: white; } .dropdown { position: relative; display: inline-block; } .dropdown-content { display: none; position: absolute; background-color: #222; min-width: 160px; box-shadow: 0px 8px 16px 0px rgba(0, 0, 0, 0.2); padding: 1px 10px; border-radius: 15px; z-index: 1; } .dropdown:hover .dropdown-content { display: block; } </style> </head>");
            client.println("<body data-gr-c-s-loaded=\"true\"> <h1>Watering System</h1>");
            client.println("<p class=\"auto\">A U T O :"+ output2State +"</p>");
            if (output2State=="OFF") {
              client.println("<p class=\"auto1\"><a href=\"/2/on/auto\"><button class=\"button\">A U T O</button></a></p>");
            } else {
              client.println("<p class=\"auto1\"><a href=\"/2/off/auto\"><button class=\"button buttonon\">A U T O</button></a></p>");
            } 
             if (output4State=="off") {
              client.println("<p class=\"auto1\"><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<pclass=\"auto1\"><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            
             
           
            client.println("</body></html>");


            
//***********************************************************************************************************************************************************************************************************************************************************            
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
