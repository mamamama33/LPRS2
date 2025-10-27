c#include <TinyGPSPlus.h>

extern TinyGPSPlus gpsClient;

class gpsData
{
  public:
    char timeBuff[6] = "";
    char dateBuff[11] = "";
    char latBuff[11] = "";
    char lngBuff[11] = "";

  private:

};

class gpsClass : public gpsData
{

  public:

    gpsClass(TinyGPSPlus &addressTinyGPSPlus) : gpsClient(addressTinyGPSPlus){}

    void updGPSdata(void);

  private:
    TinyGPSPlus &gpsClient;
    void ResetAll(void);
    bool getGPSdata(void);
    uint8_t checkDST(uint8_t _hour);

    uint8_t hour = 0, day = 0, month = 0;
    uint16_t year = 0;

};