#ifndef CONFIG_HPP
#define CONFIG_HPP

//-------------------------------------------------------------------------------------------------------------------------------------

#define PIN_RX        0     //serial pin, do not use
#define PIN_TX        1     //serial pin, do not use

#define PIN_IN_FLO    2     //flowmeter pwm pin
#define PIN_IN_WLS    3     //infrared water level data
#define PIN_IN_DHT    4     //dht22 data

#define PIN_IN_BTN    5     //debounced button
//#define PIN_IN_ACD    5     //pH sensor analog
//#define PIN_IN_TDS    6     //dissolved solids analog

#define PIN_IN_BLE    6     //bluetooth serial rx
#define PIN_OUT_BLE   7     //bluetooth serial tx

#define PIN_OUT_PMP   A5    //water pump relay
#define PIN_OUT_FAN   A4    //fan relay
#define PIN_OUT_ACU   A3    //AC unit relay
#define PIN_OUT_LIT   A2    //light relay

//-------------------------------------------------------------------------------------------------------------------------------------
//instantiate framework singletons

static Clock            clock;
static CommandConsole   console;
static SerialManager    serial(&console, PIN_IN_BLE, PIN_OUT_BLE);
static VariableManager  variables;
static SensorManager    sensors;
static LatchManager     latches;
//static Display          display;

//-------------------------------------------------------------------------------------------------------------------------------------
//declare control variables (all should be float)

static float humidity;//OR to store a history of the value, use Metric<float> airHumidity; SUBJECT TO MEMORY LIMITS
static float airTemp;
static float waterContact;
static float ppm;
static float acidity;
static float flowRate;

//-------------------------------------------------------------------------------------------------------------------------------------
//additional control variables that can be modified by the user at runtime

static float low = LOW;
static float high = HIGH;
static float humidityLow = 55.f;
static float humidityHigh = 65.f;
static float airTempLow = 24.f;
static float airTempHigh = 27.f;

//-------------------------------------------------------------------------------------------------------------------------------------

static void configure() {
  
  //-------------------------------------------------------------------------------------------------------------------------------------
  //configure dynamic vars            Variable            ID                  Description
  //                                  ------------------------------------------------------------------------------------    
  variables.addCvar(                  &humidityLow,       "HUM_FAN_OFF",      "[0-100] humidity, ≤x fan turns off"        );
  variables.addCvar(                  &humidityHigh,      "HUM_FAN_ON",      " [0-100] humidity, ≥x fan turns on"         );
  variables.addCvar(                  &airTempLow,        "TMP_ACU_OFF",      "[0-100] air temp, ≤x AC turns off"         );
  variables.addCvar(                  &airTempHigh,       "TMP_ACU_ON",      " [0-100] air temp, ≥x AC turns on"          );
  
  //-------------------------------------------------------------------------------------------------------------------------------------
  //configure latches                 Signal            Set(≤)              Reset(≥)              Qlow      Pin
  //                                  ------------------------------------------------------------------------------------
  latches.add(new Latch(              &humidity,        &humidityLow,       &humidityHigh,        LOW,      PIN_OUT_FAN   ));   //fan turns OFF at <=humidityLow, turns ON at >=humidityHigh
  latches.add(new Latch(              &airTemp,         &airTempLow,        &airTempHigh,         LOW,      PIN_OUT_ACU   ));   //ac turns OFF at <=airTempLow, turns ON at >=airTempHigh
  latches.add(new Latch(              &waterContact,    &low,               &high,                HIGH,     PIN_OUT_PMP   ));   //pump turns ON at LOW, turns OFF at HIGH

  //-------------------------------------------------------------------------------------------------------------------------------------
  //configure sensors                 Interval      Pin           Output
  //                                  --------------------------------------------------
  sensors.add(new SensorDht22(        1000UL,       PIN_IN_DHT,   &airTemp, &humidity   ));   //read airTemp and humidity every 1s
  sensors.add(new SensorWaterLevel(   500UL,        PIN_IN_WLS,   &waterContact         ));   //read water level every 0.5s
  sensors.add(new SensorFlow(         250UL,        PIN_IN_FLO,   &flowRate             ));   //read flowrate every 0.25s
//sensors.add(new SensorTds(          10000UL,      PIN_IN_TDS,   &ppm                  ));   //read ppm every 10s
//sensors.add(new SensorPh(           10000UL,      PIN_IN_ACD,   &acidity              ));   //read pH level every 10s

  //-------------------------------------------------------------------------------------------------------------------------------------
  //configure switches                Signal        Compare       OnClose         OnOpen
  //                                  ------------------------------------------------------------
//switches.add( new Switch(           &master,      LOW,          &pauseSystem,   &resumeSystem   ));       
 
  //-------------------------------------------------------------------------------------------------------------------------------------  
  //configure timers                  Interval          OnReset       Enabled         
  //                                  ------------------------------------------
//timers.add( new Timer(              500UL             &lightOn      false     ));
}

#endif
