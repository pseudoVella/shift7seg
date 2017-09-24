#ifndef shift7seg_h
#define shift7seg_h

#include "Arduino.h"

class shift7seg
{
	public:
	
		shift7seg(uint8_t _dataPin,   					 //pins for controlling and sending data to shift registers
		          uint8_t _latchPin,
				  uint8_t _clkPin);
		
		static const uint8_t numbers[10] =               // 7 segment values for decimals 0..9
		{
	//TRUTH TABLE        |   0 = segment on
	   //ABCDEFGH        |   1 = segment off
		B00000011,  //0  |        A
		B10011111,  //1  |      -----
		B00100101,  //2  |   F |     | B
		B00001101,  //3  |     |  G  |
		B10011001,  //4  |      -----
		B01001001,  //5  |   E |     | C
		B01000001,  //6  |     |     |
		B00011111,  //7  |      -----
		B00000001,  //8  |        D
		B00011001  //9  |
		};
		
		static const uint8_t capital[11] = 
		{
		B00010001,  //A 0
		B00000001,  //B 1
		B01100011,  //C 2
		B00000011,  //D or O 3
		B01100001,  //E 4
		B01110001,  //F 5
		B10010001,  //H 6
		B11100011,  //L 7
		B00110001,  //P 8
		B01001001,  //S 9
		B10000011  //U 10
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
		B11000111   //u 8
		};
		
		static const uint8_t blank = 255;  				// value B11111111 for blanking the 7 segment display
	
		void blank_display();  // turns off all segments
		void err_display();    // used for handling/debugging errors
		
		//load_data()  
		void load_data(uint8_t _data1, uint8_t _data2, uint8_t _data3, uint8_t _data4);
		void load_data(uint8_t *_data, uint8_t howMany = 1);
		void load_data(int _data);
		
		void load_data(char *_data, uint8_t howMany = 1);
		void load_data(String _data, uint8_t repeat = 1, int howMany = 1, uint8_t start = 0);
	
	private:
		
		uint8_t data1;           						//byte to store data being sent to shift register  1
		uint8_t data2;
		uint8_t data3;
		uint8_t data4;
		
		uint8_t dataPin;   					 //pins for controlling and sending data to shift registers
		uint8_t latchPin;
		uint8_t clkPin;
		
		void update_display(); // updates 7 seg display with values stored in data1..data4
		uint8_t convert_num(uint8_t OGnum);                //converts decimals into 7 segment numbers[0..9]
		uint8_t convert_char(char OGchar);

};

#endif		