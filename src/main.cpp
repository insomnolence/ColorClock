/*
 * Color Clock
 * Use colors to approximate hour / minute. Used for those
 * who need glasses to see the time, but can make out colors.
 * Currently being used at night to approximate time.
 *
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <Timezone.h>
#include "ColorFader.h"

// Pixel Strip information
#define LED_PIN 14
Adafruit_NeoPixel strip = Adafruit_NeoPixel(14, LED_PIN, NEO_GRB + NEO_KHZ800);

ColorFader hourFader(strip, 7, 13);  // LEDs 7 -> 13 represent the hour lights of the strip
ColorFader minuteFader(strip, 0, 6); // LEDs 0 -> 6 represent the minute lights of the strip

// Define colors
uint32_t colors[12] = {
    strip.Color(0, 255, 255), // Cyan
    strip.Color(0, 127, 255), // Blue Cyan
    strip.Color(0, 0, 255),   // Blue
    strip.Color(127, 0, 255), // Blue Magenta
    strip.Color(255, 0, 255), // Magenta
    strip.Color(255, 0, 127), // Red Magenta
    strip.Color(255, 0, 0),   // Red
    strip.Color(255, 127, 0), // Orange
    strip.Color(255, 255, 0), // Yellow
    strip.Color(127, 255, 0), // Green Yellow
    strip.Color(0, 255, 0),   // Green
    strip.Color(0, 255, 127), // Green Cyan
};

const char ssid[] = "";  //  your network SSID (name)
const char pass[] = ""; // your network password

TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240}; // UTC -4 hours
TimeChangeRule usEST = {"EDT", First, Sun, Nov, 2, -300}; // UTC -5 hours
Timezone myTZ(usEDT, usEST);

unsigned int localPort = 8888; // local port to listen for UDP packets

WiFiUDP Udp;

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
time_t GetNtpTime();
void SendNTPpacket(IPAddress &address);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ; // Needed for Leonardo only
  delay(250);
  strip.begin();
  strip.setBrightness(25);
  strip.show();

  Serial.println("Color Clock Starting . . .");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Udp = WiFiUDP();
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);
  Serial.println("waiting for sync");
  setSyncProvider(GetNtpTime);
  setSyncInterval(14400);
}

time_t prevDisplay = 0; // when the digital clock was displayed
int currentHour = -1;
int currentMinute = -1;

void loop()
{

  // Update the Pixels
  hourFader.Update();
  minuteFader.Update();

  if (timeStatus()) // Make sure time was set properly
  {
    // All time returned by now() is UTC time. Convert to local time 
    // via Timezone library which takes DST into consideration.
    time_t localTime = myTZ.toLocal(now());
    int checkHour = hourFormat12(localTime);
    int checkMin = minute(localTime);

    if (checkHour != currentHour || checkMin != currentMinute)
    {     
      if (checkHour != currentHour)
      {
        currentHour = checkHour;
        // Change color of pixels
        Serial.print(" Changing Hour Color for Hour: ");
        Serial.print(currentHour);
        Serial.println();
        hourFader.Fade(colors[currentHour % 12], 200, 10);
      }

      if (checkMin != currentMinute)
      {
        if (currentMinute == -1)
        {
          // Starting, lets guess.
          checkMin = checkMin - (checkMin % 5);
        }

        currentMinute = checkMin;
        if (checkMin % 5 == 0)
        {
          Serial.print(" Changing Minute Color for Minute: ");
          Serial.println(currentMinute);
          Serial.println();
          // Change color of pixels
          int colorNum = currentMinute / 5;
          minuteFader.Fade(colors[colorNum], 200, 10);
        }
      }
    }
  }
}

// NTP Information for Updating/Syncing the time. Note that all time is
// in UTC and the Timezone library is responsible for converting to local time
// with and without DST changes. 

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t GetNtpTime()
{
    IPAddress ntpServerIP; // NTP server's ip address

    while (Udp.parsePacket() > 0)
        ; // discard any previously received packets
    Serial.println("Transmit NTP Request");
    // get a random server from the pool
    WiFi.hostByName(ntpServerName, ntpServerIP);
    Serial.print(ntpServerName);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    SendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            Serial.println("Receive NTP Response");
            Udp.read(packetBuffer, NTP_PACKET_SIZE); // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            return secsSince1900 - 2208988800UL;
        }
    }
    Serial.println("No NTP Response :-(");
    return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void SendNTPpacket(IPAddress &address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}
