#ifndef shift7seg_h
#define shift7seg_h

#include "Arduino.h"

class shift7seg{
	public:
		//constructor, choose pins to use as well as display size
		shift7seg(const uint8_t& _dataPin, const uint8_t& _latchPin, const uint8_t& _clkPin, const uint8_t& _num_digits);

		static const uint8_t numbers[10];
		static const uint8_t capital[13];
		static const uint8_t lower[9];

		// value B11111111 for blanking the 7 segment display
		static const uint8_t blank = 255;

		void blank_display();  // turns off all segments
		void err_display();    // used for handling/debugging errors

		//used to display an array of values from numbers[], capital[] and lower[]
		void load_rdata(const uint8_t* _data, const uint8_t& howMany);

		//the values of _data should all be lower than 10
		//values greater than 10 become a blank space
		void load_data(const uint8_t *_data, const uint8_t& howMany);

		//splits decimals up and converts them ie: 256 = 2, 5, 6
		void load_data(const uint8_t& _data);
		void load_data(const int& _data);

		//the values of _data should all be chosen from lower[] and capital[]
		//values that are not available become a blank space
		void load_data(const char *_data, const uint8_t& howMany = 1);
		void load_data(const String& _data, const uint8_t& repeat = 1, size_t howMany = 1, const uint8_t& start = 0);

	private:
		/*you can change this value, to whatever you want
		just make sure its either larger than or equal to
		your displays size*/
		static const uint8_t max_digits = 16;
		uint8_t num_digits;
		uint8_t data[max_digits];

		//pins for controlling and sending data to shift registers
		uint8_t dataPin;
		uint8_t latchPin;
		uint8_t clkPin;

		// updates 7 seg display with values stored in data[]
		void update_display();

		//converts decimals into 7 segment numbers[0..9]
		uint8_t convert_num(const uint8_t& OGnum);

		//converts characters into 7 segment characters
		uint8_t convert_char(const char& OGchar);
};
#endif
