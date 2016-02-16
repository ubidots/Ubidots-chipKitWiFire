# ChipKIT WiFire 

The chipKIT family is based on the popular Arduino open-source hardware prototyping platform and adds the performance of the Microchip PIC microcontrollers.
The WiFire is significantly faster than its WF32 counterpart, with 200MHz operation speed, 2MB of Flash, 512kB RAM, High Speed USB and a 50MHz SPI. The PIC32MZ core includes the MIPS MicroAptiv CPU, a highly efficient, compact, core that is optimized for cloud-connected based projects.
Here you will lear how to post values to the Ubidots API using TCP method and HTTP method.

## Requiremets

*	[A chipKIT WiFire](http://www.digilentinc.com/Products/Detail.cfm?NavPath=2,719,1265&Prod=CHIPKIT-WIFIRE)
* Mini USB cable

## Setup

1. Download Arduino IDE version 1.6.5 or higher, [here](https://www.arduino.cc/).
2. From within the Arduino IDE, go to **File->Preferences** dialog box.
3. Look at the text entry field called **"Additional Boards Manager URLs:"**.
4. just copy/paste the following URL into that text field "https://github.com/chipKIT32/chipKIT-core/raw/master/package_chipkit_index.json". Then click **OK** to close.
5. Now select the **Tools->Board->Board Manager** menu from the Arduino IDE, and it will open up the Boards Manager window. From there, scroll down until you see the chipKIT board.
6. Click once on any of the text in the chipKIT section, and you will see a button appear that says "Install". It will take some time to download all of the chipKIT components and install them, but when it's done, you can click the Close button to close the Board Manager window.
7. Now choose a chipKIT WiFire board from the **Tools->Board** menu and program your chipKIT WiFire!


## Send one value to Ubidots

To send a value to Ubidots, go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsSendValues.cpp**, copy it and paste to MYAPP.ino
Put your Ubidots token, in **quantityOfVariables** put "1" and set the name of your variable.
Upload the code, open the Serial monitor to check the results. If no response is seen, try reset your Particle Photon. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
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
```


## Get one value from Ubidots

To get the last value of a variable from Ubidots,  go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsGetValue.cpp**, copy it and paste to MYAPP.ino
Upload the code, open the Serial monitor to check the results. If no response is seen, try reset your Particle Photon. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
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
    float value = client.getValue(ID)
}
```

## Send multiple values to Ubidots 

To send a value to Ubidots, go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsSendValues.cpp**, copy it and paste to MYAPP.ino
Put your Ubidots token, in **quantityOfVariables** put the quantity of variables that you will send and set the name of yours variables.
Upload the code, open the Serial monitor to check the results. If no response is seen, try reset your Particle Photon. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
#include <MRF24G.h>
#include <UbidotsWiFire.h>

#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define ID2 "Your_variable_ID2_here"
#define ID3 "Your_variable_ID3_here"
#define ID4 "Your_variable_ID4_here"

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
    float value2 = analogRead(A0);
    float value3 = analogRead(A1);
    float value4 = analogRead(A2);
    client.add(ID, value);
    client.add(ID2, value2);
    client.add(ID3, value3);
    client.add(ID4, value4);
    client.sendAll();
}
```