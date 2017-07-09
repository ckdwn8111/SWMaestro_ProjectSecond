/* Define the DIO pins used for the RTC module */

#define SCK_PIN 7//rtc clock
#define IO_PIN 3//rtc data
#define RST_PIN 2//rtc reset
#define DHTpin 6//dht pin
/* Include the DS1302 library */
#include <DS1302.h>

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <DHT11.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define SMD_BLUE 9
#define SMD_RED 10
#define SMD_GREEN 11

#define PIN_HOUR A3 //neopixel pin
#define PIN_MIN A4
#define PIN_TEMP 8
#define NUM_LEDS 24      // 네오픽셀 LED 갯수
#define BRIGHTNESS 10  //0~255   밝기를 제어(약간 Frame 느낌이 남)

#define MotorA1 12
#define MotorA2 13

Adafruit_NeoPixel strip_hour = Adafruit_NeoPixel(NUM_LEDS, PIN_HOUR, NEO_GRBW + NEO_KHZ800); // 기본 설정 우노의 경우는 이대로 사용
Adafruit_NeoPixel strip_minute = Adafruit_NeoPixel(NUM_LEDS - 12, PIN_MIN, NEO_GRBW + NEO_KHZ800); // 기본 설정 우노의 경우는 이대로 사용
Adafruit_NeoPixel strip_temp = Adafruit_NeoPixel(8, A1, NEO_GRBW + NEO_KHZ800); // 기본 설정 우노의 경우는 이대로 사용
Adafruit_NeoPixel strip_hum = Adafruit_NeoPixel(8, A2, NEO_GRBW + NEO_KHZ800); // 기본 설정 우노의 경우는 이대로 사용

DHT11 dht11(DHTpin);

/* Initialise the DS1302 library */
DS1302 rtc(RST_PIN, IO_PIN, SCK_PIN);
SoftwareSerial BTSerial(4, 5);
//String hour = "";
String myString = "";
char select = 0;

void setup()
{

	strip_hour.setBrightness(BRIGHTNESS);
	strip_hour.begin();
	strip_hour.show();//initialize all pixels 'off'

	strip_minute.setBrightness(BRIGHTNESS);
	strip_minute.begin();
	strip_minute.show();//initialize all pixels 'off'

	strip_temp.setBrightness(BRIGHTNESS);
	strip_temp.begin();
	strip_temp.show();//initialize all pixels 'off'

	strip_hum.setBrightness(BRIGHTNESS);
	strip_hum.begin();
	strip_hum.show();//initialize all pixels 'off'

	pinMode(8, OUTPUT);
	/* Clear the 1302's halt flag */
	rtc.halt(false);
	/* And disable write protection */
	rtc.writeProtect(false);
	/* Initialise the serial port */
	Serial.begin(9600);


	/* Set the time and date */

	////추가 여기에 날짜 시간 입력해야하는곳///////////////////
	BTSerial.begin(9600);

	

	Serial.println("Hello! OrangeBoard");
	//   BTSerial.begin(9600);

	pinMode(SMD_BLUE, OUTPUT);
	pinMode(SMD_RED, OUTPUT);
	pinMode(SMD_GREEN, OUTPUT);
	pinMode(MotorA1, OUTPUT);
	pinMode(MotorA2, OUTPUT);

	rtc.setTime(4,15,30); //시,분,초  
	MotorStop();
}

int hour(void) {

	int i = 0;
	char *time_data = nullptr;
	time_data = rtc.getTimeStr();
	char time_hour[10] = { 0. };
	for (i = 0; i < 10; i++) {
		time_hour[i] = *(time_data + i);
	}

	for (i = 3; i < 10; i++) {
		time_hour[i] = 0;
	}

	int hour_val = atoi(time_hour);

	if (hour_val == 0) {
		return 24;
	}
	else {
		return hour_val;
	}
}

int minute(void) {

	int i = 0;
	char *time_data = nullptr;
	time_data = rtc.getTimeStr();
	char time_minute[10] = { 0. };
	for (i = 0; i < 10; i++) {
		time_minute[i] = *(time_data + i);
	}

	char time_min[3] = { 0. };

	for (i = 0; i < 2; i++) {
		time_min[i] = time_minute[i + 3];
	}

	int hour_val = atoi(time_min);

	return hour_val;

}

int sec(void) {

	int i = 0;
	char *time_data = nullptr;
	time_data = rtc.getTimeStr();
	char time_second[10] = { 0. };
	for (i = 0; i < 10; i++) {
		time_second[i] = *(time_data + i);
	}

	char time_sec[3] = { 0. };

	for (i = 0; i < 2; i++) {
		time_sec[i] = time_second[i + 6];
	}

	int hour_val = atoi(time_sec);

	return hour_val;

}


char BLE_response(void) {

	char ble_val = { 0 };
	char myChar = { 0 };
	char year[5] = { 0. };
	char month[3] = { 0. };
	char date[3] = { 0. };
	char hour[3] = { 0. };
	char minute[3] = { 0. };
	char sec[3] = { 0. };

	while (BTSerial.available())  //mySerial에 전송된 값이 있으면
	{
		myChar = (char)BTSerial.read();  //mySerial int 값을 char 형식으로 변환
		myString += myChar;								 //myString += myChar;   //수신되는 문자를 myString에 모두 붙임 (1바이트씩 전송되는 것을 연결)
		delay(5);           //수신 문자열 끊김 방지
		//BTSerial.flush();
	}

	if (myString != "")  //myString 값이 있다면
	{
		Serial.print("receive data : ");
		Serial.println(myString);
		switch (myString[0]) {
		case 'a'://난초류 1:1
			ble_val = 'a';
			myString = "";
			return ble_val;
		case 'b'://화초류 5:1
			ble_val = 'b';
			myString = "";
			return ble_val;
		case 'c'://채소류 2:1
			ble_val = 'c';
			myString = "";
			return ble_val;
		case 'd':
			ble_val = 'd';
			myString = "";
			return ble_val;
		case 'e':
			ble_val = 'e';
			myString = "";
			return ble_val;
		case '2' :
			for (int i = 0; i < 4; i++) {
				year[i] = myString[i];
			}
			for (int i = 0; i < 2; i++) {
				month[i] = myString[i+4];
				date[i] = myString[i+6];
				hour[i] = myString[i+8];
				minute[i] = myString[i+10];
				sec[i] = myString[i+12];
			}
			strip_hour.clear();
			strip_minute.clear();
			break;
		}
		Serial.print("complete data : ");
		Serial.print(year);
		Serial.print(" ");
		Serial.print(month);
		Serial.print(" ");
		Serial.print(date);
		Serial.print(" ");
		Serial.print(hour);
		Serial.print(" ");
		Serial.print(minute);
		Serial.print(" ");
		Serial.print(sec);
		Serial.print(" ");
		
		
		int year_int = atoi(year);
		if (year_int != 0) {
			rtc.setTime(atoi(hour), atoi(minute), atoi(sec)); //시,분,초  
			rtc.setDate(atoi(date), atoi(month), atoi(year));//일,월,년도
		}
		strip_hour.clear();
		strip_minute.clear();
		myString = "";
	}
	return ble_val;
}

/* Main program */
void MotorStart(void) {

	digitalWrite(MotorA1, HIGH);
	digitalWrite(MotorA2, LOW);

}

void MotorStop(void) {

	digitalWrite(MotorA1, LOW);
	digitalWrite(MotorA2, LOW);

}

void MotorSelect(char ble_re, int hour, int min, int sec) {

	switch (ble_re) {
	case 'a':
		if (hour == 21 && min == 19) {
			if (sec > 0 && sec < 10) {
				MotorStart();
			}
			else {
				MotorStop();
			}
		}
		else{
			MotorStop();
		}
		/*if (sec >= 0 && sec <= 20) {
			analogWrite(SMD_BLUE, 1023);
			analogWrite(SMD_RED, 1023);
			analogWrite(SMD_GREEN, 1023);
		}
		else if (sec >= 30 && sec <= 59) {
			analogWrite(SMD_BLUE, 1023);
			analogWrite(SMD_RED, 1023);
			analogWrite(SMD_GREEN, 1023);
		}*/
		if (sec % 12 == 0 || sec % 12 == 1) {
			analogWrite(SMD_BLUE, 0);
			analogWrite(SMD_RED, 0);
			analogWrite(SMD_GREEN, 0);
		}
		else {
			analogWrite(SMD_BLUE, 1023);
			analogWrite(SMD_RED, 1023);
			analogWrite(SMD_GREEN, 1023);
		}
		break;
	case 'b':
		if (hour == 8 && min == 20) {
			if (sec > 15 && sec < 20) {
				MotorStart();
			}
			else {
				MotorStop();
			}
		}
		else {
			MotorStop();
		}
		if (sec % 8 == 0 || sec % 8 == 1) {
			analogWrite(SMD_BLUE, 0);
			analogWrite(SMD_RED, 0);
			analogWrite(SMD_GREEN, 0);
		}
		else {
			analogWrite(SMD_BLUE, 1023);
			analogWrite(SMD_RED, 1023 / 9);
			analogWrite(SMD_GREEN, 0);
		}
		break;
	case 'c':
		if (hour == 8 && min == 40) {
			if (sec > 45 && sec < 60) {
				MotorStart();
			}
			else {
				MotorStop();
			}
		}
		else {
			MotorStop();
		}
		if (sec % 2 == 0) {
			analogWrite(SMD_BLUE, 0);
			analogWrite(SMD_RED, 0);
			analogWrite(SMD_GREEN, 0);
		}
		else {
			analogWrite(SMD_BLUE, 1023);
			analogWrite(SMD_RED, 1023 / 2);
			analogWrite(SMD_GREEN, 0);
		}
		break;
	case 'd':
		MotorStart();
		break;
	case 'e':
		MotorStop();
		break;
	default:
		MotorStop();
		break;
	}
}



void loop()
{
	int hour_val = hour();
	int sec_val = sec();
	int min_val = minute();
	Serial.print(hour_val);
	Serial.print(" : ");
	Serial.print(min_val);
	Serial.print(" : ");
	Serial.println(sec_val);

	

	char ble_val = BLE_response();
	
	if (ble_val != 0) {
		Serial.println(ble_val);
		if (ble_val == 'a') {
			analogWrite(SMD_BLUE, 1023);
			analogWrite(SMD_RED, 1023);
			analogWrite(SMD_GREEN, 1023);
			select = 'a';
		}
		else if (ble_val == 'b') {
			analogWrite(SMD_BLUE, 1023);
			analogWrite(SMD_RED, 1023 / 9);
			analogWrite(SMD_GREEN, 0);
			select = 'b';
		}
		else if (ble_val == 'c') {
			analogWrite(SMD_BLUE, 1023);
			analogWrite(SMD_RED, 1023 / 2);
			analogWrite(SMD_GREEN, 0);
			select = 'c';
		}
		else if (ble_val == 'd') {
			select = 'd';
			//MotorStart();
		}
		else if (ble_val == 'e') {
			select = 'e';
			//MotorStop();
		}
		else {
			analogWrite(SMD_BLUE, 0);
			analogWrite(SMD_RED, 0);
			analogWrite(SMD_GREEN, 0);
			
		}
	}

	MotorSelect(select, hour_val, min_val, sec_val);

	int err;
	float temp, humi;
	if ((err = dht11.read(humi, temp)) == 0)
	{
		Serial.print("temperature:");
		Serial.print(temp);
		Serial.print(" humidity:");
		Serial.print(humi);
		Serial.println();
		//////////////추가 블루투스에서 앱으로 문자를 보내는 부분 //////////////////
		BTSerial.print("temperature:");
		BTSerial.println(temp);
		BTSerial.print("humidity:");
		BTSerial.print(humi);
		//////////////////////////////////////////////////////////////////////////////     
	}
	else
	{
		Serial.println();
		Serial.print("Error No :");
		Serial.print(err);
		Serial.println();
	}
	
	int tempUnit = map(temp, 0, 40, 1, 8);
	int humiUnit = map(humi, 0, 40, 1, 8);
	
	
	delay(DHT11_RETRY_DELAY); //delay for reread
	int min = map(min_val, 0, 59, 1, 12);
	// Some example procedures showing how to display to the pixels:
	colorWipe_hour(strip_hour.Color(71, 200, 62), hour_val); // Red (Color, wait)
	colorWipe_min(strip_minute.Color(121, 21, 110), min);
	//colorWipe(strip.Color(0, 255, 0), 0); // Green
	//colorWipe(strip.Color(0, 0, 255), 0); // Blue
	//colorWipe(strip.Color(0, 0, 0, 255), 0); // White

	colorWipe_temp(strip_temp.Color(255, 0, 0), tempUnit); //온도표시
	colorWipe_hum(strip_temp.Color(0, 0, 255), humiUnit); //습도표시


}
// Fill the dots one after the other with a color


void colorWipe_hour(uint32_t c, uint8_t wait) {
	if (wait > 24) {
		strip_hour.show();
		strip_hour.clear();
	}
	else {
		for (uint16_t i = 0; i < wait; i++) {
			strip_hour.setPixelColor(i, c);   //LED 순서, LED 색상
			strip_hour.show();
			//delay(wait);

		}
	}
}

void colorWipe_min(uint32_t c, uint8_t wait) {
	if (wait > 12) {
		strip_minute.show();
		strip_minute.clear();
	}
	else {
		for (uint16_t i = 0; i < wait; i++) {
			strip_minute.setPixelColor(i, c);   //LED 순서, LED 색상
			strip_minute.show();
			//delay(wait);

		}
	}
}

void colorWipe_temp(uint32_t c, uint8_t wait) {
	for (uint16_t i = 0; i < wait; i++) {
		strip_temp.setPixelColor(i, c);   //LED 순서, LED 색상
		strip_temp.show();
		//delay(wait);

	}
}

void colorWipe_hum(uint32_t c, uint8_t wait) {
	for (uint16_t i = 0; i < wait; i++) {
		strip_hum.setPixelColor(i, c);   //LED 순서, LED 색상
		strip_hum.show();
		//delay(wait);

	}
}