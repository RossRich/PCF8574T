#include "PCF8574T.h"

PCF8574T::PCF8574T() {
  _address = PCF_ADDRESS_FIXED + PCF_ADDRESS_HARDWARE;
  errorStatus = 0;
  init();
}

PCF8574T::PCF8574T(uint8_t address) : PCF8574T() { _address = address; }

void PCF8574T::init() {
  errorStatus = 0;
  Wire.begin();
  // Wire.setClock(PCF_MAX_SPEED);
  send(0x0);
  delay(50);
}

int8_t PCF8574T::read(uint8_t length, bool isEnd, bool clear) {
  if (clear)
    send(0xFFU, false);

  // return number of bytes read
  uint8_t res = Wire.requestFrom(_address, length, (uint8_t)isEnd);
  if (res != 0) {
    return Wire.read();
  } else {
    setError(TWI_ERR_RECEIVE);
    return 0;
  }
}

void PCF8574T::send(uint8_t data, bool isEnd) {

#if defined(_PCF_DEBUG_)

  Serial.println(F("== Send =="));
  Serial.println(data, BIN);

#endif // _PCF_DEBUG_

  Wire.beginTransmission(_address);

  if (!Wire.write(data)) {
    setError(TWI_ERR_WRITE);
  } else {
    setError(Wire.endTransmission((uint8_t)isEnd));
  }
}

void PCF8574T::send(const uint8_t data[], uint8_t length, bool isEnd) {

#if defined(_PCF_DEBUG_)

  Serial.println(F("== Send arr =="));
  Serial.print("Length: ");
  Serial.println(length);

  uint8_t block = 0;
  for (uint8_t i = 0; i < length; i++) {
    Serial.print(block++);
    Serial.print("\t");
    Serial.println(data[i], BIN);
    if (block >= 6) {
      Serial.println();
      block = 0;
    }
  }

#endif // _PCF_DEBUG_

  Wire.beginTransmission(_address);
  Wire.write(data, length);

  if (Wire.getWriteError()) {
    setError(TWI_ERR_WRITE);
  } else {
    setError(Wire.endTransmission((uint8_t)isEnd));
  }
}