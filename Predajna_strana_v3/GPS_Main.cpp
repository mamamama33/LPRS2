#include "GPS_Main.h"

void gpsClass::ResetAll(void)
{

  memset(latBuff, 0, sizeof(latBuff));
  memset(lngBuff, 0, sizeof(lngBuff));
  memset(dateBuff, 0, sizeof(dateBuff));
  memset(timeBuff, 0, sizeof(timeBuff));

  hour = 0;
  day = 0;
  month = 0;
  year = 0;

}

uint8_t gpsClass::checkDST(uint8_t _hour) // letnje računanje vremena
{
  if (month < 3 || month > 10) {
    return 1;
  }

  if (month > 3 && month < 10) {
    return 2;
  }

  if (day < 25) {
   return ((month == 10) ? 2 : 1);
  }

  if (year >= 2000) {
    year -= 2000;
  }
  uint8_t dayOfWeekValue = ((year + year / 4) % 7 + day + ((month == 10) ? 6 : 9)) % 7; // skraćeni proračun za 21. vek
  uint8_t previousSunday = day - dayOfWeekValue;

  if (month == 3) { // March
    if (dayOfWeekValue == 0) { // Sunday
      if ((previousSunday >= 25) && (_hour >= 2)) { // on or after 25th and greater than or equal to 2:00h
        return 2;
      }
    } else if (previousSunday >= 25) {
      return 2;
    }
  }

  if (month == 10) { // October
    if (dayOfWeekValue == 0) { // Sunday
      if ((previousSunday >= 25) && (_hour <= 1)) { // on or after 25th and less than 2:00h
        return 2;
      } else if (previousSunday < 25) { // it is not yet the last Sunday
        return 2;
      }
    } else if (previousSunday < 25) {
      return 2;
    }
  }

  return 1;
}
void gpsClass::updGPSdata(void)
{

  ResetAll();

  if (gpsClient.location.isValid())
  {
    dtostrf(gpsClient.location.lat(), 9, 6, latBuff);
    dtostrf(gpsClient.location.lng(), 9, 6, lngBuff);
  }
  else
  {
    sprintf(latBuff, "----------");
    sprintf(lngBuff, "----------");
  }

  if (gpsClient.date.isValid())
  {
    year = gpsClient.date.year();
    month = gpsClient.date.month();
    day = gpsClient.date.day();
    sprintf(dateBuff, "%.2d.%.2d.%.4d", day, month, year);
  }
  else
  {
    sprintf(dateBuff, "----------");
  }

  if (gpsClient.time.isValid())
  {
    hour = gpsClient.time.hour() + checkDST(gpsClient.time.hour()); // uračunat i + 1 time offset u odnosu na GMT
    if (hour >= 24) {
      hour -= 24;
    }
    sprintf(timeBuff, "%.2d:%.2d", hour, gpsClient.time.minute());
  }
  else
  {
    sprintf(timeBuff, "-----");
  }

  return;
};