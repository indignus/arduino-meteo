/*
meteo v0.2
*/
#include <LiquidCrystal.h>
#include <DHT.h>

/*

Pinout for Zyxel U-336S lc board main connector 34-pin
1  3  5  7  9  11 13 15 17 19 21 23 25 27 29 31 33
:  :  :  :  :  :  :  :  :  :  :  :  :  :  :  :  :
2  4  6  8  10 12 14 16 18 20 22 24 26 28 30 32 34

1 DTR led |3 TXD led |5 RXD led |7 GND |9  V0 |11 RW |13 D0 |15 D2|17 D4 |19 D6 |21-GND |23-SW3 |25-SW4 |27-GND    | 29-CD led |31-GND    |33-VCC
2 PWR led |4 OH led  |6 AA led  |8 VCC |10 RS |12 E  |14 D1 |16 D3|18 D5 |20 D7 |22-VCC |24-SW1 |26-SW2 |28-CTS led| 30-DSR led|32-RTS led|34-VCC

Pinout for HD44780-compatible display on Zyxel U-336S lcd board 16-pin
1  GND|2  VCC
3  V0 |4  RS
5  RW |6  E
7  D0 |8  D1
9  D2 |10 D3
11 D4 |12 D5
13 D6 |14 D7
15 BLA|16 BLK
(BLA=VCC, BLK=GND)

Connect display to Arduino:
 RS (LCD pin 4) to Arduino pin 12
 RW (LCD pin 5) to Arduino pin 11
  E (LCD pin 6) to Arduino pin 10
 LCD pins D4, D5, D6, D7 to Arduino pins 6, 5, 4, 3

*/

dht DHT;
LiquidCrystal lcd(12, 11, 10, 6, 5, 4, 3);
const byte SW_1=8;
const byte LED_PW=13;
const byte LED_OH=7;
const byte DHT22_PIN=9;
const long interval=1000;
byte sw1_state=0;
byte sw2_state=0;
byte sw3_state=0;
byte sw4_state=0;
unsigned long previousMillis = 0;
int stateOH = HIGH;
int buttons_state = 0;

struct
{
    uint32_t total;
    uint32_t ok;
    uint32_t crc_error;
    uint32_t time_out;
    uint32_t connect;
    uint32_t ack_l;
    uint32_t ack_h;
    uint32_t unknown;
} stat = { 0,0,0,0,0,0,0,0};

void setup()
{  
  lcd.begin(20,2);              // columns, rows.  use 16,2 for a 16x2 LCD, etc.
  lcd.clear();                  // start with a blank screen
  pinMode(LED_PW, OUTPUT);
  pinMode(LED_OH, OUTPUT);
  pinMode(SW_1,OUTPUT);
  digitalWrite(SW_1, HIGH);		// 
}

void displayTemp()
{
	lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
  	lcd.print("Temperature (C)");    // change this text to whatever you like. keep it clean.
  	lcd.setCursor(0,1);           // set cursor to column 0, row 1
  	lcd.print(DHT.temperature);   // print temperature
}

void displayHumid()
{
	lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
	lcd.print("Humidity (%)   ");    // change this text to whatever you like. keep it clean.
  	lcd.setCursor(0,1);           // set cursor to column 0, row 1
  	lcd.print(DHT.humidity);      // print humidity
}

void DHT_stat()
{
	if (stat.total % 20 == 0)
    {   lcd.println("TOT OK CRC TO UNK");
        lcd.print(stat.total);
        lcd.print(" ");
        lcd.print(stat.ok);
        lcd.print(" ");
        lcd.print(stat.crc_error);
        lcd.print(" ");
        lcd.print(stat.time_out);
        lcd.print(" ");
        lcd.print(stat.connect);
        lcd.print(" ");
        lcd.print(stat.ack_l);
        lcd.print(" ");
        lcd.print(stat.ack_h);
        lcd.print(" ");
        lcd.print(stat.unknown);
    }
}

void loop()
{   unsigned long currentMillis=millis();
    if (currentMillis - previousMillis >= interval) 
    {
    	previousMillis = currentMillis;
    	int chk = DHT.read22(DHT22_PIN);
    	switch (chk)
    	{	case DHTLIB_OK:
        		stat.ok++;
        		Serial.print("OK");
        		break;
    		case DHTLIB_ERROR_CHECKSUM:
        		stat.crc_error++;
        		Serial.print("CHK SUM ERROR");
        		break;
    		case DHTLIB_ERROR_TIMEOUT:
        		stat.time_out++;
        		Serial.print("TIME OUT ERROR");
        		break;
    		default:
        		stat.unknown++;
        		Serial.print("UNKNOWN ERROR");
        		break;
    	}
    }
    else
    {
    	sw1_state = digitalRead(SW_1);
        switch (buttons_state) {
            case 1:
                
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;

        }
    	if (sw1_state==LOW && previousMillis==1)
    	{ 
    		if (stateOH==HIGH)
    			{
    				stateOH=LOW;
    		}
    		else 
    		{
    			stateOH=HIGH;
    		}
    	}	
    	digitalWrite(LED_OH, stateOH);
    	previousMillis=sw1_state;
    	if (stateOH==LOW)
    		displayHumid();
    	else 
    		displayTemp();
    }

    stat.total++;
}

/* ------------------------------------------------------------------------------- */
