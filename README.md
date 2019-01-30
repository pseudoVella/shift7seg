# shift7seg
**arduino library for simplifying the controll of a 7 segment display driven by 74hc595 shift registers and an arduino.
tested on an uno but should work on most avr boards.**

created by Alex B. Vella sept 23, 2017

- **constructor**
	*declare pins for controlling 74hc595 shift registers
	 declare number of digits for 7seg display*
  -shift7seg(uint8_t _data, uint8_t _latch, uint8_t _clock, uint8_t _num_digits) 

- **main functions**
  - load_data() *loads data into registers displaying it on our seven segment display. overloaded to handle 
                 most variable types, this function uses a 500 millisecond delay() only when the data passed
                 is larger than num_ digits/characters. the delay is there so that the data will scroll from right
                 to left at a legible speed. check either the .h, .cpp or my example sketch for the different 
                 versions of load_data().*
                
  - blank_display() *clears data from buffers and turns off all segments of the display*
                
  - err_display() *displays the word Error 4 times , can be used to simplify debugging*
               
- **constants** 
  - blank *binary value for blanking individual digits*
  - capital[13] *holds binary values for uppercase letters, will be updating this array to account for some missing letters*
  - lower[9] *binary values for lowercase letters*
  - numbers[10] *binary values for numbers 0-9*
