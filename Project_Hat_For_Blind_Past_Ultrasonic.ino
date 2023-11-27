unsigned long time1;  //ประกาศตัวแปร
int i = 150 ;         //ประกาศตัวแปร และให้เก็บค่า 150
int j = 200 ;         //ประกาศตัวแปร และให้เก็บค่า 200
int new_delay1;       //ประกาศตัวแปร

//เซนเซอร์อัลตร้าโซนิคตัวที่ 1 
const int tingPin1 = D1; //ประกาศตัวแปร Trig ที่ขา D1
int echoPin1 = D2;       //ประกาศตัวแปร Echo ที่ขา D2
int buzzerpin1 = D0;     //ประกาศตัวแปร Buzzer ที่ขา D0 
//เซนเซอร์อัลตร้าโซนิคตัวที่ 2 
const int tingPin2 = D3; //ประกาศตัวแปร Trig ที่ขา D3
int echoPin2 = D4;       //ประกาศตัวแปร Echo ที่ขา D4 
int buzzerpin2 = D6;     //ประกาศตัวแปร Buzzer ที่ขา D6  
//เซนเซอร์อัลตร้าโซนิคตัวที่ 3   
const int tingPin3 = D5; //ประกาศตัวแปร Trig ที่ขา D5
int echoPin3 = D8;       //ประกาศตัวแปร Echo ที่ขา D8
int buzzerpin3 = D7;     //ประกาศตัวแปร Buzzer ที่ขา D7

void setup() {

  Serial.begin(9600); //ใช้การสื่อสารแบบ ซีเรียล ความเร็ว 9600

}
void loop()
{
  if(millis()>=time1) //ถ้ามีการนับเวลาให้เพิ่มเวลาขึ้นทุกๆ 0.2 วินาทีวนซ้ำไปเรื่อยๆ
  {
    time1 = 200+millis(); //เพิ่มเวลาขึ้นทุกๆ 0.2 วินาที
    utl1(); //ฟังก์ชั่นการทำงานของเซนเซอร์อัลตร้าโซนิคตัวที่ 1
    utl2(); //ฟังก์ชั่นการทำงานของเซนเซอร์อัลตร้าโซนิคตัวที่ 2
    utl3(); //ฟังก์ชั่นการทำงานของเซนเซอร์อัลตร้าโซนิคตัวที่ 3
  }
}

void utl1() //ฟังก์ชั่นการทำงานของเซนเซอร์อัลตร้าโซนิคตัวที่ 1
{
  long duration, cm; //ประกาศตัวแปร
  pinMode(tingPin1, OUTPUT); //กำหนดเป็น เอาท์พุต
  pinMode(buzzerpin1, OUTPUT); //กำหนดเป็น เอาท์พุต
  digitalWrite(tingPin1, LOW); //กำหนดสัญญาณเป็น 0
  delayMicroseconds(2); //ดีเลย์ 2 ไมโครวินาที
  digitalWrite(tingPin1, HIGH); //กำหนดสัญญาณเป็น 1
  delayMicroseconds(5); //ดีเลย์ 5 ไมโครวินาที
  digitalWrite(tingPin1, LOW);  //กำหนดสัญญาณเป็น 0
  pinMode(echoPin1, INPUT); //กำหนดเป็น อินพุต
  duration = pulseIn(echoPin1, HIGH); //อ่านค่าสัญญาณพัลส์ที่ขา echoPin1
  cm = microsecondsToCentimeters(duration); //คำนวณค่าออกมาเป็นหน่วยเซนติเมตร
  Serial.print("UTL1 = "); //แสดงข้อความ UTL1 =  ออกซีเรียลมอนิเตอร์
  Serial.print(cm); //แสดงค่าของ cm ออกซีเรียลมอนิเตอร์
  Serial.print("cm"); //แสดงข้อความ cm ออกซีเรียลมอนิเตอร์
  Serial.println(); //เว้นบรรทัด
  new_delay1 = (cm * 3) + 30; //สูตรในการคำนวณค่าดีเลย์โดยนำระยะทางมาคำนวณตามสูตรสมการ

  if (cm < i) { //ถ้า cm มีค่าน้อยกว่า i ให้ทำงานเงื่อนไขนี้
    //บัซเซอร์ทำงาน
    digitalWrite(buzzerpin1, HIGH);
    delay(new_delay1); 
    digitalWrite(buzzerpin1, LOW);
  } else if (cm > j) { //ถ้า cm มีค่ามากกว่า j ให้ทำงานเงื่อนไขนี้
    //บัซเซอร์ไม่ทำงาน
    digitalWrite(buzzerpin1, HIGH);
  }
}
void utl2() //ฟังก์ชั่นการทำงานของเซนเซอร์อัลตร้าโซนิคตัวที่ 2
{
  long duration, cm; //ประกาศตัวแปร
  pinMode(tingPin2, OUTPUT); //กำหนดเป็น เอาท์พุต
  pinMode(buzzerpin2, OUTPUT); //กำหนดเป็น เอาท์พุต
  digitalWrite(tingPin2, LOW); //กำหนดสัญญาณเป็น 0
  delayMicroseconds(2); //ดีเลย์ 2 ไมโครวินาที
  digitalWrite(tingPin2, HIGH); //กำหนดสัญญาณเป็น 1
  delayMicroseconds(5); //ดีเลย์ 5 ไมโครวินาที
  digitalWrite(tingPin2, LOW); //กำหนดสัญญาณเป็น 0
  pinMode(echoPin2, INPUT); //กำหนดเป็น อินพุต
  duration = pulseIn(echoPin2, HIGH); //อ่านค่าสัญญาณพัลส์ที่ขา echoPin2
  cm = microsecondsToCentimeters(duration); //คำนวณค่าออกมาเป็นหน่วยเซนติเมตร
  Serial.print("UTL2 = "); //แสดงข้อความ UTL2 =  ออกซีเรียลมอนิเตอร์
  Serial.print(cm); //แสดงค่าของ cm ออกซีเรียลมอนิเตอร์
  Serial.print("cm"); //แสดงข้อความ cm ออกซีเรียลมอนิเตอร์
  Serial.println(); //เว้นบรรทัด
  new_delay1 = (cm * 3) + 30; //สูตรในการคำนวณค่าดีเลย์โดยนำระยะทางมาคำนวณตามสูตรสมการ
  if (cm < i) { //ถ้า cm มีค่าน้อยกว่า i ให้ทำงานเงื่อนไขนี้
    //บัซเซอร์ทำงาน
    digitalWrite(buzzerpin2, HIGH);
    delay(new_delay1);
    digitalWrite(buzzerpin2, LOW);
  } else if (cm > j) { //ถ้า cm มีค่ามากกว่า j ให้ทำงานเงื่อนไขนี้
    //บัซเซอร์ไม่ทำงาน
    digitalWrite(buzzerpin2, HIGH);
  }

}
void utl3() //ฟังก์ชั่นการทำงานของเซนเซอร์อัลตร้าโซนิคตัวที่ 2
{
  long duration, cm; //ประกาศตัวแปร
  pinMode(tingPin3, OUTPUT); //กำหนดเป็น เอาท์พุต
  pinMode(buzzerpin3, OUTPUT); //กำหนดเป็น เอาท์พุต
  digitalWrite(tingPin3, LOW); //กำหนดสัญญาณเป็น 0
  delayMicroseconds(2); //ดีเลย์ 2 ไมโครวินาที
  digitalWrite(tingPin3, HIGH); //กำหนดสัญญาณเป็น 1
  delayMicroseconds(5); //ดีเลย์ 5 ไมโครวินาที
  digitalWrite(tingPin3, LOW); //กำหนดสัญญาณเป็น 0
  pinMode(echoPin3, INPUT); //กำหนดเป็น อินพุต
  duration = pulseIn(echoPin3, HIGH); //อ่านค่าสัญญาณพัลส์ที่ขา echoPin2
  cm = microsecondsToCentimeters(duration); //คำนวณค่าออกมาเป็นหน่วยเซนติเมตร
  Serial.print("UTL3 = "); //แสดงข้อความ UTL3 =  ออกซีเรียลมอนิเตอร์
  Serial.print(cm); //แสดงค่าของ cm ออกซีเรียลมอนิเตอร์
  Serial.print("cm"); //แสดงข้อความ cm ออกซีเรียลมอนิเตอร์
  Serial.println(); //เว้นบรรทัด
  new_delay1 = (cm * 3) + 30; //สูตรในการคำนวณค่าดีเลย์โดยนำระยะทางมาคำนวณตามสูตรสมการ
  if (cm < i) { //ถ้า cm มีค่าน้อยกว่า i ให้ทำงานเงื่อนไขนี้
    //บัซเซอร์ทำงาน
    digitalWrite(buzzerpin3, HIGH);
    delay(new_delay1);
    digitalWrite(buzzerpin3, LOW);
  } else if (cm > j) {
    //บัซเซอร์ไม่ทำงาน
    digitalWrite(buzzerpin3, HIGH);
  }
}
long microsecondsToCentimeters(long microseconds) //ฟังก์ชั่นในการแปลงหน่วยไมโครวินาทีเป็นหน่วยเซนติเมตร
{
  /*
  ความเร็วเสียงเดินทางในอากาศคือ 340 เมตร/วินาที หรือ 29 ไมโครวินาที ต่อเซนติเมตร
  เวลาที่ Ping ออกไปกระทบวัตถุจนถึงกลับมาที่ตัวรับ ใช้หาระยะทางได้
  วัตถุอยู่มีระยะทางเท่ากับครึ่งหนึ่ง(2)ของเวลาที่จับได้
  เขียนเป็นสมการคำนวนระยะทาง ได้ดังนี้ microseconds / 29 / 2
  */
  return microseconds / 29 / 2;
}
