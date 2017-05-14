#include <YoutubeApi.h>
#include "SH1106.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

SH1106  display(0x3c, D3, D5);

//------- Replace the following! ------
char ssid[] = "WIFI_NAME";       // your network SSID (name)
char password[] = "WIFI_PASS";  // your network key
#define API_KEY "YOUR_YOUTUBE_API"  // your google apps API Token
#define CHANNEL_ID "CHANNEL_ID" // makes up the url of channel

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

int api_mtbs = 60000; //mean time between api requests
long api_lasttime;   //last time api request has been done

long subs = 0;
void setup() {

  Serial.begin(115200);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {
  if (millis() > api_lasttime + api_mtbs)  {
    if (api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");

    }
    api_lasttime = millis();
  }

int subcountme = api.channelStats.subscriberCount;

  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0, "Subscribers");

  if (subcountme == 0)
{
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 24, "loading ...");
  }
else {
  display.drawString(64, 24, String(api.channelStats.subscriberCount));
  }
  
  
  //display.drawString(12, 16, "View:");
  //display.drawString(72, 16, String(api.channelStats.viewCount));
  
  //display.drawString(12, 32, "Video:");
  //display.drawString(72, 32, String(api.channelStats.videoCount));

  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 50, "YouTube: 150watt");

  display.display();
}
