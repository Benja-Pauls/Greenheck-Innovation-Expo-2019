#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

#include <SPI.h>
#include <WiFiNINA.h>

//variables
int b_power_v = 0;
int b_heat_v = 0;
int s_lock_v = 0;
int s_lock_v2 = 0; // virtual lock
int filter = 0;
int x = 0;
int y = 0;
int z = 0;
int x2 = 0;
int y2 = 0;
int z2 = 0;
int x_c = 0;
int x_s = 0;
int x_ls = 0;
int y_c = 0;
int y_s = 0;
int y_ls = 0;


//relay
int r_power = A1;
int r_heat = A0;

//buttons and switches
int p_b_power = 3;
int p_b_heat = 4;
int p_s_lock = 5;

char ssid[] = "TDSV1000W1280";      //network's id
char pass[] = "PT5GTTJT3NKY6N6Z";   //network's password
int keyIndex = 0;                   

int status = WL_IDLE_STATUS;
WiFiServer server{80};              //80 is for wifi, not ethernet

//special characters
char quote = '"';
char slash = '/';

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16,2);


pinMode(A1,OUTPUT);
pinMode(A0, OUTPUT);

  pinMode(p_b_power, INPUT_PULLUP);
  pinMode(p_b_heat, INPUT_PULLUP);
  pinMode(p_s_lock, INPUT_PULLUP);



  //check for shield
  if (WiFi.status() == WL_NO_SHIELD)  {
    Serial.println("WiFi shield not present");
    while (true);   //don't continue
  }

  //attempt to connect to the network

  while ( status != WL_CONNECTED) {
    Serial.println("Attempting to connect to Network named: ");
    Serial.println(ssid);     //printing the network name

    //If Greenheck is a WEP network change these lines:
    status = WiFi.begin(ssid, pass);
    //wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();   
  printWiFiStatus();//start the web server
}

void loop() {
  x_s = digitalRead(p_b_power);
  s_lock_v2 = 0;
 
//power
    if(x_s == 0){
      x_c++;
      delay(1000);
    }
    delay(50);
    
 if(x_c == 1){
  if(s_lock_v2 == 0) digitalWrite(r_power, HIGH);
 }
 else digitalWrite(r_power, LOW);
 if(x_c == 2){
  x_c--;
  x_c--;
 }


//heat
  y_s = digitalRead(p_b_heat);

 
  //compare
    if(y_s == 0)
      if(x_c == 1){
      y_c++;
      delay(1000);
    }
    delay(50);
    
 if(y_c == 1){
  if(s_lock_v2 == 0) digitalWrite(r_heat, HIGH);
 }
 else digitalWrite(r_heat, LOW);
 if(y_c == 2){
  y_c--;
  y_c--;
 }

s_lock_v = digitalRead(p_s_lock);

lcd.setCursor(0,0);
if(s_lock_v2 == 1) 
lcd.print("Lock = ON");
else lcd.print("Lock = OFF");



  //attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);   //printing the network name

    //If Greenheck is a WEP network change these lines:
    status = WiFi.begin(ssid, pass);
    //wait 10 seconds for connection:
    delay(10000);
  }

  WiFiClient client = server.available();   //listen for incoming clients (buttons on server being pressed)

  if (client) {                     //if a client is heard,
    Serial.println("new client");   //print a message
    String currentLine = "";        //make a string to hold incoming data
    while (client.connected()) {    //loop while the client is connected
      if (client.available()) {     //if there's bytes to read from the client,
        
        char c = client.read();     //read a byte, then
        Serial.write(c);            //print it out (in SM)
        if (c == '\n') {            //if the byte is a newline character...

          //if the current line is blank, you got two newline characters in a row,
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            //HTTP headers always start with a response code (HTTP/1.1 200 OK)
            //and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");        //print HTTP format
            client.println("Content-type:text/html");
            client.println();
            
            //The content of the hTTP response follows the header:
            client.println("<!DOCTYPE HTML>");

            client.print("<head>");
            client.print("<link rel=");
            client.print(quote);
            client.print("stylesheet");
            client.print(quote);
            client.print("href=");
            client.print(quote);
            client.print("https://ioio.mah.se/courses/IDK17/student_007/mkrsheet.css"); //our HTTP style sheet
            client.print(quote);
            client.print(slash);
            client.print(">");
            
            //POWER
            client.print("</head>");
            client.print("<body>");
            client.println("<center><br><br><div class='container'><h1>POWER<h1/></div></center>");
            client.println("<center><div class='container'><left><button class='on' type='submit' value='ON' onmousedown=location.href='/O\'>ON</button>");
            client.println("<button class='off' type='submit' value='OFF' onmousedown=location.href='/F\'>OFF</button></div><br>");
            client.println("</body>");
            client.println("</html>");
            
            //HEAT
            client.print("</head>");
            client.print("<body>");
            client.println("<center><br><br><div class='container'><h1>HEAT<h1/></div></center>");
            client.println("<center><div class='container'><left><button class='on' type='submit' value='ON' onmousedown=location.href='/H\'>ON</button>");
            client.println("<button class='off' type='submit' value='OFF' onmousedown=location.href='/C\'>OFF</button></div><br>");
            client.println("</body>");
            client.println("</html>");
           
            //LOCK
            client.print("</head>");
            client.print("<body>");
            client.println("<center><br><br><div class='container'><h1>LOCK<h1/></div></center>");
            client.println("<center><div class='container'><left><button class='on' type='submit' value='ON' onmousedown=location.href='/L\'>ON</button>");
            client.println("<button class='off' type='submit' value='OFF' onmousedown=location.href='/U\'>OFF</button></div><br>");
            client.println("</body>");
            client.println("</html>");

            client.println("<html>");
          // output the value of each digital input pin
            for (int digitalChannel = 3; digitalChannel < 4; digitalChannel++) {
            int sensorReading = digitalRead(digitalChannel);
            client.print("Digital Input ");
            client.print("3");
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />"); }

            client.println("<html>");
          // output the value of each digital input pin
            for (int digitalChannel = 8; digitalChannel < 9; digitalChannel++) {
            int sensorReading = digitalRead(digitalChannel);
            client.print("Digital Input ");
            client.print(digitalChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />"); }

            client.println("<html>");
          // output the value of each digital input pin
            for (int digitalChannel = 9; digitalChannel < 10; digitalChannel++) {
            int sensorReading = digitalRead(digitalChannel);
            client.print("Digital Input ");
            client.print(digitalChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />"); }

            client.println("<html>");
          // output the value of each digital input pin
            for (int digitalChannel = 10; digitalChannel < 11; digitalChannel++) {
            int sensorReading = digitalRead(digitalChannel);
            client.print("Digital Input ");
            client.print(digitalChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />"); }

            client.println("<html>");
          // output the value of each digital input pin
            for (int digitalChannel = 11; digitalChannel < 12; digitalChannel++) {
            int sensorReading = digitalRead(digitalChannel);
            client.print("Digital Input ");
            client.print(digitalChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />"); }

            //The HTTP response ends with another blank line:
            client.println();
            break;
            }
        
          
          else {    //if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {   //if you got anything else but a carriage return character,
          currentLine += c;     //add it to the end of the currentLine
        }

        //Pin outputs for POWER
        if (currentLine.endsWith("GET /O"))  {
          if (s_lock_v == 0) {
           x_c++;
           delay(1000);
          }                             //Get /O turns 3 on
        }
        if (currentLine.endsWith("GET /F"))  {
          if(s_lock_v == 0){
          if(x_c == 1)
          x_c++;                        //Get /F turns 3 off
          delay(1000);
          }
        }

        //Pin outputs for HEAT
        if (currentLine.endsWith("GET /H"))  {
          if (s_lock_v == 0) {
            if(x_c == 1) y_c++;
          }                             //Get /H turns 4 on
        }
        if (currentLine.endsWith("GET /C"))  {
         if(s_lock_v == 0)
         if(y_c == 1) y_c++;            //Get /C turns 4 off
        }

        //Pin outputs for LOCK
        if (currentLine.endsWith("GET /L"))  {
          s_lock_v2++;
          //Get /L turns 5 on
        }
        if (currentLine.endsWith("GET /U"))  {
          s_lock_v2--;                //Get /U turns 5 off
        }

      }
    }
    //close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
void printWiFiStatus()  {
  //print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  //print the MKR's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  //print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signla Strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");

  //print where to go in browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
