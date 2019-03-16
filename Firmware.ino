
// Include Libraries
#include "Arduino.h"
#include "SolenoidValve.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <Update.h>



// Set these to your desired credentials.
const char *ssid = "Spaceballesp32";
const char *password = "capstone";

WiFiServer server(80);
WebServer server2(81);

// Pin Definitions
#define SOLENOIDVALVE_1_PIN_COIL1  13
#define SOLENOIDVALVE_2_PIN_COIL1 12
#define SOLENOIDVALVE_3_PIN_COIL1 27
#define SOLENOIDVALVE_4_PIN_COIL1 33
//Movement Definitions
#define maxrolllefttoright 22
#define maxrollrighttoleft 35
#define maxpitchfronttoback 30
#define maxpitchbacktofront 22
#define centerfrompitchforward 12
#define centerfrompitchback 21
#define centerfromrollleft 16
#define centerfromrollright 10


// Global variables and defines

// object initialization
SolenoidValve solenoidValve_1(SOLENOIDVALVE_1_PIN_COIL1);
SolenoidValve solenoidValve_2(SOLENOIDVALVE_2_PIN_COIL1);
SolenoidValve solenoidValve_3(SOLENOIDVALVE_3_PIN_COIL1);
SolenoidValve solenoidValve_4(SOLENOIDVALVE_4_PIN_COIL1);

/*
 * Login page
 */

const char* loginIndex = 
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32 Login Page</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<td>Username:</td>"
        "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
"</script>";
 
/*
 * Server Index Page
 */
 
const char* serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

/*
 * setup function
 */

void rightroll(int j)
{ 
  int i = 0;
  while (i<j){
    solenoidValve_1.on(); // 1. turns on
    server.println(j);
    delay(10);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    server.println("Solenoid 1 OFF");
    solenoidValve_1.off();// 3. turns off
    delay(1);
    i=i+1;
  }
  
}
void leftroll(int j)
{ 
  int i = 0;
  while (i<j){
    solenoidValve_2.on(); // 1. turns on
    server.println(j);
    delay(10);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    server.println("Solenoid 2 OFF");
    solenoidValve_2.off();// 3. turns off
    delay(1);
    i=i+1;
  }
  
}
void pitchback(int j)
{ 
  int i = 0;
  while (i<j){
    solenoidValve_3.on(); // 1. turns on
    server.println(j);
    delay(10);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    server.println("Solenoid 3 OFF");
    solenoidValve_3.off();// 3. turns off
    delay(1);
    i=i+1;
  }
  
}
void pitchforward(int j)
{ 
  int i = 0;
  while (i<j){
    solenoidValve_4.on(); // 1. turns on
    server.println(j);
    delay(10);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    server.println("Solenoid 4 OFF");
    solenoidValve_4.off();// 3. turns off
    delay(1);
    i=i+1;
  }
  
}
void calibrate()
{
  //going all the way right then going to center from right
  rightroll(maxrolllefttoright);
   delay(100);
  leftroll(centerfromrollright);
  delay(1000);
  // going all the way back then back to center
  pitchback(maxpitchfronttoback);
  delay(100);
  pitchforward(centerfrompitchback);
  
  

}


// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(115200);
    //while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);


  
//  /*use mdns for host name resolution*/
//  if (!MDNS.begin(host)) { //http://esp32.local
//    Serial.println("Error setting up MDNS responder!");
//    while (1) {
//      delay(1000);
//    }
//  }
//  Serial.println("mDNS responder started");
//  /*return index page which is stored in serverIndex */

  server2.on("/", HTTP_GET, []() {
    server2.sendHeader("Connection", "close");
    server2.send(200, "text/html", loginIndex);
  });
  server2.on("/serverIndex", HTTP_GET, []() {
    server2.sendHeader("Connection", "close");
    server2.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server2.on("/update", HTTP_POST, []() {
    server2.sendHeader("Connection", "close");
    server2.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server2.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  
  server.begin();
  server2.begin();
  Serial.println("Server started");
  Serial.println("Web Server started");


}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
  
  
  WiFiClient client = server.available();   // listen for incoming clients
  server2.handleClient();
  
  delay(1);
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected

      if (client.available()) {             // if there's bytes to read from the client,
        
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H1\">here</a> to turn ON the Solenoid 1.<br>");
            //client.print("Click <a href=\"/L1\">here</a> to turn OFF the Solenoid 1.<br>");

            client.print("Click <a href=\"/H2\">here</a> to turn ON the Solenoid 2.<br>");
            //client.print("Click <a href=\"/L2\">here</a> to turn OFF the Solenoid 2.<br>");

            client.print("Click <a href=\"/H3\">here</a> to turn ON the Solenoid 3.<br>");
            //client.print("Click <a href=\"/L2\">here</a> to turn OFF the Solenoid 2.<br>");
            
            client.print("Click <a href=\"/H4\">here</a> to turn ON the Solenoid 4.<br>");
            //client.print("Click <a href=\"/L2\">here</a> to turn OFF the Solenoid 2.<br>");
            client.print("Click <a href=\"/H5\">here</a> to Calibrate.<br>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H1")) {
          solenoidValve_1.on(); // 1. turns on
          server.println("Solenoid 1 ON");
          delay(10);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
          server.println("Solenoid 1 OFF");
          solenoidValve_1.off();// 3. turns off
          //delay(1000);       // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        }
               
        if (currentLine.endsWith("GET /H2")) {
          solenoidValve_2.on(); // 1. turns on
          server.println("Solenoid 2 ON");
          delay(10);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
          server.println("Solenoid 2 OFF");
          solenoidValve_2.off();// 3. turns off
          //delay(1000);       // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        }

        if (currentLine.endsWith("GET /H3")) {
          solenoidValve_3.on(); // 1. turns on
          server.println("Solenoid 3 ON");
          delay(10);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
          server.println("Solenoid 3 OFF");
          solenoidValve_3.off();// 3. turns off
          //delay(1000);       // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        }

        if (currentLine.endsWith("GET /H4")) {
          solenoidValve_4.on(); // 1. turns on
          server.println("Solenoid 4 ON");
          delay(10);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
          server.println("Solenoid 4 OFF");
          solenoidValve_4.off();// 3. turns off
          //delay(1000);       // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        }
        if (currentLine.endsWith("GET /H5")) {
          calibrate();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
********************************************************/
