/*

    This file is part of Arduino Screenkey Library.   

    Copyright (c) 2011, 2012  Shaw Systems Ltd, UK.    

    Arduino Screenkey Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

    This file contains high level functions based on ;

    Arduino library support for graphic LCDs written by Michael Margolis and Bill Perry. 
    You can obtain a copy of the original work here:
    http://code.google.com/p/glcd-arduino/
 
    Arduino tv-out library written by Myles Metzer.
    You can obtain a copy of the original work here:
    http://code.google.com/p/arduino-tvout/
 


*/
#include "Arduino.h"
#include "Screenkey.h"

Screenkey::Screenkey(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

/* Prepares a word to be sent to the lcd */
/* Parity is 1 for odd parity, 0 for even */
void Screenkey::screenkey_write(uint8_t data, uint8_t parity)
{
  /* Block while theres still a word being sent */
  while (cnt);

  /* Calculate parity */
  uint8_t pb = data ^ (data >> 1) ^ (data >> 2) ^ (data >> 3) ^
    (data >> 4) ^ (data >> 5) ^ (data >> 6) ^ (data >> 7)
    ^ parity;

  /* Setup the bits to send */
  bits = 0x0C00 | (data << 1) | ((pb & 0x1) << 9);

  /* queue it up */
  cnt = 12;
}

void Screenkey::screenkey_start()
{
  screenkey_write(0x00, 0);
}

void Screenkey::screenkey_stop()
{
  screenkey_write(0xAA, 0);
}

// Write a 1 byte register
void Screenkey::screenkey_reg_1(uint8_t reg, uint8_t val)
{
  screenkey_start();
  screenkey_write(reg,1);
  screenkey_write(val,1);
  screenkey_stop();

}

// Write a 2 byte register
void Screenkey::screenkey_reg_2(uint8_t reg, uint8_t val1, uint8_t val2)
{
  screenkey_start();
  screenkey_write(reg,1);
  screenkey_write(val1,1);
  screenkey_write(val2,1);
  screenkey_stop();
}

// Write a 108 byte image to the screen
// Data is a pointer to 108 bytes of image data to display
void Screenkey::screenkey_write_img(uint8_t * data)
{

  uint8_t i;

  screenkey_start();
  screenkey_write(0x80,1);
  for (i=0; i<108; i++)
    screenkey_write(data[i], 1);

  screenkey_stop();

}


void Screenkey::fill()
{

  uint8_t i;
  for (i=0; i<108; i++)
    dwg_buff[i] = 0xFF;

}


void Screenkey::clear()
{

  uint8_t i;
  for (i=0; i<108; i++)
    dwg_buff[i] = 0x00;

}

void Screenkey::set_color(uint8_t color)
{
  screenkey_reg_1(0xED, color);
}







void Screenkey::init()
{
 
screenkey_reg_1(0xEE, 0x00);
  screenkey_reg_2(0xEF, 0x07, 0x00);
  set_color(BL_NONE);
}

void Screenkey::load_img(uint8_t * data)
{
// dwg_buff = data;
  uint8_t i;
  for (i=0; i<108; i++)
   dwg_buff[i] = data[i];



}
void Screenkey::refresh()
{
 screenkey_write_img(dwg_buff);
}

void Screenkey::set_pixel(uint8_t x, uint8_t y) 
{
int _bitnum, _buff_byte, _buff_bit;
//extern unsigned char dwg_buff[108];
    if (x > XRES || y > YRES)
      {
//		return;
      }
   else
      {
      // for (_bitnum=0; _bitnum<864; _bitnum++)
      // {
          _bitnum = 864-( (x+1) + ((y)*36));
          _buff_byte = _bitnum/8;
          _buff_bit = _bitnum%8;
   	  dwg_buff[_buff_byte] |= (1<<_buff_bit);	
     //  }
      }

}

void Screenkey::draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) 
{
	uint8_t length, i, y, yAlt, xTmp, yTmp;
	int16_t m;
	//
	// vertical line
	//
	if(x1 == x2) {
		// x1|y1 must be the upper point
		if(y1 > y2) {
			yTmp = y1;
			y1 = y2;
			y2 = yTmp;
		}
		//this->DrawVertLine(x1, y1, y2-y1, color);
                for (i=y1; i<=y2; i++)
                     set_pixel(x1, i);


	
	//
	// horizontal line
	//
	} else if(y1 == y2) {	
		// x1|y1 must be the left point
		if(x1 > x2) {
			xTmp = x1;
			x1 = x2;
			x2 = xTmp;
		}
		//this->DrawHoriLine(x1, y1, x2-x1, color);
	        for (i=x1; i<=x2; i++)
                     set_pixel(i, y1);
	//
	// angled line :)
	//
	} else {
		int e;
		signed int dx,dy,j, temp;
		signed char s1,s2, xchange;
		signed int x,y;

		x = x1;
		y = y1;
	
		//take absolute value
		if (x2 < x1) {
			dx = x1 - x2;
			s1 = -1;
		}
		else if (x2 == x1) {
			dx = 0;
			s1 = 0;
		}
		else {
			dx = x2 - x1;
			s1 = 1;
		}

		if (y2 < y1) {
			dy = y1 - y2;
			s2 = -1;
		}
		else if (y2 == y1) {
			dy = 0;
			s2 = 0;
		}
		else {
			dy = y2 - y1;
			s2 = 1;
		}

		xchange = 0;   

		if (dy>dx) {
			temp = dx;
			dx = dy;
			dy = temp;
			xchange = 1;
		} 

		e = ((int)dy<<1) - dx;  
	 
		for (j=0; j<=dx; j++) {
			set_pixel(x,y);
		 
			if (e>=0) {
				if (xchange==1) x = x + s1;
				else y = y + s2;
				e = e - ((int)dx<<1);
			}
			if (xchange==1)
				y = y + s2;
			else
				x = x + s1;
			e = e + ((int)dy<<1);
		
	}
	}
}

/* place a bitmap at x,y where the bitmap is defined as {width,height,imagedata....}
 *
 * Arguments:
 *	x:
 *		The x coordinate of the upper left corner.
 *	y:
 *		The y coordinate of the upper left corner.
 *	bmp:
 *		The bitmap data to print.
 *	i:
 *		The offset into the image data to start at.  This is mainly used for fonts.
 *		default	=0
 *	width:
 *		Override the bitmap width. This is mainly used for fonts.
 *		default =0 (do not override)
 *	height:
 *		Override the bitmap height. This is mainly used for fonts.
 *		default	=0 (do not override)
*/
void Screenkey::bitmap(uint8_t x, uint8_t y, const unsigned char * bmp,
				   uint16_t i, uint8_t width, uint8_t height) {

	uint8_t temp; //, lshift, rshift, save, xtra;
//	uint16_t si = 0;
	
//	rshift = x&7;
//	lshift = 8-rshift;
	if (width == 0) {
		width = pgm_read_byte((uint32_t)(bmp) + i);
		i++;
	}
	if (height == 0) {
		height = pgm_read_byte((uint32_t)(bmp) + i);
		i++;
	}
        // 1px at each corner for debug
        //set_pixel(x, y);
        //set_pixel(x+width-1, y);
        //set_pixel(x, y-height);
        //set_pixel(x+width-1,y-height );

		
//	if (width&7) {
//		xtra = width&7;
//		width = width/8;
//		width++;
//	}
//	else {
//		xtra = 8;
//		width = width/8;
//	}
	
	for (uint8_t h = 0; h < height; h++) {
         	temp = pgm_read_byte((uint32_t)(bmp) + i++);

		for ( uint8_t b = 0; b < width; b++) {
                        if (temp & (0x80>>b)) set_pixel(x+b,y-h);
		}
	}
} // end of bitmap


void Screenkey::select_font(const unsigned char * f) {
	font = f;
}

/*
 * print an 8x8 char c at x,y
 * x must be a multiple of 8
 */
void Screenkey::print_char(uint8_t x, uint8_t y, unsigned char c) {

	c -= pgm_read_byte(font+2);
	bitmap(x,y,font,(c*pgm_read_byte(font+1))+3,pgm_read_byte(font),pgm_read_byte(font+1));
}


void Screenkey::inc_txtline() {
	cursor_y -= pgm_read_byte(font+1);
}

void Screenkey::set_cursor(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;
}

/* default implementation: may be overridden */
void Screenkey::write(const char *str)
{
  while (*str)
    write(*str++);
}

/* default implementation: may be overridden */
void Screenkey::write(const uint8_t *buffer, uint8_t size)
{
  while (size--)
    write(*buffer++);
}

void Screenkey::write(uint8_t c) {
	switch(c) {
		case '\0':			//null
			break;
		case '\n':			//line feed
			cursor_x = 0;
			inc_txtline();
			break;
		case 8:				//backspace
			cursor_x -= pgm_read_byte(font);
			print_char(cursor_x,cursor_y,' ');
			break;
		case 13:			//carriage return !?!?!?!VT!?!??!?!
			cursor_x = 0;
			break;
		case 14:			//form feed new page(clear screen)
			//clear_screen();
			break;
		default:
			if (cursor_x >= (YRES*8 - pgm_read_byte(font))) {
				cursor_x = 0;
				inc_txtline();
				print_char(cursor_x,cursor_y,c);
			}
			else
				print_char(cursor_x,cursor_y,c);
			cursor_x += pgm_read_byte(font);
	}
}


void Screenkey::print(const char str[])
{
  write(str);
}

void Screenkey::print(char c, int base)
{
  print((long) c, base);
}

void Screenkey::print(unsigned char b, int base)
{
  print((unsigned long) b, base);
}

void Screenkey::print(int n, int base)
{
  print((long) n, base);
}

void Screenkey::print(unsigned int n, int base)
{
  print((unsigned long) n, base);
}

void Screenkey::print(long n, int base)
{
  if (base == 0) {
    write(n);
  } else if (base == 10) {
    if (n < 0) {
      print('-');
      n = -n;
    }
    printNumber(n, 10);
  } else {
    printNumber(n, base);
  }
}

void Screenkey::print(unsigned long n, int base)
{
  if (base == 0) write(n);
  else printNumber(n, base);
}

void Screenkey::print(double n, int digits)
{
  printFloat(n, digits);
}

void Screenkey::println(void)
{
  print('\r');
  print('\n');  
}

void Screenkey::println(const char c[])
{
  print(c);
  println();
}

void Screenkey::println(char c, int base)
{
  print(c, base);
  println();
}

void Screenkey::println(unsigned char b, int base)
{
  print(b, base);
  println();
}

void Screenkey::println(int n, int base)
{
  print(n, base);
  println();
}

void Screenkey::println(unsigned int n, int base)
{
  print(n, base);
  println();
}

void Screenkey::println(long n, int base)
{
  print(n, base);
  println();
}

void Screenkey::println(unsigned long n, int base)
{
  print(n, base);
  println();
}

void Screenkey::println(double n, int digits)
{
  print(n, digits);
  println();
}

void Screenkey::printPGM(const char str[]) {
	char c;
	while ((c = pgm_read_byte(str))) {
		str++;
		write(c);
	}
}

void Screenkey::printPGM(uint8_t x, uint8_t y, const char str[]) {
	char c;
	cursor_x = x;
	cursor_y = y;
	while ((c = pgm_read_byte(str))) {
		str++;
		write(c);
	}
}



void Screenkey::print(uint8_t x, uint8_t y, const char str[]) {
	cursor_x = x;
	cursor_y = y;
	write(str);
	
}
void Screenkey::print(uint8_t x, uint8_t y, char c, int base) {
	cursor_x = x;
	cursor_y = y;
	print((long) c, base);
}
void Screenkey::print(uint8_t x, uint8_t y, unsigned char b, int base) {
	cursor_x = x;
	cursor_y = y;
	print((unsigned long) b, base);
}
void Screenkey::print(uint8_t x, uint8_t y, int n, int base) {
	cursor_x = x;
	cursor_y = y;
	print((long) n, base);
}
void Screenkey::print(uint8_t x, uint8_t y, unsigned int n, int base) {
	cursor_x = x;
	cursor_y = y;
	print((unsigned long) n, base);
}
void Screenkey::print(uint8_t x, uint8_t y, long n, int base) {
	cursor_x = x;
	cursor_y = y;
	print(n,base);
}
void Screenkey::print(uint8_t x, uint8_t y, unsigned long n, int base) {
	cursor_x = x;
	cursor_y = y;
	print(n,base);
}
void Screenkey::print(uint8_t x, uint8_t y, double n, int digits) {
	cursor_x = x;
	cursor_y = y;
	print(n,digits);
}

void Screenkey::println(uint8_t x, uint8_t y, const char c[])
{
	cursor_x = x;
	cursor_y = y;
	print(c);
	println();
}

void Screenkey::println(uint8_t x, uint8_t y, char c, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(c, base);
	println();
}

void Screenkey::println(uint8_t x, uint8_t y, unsigned char b, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(b, base);
	println();
}

void Screenkey::println(uint8_t x, uint8_t y, int n, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(n, base);
	println();
}

void Screenkey::println(uint8_t x, uint8_t y, unsigned int n, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(n, base);
	println();
}

void Screenkey::println(uint8_t x, uint8_t y, long n, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(n, base);
	println();
}

void Screenkey::println(uint8_t x, uint8_t y, unsigned long n, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(n, base);
	println();
}

void Screenkey::println(uint8_t x, uint8_t y, double n, int digits)
{
	cursor_x = x;
	cursor_y = y;
	print(n, digits);
	println();
}

void Screenkey::printNumber(unsigned long n, uint8_t base)
{
  unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
  unsigned long i = 0;

  if (n == 0) {
    print('0');
    return;
  } 

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    print((char) (buf[i - 1] < 10 ?
      '0' + buf[i - 1] :
      'A' + buf[i - 1] - 10));
}

void Screenkey::printFloat(double number, uint8_t digits) 
{ 
  // Handle negative numbers
  if (number < 0.0)
  {
     print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    print(toPrint);
    remainder -= toPrint; 
  } 
}

