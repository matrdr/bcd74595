#ifndef BCD74595_H
#define BCD74595_H

#include <Arduino.h>

#include <stdint.h>
#include <stddef.h>
#include <math.h>

template <unsigned int segments, typename BcdType = uint32_t>
class Bcd74595Base {
public:
	Bcd74595Base(const uint8_t sckPin,
	             const uint8_t siPin,
	             const uint8_t rckPin) :
		sckPin{sckPin},
		siPin{siPin},
		rckPin{rckPin},
		maxNumber{static_cast<unsigned int>(pow(10U, segments)) - 1U}
	{
		static_assert(segments > 0U && segments <= 8U,
		              "Invalid amount of segments. Library supports 1 "
			      "to 8 segments.");

		digitalWrite(rckPin, LOW);
		digitalWrite(sckPin, LOW);
	}

	unsigned int GetMaxDisplayableNumber() const
	{
		return maxNumber;
	}

	template <class T>
	bool WriteNumber(T number)
	{
		auto of{false};

		if (number > maxNumber) {
			of = true;
			number = maxNumber;
		}

		ProcessNumber(number);

		return of;
	}

protected:
	~Bcd74595Base() = default;

private:
	const uint8_t sckPin;
	const uint8_t siPin;
	const uint8_t rckPin;
	const unsigned int maxNumber;

	template <class T>
	static BcdType ToBcdByteArray(T number)
	{
		BcdType data{0U};

		for (size_t i{0U}; i < 2U * sizeof(BcdType); i++) {
			const T quot{number / 10U};
			const T rem{number - 10U * quot};

			data |= (rem << (4U * i));
			number = quot;
		}

		return data;
	}

	template <class T>
	void ProcessNumber(const T number)
	{
		const auto data{ToBcdByteArray(number)};

		ShiftData(data);
	}

	void ShiftData(const BcdType data)
	{
		const auto pData{reinterpret_cast<const uint8_t *>(&data)};

		for (size_t i{0U}; i < sizeof(BcdType); i++) {
			shiftOut(siPin, sckPin, LSBFIRST, pData[i]);
		}

		// Create a rising edge on RCK.
		digitalWrite(rckPin, HIGH);
		digitalWrite(rckPin, LOW);
	}
};

template <unsigned int segments>
class Bcd74595 : public Bcd74595Base<segments> {
	using Bcd74595Base<segments>::Bcd74595Base;
};

template <>
class Bcd74595<1U> : public Bcd74595Base<1U, uint8_t> {
	using Bcd74595Base::Bcd74595Base;
};

template <>
class Bcd74595<2U> : public Bcd74595Base<2U, uint8_t> {
	using Bcd74595Base::Bcd74595Base;
};

template <>
class Bcd74595<3U> : public Bcd74595Base<3U, uint16_t> {
	using Bcd74595Base::Bcd74595Base;
};

template <>
class Bcd74595<4U> : public Bcd74595Base<4U, uint16_t> {
	using Bcd74595Base::Bcd74595Base;
};

#endif
