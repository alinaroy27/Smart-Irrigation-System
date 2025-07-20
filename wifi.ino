#include <ESP8266WiFi.h>
#define soil_moisture_pin A0

const char* ssid = "Alina";
const char* password = "123456789";

int ledPin = 13; // GPIO13 on some boards, D7 on others
WiFiServer server(80);
bool pumpStatus = false;
//bool autoControl=false;
int sensor_ctrl_pin=4;

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(ledPin, OUTPUT);
  pinMode(sensor_ctrl_pin,OUTPUT);
  digitalWrite(ledPin,HIGH);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Display the IP address

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  digitalWrite(sensor_ctrl_pin,HIGH);
  int soilMoistureValue = analogRead(soil_moisture_pin);
  Serial.print("Soil Moisture Sensor Value: ");
  Serial.println(soilMoistureValue);
  
  digitalWrite(sensor_ctrl_pin,LOW);
  

  WiFiClient client = server.available();
  if (client) {
    // Wait until the client sends some data
    Serial.println("New client connected");
    while (!client.available()) {
      delay(1);
    }

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Match the reques
    
    if(soilMoistureValue < 750)
    {
      digitalWrite(ledPin,HIGH);
       
    }
    if (request.indexOf("/PUMP=ON") != -1)  
    {
        //digitalWrite(ledPin, HIGH);
        pumpStatus = true;
        Serial.print(" Pump Status :"+String(pumpStatus));
    }
    if (request.indexOf("/PUMP=OFF") != -1)  
    {
        //digitalWrite(ledPin, LOW);
        pumpStatus = false;
        Serial.print(" Pump Status :"+String(pumpStatus));
    }
    
    
    

    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); // do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<title>SMART IRRIGATION SYSTEM</title>");
    client.println("<script>setTimeout(() => {window.location.reload();}, 7000);</script>");
    client.println("<style>");
    client.println("body {");
    client.println("  margin: 0;");
    client.println("  padding: 0;");
    client.println("  font-family: Verdana, Geneva, Tahoma, sans-serif;");
    client.println("}");
    client.println(".box {");
    client.println("  font-size: 5vw;");
    client.println("  color: green;");
    client.println("  font-weight: bolder;");
    client.println("  text-decoration: underline;");
    client.println("  text-decoration-color: black;");
    client.println("  padding: 20px;");
    client.println("  text-align: center;");
    client.println("}");
    client.println(".container {");
    client.println("  border: 3px solid black;");
    client.println("  border-radius: 10px;");
    client.println("  font-size: 4vw;");
    client.println("  margin: auto;");
    client.println("  padding: 20px;");
    client.println("  background-color: blanchedalmond;");
    client.println("  max-width: 90%;");
    client.println("  box-sizing: border-box;");
    client.println("}");
    client.println(".box1 {");
    client.println("  margin-bottom: 20px;");
    client.println("}");
    client.println(".box2 {");
    client.println("  margin-bottom: 20px;");
    client.println("}");
    client.println(".buttons {");
    client.println("  display: flex;");
    client.println("  justify-content: space-around;");
    client.println("}");
    client.println(".button_size {");
    client.println("  height: 50px;");
    client.println("  width: 150px;");
    client.println("  border-radius: 20px;");
    client.println("  font-size: 2vw;");
    client.println("}");
    client.println("@media (min-width: 768px) {");
    client.println("  .box { font-size: 3vw; }");
    client.println("  .container { font-size: 2vw; max-width: 70%; }");
    client.println("  .button_size { font-size: 1.5vw; }");
    client.println("}");
    client.println("@media (min-width: 1024px) {");
    client.println("  .box { font-size: 2.5vw; }");
    client.println("  .container { font-size: 1.5vw; max-width: 50%; }");
    client.println("  .button_size { font-size: 1vw; }");
    client.println("}");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<div class=\"box\">SMART IRRIGATION SYSTEM</div>");
    client.println("<div class=\"container\">");
    client.println("<div class=\"box1\">Soil Moisture Sensor Value is: " + String(soilMoistureValue) + "</div>");
    
    if (soilMoistureValue < 300 ) {
      
      client.println("<div class=\"box1\">Soil Moisture Status: Soil is Wet</div>");

    } 
    else if (soilMoistureValue >= 300 && soilMoistureValue <= 750) 
    {
      
      client.println("<div class=\"box1\">Soil Moisture Status: Soil is Semi Wet</div>");
    } 
    else if (soilMoistureValue > 750 && pumpStatus) 
    {
      digitalWrite(ledPin, LOW);
      client.println("<div class=\"box1\">Soil Moisture Status: Soil is Dry</div>");
      delay(2000);
      digitalWrite(ledPin, HIGH);
    }
    else if (soilMoistureValue > 750 && !pumpStatus) 
    {
      client.println("<div class=\"box1\">Soil Moisture Status: Soil is Dry , Initiate Automated Irrigation !</div>");
    }
    

    if (pumpStatus) {
      client.println("<p class=\"box2\">Automated Irrigation Initiated</p>");
    } 
    else 
    {
      client.println("<p class=\"box2\">Automated Irrigation State Idle</p>");
    }
    
    client.println("<p class=\"box_ledON\"><a href=\"/PUMP=ON\"><button class=\"button_size\">Turn On</button></a></p>");
    client.println("<p class=\"box_ledOFF\"><a href=\"/PUMP=OFF\"><button class=\"button_size\">Turn Off</button></a></p>");
    client.println("</div>");
    client.println("</body>");
    client.println("</html>");

    delay(1);
    Serial.println("Client disconnected");
    client.stop();
  }
  delay(5000);
}
