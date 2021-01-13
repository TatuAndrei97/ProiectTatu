#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char* ssid     = "Tenda_3E3D48"; // nume si parola retelei wifi
const char* password = "12345678";
// Setare web server port la 80
WiFiServer server(80);

String header;
String output15State = "OPRIT";
// timp curent
unsigned long currentTime = millis();
// timp precedent
unsigned long previousTime = 0; 
// Definire timeout time in millisecunde ( 2000ms = 2s)
const long timeoutTime = 2000;

int Lane1[] = {14,12,13}; // Lane 1 Red, Yellow and Green
int Lane2[] = {4,16,5};     // Lane 2 Red, Yellow and Green 
const int button = 15;            // GPIO 15 pentru button
const int led =2;                // GPIO 2 pentru LED
int ledflag=0;                   // LED status flag

void setup() {
    Serial.begin(115200);
// Conectare  la  Wi-Fi cu SSID si password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //afisare IP  si pornire server  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  pinMode(button,INPUT);         // definere button ca intrare
  pinMode(led,OUTPUT);           // definire LED ca iesire
  digitalWrite(led,LOW);         // La pornire ledul sa fie stins
  
      for (int i = 0; i < 3; i++)
      
      {
      
      pinMode(Lane1[i], OUTPUT);          // atribuire valori pentru culori (rosu=0 , galben=1 si verde=2)
      pinMode(Lane2[i], OUTPUT);
      
      
      }
      
      for (int i = 0; i < 3; i++)
      
      {
      
      digitalWrite(Lane1[i], LOW);
      digitalWrite(Lane2[i], LOW);
      
      }
}

void loop() {
WiFiClient client = server.available();   // scanare clienti ce intra

  if (client) {                             // daca exista un nou client 
    Serial.println("New Client.");          // printeaza mesajul 
    String currentLine = "";                // creare sir cu date despre client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // cat timp clientul este conectat se realizeaza
      currentTime = millis();         
      if (client.available()) {             // daca exista biti de citit 
        char c = client.read();             // cirere bit
        Serial.write(c);                    // afisare
        header += c;
        if (c == '\n') {                    // daca bitu este caracter nou
          // daca linia curenta este goala, exista 2 caractere noi la rand
          //Sfarsitul clientului HTTP,trimitere un raspuns
          if (currentLine.length() == 0) {
            // Anteturile HTTP încep întotdeauna cu un cod de răspuns (de exemplu, HTTP / 1.1 200 OK)
            // și un tip de conținut, astfel încât clientul să știe ce urmează, apoi o linie goală:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

 while(digitalRead(button)==  LOW){   // cat timp butonul nu este apasat semafoarele ruleaza normal
 TraficCycle();
 }
 if (digitalRead(button) == HIGH){
        delay(15); //     daca butonu este apasat  schimbare status led din 0 in 1 si aprinderea culorii rosi la semafor+ led de urgenta
        if (digitalRead(button) == HIGH) {        
          ledflag=1;  
          Sos_button_is_push();

          

       
        }
  
   else { 
   ledflag=0;  
   digitalWrite(Lane1[0],   LOW);   //daca nu este apasat , status-ul ledului ramane 0 si luminile de urgenta inchise
   digitalWrite(Lane2[0], LOW);
   digitalWrite(led, LOW);
   }
    
    }
 // Afisare pagina web
            client.println("<!DOCTYPE html><html>"); // declarere faptului ca este un document html
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS  design-ul pentru pagina web si buton
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Titlu pagina web
            client.println("<body><h1>Tatu Andrei Proiect Intersectie</h1>");
            
            // AFisare buton de regenerare a functionarii normale a semaforului
            client.println("<p>Traficul este: " + output15State + "</p>");
            // daca traficul este oprit   
           if(output15State=="OPRIT") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">REPORNESTE</button></a></p>");
                 digitalWrite(Lane1[0],   HIGH);
                 digitalWrite(Lane2[0], HIGH);
                 digitalWrite(led, HIGH);
                 delay(8000);
                 digitalWrite(Lane1[0],   LOW);
                 digitalWrite(Lane2[0], LOW);
                 digitalWrite(led, LOW);
                
             
           } else {
              client.println("<p><a href=\"/2/OPRIT\"><button class=\"button button2\">OPRIT</button></a></p>");
              
                 
            } 
              
            client.println();
            // iesire din bucla
            break;
          } else { // dacă ai o nouă linie, atunci șterge Linia curentă
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      // adaugare la final de linie curenta
        }
      }
    }
    // Curatare variabile header
    header = "";
    //inchidere conexiune
  
    client.stop();
    
    Serial.println("Client disconnected.");
    Serial.println("");
  }

    
 }
        

void TraficCycle() {
            
              digitalWrite(Lane1[2], HIGH); //functionalitate semafoare in sincron
              digitalWrite(Lane2[0], HIGH);
              
              delay(7000);
         
              digitalWrite(Lane1[2], LOW);
              digitalWrite(Lane2[0], HIGH);
              digitalWrite(Lane1[1], HIGH);

              delay(3000);
                       
              digitalWrite(Lane1[1], LOW);
              digitalWrite(Lane2[0], LOW);
              digitalWrite(Lane1[0], HIGH);
              digitalWrite(Lane2[2], HIGH);
              digitalWrite(Lane1[2], LOW);
              
              delay(3000);
              
              digitalWrite(Lane1[0], HIGH);
              digitalWrite(Lane2[2], LOW);
              digitalWrite(Lane1[1], LOW);
              digitalWrite(Lane2[1], HIGH);
              
              delay(3000);
             
              digitalWrite(Lane1[0], LOW);
              digitalWrite(Lane2[1], LOW);
              digitalWrite(Lane1[1], LOW);
              
  }
 void Sos_button_is_push(){
  
   digitalWrite(Lane1[0], HIGH);
   digitalWrite(Lane2[0], HIGH); // aprindere lumini de urgenta in cazul in care buton este apasat
   digitalWrite(led, HIGH);

 }
 
