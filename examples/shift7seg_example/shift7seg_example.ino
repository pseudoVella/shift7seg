#include <shift7seg.h>

shift7seg shift(8, 9, 10);

void setup()
{
//nothing to do here
}

void loop()
{
  uint8_t i_leng = 10;
  for (int q = 0; q < 4; q++)
  {
   uint8_t i[i_leng] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  shift.load_data(i, i_leng);   //load an array of bytes, specifying how many bytes you'd like to display. if more than 4 bytes are loaded, the display scrolls through them.
  
  shift.load_data(shift.blank, shift.blank, shift.blank, shift.blank);      // load 4 predefined bytes, see shift7seg.h for a full list
  
  delay(500);
  }
  
  delay(2000);

  String t = "PEOPLE UndEr PrESSUrE ";
  
  shift.load_data(t, 4, 7);   //load a String, if the String is larger than 4 chars the display will scroll 
                              //(String, repeat multiplier(default 1), string length (default is length of string), string start position(default 0))
                              //if you only fill the first parameter it will display your full string once, scrolling from right to left.
                              //try removing the third parameter to see the full string

  shift.blank_display();      //function to clear the display
  delay(200);

  char y[11] = {'c', 'o', 'o', 'l', ' ', 'b', 'E', 'A', 'n', 'S'}; 
  shift.load_data(y, 11);     //load and array of char's just like we did earlier using an array of bytes(a.k.a. uint8_t).
  shift.blank_display();      //function to clear the display
  delay(200);
  
  int x = 10000;        //displays an error when int are larger than 9999. this function wont scroll , use an array of bytes for that.
  shift.load_data(x);   // takes a single parameter (int)

  
  for (int x = 0; x < 10000; x++)   
  {
    shift.load_data(x);      //demonstrating how to load numbers 1 through 9999 using a single int variable.
    delay(50);
  }

}
