#define BLINKER_WIFI
#define BLINKER_MIOT_SENSOR   //小爱同学定义为传感器设备
 
#include <Blinker.h>
#include <DHT.h>
#include <SoftwareSerial.h>
 
char auth[] = "**********";    //设备key
char ssid[] = "**********";         //wifi ssid
char pswd[] = "**********";     //wifi 密码
 
BlinkerNumber HUMI("humi");    //定义湿度数据键名
BlinkerNumber TEMP("temp");    //定义温度数据键名
BlinkerNumber MQ_2("mq");      //定义气体浓度数据键名
  
#define DHTPIN 2      //定义DHT11模块连接管脚io2
#define DHTTYPE DHT11   // 使用DHT 11温度湿度模块 
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);    //定义dht

int smoke_sensor_pin = A0;  //定义mq—2传感器端口
float humi_read = 0, temp_read = 0, mq_2_read = 0;
 
void heartbeat()
{
    HUMI.print(humi_read);        //给blinkerapp回传湿度数据
    TEMP.print(temp_read);        //给blinkerapp回传温度数据
    MQ_2.print(mq_2_read);
    
}

void miotQuery(int32_t queryCode)      //小爱同学语音命令反馈
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

            int humi_read_int=humi_read=mq_2_read;     //去掉湿度浮点
            BlinkerMIOT.humi(humi_read_int);     //小爱接收湿度
            BlinkerMIOT.temp(temp_read);      //小爱接收温度
            BlinkerMIOT.print();

}


void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();
    Blinker.begin(auth, ssid, pswd);   
    Blinker.attachHeartbeat(heartbeat);
    dht.begin();
    pinMode(smoke_sensor_pin, INPUT);
    BlinkerMIOT.attachQuery(miotQuery);
}
 
void loop()
{
    Blinker.run();
 
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int m = analogRead(smoke_sensor_pin);
    if (isnan(h) || isnan(t) || isnan(m))
    {
        BLINKER_LOG("Failed to read from DHT sensor!");
        BLINKER_LOG("Failed to read from MQ-2 sensor!");
    }
    else
    {
        BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");
        BLINKER_LOG("qiti: ", m, " %");
        humi_read = h;
        temp_read = t;
        mq_2_read = m;
    }
    Blinker.delay(2000);
}
