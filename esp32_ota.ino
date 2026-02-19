#include <ArduinoJson.h>
#include <qrcode.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <WiFiClientSecure.h>


QRCode qrcode;
#define PORTRAIT 0
#define LANDSCAPE 1
#define USE_XPT2046 0
#define USE_LOCAL_KBV 1

#define TOUCH_ORIENTATION PORTRAIT

#if defined(USB_PID) && USB_PID == 0x804E // Arduino M0 Native
#define Serial SerialUSB
#endif

#define SWAP(x, y) \
    {              \
        int t = x; \
        x = y;     \
        y = t;     \
    }

#define TITLE "TouchScreen.h GFX Calibration"
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

#if USE_LOCAL_KBV
#include "TouchScreen_kbv.h" //my hacked version
#define TouchScreen TouchScreen_kbv
#define TSPoint TSPoint_kbv
#else
#include <TouchScreen.h> //Adafruit Library
#endif

/*_______End of defanitions______*/
/*______Assign names to colors and pressure_______*/
#define WHITE 0x0000  // Black->White
#define YELLOW 0xFFE0 // Yellow->Blue// 0x001F //Blue->Yellow//
#define CYAN 0x07FF   // Cyan -> Red //0xF800 //Red->Cyan
#define PINK 0xF81F   // Pink -> Green //0x07E0 //Green-> Pink//
#define RED 0xF800    // Red->Cyan//0x07FF //Cyan -> Red
#define GREEN 0x07E0  // Green-> Pink//0xF81F //Pink -> Green //
#define BLUE 0x001F   // Blue->Yellow//0xFFE0 //Yellow->Blue
#define BLACK 0xFFFF  // White-> Black
#define GREY 0x8410   // Medium Grey (RGB565)
#define ORANGE 0xFD20 // Jingga 🍊
/*____Calibrate TFT LCD_____*/

String version_ = "VER.0.1";

int MINPRESSURE = 460;
//String header="yellow";
//const int XP=27,XM=15,YP=4,YM=14; //240x320 ID=0x9341
//const int TS_MINX=957,TS_MAXX=165,TS_MAXY=890,TS_MINY=153;


//YELLOW HEADER
//UI1
 String ui_nya="UI1.bin";
 String header="yellow";
 int YP=15,XM=14,YM=27,XP=4;
//YELLOW HEADER
 int TS_MINX =920;
 int TS_MINY =940;
 int TS_MAXX =150;
 int TS_MAXY =120;

 
////YELLOW HEADER tompel naik banyak
////UI2
//String ui_nya="UI2.bin";
// String header="yellow";
// int YP=15,XM=14,YM=27,XP=4;
////YELLOW HEADER
// int TS_MINX =920;
// int TS_MINY =940;
// int TS_MAXX =210;
// int TS_MAXY =120;

////YELLOW HEADER x_y_rotate to left
////UI3
//String ui_nya="UI3.bin";
// String header="yellow";
// int YP=27,XM=4,YM=15,XP=14;
////YELLOW HEADER
// int TS_MINX =1000;
// int TS_MINY =950;
// int TS_MAXX =180;
// int TS_MAXY =120;


////YELLOW HEADER x_y_rotate
////UI4
//String ui_nya="UI4.bin";
// String header="yellow";
// int YP=27,XM=4,YM=15,XP=14;
////YELLOW HEADER
// int TS_MINX =920;
// int TS_MINY =940;
// int TS_MAXX =150;
// int TS_MAXY =120;

////YELLOW OTHER HEADER
//UI5
//String ui_nya="UI5.bin";
// String header="y_other";
// int YP=4,XM=27,YM=14,XP=15;
////YELLOW OTHER HEADER
////TS_MINX =120,TS_MINY =120,TS_MAXX =1195,TS_MAXY =740;
// int TS_MINX =120; //paint dot,semakin kecil, semakin ke tengah layar dr titik sentuh
// int TS_MINY =120;
// int TS_MAXX =1195; //paint dot,semakin kecil, semakin ke tengah layar dr titik sentuh
// int TS_MAXY =740;


////BLACK HEADER
////UI6
// String ui_nya="UI6.bin";
// String header="black";
// int YP=12,XM=33,YM=15,XP=13;//BLACK HEADER
////BLACK HEADER
// #define TS_MINX 139
// #define TS_MINY 141
// #define TS_MAXX 928
// #define TS_MAXY 962

 
//// BLACK HEADER x y rotate _2
//UI7
//String ui_nya="UI7.bin";
//String header = "black";
//int XP = 15, YP = 14, XM = 27, YM = 4; // M;P DIBALIK UNTUK TOUCH TERBALIK,TERGANTUNG AXIS
//// BLACK HEADER
//int TS_MINX = 925;
//int TS_MINY = 950;
//int TS_MAXX = 210;
//int TS_MAXY = 120;


////BLACK FLASH ROTATE YELLOW HEADER y1
////UI8
//String ui_nya="UI8.bin";
// String header="black";
// int YP=12,XM=13,YM=15,XP=33;//BLACK HEADER
////BLACK HEADER
// #define TS_MINX 139
// #define TS_MINY 141
// #define TS_MAXX 928
// #define TS_MAXY 962

////BLACK OTHER HEADER
////UI9
//String ui_nya="UI9.bin";
// String header="b_other";
// int YP=15,XM=13,YM=12,XP=33;
////BLACK OTHER HEADER
// int TS_MINX =150;
// int TS_MINY =120;
// int TS_MAXX =920;
// int TS_MAXY =940;


TouchScreen ts(XP, YP, XM, YM, 300); // re-initialised after diagnose
TSPoint tp;                          // global point

static const unsigned char PROGMEM logo_knn2[] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x7f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x03, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0xff, 0xff, 0xe7, 0xff, 0xe0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xf0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xff, 0xf0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0xff, 0xfc, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x07, 0xff, 0xf3, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x0f, 0xff, 0xe3, 0xff, 0xff, 0xf0, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1f, 0xff, 0xc1, 0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3f, 0xff, 0x81, 0xff, 0xff, 0xe0, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x7f, 0xff, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x1f, 0xff, 0x80, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x00,
        0x00, 0x00, 0x01, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x00, 0x00,
        0x00, 0x00, 0x03, 0xff, 0xf8, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00,
        0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00,
        0x00, 0x00, 0x0f, 0xff, 0xe0, 0x1f, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00,
        0x00, 0x00, 0x1f, 0xff, 0xc0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00,
        0x00, 0x00, 0x3f, 0xff, 0x80, 0x7f, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00,
        0x00, 0x00, 0x7f, 0xff, 0x00, 0x7f, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xfc, 0x01, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
        0x00, 0x01, 0xff, 0xf8, 0x03, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
        0x00, 0x03, 0xff, 0xf8, 0x07, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00,
        0x00, 0x07, 0xff, 0xf0, 0x07, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00,
        0x00, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x03, 0xe1, 0xfc, 0x1f, 0xc0, 0xf8, 0x3e, 0x7c, 0xf8, 0x3e, 0x7f, 0xf0, 0x07, 0xfc, 0x00,
        0x07, 0xe7, 0xf8, 0x3f, 0xc1, 0xfc, 0x3e, 0xfd, 0xfc, 0x7e, 0xff, 0xf8, 0x3f, 0xff, 0x00,
        0x07, 0xcf, 0xe0, 0x7f, 0xc1, 0xfc, 0x7e, 0xf9, 0xfc, 0x7d, 0xff, 0xfc, 0x7f, 0xff, 0x80,
        0x0f, 0xff, 0x80, 0xff, 0xc3, 0xfe, 0x7d, 0xfb, 0xfc, 0xfd, 0xf8, 0x7e, 0xfc, 0x1f, 0x80,
        0x0f, 0xfe, 0x01, 0xff, 0xc3, 0xfe, 0xf9, 0xf3, 0xfe, 0xf9, 0xf0, 0x7f, 0xf8, 0x1f, 0x80,
        0x1f, 0xfe, 0x03, 0xf7, 0xc7, 0xff, 0xfb, 0xf7, 0xff, 0xfb, 0xf0, 0x7f, 0xf0, 0x1f, 0x80,
        0x1f, 0xff, 0x07, 0xff, 0xc7, 0xff, 0xfb, 0xe7, 0xff, 0xf3, 0xe0, 0xff, 0xf0, 0x1f, 0x00,
        0x3f, 0xbf, 0x0f, 0xff, 0xc7, 0xdf, 0xf7, 0xef, 0x9f, 0xf7, 0xe1, 0xff, 0xf0, 0x3f, 0x00,
        0x3e, 0x1f, 0x9f, 0xff, 0xcf, 0x9f, 0xf7, 0xcf, 0x9f, 0xe7, 0xc3, 0xfb, 0xf0, 0xfe, 0x00,
        0x7e, 0x1f, 0xff, 0x07, 0xcf, 0x8f, 0xef, 0xdf, 0x0f, 0xef, 0xff, 0xe1, 0xff, 0xfc, 0x00,
        0x7e, 0x0f, 0xfe, 0x07, 0xff, 0x87, 0xef, 0x9f, 0x0f, 0xcf, 0xff, 0xc0, 0xff, 0xf0, 0x00,
        0x7c, 0x07, 0xfc, 0x07, 0xdf, 0x07, 0xcf, 0xbe, 0x07, 0xdf, 0xfc, 0x00, 0x3f, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const char *ssid = "DESKTOP_ku"; 
const char *password = "12345678";
int masuk_tab=0;
int lebar_tombol=80;
int tinggi_tombol=60;
String keypad[4][3] = {
  { "7", "8", "9"},
  { "4", "5", "6"},
  { "1", "2", "3"},
  { "Hapus", "0", "OK"}
};
int X,Y;
String pass_ota_edit = "";
String Angka_type = "";
const int qrCodeVersion = 1;
const int pixelSize = 7;
// int set_bgbutton=0;
int sparator_btn = 20;
int masuk_tombolangka = 0;

/* ============ OTA FILE URL ============ */
const char *firmwareURL ="https://raw.githubusercontent.com/giyong7/firmware_ota/main/esp32_ota.bin";
//="https://drive.google.com/uc?export=download&id=1ChXUq8ImvtqXHhC0pZf6PYEL6H4VKC29";// "http://192.168.0.65/it/firmware_ota/esp32_ota.bin"; // ganti sesuai server
void Logo_OTA()
{
    tft.drawBitmap(60, 90, logo_knn2, 114, 66, GREEN);
    tft.setCursor(29, 168); //(27,168);
    tft.setTextSize(3);
    tft.setTextColor(GREEN);
    tft.print("OTA UPDATE");
    tft.setCursor(130, 193); //(124,300);//(10,5);
    tft.setTextSize(2);
    tft.setTextColor(YELLOW);
    // tft.print(version_);
}
void OTA_progres(int cb_wifi)
{
    tft.drawBitmap(60, 90, logo_knn2, 114, 66, PINK);
    tft.setCursor(29, 168); //(27,168);
    tft.setTextSize(3);
    tft.setTextColor(PINK);
    tft.print("OTA UPDATE");
    tft.setCursor(5, 206); //(27,168);
    tft.setTextSize(2);
    tft.setTextColor(PINK);
    tft.print("Connecting ");
    tft.print(ssid);
    tft.setCursor(110, 250); //(27,168);
    tft.setTextSize(4);
    tft.setTextColor(PINK);
    tft.print(cb_wifi);
}
// void tampilkan(String teks) {
//   tft.setTextSize(2);
//   tft.setTextColor(PINK);
//   tft.setCursor(5, 150);
//   tft.println(teks);
// }
int yPos = 0;
const int lineHeight = 16;
const int layarBawah = 300; // tinggi TFT kamu

void tampilkan(String teks)
{
    tft.setTextSize(2);
    tft.setTextColor(PINK);
    tft.setCursor(5, yPos);
    tft.println(teks);

    yPos += lineHeight;

    if (yPos > layarBawah)
    {
        tft.fillRect(0, 0, 320, 480, WHITE); // bersihkan area teks
        yPos = 0;
    }
}
//void tunggu_ota_success()
//{
//    String data_tx = "";
//
//    while (true)
//    { // ⬅ tunggu sampai OTA SUCCESS
//        while (Serial.available())
//        {
//            char c = Serial.read();
//
//            if (c == '\n')
//            {
//                if (data_tx.indexOf("OTA SUCCESS") >= 0)
//                {
//                    tft.fillScreen(WHITE);
//                    Logo_OTA();
//                    delay(100);
//                    return; // ⬅ KELUAR fungsi (sudah sukses)
//                }
//
//                tampilkan(data_tx);
//                data_tx = "";
//            }
//            else
//            {
//                data_tx += c;
//            }
//
//            // pengaman buffer
//            if (data_tx.length() > 200)
//                data_tx = "";
//        }
//
//        delay(5); // ⬅ penting biar watchdog aman
//    }
//}
void ota_1()
{
    tft.fillScreen(WHITE);
    //tunggu_ota_success();
     Logo_OTA();
    delay(200);

    // URUTAN OTA
    // 1.aktifkan wifi
    WiFi.mode(WIFI_STA);
    Serial.println("🔌 Connecting WiFi...");

    WiFi.begin(ssid, password);
    int kali_check_wifi = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        OTA_progres(kali_check_wifi);
        delay(500);
        kali_check_wifi++;
        tft.fillScreen(WHITE);
        if (kali_check_wifi >= 30)
        {
            Serial.println(String(kali_check_wifi) + " kali coba");
            tft.fillScreen(RED);
            tft.setCursor(22, 30); //(27,168);
            tft.setTextSize(8);
            tft.setTextColor(YELLOW);
            tft.print("WiFi");
            tft.setCursor(22, 130); //(27,168);
            tft.setTextSize(8);
            tft.setTextColor(YELLOW);
            tft.print("Fail");
            delay(500);
            tft.fillScreen(WHITE);
            tft.setCursor(22, 30); //(27,168);
            tft.setTextSize(8);
            tft.setTextColor(GREEN);
            tft.print("WiFi");
            tft.setCursor(22, 130); //(27,168);
            tft.setTextSize(8);
            tft.setTextColor(GREEN);
            tft.print("Fail");
            delay(500);
            tft.fillScreen(BLACK);
            tft.setCursor(22, 30); //(27,168);
            tft.setTextSize(8);
            tft.setTextColor(RED);
            tft.print("WiFi");
            tft.setCursor(22, 130); //(27,168);
            tft.setTextSize(8);
            tft.setTextColor(RED);
            tft.print("Fail");
            tft.setCursor(17, 250); //(27,168);
            tft.setTextSize(2);
            tft.setTextColor(BLUE);
            tft.print("RESTART DEVICE");
            delay(500);
            // return;
            ESP.restart();
        }
    }

    Serial.println("\n✅ WiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    /* mulai OTA */
    otaUpdate();
}
//void otaUpdate()
//{
//    WiFiClient client;
//    HTTPClient http;
//
//    Serial.println("⬇️ Download firmware...");
//
//    if (!http.begin(client, firmwareURL))
//    {
//        Serial.println("❌ HTTP begin failed");
//        return;
//    }
//
//    int httpCode = http.GET();
//    if (httpCode != HTTP_CODE_OK)
//    {
//        Serial.printf("❌ HTTP error: %d\n", httpCode);
//        http.end();
//        return;
//    }
//
//    int contentLength = http.getSize();
//    if (contentLength <= 0)
//    {
//        Serial.println("❌ Invalid content length");
//        http.end();
//        return;
//    }
//
//    Serial.printf("📦 Firmware size: %d bytes\n", contentLength);
//
//    if (!Update.begin(contentLength))
//    {
//        Serial.println("❌ Not enough space");
//        http.end();
//        return;
//    }
//
//    WiFiClient *stream = http.getStreamPtr();
//    size_t written = 0;
//    uint8_t buff[1024];
//    tft.fillScreen(WHITE);
//    tft.setCursor(12, 175); //(27,168);
//    tft.setTextSize(3);
//    tft.setTextColor(GREEN);
//    tft.print("UPGRADING...");
//    delay(300);
//    tft.fillScreen(WHITE);
//    while (http.connected() && written < contentLength)
//    {
//        static int lastProgress = -1;
//        int progress = (written * 100) / contentLength;
//        String teks = "PROSES ";
//        String hasil;
//        if (progress < 10)
//        {
//            hasil = "00" + String(progress);
//        }
//        else if (progress < 100)
//        {
//            hasil = "0" + String(progress);
//        }
//        else
//        {
//            hasil = String(progress);
//        }
//        teks += hasil;
//        teks += " %";
//        size_t avail = stream->available();
//        if (avail)
//        {
//            int len = stream->readBytes(buff, min((size_t)1024, avail));
//            Update.write(buff, len);
//            written += len;
//            // Serial.printf("⬇️ %d%%\r", (written * 100) / contentLength);
//        }
//        delay(1);
//        int blok = 0;
//        if (progress != lastProgress)
//        {
//            blok++;
//            if (blok == 1)
//            {
//                tft.fillRect(144, 165, 60, 30, WHITE);
//            }
//            tft.setCursor(20, 165); //(27,168);
//            tft.setTextSize(3);
//            tft.setTextColor(GREEN);
//            tft.print(teks);
//            
//                // ===== BAR =====
//                int barWidth = (progress * 196) / 100;
//                tft.fillRect(22, 200, barWidth, 8, GREEN);
//               // Serial.printf("⬇️ %d%%\n", progress);
//        }
//    }
//
//    if (Update.end())
//    {
//        if (Update.isFinished())
//        {
//            Serial.println("\n✅ OTA SUCCESS, REBOOT...");
//            tft.fillScreen(WHITE);
//            tft.setCursor(12, 175); //(27,168);
//            tft.setTextSize(3);
//            tft.setTextColor(GREEN);
//            tft.print("OTA SUCCESS");
//            tft.setCursor(12, 215); //(27,168);
//            tft.setTextSize(3);
//            tft.setTextColor(GREEN);
//            tft.print("REBOOT...");
//            delay(1000);
//            WiFi.disconnect(true); // true = hapus config WiFi
//            delay(100);
//            WiFi.mode(WIFI_OFF); // matikan radio WiFi
//            ESP.restart();
//        }
//        else
//        {
//            Serial.println("❌ OTA NOT FINISHED");
//            tft.fillScreen(BLACK);
//            tft.setCursor(60, 175); //(27,168);
//            tft.setTextSize(3);
//            tft.setTextColor(RED);
//            tft.print("OTA");
//            tft.setCursor(5, 215); //(27,168);
//            tft.setTextSize(3);
//            tft.setTextColor(RED);
//            tft.print("NOT FINISHED");
//            delay(1000);
//            WiFi.disconnect(true); // true = hapus config WiFi
//            delay(100);
//            WiFi.mode(WIFI_OFF); // matikan radio WiFi
//            ESP.restart();
//        }
//    }
//    else
//    {
//        Serial.printf("❌ Update error: %s\n", Update.errorString());
//        tft.fillScreen(BLACK);
//        tft.setCursor(5, 175); //(27,168);
//        tft.setTextSize(2);
//        tft.setTextColor(RED);
//        tft.print("Update error:");
//        tft.setCursor(5, 215); //(27,168);
//        tft.setTextSize(3);
//        tft.setTextColor(RED);
//        tft.print(Update.errorString());
//        delay(1000);
//        WiFi.disconnect(true); // true = hapus config WiFi
//        delay(100);
//        WiFi.mode(WIFI_OFF); // matikan radio WiFi
//        ESP.restart();
//    }
//
//    http.end();
//}

void otaUpdate()
{
    WiFiClientSecure client;
    client.setInsecure();   // GitHub HTTPS

    HTTPClient http;
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

    Serial.println("⬇️ Download firmware (GitHub)...");

    if (!http.begin(client, firmwareURL))
    {
        Serial.println("❌ HTTP begin failed");
        return;
    }

    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("❌ HTTP error: %d\n", httpCode);
        http.end();
        WiFi.disconnect(true); // true = hapus config WiFi
            delay(100);
            WiFi.mode(WIFI_OFF); // matikan radio WiFi
            ESP.restart();
    }

    int contentLength = http.getSize();
    Serial.printf("📦 Firmware size: %d\n", contentLength);

    bool canBegin = (contentLength > 0)
                    ? Update.begin(contentLength)
                    : Update.begin(UPDATE_SIZE_UNKNOWN);

    if (!canBegin)
    {
        Serial.println("❌ OTA begin failed");
        http.end();
        delay(100);
           tft.fillScreen(BLACK);
            tft.setCursor(60, 175); //(27,168);
            tft.setTextSize(3);
            tft.setTextColor(RED);
            tft.print("OTA");
            tft.setCursor(5, 215); //(27,168);
            tft.setTextSize(3);
            tft.setTextColor(RED);
            tft.print("BEGIN FAILED");
            delay(1000);
            WiFi.disconnect(true); // true = hapus config WiFi
            delay(100);
            WiFi.mode(WIFI_OFF); // matikan radio WiFi
            ESP.restart();
    }

    WiFiClient *stream = http.getStreamPtr();
    size_t written = 0;
    uint8_t buff[1024];
    
    tft.fillScreen(WHITE);
    tft.setCursor(12, 175);
    tft.setTextSize(3);
    tft.setTextColor(GREEN);
    tft.print("UPGRADING...");
    delay(300);
    tft.fillScreen(WHITE);
    
    int lastProgress = -1;
    
    while (http.connected() && (written < contentLength || contentLength == -1))
    {
        size_t avail = stream->available();
        if (avail)
        {
            int len = stream->readBytes(buff, min((size_t)1024, avail));
            Update.write(buff, len);
            written += len;
    
            // ===== HITUNG PROGRESS =====
            int progress = (contentLength > 0)
                           ? (written * 100) / contentLength
                           : 0;
    
            if (progress != lastProgress)
            {
                lastProgress = progress;
    
                // ===== TEKS =====
                tft.fillRect(20, 165, 220, 30, WHITE);
                tft.setCursor(20, 165);
                tft.setTextSize(3);
                tft.setTextColor(GREEN);
    
                String teks = "PROSES ";
                if (progress < 10) teks += "00";
                else if (progress < 100) teks += "0";
                teks += String(progress);
                teks += " %";
                tft.print(teks);
    
                // ===== BAR =====
                int barWidth = (progress * 196) / 100;
                tft.fillRect(22, 200, barWidth, 8, GREEN);
    
                Serial.printf("⬇️ %d%%\n", progress);
            }
        }
        delay(1);
    }

    if (Update.end())
    {
        if (Update.isFinished())
        {
            Serial.printf("✅ OTA SUCCESS (%d bytes)\n", written);
            delay(100);
            tft.fillScreen(WHITE);
            tft.setCursor(12, 175); //(27,168);
            tft.setTextSize(3);
            tft.setTextColor(GREEN);
            tft.print("OTA SUCCESS");
            tft.setCursor(12, 215); //(27,168);
            tft.setTextSize(3);
            tft.setTextColor(GREEN);
            tft.print("REBOOT...");
            delay(1000);
            WiFi.disconnect(true); // true = hapus config WiFi
            delay(100);
            WiFi.mode(WIFI_OFF); // matikan radio WiFi
            ESP.restart();
        }
        else
        {
            Serial.println("❌ OTA NOT FINISHED");
            tft.fillScreen(BLACK);
            tft.setCursor(60, 175); //(27,168);
            tft.setTextSize(3);
            tft.setTextColor(RED);
            tft.print("OTA");
            tft.setCursor(5, 215); //(27,168);
            tft.setTextSize(3);
            tft.setTextColor(RED);
            tft.print("NOT FINISHED");
            delay(1000);
            WiFi.disconnect(true); // true = hapus config WiFi
            delay(100);
            WiFi.mode(WIFI_OFF); // matikan radio WiFi
            ESP.restart();
        }
    }
    else
    {
        Serial.printf("❌ Update error: %s\n", Update.errorString());
        tft.fillScreen(BLACK);
        tft.setCursor(5, 175); //(27,168);
        tft.setTextSize(2);
        tft.setTextColor(RED);
        tft.print("Update error:");
        tft.setCursor(5, 215); //(27,168);
        tft.setTextSize(3);
        tft.setTextColor(RED);
        tft.print(Update.errorString());
        delay(1000);
        WiFi.disconnect(true); // true = hapus config WiFi
        delay(100);
        WiFi.mode(WIFI_OFF); // matikan radio WiFi
        ESP.restart();
    }

    http.end();
}


void Logo_readTouch()
{
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > MINPRESSURE)
    { // Adjust threshold to avoid noise((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE)){//
        if (header == "black" || header == "yellow")
        {
            Y = map(p.x, TS_MINX, TS_MAXX, 0, 320);
            X = map(p.y, TS_MINY, TS_MAXY, 0, 240);
        }
        else
        {
            Y = map(p.y, TS_MINY, TS_MAXY, 0, 240);
            X = map(p.x, TS_MINX, TS_MAXX, 0, 320);
        }
        tft.fillCircle(X, Y, 10, BLUE);
        tft.fillCircle(X + 3, Y, 5, GREEN);
        delay(50);
    }
}
int masuk_logo = 0;
void Logo()
{
    if (masuk_logo <= 1)
    {
        tft.drawBitmap(60, 90, logo_knn2, 114, 66, GREEN);
        tft.setCursor(29, 168); //(27,168);
        tft.setTextSize(3);
        tft.setTextColor(GREEN);
        tft.print("MANUAL SMS");
        tft.setCursor(130, 193); //(124,300);//(10,5);
        tft.setTextSize(2);
        tft.setTextColor(YELLOW);
        tft.print(version_);
        masuk_logo++;
    }
    Logo_readTouch();
}

void setup()
{
    Serial.begin(115200); 
    tft.reset();       // Always reset at start
    tft.begin(0x9341); // My LCD uses LIL9341 Interface driver IC
    // tft.setRotation(2);
    tft.fillScreen(WHITE);
    Logo();
    delay(600);
    tft.fillScreen(WHITE);
   
}

void loop()
{
  
        if (masuk_tab == 0)
        {
            tft.fillScreen(WHITE);
            masuk_tab = 1;
            pass_ota_display();
        }
        pass_ota_display_touch();
    readTouch();
}
//*new dot on touch
void readTouch()
{
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > MINPRESSURE)
    { // Adjust threshold to avoid noise((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE)){//
        if (header == "black" || header == "yellow")
        {
            Y = map(p.x, TS_MINX, TS_MAXX, 0, 320);
            X = map(p.y, TS_MINY, TS_MAXY, 0, 240);
        }
        else
        {
            Y = map(p.y, TS_MINY, TS_MAXY, 0, 240);
            X = map(p.x, TS_MINX, TS_MAXX, 0, 320);
        }
        //        tft.fillCircle(X, Y, 10, YELLOW);
        //        tft.fillCircle(X+3, Y, 5, GREEN);
        // delay(70);

        //        Serial.print("Touch X: "); Serial.print(X);
        //        Serial.print(" Y: "); Serial.print(Y);Serial.print("");
        //        Serial.print(" Z: "); Serial.println(p.z);
    }
}

void pass_ota_display()
{
    // Draw the Result Box
    tft.fillRect(0, 0, 240, 80, CYAN);
    // //Draw First Column
    tft.fillRect(0, 260, lebar_tombol, tinggi_tombol, YELLOW);

    for (int g = 80; g < 201; g += 60)
    {
        tft.fillRect(0, g, lebar_tombol, tinggi_tombol, GREY);
    }

    //   //Draw Secend Column
    for (int g = 80; g < 261; g += 60)
    {
        tft.fillRect(lebar_tombol, g, lebar_tombol, tinggi_tombol, GREY);
    }

    //// //Draw Third Column
    tft.fillRect(lebar_tombol * 2, 260, lebar_tombol, tinggi_tombol, GREEN);
    for (int g = 80; g < 201; g += 60)
    {
        tft.fillRect(lebar_tombol * 2, g, lebar_tombol, tinggi_tombol, GREY);
    }
    // Draw Horizontal Lines
    for (int h = 80; h <= 320; h += tinggi_tombol)
        tft.drawFastHLine(0, h, 240, YELLOW);
    // Draw Vertical Lines
    for (int v = 0; v <= 240; v += lebar_tombol)
        tft.drawFastVLine(v, 80, 240, YELLOW);
    // Display keypad lables
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            tft.setCursor(22 + (lebar_tombol * i), 100 + (tinggi_tombol * j));
            tft.setTextSize(4);
            tft.setTextColor(BLUE);
            if (j == 3 && i == 0)
            {
                tft.setTextColor(RED);
                tft.setTextSize(2);
                tft.setCursor(8 + (lebar_tombol * i), 105 + (tinggi_tombol * j));
            }
            tft.println(keypad[j][i]);
        }
    }

    tft.fillRect(166, 5, lebar_tombol - 13, tinggi_tombol - 25, WHITE); // BACK
    tft.setTextSize(2);
    tft.setTextColor(YELLOW);
    tft.setCursor(178, 12);
    tft.print("BACK");

    tft.fillRect(5, 60, 220, tinggi_tombol - 30, WHITE); // BACK
    tft.setTextSize(2);
    tft.setTextColor(YELLOW);
    tft.setCursor(17, 67);
    tft.print("KEY TO UPDATE OTA");
}
void pass_ota_edit_Result()
{
    tft.fillRect(0, 0, 160, 50, CYAN); // clear result box
    tft.setCursor(10, 20);
    tft.setTextSize(4);
    tft.setTextColor(BLUE);
    tft.println(pass_ota_edit); // update new value
    tft.setCursor(10, 60);
    tft.setTextSize(2);
    tft.setTextColor(BLUE);
}
void pass_ota_display_touch()
{
    if (X < (lebar_tombol - sparator_btn) && X > 0) // Detecting Buttons on Column 1
    {
        if (Y > 267 && Y < 318) // if (Y>0 && Y<85) //"HAPUS" If cancel Button is pressed
        {
            int back = pass_ota_edit.length();
            if (pass_ota_edit.length() >= 0)
            {
                pass_ota_edit.remove(pass_ota_edit.length() - 1);
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
        }
        if (Y > 205 && Y < 258) // if (Y>85 && Y<140) //If Button 1 is pressed
        {
            if (pass_ota_edit.length() <= 6)
            {
            pass_ota_edit += "1";
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
        }

        if (Y > 144 && Y < 198) // If Button 4 is pressed
        {
          if (pass_ota_edit.length() <= 6)
            {
            pass_ota_edit += "4";
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
        }

        if (Y > 83 && Y < 140) // if (Y>192 && Y<245) //If Button 7 is pressed
        {
          if (pass_ota_edit.length() <= 6)
            {
            pass_ota_edit += "7";
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
        }
        pass_ota_edit_Result();
    }
    if (X > (lebar_tombol - (sparator_btn / 2)) && X < ((lebar_tombol * 2) - sparator_btn)) // Detecting Buttons on Column 2
    {
        if (Y > 267 && Y < 318) // if (Y>0 && Y<85)
        {
            if (pass_ota_edit.length() > 0)
            {   if (pass_ota_edit.length() <= 6)
                {
                pass_ota_edit += "0";
                }
                tft.fillCircle(X, Y, 10, YELLOW);
                tft.fillCircle(X + 3, Y, 5, GREEN);
                delay(240);
                Y = 0;
                X = 0;
            }
        }
        if (Y > 205 && Y < 258) // if (Y>85 && Y<140)
        {
            if (pass_ota_edit.length() <= 6)
            {
            pass_ota_edit += "2";
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
        }
        if (Y > 144 && Y < 198)
        {
            if (pass_ota_edit.length() <= 6)
            {
            pass_ota_edit += "5";
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
        }
        if (Y > 83 && Y < 140)
        {
            if (pass_ota_edit.length() <= 6)
            {
            pass_ota_edit += "8";
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
        }
        pass_ota_edit_Result();
    }
    if (X < (lebar_tombol * 3) && X > ((lebar_tombol * 2) - (sparator_btn / 2))) // Detecting Buttons on Column 3
    {
        if (Y > 267 && Y < 318) // TEKAN OK PASS
        {
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(50);
            Y = 0;
            X = 0;
            // Serial.println("OK");
            if (pass_ota_edit == "5555")
            {
            String krim_dt = "{\"ota\":\"1\"}";
            //Serial.println(krim_dt);
            delay(50);
            ota_1();
            }            
            
            else if (pass_ota_edit != "5555")
            {              
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(50);
            Y = 0;
            X = 0;
            for(int mn=0;mn<3;mn++)
            {
            tft.fillScreen(BLACK);
            tft.setCursor(2, 75);
            tft.setTextSize(2);
            tft.setTextColor(RED);
            tft.print("XX GAGAL UPDATE XX");
            tft.setCursor(10, 110);
            tft.setTextSize(3);
            tft.setTextColor(RED);
            tft.print("KEY WRONG !!");
            delay(300);
            }
            for(int mn=0;mn<2;mn++)
            {
            tft.fillScreen(WHITE);
            tft.setCursor(2, 75);
            tft.setTextSize(2);
            tft.setTextColor(BLUE);
            tft.print("XX GAGAL UPDATE XX");
            tft.setCursor(10, 110);
            tft.setTextSize(3);
            tft.setTextColor(GREEN);
            tft.print("RESTART !!");
            delay(300);
            }
            ESP.restart();
            }
        }
        if (Y > 205 && Y < 258) // if (Y>85 && Y<140)
        {
          if (pass_ota_edit.length() <= 6)
            {
            pass_ota_edit += "3";
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
            pass_ota_edit_Result();
        }
        if (Y > 144 && Y < 198)
        {
            if (pass_ota_edit.length() <= 6)
            {
            pass_ota_edit += "6";
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
            pass_ota_edit_Result();
        }
        if (Y > 83 && Y < 140) // if (Y>192 && Y<245)
        {
          if (pass_ota_edit.length() <= 6)
            {
            pass_ota_edit += "9";
            }
            tft.fillCircle(X, Y, 10, YELLOW);
            tft.fillCircle(X + 3, Y, 5, GREEN);
            delay(240);
            Y = 0;
            X = 0;
            pass_ota_edit_Result();
        }
    }
    if (X > 167 && X < 244) //
    {
        if (Y < 75 && Y > (-20)) // BACK BATAL OTA
        {
//            Serial.println("LOGOUT");
//            String krim_dt = "{\"reset\":\"3\"}";
//            Serial.println(krim_dt);
//            log_out_ = true;
            delay(100);
            Y = 0;
            X = 0;
            ESP.restart();
        }
    }
}
