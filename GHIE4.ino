#include <SPI.h>
#include <WiFiNINA.h>

//Our pin #'s assigned to a variable
const int x = 3;

//Our changed variables
int x_counter = 0;
int x_state = 0;
int x_last_state = 0;

char ssid[] = "TDSV1000W1280";      //network's id
char pass[] = "PT5GTTJT3NKY6N6Z";   //network's password
int keyIndex = 0;                   //is Greenheck WEP?

int status = WL_IDLE_STATUS;
WiFiServer server{80};              //80 is for wifi, not ethernet

//special characters
char quote = '"';
char slash = '/';

void setup() {

Serial.begin(9600);

  //DIGITAL PINS FOR LED
  pinMode(1, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(6, OUTPUT);

  //Read Values
  pinMode(x, INPUT_PULLUP); 
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

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
  server.begin();         //start the web server 
}


void loop() {

  x_state = digitalRead(x);

  //compare states (display button press count when pressed)
  if (x_state != x_last_state) {
    if(x_state == 0) {
      x_counter++;
      Serial.println("on");
      Serial.print("The button has been pressed: ");
      Serial.println(x_counter);
      Serial.print(" times");
    }else {
      Serial.println("off");
    }
    delay(50);
  }
  x_last_state = x_state;

  if (x_counter %2 == 0) {
    Serial.print("OFF");
  }else {
    Serial.print("ON");
  }
  


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
            client.print(x_last_state);
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
          digitalWrite(6, HIGH);                 //Get /O turns 7 on
          }
        if (currentLine.endsWith("GET /F"))  {
          digitalWrite(6, LOW);                  //Get /F turns 7 off
          }

        //Pin outputs for HEAT
        if (currentLine.endsWith("GET /H"))  {
          digitalWrite(0, HIGH);                //Get /H turns 6 on
          }
        if (currentLine.endsWith("GET /C"))  {
          digitalWrite(0, LOW);                 //Get /C turns 6 off
          }

        //Pin outputs for LOCK
        if (currentLine.endsWith("GET /L"))  {
          digitalWrite(1, HIGH);                //Get /L turns 5 on
        }
        if (currentLine.endsWith("GET /U"))  {
          digitalWrite(1, LOW);                 //Get /U turns 5 off
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
