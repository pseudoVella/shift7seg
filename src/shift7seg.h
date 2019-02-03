#ifndef shift7seg_h
#define shift7seg_h

#include "Arduino.h"

class shift7seg{
	public:
		//constructor, choose pins to use as well as display size
		shift7seg(const uint8_t _dataPin, const uint8_t _latchPin, const uint8_t _clkPin, const uint8_t _num_digits);

		static const uint8_t num_size = 10;
		static const uint8_t numbers[num_size] =               // 7 segment values for decimals 0..9
		{
		//TRUTH TABLE    |   0 = segment on
		//ABCDEFGH       |   1 = segment off
		B00000011,  //0  |        A
		B10011111,  //1  |      -----
		B00100101,  //2  |   F |     | B
		B00001101,  //3  |     |  G  |
		B10011001,  //4  |      -----
		B01001001,  //5  |   E |     | C
		B01000001,  //6  |     |     |
		B00011111,  //7  |      -----
		B00000001,  //8  |        D
		B00011001	//9  |
		};

		static const uint8_t capital[13] =
		{
		B00010001,  //A or R, 0
		B00000001,  //B 1
		B01100011,  //C 2
		B00000011,  //D or O, 3
		B01100001,  //E 4
		B01110001,  //F 5
		B01000001,	//G 6
		B10010001,  //H 7
		B10000111,	//J 8
		B11100011,  //L 9
		B00110001,  //P 10
		B01001001,  //S 11
		B10000011  //U or V, 12
		};

		static const uint8_t lower[9] =
		{
		B11000001,  //b 0
		B11100101,  //c 1
		B10000101,  //d 2
		B11010001,  //h 3
		B10011111,  //l 4
		B11010101,  //n 5
		B11000101,  //o 6
		B11110101,  //r 7
		B11000111   //u or v, 8
		};

		// value B11111111 for blanking the 7 segment display
		static const uint8_t blank = 255;

		void blank_display();  // turns off all segments
		void err_display();    // used for handling/debugging errors

		//used to display an array of values from numbers[], capital[] and lower[]
		void load_rdata(const uint8_t* _data, const uint8_t howMany);

		//the values of _data should all be lower than 10
		//values greater than 10 become a blank space
		void load_data(const uint8_t *_data, const uint8_t howMany);

		//splits decimals up and converts them ie: 256 = 2, 5, 6
		void load_data(const uint8_t _data);
		void load_data(const int _data);

		//the values of _data should all be chosen from lower[] and capital[]
		//values that are not available become a blank space
		void load_data(const char *_data, const uint8_t howMany = 1);
		void load_data(const String _data, const uint8_t repeat = 1, size_t howMany = 1, const uint8_t start = 0);

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
