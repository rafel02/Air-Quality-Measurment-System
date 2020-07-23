#include <LDateTime.h>

datetimeInfo t;
unsigned int rtc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  datetimeInfo now;
  now.year = 2015;
  now.mon = 12;
  now.day = 21;
  LDateTime.setTime(&now);
}

void loop() {
  // put your main code here, to run repeatedly:
  LDateTime.getTime(&t);
  LDateTime.getRtc(&rtc);

  Serial.println(dateString(t));
  
  delay(1000);
}

String dateString(datetimeInfo dti) {
  String dateStr = "m/d/yyyy = ";
  dateStr += dti.mon;
  dateStr += "/";
  dateStr += dti.day;
  dateStr += "/";
  dateStr += dti.year;
  return dateStr;
}

