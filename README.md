# 74595 BCD library

Useful, if you want to encode a number into BCD and then output it to a shift
register compatible with the `shiftOut()` function provided by the Arduino
library.

In my case I use it to control seven segment displays by connecting the outputs
of the shift registers to the inputs of 74LS247 ICs.

This library is implemented using templates to provide a fitting treatment for
different numeric data types without the need for conversions.

Currently it is only possible to display numbers with up to eight digits as
this is the highest possible BCD encoded number that fits into a 32 bit value.

The behaviour of the library adapts to the amount of digits that shall be
displayed by using smaller data types for BCD representation if less digits are
needed. This is supposed to save processing power.
