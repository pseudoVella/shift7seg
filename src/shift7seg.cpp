/*****************************************************************************************
shift7seg library created by Alex B. Vella september 23 2017.
Uses 4 74hc595 serial in/parallel out shift registers to drive
a 4 digit 7 segment display.
******************************************************************************************/

#include <shift7seg.h>

// 7 segment values for decimals 0..9
const uint8_t shift7seg::numbers[10] =
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

const uint8_t shift7seg::capital[13] =
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

const uint8_t shift7seg::lower[9] =
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


/*****************************************************************************************
initial constructor for shift7seg library
sets appropriate pins as outputs or inputs and prepares display for loading data
******************************************************************************************/

shift7seg::shift7seg(const uint8_t& _dataPin, const uint8_t& _latchPin, const uint8_t& _clkPin, const uint8_t& _num_digits){
	dataPin = _dataPin;
	latchPin = _latchPin;
	clkPin = _clkPin;
	if(_num_digits > max_digits){
		num_digits = max_digits;
	}
	else{
		num_digits = _num_digits;
	}
	pinMode(dataPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
	pinMode(clkPin, OUTPUT);

	blank_display();				//clear the display
}

/*****************************************************************************************/
//load_data()
//Public function
//handles all kinds of variable types, and converts them into values that can be
//displayed on a 7 segment display. it then passes those values to update_display().
/*****************************************************************************************/

void shift7seg::load_rdata(const uint8_t *_data, const uint8_t& howMany){
	uint8_t pos;
  uint8_t offset = 0;

	if(howMany<num_digits){
		while((howMany+offset)<num_digits){
			data[offset] = blank;
			offset++;
		}
		for(pos=offset;pos<num_digits;pos++){
			data[pos] = _data[pos-offset];
		}
	}
	else{
		for(pos=0;pos<num_digits;pos++){
			data[pos] = _data[pos];
		}
		if(pos < (howMany-1)){
			offset = num_digits - 1;
			while(pos<howMany){
				update_display();
				delay(500);
				for(uint8_t b=0;b<offset;b++){
					data[b] = data[b+1];
				}
				data[offset] = _data[pos];
				pos++;
			}
		}
	}
	update_display();
}

void shift7seg::load_data(const uint8_t *_data, const uint8_t& howMany){
	uint8_t pos;
  uint8_t offset = 0;

	if(howMany<num_digits){
		while((howMany+offset)<num_digits){
			data[offset] = blank;
			offset++;
		}
		for(pos=offset;pos<num_digits;pos++){
			data[pos] = convert_num(_data[pos-offset]);
		}
	}
	else{
		for(pos=0;pos<num_digits;pos++){
			data[pos] = convert_num(_data[pos]);
		}
		if(pos < (howMany-1)){
			offset = num_digits - 1;
			while(pos<howMany){
				update_display();
				delay(500);
				for(uint8_t b=0;b<offset;b++){
					data[b] = data[b+1];
				}
				data[offset] = convert_num(_data[pos]);
				pos++;
			}
		}
	}
	update_display();
}

void shift7seg::load_data(const uint8_t& _data){
	uint8_t tmp = 10;
	uint16_t x = tmp;
	uint8_t howMany = 1;
	uint8_t pos = 0;

	while(x < _data){
		x *= tmp;
		howMany++;
	}
	x /= tmp;

	if(howMany < num_digits){
		tmp = 0;
		while((howMany + tmp) < num_digits){
			data[tmp] = blank;
			tmp++;
		}
		data[tmp] = _data / x;
		tmp++;
		for(pos = tmp; pos < num_digits; pos++){
			data[pos] = convert_num(((_data % x)/(x / 10)));
			x /= 10;
		}
	}
	else{
		data[pos] = _data / x;
		for(pos = 1; pos < num_digits; pos++){
			data[pos] = convert_num(((_data % x)/(x / tmp)));
			x /= tmp;
		}

		if(pos < howMany){
			tmp = num_digits - 1;
			while(pos < howMany){
				update_display();
				delay(500);
				for(uint8_t b=0;b<tmp;b++){
					data[b] = data[b+1];
				}
				data[tmp] = convert_num(((_data % x)/(x / 10)));
				x /= 10;
				pos++;
			}
		}
	}
	update_display();
}

void shift7seg::load_data(const int& _data){
	uint8_t tmp = 10;
	long x = tmp;
	uint8_t howMany = 1;
	uint8_t pos = 0;

	while(x < _data){
		x *= tmp;
		howMany++;
	}
	x /= tmp;

	if(howMany < num_digits){
		tmp = 0;
		while((howMany + tmp) < num_digits){
			data[tmp] = blank;
			tmp++;
		}
		data[tmp] = _data / x;
		tmp++;
		for(pos = tmp; pos < num_digits; pos++){
			data[pos] = convert_num(((_data % x)/(x / 10)));
			x /= 10;
		}
	}
	else{
		data[pos] = _data / x;
		for(pos = 1; pos < num_digits; pos++){
			data[pos] = convert_num(((_data % x)/(x / tmp)));
			x /= tmp;
		}

		if(pos < howMany){
			tmp = num_digits - 1;
			while(pos < howMany){
				update_display();
				delay(500);
				for(uint8_t b=0;b<tmp;b++){
					data[b] = data[b+1];
				}
				data[tmp] = convert_num(((_data % x)/(x / 10)));
				x /= 10;
				pos++;
			}
		}
	}
	update_display();
}

void shift7seg::load_data(const char *_data, const uint8_t& howMany){
	uint8_t pos;
  uint8_t offset = 0;

	if(howMany<num_digits){
		while((howMany+offset)<num_digits){
			data[offset] = blank;
			offset++;
		}
		for(pos=offset;pos<num_digits;pos++){
			data[pos] = convert_char(_data[pos-offset]);
		}
	}
	else{
		for(pos=0;pos<num_digits;pos++){
			data[pos] = convert_char(_data[pos]);
		}
		if(pos < (howMany-1)){
			offset = num_digits - 1;
			while(pos<howMany){
				update_display();
				delay(500);
				for(uint8_t b=0;b<offset;b++){
					data[b] = data[b+1];
				}
				data[offset] = convert_char(_data[pos]);
				pos++;
			}
		}
	}
	update_display();
}

void shift7seg::load_data(const String& _data, const uint8_t& repeat, size_t howMany, const uint8_t& start){
	uint8_t pos = 0;
	uint8_t offset = 0;
	uint8_t count = 0;
	uint8_t tmp;

	if (howMany <= 1){
		howMany = _data.length();
	}

	if(howMany<num_digits){
		while((howMany+offset)<num_digits){
			data[offset] = blank;
			offset++;
		}
		for(pos=offset;pos<num_digits;pos++){
			data[pos] = convert_char(_data.charAt(start + (pos-offset)));
		}
		count++;
		if(count < repeat){
			offset = num_digits - 1;
			pos = 0;
			while(count < repeat){
				update_display();
				delay(500);
				tmp = data[0];
				for(uint8_t b=0;b<offset;b++){
					data[b] = data[b+1];
				}
				data[offset] = tmp;
				pos++;
				if(pos == num_digits){
					count++;
					pos = 0;
				}
			}
		}
	}

	else{
		for(pos=0;pos<num_digits;pos++){
			data[pos] = convert_char(_data.charAt(start + pos));
		}
		if(pos < (howMany-1)){
			offset = num_digits - 1;
			while(pos<howMany){
				update_display();
				delay(500);
				for(uint8_t b=0;b<offset;b++){
					data[b] = data[b+1];
				}
				data[offset] = convert_char(_data.charAt(start + pos));
				pos++;
			}
		}

		count++;
		if(count < repeat){
			offset = num_digits - 1;
			pos = 0;
			while(count < repeat){
				update_display();
				delay(500);
				for(uint8_t b=0;b<offset;b++){
					data[b] = data[b+1];
				}
				data[offset] = convert_char(_data.charAt(start + pos));
				pos++;
				if(pos == howMany){
					count++;
					pos = 0;
				}
			}
		}
	}
	update_display();
}

/********************************************************************************************/
//err_display()
//Public function
//Displays the word Error (scrolling from right to left)
/********************************************************************************************/

void shift7seg::err_display(){
	load_data("Error ", 4, 6, 0);
}

void shift7seg::blank_display(){
	for(uint8_t pos=0;pos<num_digits;pos++){
		data[pos] = blank;
	}
	update_display();
}

void shift7seg::update_display(){
	digitalWrite(latchPin, LOW);
	for(uint8_t pos=0;pos<num_digits;pos++){
		shiftOut(dataPin, clkPin, LSBFIRST, data[pos]);
	}
	digitalWrite(latchPin, HIGH);
}

uint8_t shift7seg::convert_char(const char& OGchar){
	uint8_t converted;
	switch (OGchar){
		case 'A':
			converted = capital[0];
			break;
		case 'B':
			converted = capital[1];
			break;
		case 'C':
			converted = capital[2];
			break;
		case 'D':
			converted = capital[3];
			break;
		case 'E':
			converted = capital[4];
			break;
		case 'F':
			converted = capital[5];
			break;
		case 'G':
			converted = capital[6];
			break;
		case 'H':
			converted = capital[7];
			break;
		case 'J':
			converted = capital[8];
			break;
		case 'L':
			converted = capital[9];
			break;
		case 'O':
			converted = capital[3];
			break;
		case 'P':
			converted = capital[10];
			break;
		case 'R':
			converted = capital[0];
			break;
		case 'S':
			converted = capital[11];
			break;
		case 'U':
			converted = capital[12];
			break;
		case 'V':
			converted = capital[12];
			break;
		case 'b':
			converted = lower[0];
			break;
		case 'c':
			converted = lower[1];
			break;
		case 'd':
			converted = lower[2];
			break;
		case 'h':
			converted = lower[3];
			break;
		case 'l':
			converted = lower[4];
			break;
		case 'n':
			converted = lower[5];
			break;
		case 'o':
			converted = lower[6];
			break;
		case 'r':
			converted = lower[7];
			break;
		case 'u':
			converted = lower[8];
			break;
		case 'v':
			converted = lower[8];
			break;
		default:
			converted = blank;
			break;
	}
	return converted;
}

//converts decimals into 7 segment numbers[0..9]
uint8_t shift7seg::convert_num(const uint8_t& OGnum){
	uint8_t converted;

	switch(OGnum){
		case 0:
			converted = numbers[0];
			break;
		case 1:
			converted = numbers[1];
			break;
		case 2:
			converted = numbers[2];
			break;
		case 3:
			converted = numbers[3];
			break;
		case 4:
			converted = numbers[4];
			break;
		case 5:
			converted = numbers[5];
			break;
		case 6:
			converted = numbers[6];
			break;
		case 7:
			converted = numbers[7];
			break;
		case 8:
			converted = numbers[8];
			break;
		case 9:
			converted = numbers[9];
			break;
		default:
			converted = blank;
			break;
	}
	return converted;
}
