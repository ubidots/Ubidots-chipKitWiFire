#include <MRF24G.h>
#include <UbidotsWiFire.h>

#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define SSIDWIFI "Your_WiFi_SSID_here"  // Put here your WiFi SSID
#define PASS "Your_WiFi_PASS_here"  // Put here your WiFi password

Ubidots client(TOKEN);

void setup(){
    Serial.begin(9600);
    client.setWifiConnection(SSIDWIFI, PASS);
}
void loop(){
    float value = client.getValue(ID)
}
