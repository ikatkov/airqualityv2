#include <Arduino.h>
#include "SoftwareSerial.h"
#include "dhtnew.h"
#include "Adafruit_NeoPixel.h"
#include "AQICalculator.h"
#include "aqicolors.h"
#include "U8g2lib.h"
#include "logo.h"
#include <avr/pgmspace.h>
#include "CircularBuffer.h"

#define NEO_PIN 10
#define DHT_PIN 13
#define PMS5003_RX_PIN 2
#define PMS5003_TX_PIN 3 //not used

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
SoftwareSerial pmsSerial(PMS5003_RX_PIN, PMS5003_TX_PIN);
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, NEO_PIN, NEO_GRB + NEO_KHZ800);
DHTNEW dhtSensor(DHT_PIN);

pms5003data newSensorData = pms5003data();
uint8_t humidity;
uint8_t temperature;
CircularBuffer cbuffer;

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    // PMS5003 sensor baud rate is 9600
    pmsSerial.begin(9600);
    Serial.println(F("PMSA003 Air Quality Sensor"));

    u8g2.begin();
    u8g2.enableUTF8Print();


    // Draw logos
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.firstPage();
    do {
        u8g2.drawXBMP(0, 0, 128, 64, logo_temp);
    } while (u8g2.nextPage());

    delay(100);

    u8g2.firstPage();
    do {
        u8g2.drawXBMP(0, 0, 128, 64, logo_humidity);
    } while (u8g2.nextPage());

    delay(100);

    u8g2.firstPage();
    do {
        u8g2.drawXBMP(0, 0, 128, 64, logo_air);
    } while (u8g2.nextPage());

    // Initializes the NeoPixel library.
    pixel.begin();
    pixel.setBrightness(40);
    pixel.setPixelColor(0, 0, 0, 255);
    pixel.show();

    // Wait for PMS5003 to come online
    delay(1000);
}

void displayData(bool refreshSign) {
    uint16_t averagAQI = cbuffer.getAverage();
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_6x12_mr);
        u8g2.setCursor(0, 10);
        u8g2.print(F("PM    "));
        u8g2.print(newSensorData.pm25_standard);
        u8g2.setCursor(64, 10);
        u8g2.print(F("PM    "));
        u8g2.print(newSensorData.pm100_standard);
        u8g2.setFont(u8g2_font_micro_mn);
        u8g2.setCursor(13, 12);
        u8g2.print(F("2.5"));
        u8g2.setCursor(77, 12);
        u8g2.print(F("10.0"));

        if(refreshSign) {
            u8g2.setFont(u8g2_font_open_iconic_arrow_1x_t);
            u8g2.drawGlyph(120, 16, 87);
        }

        u8g2.setFont(u8g2_font_logisoso16_tf);
        u8g2.setCursor(98, 40);
        u8g2.print(temperature);
        u8g2.print(F("°"));
        u8g2.setCursor(98, 60);
        u8g2.print(humidity);
        u8g2.print(F("%"));

        u8g2.setFont(u8g2_font_logisoso46_tn);
        u8g2.setCursor(0,63);
        u8g2.print(averagAQI);
    } while ( u8g2.nextPage());
}


boolean readPMSData(Stream *s, pms5003data *data) {
    if (!s->available()) {
        Serial.println(F("Stream not available"));
        return false;
    }

    // Read a byte at a time until we get to the special '0x42' start-byte
    while (s->peek() != 0x42) {
        s->read();
        Serial.println(F("byte is not 0x42"));
    }

    // Now read all 32 bytes
    if (s->available() < 32) {
        Serial.println(F("less than 32 bytes available"));
        return false;
    }

    uint8_t buffer[32];
    uint16_t sum = 0;
    s->readBytes(buffer, 32);

    Serial.println(F("Read all 32 bytes to buffer"));

    // get checksum ready
    for (uint8_t i = 0; i < 30; i++) {
        sum += buffer[i];
    }

    Serial.print(F("Checksum is "));
    Serial.println(sum, HEX);

    /* debugging */
    for (uint8_t i = 2; i < 32; i++) {
        Serial.print(F("0x"));
        Serial.print(buffer[i], HEX);
        Serial.print(F(", "));
    }
    Serial.println(F("---"));


    // The data comes in endian'd, this solves it so it works on all platforms
    uint16_t buffer_u16[15];
    for (uint8_t i = 0; i < 15; i++) {
        buffer_u16[i] = buffer[2 + i * 2 + 1];
        buffer_u16[i] += (buffer[2 + i * 2] << 8);
    }

    // put it into a nice struct :)
    memcpy((void *) data, (void *) buffer_u16, 30);

    if (sum != data->checksum) {
        Serial.print(F("Checksum failure: "));
        Serial.print(sum, HEX);
        Serial.print(F("!="));
        Serial.println(data->checksum, HEX);
        return false;
    } else {
        Serial.println(F("All good"));
    }
    // success!
    return true;
}

void loop() {
    displayData(true);

    dhtSensor.read();
    humidity = round(dhtSensor.getHumidity());
    temperature = round(dhtSensor.getTemperature());
    Serial.print(F("Humidity "));
    Serial.print(humidity, 1);
    Serial.print(F("\t"));
    Serial.print(F("Temperature "));
    Serial.println(temperature, 1);

    if (readPMSData(&pmsSerial, &newSensorData)) {
        uint16_t currentAqi = AQICalculator::getAqi(newSensorData.pm25_standard, newSensorData.pm100_standard, humidity);
        Serial.print(F("AQI:"));
        Serial.println(currentAqi);
        cbuffer.add(currentAqi);

        displayData(false);

        uint8_t red = pgm_read_byte(&aiq_colors[currentAqi*3]);
        uint8_t green = pgm_read_byte(&aiq_colors[currentAqi*3+1]);
        uint8_t blue = pgm_read_byte(&aiq_colors[currentAqi*3+2]);
        pixel.setPixelColor(0, red, green, blue );
        pixel.show();


        delay(4000);
    } else {
        Serial.println(F("Failure"));
        delay(1000);  // try again in a bit!
    }
}

