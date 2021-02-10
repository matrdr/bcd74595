#include "Bcd74595.h"

Bcd74595::Bcd74595(uint8_t sckPin, uint8_t siPin,
                   uint8_t rckPin, unsigned int segments) :
	sckPin{sckPin},
	siPin{siPin},
	rckPin{rckPin},
	segments{segments}
{
	maxNumber = pow(10U, segments) - 1U;

	digitalWrite(rckPin, LOW);
	digitalWrite(sckPin, LOW);
}

unsigned int Bcd74595::GetMaxDisplayableNumber() const
{
	return maxNumber;
}
