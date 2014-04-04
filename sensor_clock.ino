#include <Wire.h>
#include <DHT.h>
#include <SFE_BMP180.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

#define LED_13_PIN 13

#define DHTPIN A0 
#define DHTTYPE DHT22 
#define BASE_PRESSURE 1012.0 //Sea level pressure in millibars. Calibrate if you must

#define CONTRAST_PIN 44
#define BACKLIGHT_PIN 46
#define BRIGHTNESS 5
#define CONTRAST 1

#define LCD_CHAR_LENGTH 16

#define UPDATE_RATE 200

#define LCD_DEGREES_SYMBOL (char) 223

#define INITIAL_DHT22_HUM 50;
#define INITIAL_DHT22_TEMP 30; 

#define REFRESH_INTERVAL 5 //Minimum 2 seconds for DHT22 to refresh. Do not exceed 59
#define SWAP_LINE2_Interval 15 //Must be multiple of refresh interval. Do not exceed 59

#define THERM_ICON_REP 1
#define HUM_ICON_REP 2

//Icons from http://www.instructables.com/id/Clock-with-termometer-using-Arduino-i2c-16x2-lcd-D/
byte thermIcon[8] = //icon for thermometer
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};

byte humIcon[8] = //icon for humidity
{
    B00100,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
};


//LCD wiring without POT from http://engineeringlearning.blogspot.sg/2013/10/interfacing-lcd-without-potentiometer.html
LiquidCrystal lcd(26, 30, 34, 38, 42, 50);
DHT dht(DHTPIN, DHTTYPE);
SFE_BMP180 pressure;
RTC_DS1307 RTC;  //Code for this works although I use DS3231. For the initial time setting convenience.

int prevIntervalReadingSecond = 0;


bool showLine1OrLine2 = false;  //True for Line 1: Temperature, Humidity. False for Line 2: Pressure

void setup(){
  Serial.begin(9600);
  
  //Disable LED Pin 13 (Optional). I just hate the constant LED light.
  pinMode(LED_13_PIN, OUTPUT);
  digitalWrite(LED_13_PIN, LOW);
  
  //Start RTC
  Wire.begin();
  RTC.begin();
  
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running");
  }

  // This section grabs the current datetime and compares it to
  // the compilation time.  If necessary, the RTC is updated.
  DateTime now = RTC.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  
  if (now.unixtime() < compiled.unixtime()) {
    Serial.println("RTC is older than compile time! Updating");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  //Start LCD
  pinMode(CONTRAST_PIN, OUTPUT);
  pinMode(BACKLIGHT_PIN, OUTPUT);
  analogWrite(CONTRAST_PIN,CONTRAST);
  analogWrite(BACKLIGHT_PIN,BRIGHTNESS);
  lcd.begin(LCD_CHAR_LENGTH, 2);


  lcd.createChar(THERM_ICON_REP,thermIcon);
  lcd.createChar(HUM_ICON_REP,humIcon);
  
  printThisOnLCDLine("By:", 1);
  printThisOnLCDLine("Yeo Kheng Meng", 2);
 

  //Start DTT
  dht.begin();
  
 
  //Start BMP180
  if (pressure.begin()){
    Serial.println("BMP180 init success");
  } else{
    Serial.println("BMP180 init fail\n\n");
  }
}

void loop(){
  
  delay(UPDATE_RATE);
  DateTime now = RTC.now(); 

  int second = now.second();  
  String dateString = generateDateTimeString(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

//   Serial.print("LCD Line 1: ");
//  Serial.println(dateString);
  printThisOnLCDLine(dateString, 0);
  
  
  //Every refresh interval
  if((second % REFRESH_INTERVAL) == 0 && second != prevIntervalReadingSecond){
      //Prevent multiple readings every interval
      prevIntervalReadingSecond = second;
      Serial.println();
      Serial.print("Take DHT22 Readings: ");
      
      
      float DHTTemp = dht.readTemperature();
      float DHTHum = dht.readHumidity();
      

      Serial.print(DHTHum);
      Serial.print("\t");
      Serial.println(DHTTemp);
      
      int measureDelay;

      // You must first get a temperature measurement to perform a pressure reading.
 
      measureDelay = pressure.startTemperature();
    
     delay(measureDelay);

     double BMPTemp;
     int status = pressure.getTemperature(BMPTemp);

     // Print out the measurement:
      
     Serial.print("BMP180 readings: ");
     Serial.print(BMPTemp,2);
     Serial.print(" deg C, ");
      
     // Start a pressure measurement:
     // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
     // If request is successful, the number of ms to wait is returned.
     // If request is unsuccessful, 0 is returned.
     measureDelay = pressure.startPressure(0);

     delay(measureDelay);
     
     double BMPPress;
     status = pressure.getPressure(BMPPress,BMPTemp);
     Serial.print("abs pressure: ");
     Serial.print(BMPPress,2);
     Serial.print(" mb, ");

     float BMPAlt = pressure.altitude(BMPPress,BASE_PRESSURE);
     Serial.print("computed altitude: ");
     Serial.print(BMPAlt,0);
     Serial.println(" meters");


    if((second % SWAP_LINE2_Interval) == 0){
        showLine1OrLine2 = !showLine1OrLine2;
    }
    
    
    String lcdLine2;
    
    if(showLine1OrLine2){
       generateAndPrintTempHumString(DHTTemp, DHTHum); 
    } else {
        lcdLine2 = generatePressureString(BMPPress, BMPAlt);
        printThisOnLCDLine(lcdLine2, 1);
    }
    


  }
  

}

void generateAndPrintTempHumString(float temp, float hum){

  String tempString = ftoa(temp, 1);
  String humString = ftoa(hum, 1);
  
  String intermediate1 = " " + tempString + LCD_DEGREES_SYMBOL + "C ";
  String intermediate2 = " " + humString + "%";
  
  lcd.setCursor (0, 1);
  lcd.write((byte) THERM_ICON_REP);
  lcd.print(intermediate1);
  lcd.write((byte) HUM_ICON_REP);
  lcd.print(intermediate2);
  
}


String generateTempHumString(float temp, float hum){
  String tempString = ftoa(temp, 1);
  String humString = ftoa(hum, 1);
  String result = "P:" + tempString + LCD_DEGREES_SYMBOL + "C H:" + humString + "%";
  return result;
  
}

String generatePressureString(double pressure, double altitude){
  String pressString = ftoa(pressure, 1);
  int alt = altitude + 0.5;  //Round to nearest integer
  
  String result = "P:" + pressString + "mb " + alt + "m";
  return result;

}



//Convert float/double to String as ardiuno sprintf does not support float/double
String ftoa(double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  
  char temp[5];
  char * a = temp;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  String result = temp;
  return result;
}

void printThisOnLCDLine(String text, int line){

  //Clear current line only if the new line cannot occupy the old line
  if(text.length() < LCD_CHAR_LENGTH){
      lcd.setCursor (0, line);  
      lcd.print("                "); 
  }

  lcd.setCursor (0, line);  
  lcd.print(text);
}

String generateDateTimeString(int year, int month, int day, int hour, int minute, int second){
  char buff[3];

  sprintf(buff, "%02d", hour);
  String hourString = buff;

  sprintf(buff, "%02d", minute);
  String minuteString = buff;
  
  sprintf(buff, "%02d", second);
  String secondString = buff;
  
  String result = hourString + ":" + minuteString + ":" + secondString + " ";
  
  String dayOfWeek = getDayOfTheWeek(year, month, day);
  
  sprintf(buff, "%02d", day);
  String dayString = buff;
    
  String monthString = getMonth(month);
  
  if(0 <= second && second <= 40){
    result += " " + dayOfWeek + " " + dayString;
  } else {
    result += monthString + year;
  }
  return result;
}



// From http://stackoverflow.com/a/21235587
String getDayOfTheWeek(int y, int m, int d) {
  String weekdayname[] = {"Sun", "Mon", "Tue",
        "Wed", "Thu", "Fri", "Sat"};

  int weekday = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;
  
  
  return weekdayname[weekday];
}

String getMonth(int month){
  switch(month){
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default : return "Nil";
  }

}





