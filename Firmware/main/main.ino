// LinkIt One sketch for MQTT

#include <LWiFi.h>
#include <LWiFiClient.h>
#include <PubSubClient.h>
#include <SparkFunCCS811.h>

#define WIFI_AP "TTU Campus"
#define WIFI_PASSWORD ""
#define WIFI_AUTH LWIFI_OPEN  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

char mqttBroker[] = "test.mosquitto.org"; 
LWiFiClient wifiClient;
PubSubClient client( wifiClient );
unsigned long lastSend;
unsigned int co2;
unsigned int voc;

CCS811 mySensor(CCS811_ADDR);
  
void InitLWiFi()
{
  LWiFi.begin();
  // Keep retrying until connected to AP
  Serial.println("Connecting to AP");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD))) {
     delay(1000);
     Serial.println("Trying again");
  }
  Serial.println("Connected to AP");
}

void InitCSS811()
{
  Serial.println("CCS811 Initiliztion");
  CCS811Core::status returnCode = mySensor.begin();
  while (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
    Serial.println(".begin() returned with an error. Trying again");
    delay(1000);
    //while (1); //Hang if there was a problem.
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    // Attempt to connect
    if ( client.connect("LinkIt One Client") ) {	// Better use some random name
      Serial.println( "[DONE]" );
      // Publish a message on topic "outTopic"
      // client.publish( "outTopic","Hello, This is LinkIt One" );
	    // Subscribe to topic "inTopic"
      // client.subscribe( "inTopic" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 5000 );  // Wait 5 seconds before retrying
    }
  }
}

void callback( char* topic, byte* payload, unsigned int length ) {
  Serial.print( "Recived message on Topic:" );
  Serial.print( topic );
  Serial.print( "    Message:");
  for (int i=0;i<length;i++) {
    Serial.print( (char)payload[i] );
  }
  Serial.println();
}

void readsensorData()
{
  //Check to see if data is ready with .dataAvailable()
  if (mySensor.dataAvailable())
  {

    mySensor.readAlgorithmResults();
    co2 = mySensor.getCO2();
    voc = mySensor.getTVOC();
    Serial.print("READING : CO2[");             //Returns calculated CO2 reading
    Serial.print(co2);
    Serial.print("] tVOC[");          //Returns calculated TVOC reading
    Serial.print(voc);
   // Serial.print("] millis[");      //Simply the time since program start
   // Serial.print(millis());
    Serial.print("]");
    Serial.println();
  }

  delay(10); //Don't spam the I2C bus
}

void sendsensorData() {
  // Read data to send
  char sensorData[100];

  // Debug messages
  Serial.print( "SENDING : CO2 and VOC : [" );
  Serial.print( co2 ); Serial.print( "  " ); Serial.print( voc );
  Serial.print( "]   -> " );
  Serial.println();
  
  // Send payload
  String(co2).toCharArray(sensorData, sizeof(sensorData));
  client.publish( "/Thingworx/Team4Thing/co2_val", sensorData );
  delay( 1000 );
  String(voc).toCharArray(sensorData, sizeof(sensorData));
  client.publish( "/Thingworx/Team4Thing/voc_val", sensorData );
  
  //Serial.println( sensorData );
}

void setup()
{
   delay( 10000 );
   Serial.begin( 9600 );
   InitLWiFi();

   client.setServer( mqttBroker, 1883 );
   client.setCallback( callback );
   delay( 10000 );
   InitCSS811();
   lastSend = 0;
}

void loop()
{
  
  if( !client.connected() ) {
    reconnect();
  }

  if( millis()-lastSend > 1000 ) {	// read and send an update only after 1 seconds
    readsensorData();
    sendsensorData();
    lastSend = millis();
  }
  
  client.loop();
}


