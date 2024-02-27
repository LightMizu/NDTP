#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
#include <ESP32Servo.h>
#include <NewPing.h>

const char* WIFI_SSID = "iPhone (Тимофей)";
const char* WIFI_PASS = "12345678";

WebServer server(80);
Servo myservo;
NewPing sonar(2, 14); 
WiFiClient client = server.client();
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);
const int VIBRO_PINS[3] = {12,0,13};
long int prev_millis = 0;
void serveJpg()
{
	auto frame = esp32cam::capture();
	if (frame == nullptr)
    {
		Serial.println("CAPTURE FAIL");
		server.send(503, "", "");
		return;
	}
	server.setContentLength(frame->size());
	server.send(200, "image/jpeg");
	WiFiClient client = server.client();
	frame->writeTo(client);
	String vibro = server.arg("vibro");
	if (vibro[1] == 1){
		digitalWrite(VIBRO_PINS[0], 1);
		digitalWrite(VIBRO_PINS[2], 1);
	}
	else
	{
		digitalWrite(VIBRO_PINS[0], vibro[0]-'0');
		digitalWrite(VIBRO_PINS[2], vibro[2]-'0');
	}


}
void saveCamera(int distance){
	if(distance < 7 && distance != 0)
	{
		myservo.write(90);
	}
	else
	{
		myservo.write(0);
	}
}

void  setup()
{
	pinMode(4,OUTPUT);
	digitalWrite(4,1);
	delay(100);
	digitalWrite(4,0);
	myservo.attach(15);
	for(int i = 0; i<3; i++)
	{
		pinMode(VIBRO_PINS[i], OUTPUT);
	}
	Serial.begin(115200);
	Serial.println();
	pinMode(12,OUTPUT);
void serveJpg(){
	using namespace esp32cam;
	Config cfg;
	cfg.setPins(pins::AiThinker);
	cfg.setResolution(hiRes);
	cfg.setBufferCount(2);
	cfg.setJpeg(80); 
	bool ok = Camera.begin(cfg);
	Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
}
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED) {
	delay(500);
    }
	Serial.print("http://");
	Serial.println(WiFi.localIP());

	server.on("/",HTTP_GET,serveJpg);

	server.begin();
} 
void loop()
{
	if(millis() - prev_millis >= 500){
		int distance = sonar.ping_cm();
		saveCamera(distance);
}

server.handleClient();
}