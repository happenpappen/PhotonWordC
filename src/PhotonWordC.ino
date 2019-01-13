/*
 * Project PhotonWordC
 * Description:
 * Author:
 * Date:
 */

#include "PhotonWordC_config.h"
#include "Utils.h"
#include "MQTT.h"
#include "MQTT_credentials.h"
//#include "WebServer.h"

#define DATA_PIN_WC D0
#define DATA_PIN_AM D1
#define COLOR_ORDER GRB

#define WEBSERVER_PREFIX ""

ApplicationWatchdog wd(60000, System.reset);

const uint8_t minutes_array[] = {0, 0, 13, 26, 39};

const uint8_t time_str_array[][4] = {

    {HOUR_TOKEN, 0, 0, 0 },                          // 0
    {WORD_FUENFM, WORD_NACH, HOUR_TOKEN, 0},         // 5
    {WORD_ZEHNM, WORD_NACH, HOUR_TOKEN, 0},          // 10
    {WORD_VIERTEL, WORD_NACH, HOUR_TOKEN, 0},        // 15
    {WORD_ZWANZIGM, WORD_NACH, HOUR_TOKEN, 0},       // 20
    {WORD_FUENFM, WORD_VOR, WORD_HALB, HOUR_TOKEN},  // 25
    {WORD_HALB, HOUR_TOKEN, 0, 0},                   // 30
    {WORD_FUENFM, WORD_NACH, WORD_HALB, HOUR_TOKEN}, // 35
    {WORD_ZWANZIGM, WORD_VOR, HOUR_TOKEN, 0},        // 40
    {WORD_VIERTEL, WORD_VOR, HOUR_TOKEN, 0},         // 45
    {WORD_ZEHNM, WORD_VOR, HOUR_TOKEN, 0},           // 50
    {WORD_FUENFM, WORD_VOR, HOUR_TOKEN, 0}           // 55
};

const uint8_t words_array[][2] = {
    {0,0},      // NULL
    {46,4},     // WORD_EINS
    {44,4},     // WORD_ZWEI
    {39,4},     // WORD_DREI
    {29,4},     // WORD_VIER
    {33,4},     // WORD_FUENF
    { 1,5},     // WORD_SECHS
    {49,6},     // WORD_SIEBEN
    {17,4},     // WORD_ACHT
    {25,4},     // WORD_NEUN
    {13,4},     // WORD_ZEHN
    {22,3},     // WORD_ELF
    {56,5},     // WORD_ZWOELF
    {99,4},     // WORD_FUENFM
    {88,4},     // WORD_ZEHNM
    {92,7},     // WORD_ZWANZIGM
    {108,2},    // WORD_ES
    {104,3},    // WORD_IST
    { 8,3},     // WORD_UHR
    {62,4},     // WORD_HALB
    {77,7},     // WORD_VIERTEL
    {72,3},     // WORD_VOR
    {68,4},     // WORD_NACH
    {46,3}      // WORD_EIN
};

const String word_strings_array[] = {
    "",
    "Eins", "Zwei","Drei",
    "Vier", "Fünf", "Sechs",
    "Sieben", "Acht", "Neun",
    "Zehn", "Elf", "Zwölf",
    "Fünf", "Zehn", "Zwanzig",
    "Es", "ist", "Uhr",
    "halb", "Viertel", "vor",
    "nach", "Ein"
};

CRGB leds_wc[NUM_LEDS_WC];
CRGB leds_am[NUM_LEDS_AM];

//WebServer webserver(WEBSERVER_PREFIX, 80);

String word_string = "";

void mqtt_callback(char *, byte *, unsigned int);

MQTT client(MQTT_HOST, 1883, mqtt_callback);
Timer PublisherTimer(5000, publishState);

// Needed for time synchronization:
//
#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
unsigned long lastSync = millis();

uint8_t brightness = 255;
CRGB wc_color = CRGB( 50, 50, 50);
CRGB am_color = CRGB(255,  0,  0);
CRGB bg_color = CRGB(  0,  0,  0);

bool displayEnabled = true;

void show_time();

Timer ShowTime_Timer(60000, show_time);

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    // handle message arrived - we are only subscribing to one topic so assume all are led related
    String myTopic = String(topic);

    bool stateChanged = false;

    char *myPayload = (char *) malloc(length + 1);

    memcpy(myPayload, payload, length);
    myPayload[length] = 0;

    String myID = System.deviceID();

    if (!client.isConnected()) {
        client.connect(myID, MQTT_USER, MQTT_PASSWORD);
    }

    client.publish("/"+myID+"/state/LastPayload", "Last Payload: " + String(myPayload));

    free(myPayload);
    myPayload = NULL;
}

void publishState()
{
    String myID = System.deviceID();

    if (!client.isConnected()) {
        client.connect(myID, MQTT_USER, MQTT_PASSWORD);
    }

    if (client.isConnected()) {
        client.publish("/"+myID+"/state/FirmwareVersion", System.version());
        client.publish("/"+myID+"/state/Wordstring", word_string);
    }
}

void clear_all_wc() {
    for (int i = 0; i < NUM_LEDS_WC; i++) {
        leds_wc[i] = bg_color;
    }

    leds_am[minutes_array[1]] = bg_color;
    leds_am[minutes_array[2]] = bg_color;
    leds_am[minutes_array[3]] = bg_color;
    leds_am[minutes_array[4]] = bg_color;
}

void clear_all_am() {
    for (int i = 0; i < NUM_LEDS_AM; i++) {
        if ((i != minutes_array[1]) &&
        (i != minutes_array[2]) &&
        (i != minutes_array[3]) &&
        (i != minutes_array[4])) {
            leds_am[i] = bg_color;
        }
    }
}

void show_minutes(uint8_t minutes) {

    if ((minutes > 0) && (minutes < 5)) {
        for (int i = 1; i < 5; i++) {
            if (i <= minutes) {
                leds_am[minutes_array[i]] = wc_color;
            } else {
                leds_am[minutes_array[i]] = bg_color;
            }
        }
    }
}

void show_word(uint8_t word) {

    uint8_t first_pos = words_array[word][WORD_POS];
    uint8_t last_pos = words_array[word][WORD_POS]+words_array[word][WORD_COUNT]-1;
    
    for (int pos=first_pos; pos <= last_pos; pos++) {
        leds_wc[pos] = wc_color;
    }
}

void show_time() {

    uint8_t hour = Time.hourFormat12();
    uint8_t minute = Time.minute();
    uint8_t count = 0;

    uint8_t minutes_over_five = minute % 5;

    clear_all_wc();

    if (minute > 24) { hour == 12 ? hour = 1 : hour++; }

    show_minutes(minutes_over_five);

    minute = minute - minutes_over_five;

    show_word(WORD_ES);
    show_word(WORD_IST);
    word_string = word_strings_array[WORD_ES]+" "+ word_strings_array[WORD_IST];

    count = 0;

    while ((time_str_array[minute/5][count] != 0) && (count < 4)) {
        if (time_str_array[minute/5][count] == HOUR_TOKEN) {
            count = 3;
            if ((hour == 1) && (minute == 0)) {
                show_word(WORD_EIN);
                word_string = word_string+" "+word_strings_array[WORD_EIN];
            } else {
                show_word(hour);
                word_string = word_string+" "+word_strings_array[hour];
            }
            if (minute == 0) {
                show_word(WORD_UHR);
                word_string = word_string+" "+word_strings_array[WORD_UHR];
            }
        } else {
            show_word(time_str_array[minute/5][count]);
            word_string = word_string+" "+word_strings_array[time_str_array[minute/5][count]];
        }
        count++;
    }
    
    LEDS.show();
}

// setup() runs once, when the device is first turned on.
void setup() {

    Time.zone(+1);

    Particle.syncTime();

    ShowTime_Timer.start();
    PublisherTimer.start();

    // Put initialization like pinMode and begin functions here.
    FastLED.addLeds<WS2812B, DATA_PIN_WC, COLOR_ORDER>(leds_wc, NUM_LEDS_WC);
    FastLED.addLeds<WS2812B, DATA_PIN_AM, COLOR_ORDER>(leds_am, NUM_LEDS_AM);
    FastLED.setBrightness(brightness);

    clear_all_wc();

    for (int i = 0; i < NUM_LEDS_AM; i++) {
        if ((i != minutes_array[1]) &&
        (i != minutes_array[2]) &&
        (i != minutes_array[3]) &&
        (i != minutes_array[4])) {
            leds_am[i] = am_color;
        }
    }

    show_time();

    LEDS.show();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
    static uint8_t hue = 0;

    // once a day sync time with cloud:
    if (millis() - lastSync > ONE_DAY_MILLIS) {
        // Request time synchronization from the Particle Cloud
        Particle.syncTime();
        lastSync = millis();
    }
}
