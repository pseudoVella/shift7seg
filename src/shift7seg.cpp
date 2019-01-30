/*****************************************************************************************
shift7seg library created by Alex B. Vella september 23 2017.
Uses 4 74hc595 serial in/parallel out shift registers to drive
a 4 digit 7 segment display.
******************************************************************************************/

#include <shift7seg.h>

/*****************************************************************************************
initial constructor for shift7seg library
sets appropriate pins as outputs or inputs and prepares display for loading data
******************************************************************************************/

shift7seg::shift7seg(const uint8_t _dataPin,
										const uint8_t _latchPin,
										const uint8_t _clkPin,
										const uint8_t _num_digits){
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

void shift7seg::load_rdata(const uint8_t *_data, const uint8_t howMany){
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
			while(pos<howmany){
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

void shift7seg::load_data(const uint8_t *_data, const uint8_t howMany){
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
			while(pos<howmany){
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

void shift7seg::load_data(const uint8_t _data){
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

void shift7seg::load_data(const int _data){
	uint8_t tmp = 10;
	uint32_t x = tmp;
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

void shift7seg::load_data(const char *_data, const uint8_t howMany){
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
			while(pos<howmany){
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

void shift7seg::load_data(const String _data, const uint8_t repeat, const int howMany, const uint8_t start){
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
			while(pos<howmany){
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

	if(OGnum<10){
		converted = numbers[OGnum];
	}
	else{
		converted = blank;
	}
	return converted;
}
