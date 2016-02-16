// This example is to send one varable to the Ubidots API
#include <MRF24G.h>
#include <UbidotsWiFire.h>

#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define SSIDWIFI "Your_WiFi_SSID_here"  // Put here your WiFi SSID
#define PASS "Your_WiFi_PASS_here"  // Put here your WiFi password

Ubidots ubidots(TOKEN);

void setup(){
    Serial.begin(9600);
    client.setWifiConnection(SSIDWIFI, PASS);
}
void loop(){
    float value = analogRead(A12);
    client.add(ID, value);
    client.sendAll();
}
