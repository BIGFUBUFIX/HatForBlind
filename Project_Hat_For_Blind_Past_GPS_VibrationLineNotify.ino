#include <WiFiManager.h> //ไลบารี่จัดการไวไฟ
#include <ArduinoJson.h> //ไลบารี่เจสัน(Json)
#include <ESP8266WiFi.h> //ไลบารี่ไวไฟของ NodeMCU ESP8266
#include <PubSubClient.h> //ไลบารี่ในการเชื่อมต่อ เอ็มคิวทีที(MQTT)
#include <TinyGPS++.h> //ไลบารี่จีพีเอส
#include <SoftwareSerial.h> //ไลบารี่ในการสื่อสารซีเรียล
#include <TridentTD_LineNotify.h> //ไลบารี่ในการใช้งาน LineNotify 

#define LINE_TOKEN  "i7MSDZi2TdeHwzzXCSbaO2NGA4VCUFFy3GHg2EectEH" //กำหนดโทเค็นของ LineNotify
#define Vibra D7 //ประกาศตัวแปร
WiFiManager wm ; //ประกาศตัวแปรของ WiFiManager
const char* mqtt_server = "broker.mqtt-dashboard.com"; //กำหนด MQTT Server
//กำหนด Topic SS
const char* outTopic = "gps/nodered"; //ประกาศตัวแปร Topic
const char* inTopic = "gps/nodered"; //ประกาศตัวแปร Topic
const char* clientName = ""; //ประกาศตัวแปรไคลเอนต์
String sChipID; //ประกาศตัวแปร
char cChipID[20]; //ขนาดของข้อความ 20 ตัวอักษร
WiFiClient espClient; //ประกาศไคลเอนต์ไวไฟ
PubSubClient client(espClient); //เรียกใช้งานไคลเอนต์
long lastMsg = 0; //ประกาศตัวแปรและเก็บค่า 0 
char msg[50]; //ขนาดของข้อความ 50 ตัวอักษร

//ใช้งาน json
StaticJsonDocument <200> doc;
DeserializationError error;
JsonObject root;

static const int RXPin = 14, TXPin = 12; //ประกาศขา Tx , Rx ของจีพีเอส
static const uint32_t GPSBaud = 9600; //กำหนดความเร็วในการส่งค่าของจีพีเอส ความเร็ว 9600
double lat,lon; //ประกาศตัวแปร
unsigned long previousMillis = 0;  //ประกาศตัวแปรและเก็บค่า 0  
const long interval = 5000; //ประกาศตัวแปรและเก็บค่า 5000       

TinyGPSPlus gps; //ประกาศตัวแปรจีพีเอสในการเรียกใช้ gps

SoftwareSerial ss(RXPin, TXPin); //ใช้งานขา Tx และ Rx ส่งข้อมูลออกซีเรียล

void setup() {
  pinMode(0, INPUT_PULLUP) ; //กำหนดอินพุตสัญญาณเป็น 1
  Serial.begin(115200); //ใช้การสื่อสารแบบ ซีเรียล ความเร็ว 115200
  //setup_wifi();
  if(wm.autoConnect("WiFi_Hat_ESP8266")){ //ถ้ามีการเชื่อมต่อไวไฟจะทำงานตามเงื่อนไข
    Serial.print(""); //เว้นบรรทัด
    Serial.println("Connected already WiFi :") ; //แสดงข้อความ Connected already WiFi : ออกซีเรียลมอนิเตอร์
    Serial.println("IP Address :") ; //แสดงข้อความ IP Address : ออกซีเรียลมอนิเตอร์
    Serial.println(WiFi.localIP());  //แสดงเลขไอพีแอดแดรส ออกซีเรียลมอนิเตอร์
    
    }else{ //หากไม่มีการเชื่อมต่อ WiFi จะแสดงข้อความ Failed to connect WiFi และ Restart Esp8266
      Serial.println("Failed to connect WiFi") ; //แสดงข้อความ Failed to connect WiFi ออกซีเรียลมอนิเตอร์
      delay(1000) ; //หน่วงเวลา 1 วินาที
      ESP.restart() ; //รีสตาร์ท NodeMcu ESP8266 
    }
  ss.begin(GPSBaud); 
  Serial.println(LINE.getVersion()); //แสดงข้อความเวอร์ชั่นของLineNotify ออกซีเรียลมอนิเตอร์
  //Set Line Token
  LINE.setToken(LINE_TOKEN); //เชื่อมต่อ โทเค็น(Token) LineNotify

  sChipID=String(ESP.getChipId(),HEX); //เข้ารหัสชิปไอดีเป็นเลขฐาน 16 เก็บในตัวแปร sChipID
  sChipID.toCharArray(cChipID,sChipID.length()+1); //แปลงค่าชิปไอดี
  clientName=&cChipID[0]; //ประกาศตัวแปรเพื่อเก็บชิปไอดี
  Serial.println(clientName); //แสดงค่าclientName ออกซีเรียลมอนิเตอร์
  client.setServer(mqtt_server, 1883); //กำหนดเซิฟเวอร์ของเอ็มคิวทีที(MQTT) ที่พอร์ต 1883
  client.setCallback(callback); //เซ็ตไคลเอนต์ callback
}
//Function callback จะมีการแสดง Message หรือ Payload และ Topic ที่ Client Subscribe Topic ที่กำหนดไว้
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived ["); //แสดงข้อความ Message arrived [ ออกซีเรียลมอนิเตอร์
  Serial.print(topic); //แสดงค่า Message arrived [ ออกซีเรียลมอนิเตอร์
  Serial.print("] "); //แสดงข้อความ [ ออกซีเรียลมอนิเตอร์
  for (int i = 0; i < length; i++) { //ให้มีการวนลูป
    Serial.print((char)payload[i]); //แสดงค่า payload ออกซีเรียลมอนิเตอร์
  }
  Serial.println(); //เว้นบรรทัด
}
/*Function Reconnect เมื่อมีการ Loop การเชื่อมต่อเอ็มคิวทีที(MQTT) ใหม่อีกครั้ง Client จะ publish ไปยัง OutTopic และ Subscribe ไปยัง InTopic 
หากเชื่อมต่อไม่สำเร็จจะ Reconnect ใหม่ทุกๆ 5 วินาที
*/
void reconnect() {
  while (!client.connected()) { //ในขณะที่มีการเชื่อมต่อไคลเอนต์
    Serial.print("Attempting MQTT connection..."); //แสดงข้อความ Attempting MQTT connection... ออกซีเรียลมอนิเตอร์
    if (client.connect(clientName)) { //ถ้ามีการเชื่อมต่อไคลเอนต์
      Serial.println("connected"); //แสดงข้อความ connected ออกซีเรียลมอนิเตอร์
      client.publish(outTopic, "Reconnected"); //Publish ข้อความ Reconnected ไปยัง Topic outTopic
      client.subscribe(inTopic); //Subscribe ไปยัง Topic inTopic
    } else { //ในขณะที่ไม่มีการเชื่อมต่อไคลเอนต์
      Serial.print("failed, rc="); //แสดงข้อความ failed, rc= ออกซีเรียลมอนิเตอร์
      Serial.print(client.state()); //แสดงสถานะของไคลเอนต์ออกซีเรียลมอนิเตอร์
      Serial.println(" try again in 5 seconds"); //แสดงข้อความ try again in 5 seconds ออกซีเรียลมอนิเตอร์
      delay(5000); //หน่วงเวลา 5 วินาที
    }
  }
}
void displayInfo() //ฟังก์ชั่นในการแสดงค่าละติจูด และลองจิจูด
{
  
  if (gps.location.isValid())//หากมีค่าจากจีพีเอส
  {
    lat=gps.location.lat(); //เก็บค่าละติจูดที่อ่านค่าได้ในตัวแปร lat
    lon=gps.location.lng(); //เก็บค่าลองจิจูดที่อ่านค่าได้ ในตัวแปร lon
  }
  else //หากไม่มีค่าจากจีพีเอส
  {
    Serial.print(F("INVALID"));  //แสดงข้อความ INVALID ออกซีเรียลมอนิเตอร์
  }
}
void loop() {
 
  if (!client.connected()) { //หากมีการเชื่อมต่อไคลเอนต์
    reconnect(); //ใช้งานฟังก์ชัน reconnect
  }
  client.loop();//ให้ไคลเอนต์มีการทำงานวนลูปไปเรื่อยๆ
  VibrationSensor() ;  //เรียกใช้งาน
  delay(1);
  while (ss.available() > 0)//เมื่อมีค่าจีพีเอสจะทำการอ่านค่าและเข้ารหัสเพื่อนำค่าจีพีเอสไปใช้งาน
    gps.encode(ss.read()); //อ่านค่าและเข้ารหัสค่าจีพีเอส
  unsigned long currentMillis = millis(); //ประกาศตัวแปรโดยมีฟังก์ชั่นมิลลิ
  
  if (currentMillis > 5000 && gps.charsProcessed() < 10)//เมื่อไม่มีมีค่าจากจีพีเอส
  {
    Serial.println(F("No GPS detected: check wiring.")); //แสดงข้อความ No GPS detected: check wiring. ออกซีเรียลมอนิเตอร์
    while(true); //เมื่อมีค่าเป็นจริง
  }
  if (currentMillis - previousMillis >= interval) { //ถ้ามีการนับเวลาในการทำงานจะทำงานตามเงื่อนไข
      previousMillis = currentMillis; //ให้เวลาก่อนหน้าเท่ากับเวลาปัจจุบัน
       
      displayInfo();//ฟังก์ชันในการแสดงค่าจีพีเอส
      if (client.connected()) {//เมื่อไคลเอนต์มีการเชื่อมต่อ เอ็มคิวทีที(MQTT)
        sprintf (msg, "\{\"lat\":%.5f,\"lon\": %.5f}",lat, lon); //ให้ค่าละติจูดและลองจิจูดมีเลขทศนิยม 5 ตำแหน่งและเก็บค่าใส่ตัวแปร msg
        Serial.println(msg); //แสดงค่า msg ออกซีเรียลมอนิเตอร์
        client.publish(outTopic, msg); //ทำการ publish msg ไปยัง Topic ที่ Subscribe ไว้
      }
  }
  resetwifi() ;  //เรียกใช้งาน Function ResetWiFi
}
void resetwifi() { //ฟังก์ชันในการรีเซ็ต WiFi ที่ NodeMCU ESP8266 เชื่อมต่ออยู่ทั้งหมด
  if(digitalRead(0) == LOW){ //หากมีการกดปุ่ม FLASH บนบอร์ด NodeMCU ESP8266 จะทำการ Reset WiFi
    Serial.println("Reset WiFi"); //แสดงข้อความ Reset WiFi ออกซีเรียลมอนิเตอร์
    wm.resetSettings() ;//Reset ค่าที่เคยเชื่อมต่อไว้ทั้งหมด
    delay(10) ; //หน่วงเวลาไว้ 0.01 วินาที
  }
}

void VibrationSensor(){ //Function ในการส่งการแจ้งเตือนเมื่อหมวกมีการตกหล่นผ่าน LineNotify
  int VibraValue = digitalRead(Vibra) ; //อ่านค่าดิจิตอลของตัวแปร Vibra
  Serial.println(VibraValue) ; //แสดงค่าVibraValue ออกซีเรียลมอนิเตอร์
  //ถ้า VibraValue มีค่าเป็น Digital Logic == 1 จะทำการส่งข้อความแจ้งเตือนไปยัง LineChat
  if(VibraValue == 1){ //ถ้าค่าดิจิตอลมีค่าเป็น 1 จะทำตามเงื่อนไข
     LINE.notify("  หมวกมีการสั่นสะเทือน  "); //ส่งข้อความ หมวกมีการสั่นสะเทือน ผ่านไลน์
     delay(10) ; //หน่วงเวลา 0.01 วินาที
     
  }
}
