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

shift7seg::shift7seg(uint8_t _dataPin, uint8_t _latchPin, uint8_t _clkPin)
{
	dataPin = _dataPin;
	latchPin = _latchPin;
	clkPin = _clkPin;
	
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

void shift7seg::load_data(uint8_t _data1, uint8_t _data2, uint8_t _data3, uint8_t _data4)
{
	data1 = _data1;				//this version of load_data should only be used with 
	data2 = _data2;				//predefined values from the 7 segment truth table
	data3 = _data3;				//in this case the values passed to load_data are NOT   
	data4 = _data4;				//converted but passed directly to data1..data4
	update_display();
}


void shift7seg::load_data(uint8_t *_data, uint8_t howMany)
{
	for (uint8_t x = 0; x < howMany; x++)
	{
		if (howMany == 1)
		{
			data1 = blank;
			data2 = blank;
			data3 = blank;
			data4 = convert_num(*_data);
		}
		else if (howMany == 2)
		{
			data1 = blank;
			data2 = blank;
			switch (x)
			{
				case 0:
					data3 = convert_num(*_data);
					_data++;
					break;
				case 1:
					data4 = convert_num(*_data);
					break;
			}
		}
		else if (howMany == 3)
		{
			data1 = blank;
			switch (x)
			{
				case 0:
					data2 = convert_num(*_data);
					_data++;
					break;
				case 1:
					data3 = convert_num(*_data);
					_data++;
					break;
				case 2:
					data4 = convert_num(*_data);
					break;
			}
		}
		else
		{
			switch (x)
			{
				case 0:
					data1 = convert_num(*_data);
					_data++;
					break;
				case 1:
					data2 = convert_num(*_data);
					_data++;
					break;
				case 2:
					data3 = convert_num(*_data);
					_data++;
					break;
				case 3:
					data4 = convert_num(*_data);
					if (x < howMany - 1)
					{
						_data++;
						update_display();
						delay(500);
					}
					break;
				default:
					data1 = data2;
					data2 = data3;
					data3 = data4;
					data4 = convert_num(*_data);
					if (x < howMany - 1)
					{
						_data++;
					}
					update_display();
					delay(500);
					break;
			}
		}
	}
	update_display();
}


void shift7seg::load_data(int _data)
{
	uint8_t x = 10;
	
	if (_data < 10)
	{
		data1 = blank;
		data2 = blank;
		data3 = blank;
		data4 = convert_num((uint8_t)_data);
		update_display();
	}
	else if ((_data >= 10) && (_data < 100))
	{
		data1 = blank;
		data2 = blank;
		data3 = convert_num((uint8_t)((_data % (x * x)) / ((x * x) / x)));
		data4 = convert_num((uint8_t)(_data % x));
		update_display();
	}
	else if ((_data >= 100) && (_data < 1000))
	{
		data1 = blank;
		data2 = convert_num((uint8_t)((_data % (x * x * x)) / ((x * x * x) / x)));
		data3 = convert_num((uint8_t)((_data % (x * x)) / ((x * x) / x)));
		data4 = convert_num((uint8_t)(_data % x));
		update_display();
	}
	else if ((_data >= 1000) && (_data < 10000))
	{
		data1 = convert_num((uint8_t)((_data % (x * x * x * x)) / ((x * x * x * x) / x)));
		data2 = convert_num((uint8_t)((_data % (x * x * x)) / ((x * x * x) / x)));
		data3 = convert_num((uint8_t)((_data % (x * x)) / ((x * x) / x)));
		data4 = convert_num((uint8_t)(_data % x));
		update_display();
	}
	else if (_data >= 10000)
	{
		err_display();
	}
}


void shift7seg::load_data(char *_data, uint8_t howMany)
{
	for (uint8_t x = 0; x < howMany; x++)
		{
			if (howMany == 1)
			{
				data1 = blank;
				data2 = blank;
				data3 = blank;
				data4 = convert_char(*_data);
			}
			else if (howMany == 2)
			{
				data1 = blank;
				data2 = blank;
				switch (x)
				{
					case 0:
						data3 = convert_char(*_data);
						_data++;
						break;
					case 1:
						data4 = convert_char(*_data);
						break;
				}
			}
			else if (howMany == 3)
			{
				data1 = blank;
				switch (x)
				{
					case 0:
						data2 = convert_char(*_data);
						_data++;
						break;
					case 1:
						data3 = convert_char(*_data);
						_data++;
						break;
					case 2:
						data4 = convert_char(*_data);
						break;
				}
			}
			else
			{
				switch (x)
				{
					case 0:
						data1 = convert_char(*_data);
						_data++;
						break;
					case 1:
						data2 = convert_char(*_data);
						_data++;
						break;
					case 2:
						data3 = convert_char(*_data);
						_data++;
						break;
					case 3:
						data4 = convert_char(*_data);
						if (x < howMany - 1)
						{
							_data++;
							update_display();
							delay(500);
						}
						break;
					default:
						data1 = data2;
						data2 = data3;
						data3 = data4;
						data4 = convert_char(*_data);
						if (x < howMany - 1)
						{
							_data++;
						}
						update_display();
						delay(500);
						break;
				}
			}
		}
		update_display();
}


void shift7seg::load_data(String _data, uint8_t repeat, int howMany, uint8_t start)
{
	if (howMany <= 0)
	{
		howMany = 1;
	}
	
	uint8_t length;
	if (howMany == 1)
	{
		length = howMany * _data.length() * repeat;
	}
	else
	{
		length = howMany * repeat;
	}
	uint8_t num[length];
	
	uint8_t o = start;
	for (uint8_t i = 0; i < length; i++)
	{
		if (o >= howMany)
		{
			o = start;
		}
		num[i] = o; 
		o++;
	}
	o = 0;
	
	if (length == 1)
	{
		data1 = blank;
		data2 = blank;
		data3 = blank;
		data4 = convert_char(_data.charAt(num[0]));
		update_display();
	}
	else if (length == 2)
	{
		data1 = blank;
		data2 = blank;
		data3 = convert_char(_data.charAt(num[0]));
		data4 = convert_char(_data.charAt(num[1]));
		update_display();
	}
	else if (length == 3)
	{
		data1 = blank;
		data2 = convert_char(_data.charAt(num[0]));
		data3 = convert_char(_data.charAt(num[1]));
		data4 = convert_char(_data.charAt(num[2]));
		update_display();
	}
	else 
	{
		for (uint8_t i = 0; i < length; i++)
		{
			switch (o)
			{
				case 0:
					data1 = convert_char(_data.charAt(num[i]));
					break;
				case 1:
					data2 = convert_char(_data.charAt(num[i]));
					break;
				case 2:
					data3 = convert_char(_data.charAt(num[i]));
					break;
				case 3:
					data4 = convert_char(_data.charAt(num[i]));
					break;
				case 4:
					data1 = data2;
					data2 = data3;
					data3 = data4;
					data4 = convert_char(_data.charAt(num[i]));
					break;
			}
			o++;
			if (o >= 4 || i == (length - 1))
			{				
				update_display();
				delay(500);
				o = 4;
			}
		}
	}
}


/********************************************************************************************/
//err_display() 
//Public function
//Displays the word Error (scrolling from right to left)
/********************************************************************************************/

void shift7seg::err_display()
{
	load_data("Error ", 4, 6, 0);
}



void shift7seg::blank_display()
{
	data1 = blank;
	data2 = blank;
	data3 = blank;
	data4 = blank;
	update_display();
}	


void shift7seg::update_display()
{
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clkPin, LSBFIRST, data1);
	shiftOut(dataPin, clkPin, LSBFIRST, data2);
	shiftOut(dataPin, clkPin, LSBFIRST, data3);
	shiftOut(dataPin, clkPin, LSBFIRST, data4);
	digitalWrite(latchPin, HIGH);
}

uint8_t shift7seg::convert_char(char OGchar)
{
	uint8_t converted;
	switch (OGchar)
	{
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
		case 'H':
			converted = capital[6];
			break;
		case 'L':
			converted = capital[7];
			break;
		case 'O':
			converted = capital[3];
			break;
		case 'P':
			converted = capital[8];
			break;
		case 'S':
			converted = capital[9];
			break;
		case 'U':
			converted = capital[10];
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
		default:
			converted = blank;
			break;
	}
	return converted;
}


uint8_t shift7seg::convert_num(uint8_t OGnum)                //converts decimals into 7 segment numbers[0..9]
{
	uint8_t converted;
	switch (OGnum)
	{
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
	}
	return converted;
}
