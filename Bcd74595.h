#ifndef BCD74595_H
#define BCD74595_H

#include <Arduino.h>

#include <stdint.h>
#include <stddef.h>

class Bcd74595 {
  public:
    Bcd74595(uint8_t sckPin, uint8_t siPin,
             uint8_t rckPin, unsigned int segments);

    unsigned int GetMaxDisplayableNumber() const;

    template <class T>
    bool WriteNumber(T number)
    {
      auto of{false};

      if (number > maxNumber) {
        of = true;
        number = maxNumber;
      }

      if (segments <= 2U) {
        ProcessNumber<T, uint8_t>(number);
      } else if (segments <= 4U) {
        ProcessNumber<T, uint16_t>(number);
      } else if (segments <= 8U) {
        ProcessNumber<T, uint32_t>(number);
      }

      return of;
    }

  private:
    const unsigned int segments;
    const uint8_t sckPin;
    const uint8_t siPin;
    const uint8_t rckPin;
    unsigned int maxNumber;

    template <class T, class Data>
    static Data ToBcdByteArray(T number)
    {
      Data data{0U};

      for (size_t i{0U}; i < 2U * sizeof(Data); i++) {
        const T quot{number / 10U};
        const T rem{number - 10U * quot};

        data |= (rem << (4U * i));
        number = quot;
      }

      return data;
    }

    template <class T, class Data>
    void ProcessNumber(T number)
    {
      const auto data{ToBcdByteArray<T, Data>(number)};

      ShiftData(data);
    }

    template <class Data>
    void ShiftData(const Data data) {
      const auto pData{reinterpret_cast<const uint8_t *>(&data)};

      for (size_t i{0U}; i < sizeof(Data); i++) {
        shiftOut(siPin, sckPin, LSBFIRST, pData[i]);
      }

      // Create a rising edge on RCK.
      digitalWrite(rckPin, HIGH);
      digitalWrite(rckPin, LOW);
    }
};

#endif
