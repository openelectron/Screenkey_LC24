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

#ifndef Screenkey_h
#define Screenkey_h

#include "Arduino.h"
#include <fontALL.h>

// Bright and dark constants for each color
#define BL_NONE    0x00

#define BR_GREEN   0x33
#define BR_RED	   0xCC
#define BR_ORANGE  0xFF

#define DK_GREEN   0x03
#define DK_RED     0x0C
#define DK_ORANGE  0x0F
#define BYTE    0

//Define LCD Size
#define XRES	   36
#define YRES       24
// Global Variables for writing to screenkeys
 extern volatile uint8_t  phase;
 extern volatile uint16_t bits;
 extern volatile uint8_t  cnt;



class Screenkey
{
  public:
    Screenkey(int pin);
    void load_img(uint8_t * data);
    void set_color(uint8_t color);
    void init();
    void refresh();
    void fill();
    void clear();
    void set_pixel(uint8_t x, uint8_t y);
    void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    void bitmap(uint8_t x, uint8_t y, const unsigned char * bmp, uint16_t i = 0, uint8_t width = 0, uint8_t lines = 0);
    void print_char(uint8_t x, uint8_t y, unsigned char c);
    void select_font(const unsigned char * f);
    void set_cursor(uint8_t x, uint8_t y);
    void write(uint8_t);
    void write(const char *str);
    void write(const uint8_t *buffer, uint8_t size);
 void print(const char[]);
    void print(char, int = BYTE);
    void print(unsigned char, int = BYTE);
    void print(int, int = DEC);
    void print(unsigned int, int = DEC);
    void print(long, int = DEC);
    void print(unsigned long, int = DEC);
    void print(double, int = 2);
	
	void print(uint8_t, uint8_t, const char[]);
	void print(uint8_t, uint8_t, char, int = BYTE);
	void print(uint8_t, uint8_t, unsigned char, int = BYTE);
	void print(uint8_t, uint8_t, int, int = DEC);
	void print(uint8_t, uint8_t, unsigned int, int = DEC);
	void print(uint8_t, uint8_t, long, int = DEC);
	void print(uint8_t, uint8_t, unsigned long, int = DEC);
	void print(uint8_t, uint8_t, double, int = 2);
	
	void println(uint8_t, uint8_t, const char[]);
    void println(uint8_t, uint8_t, char, int = BYTE);
    void println(uint8_t, uint8_t, unsigned char, int = BYTE);
    void println(uint8_t, uint8_t, int, int = DEC);
    void println(uint8_t, uint8_t, unsigned int, int = DEC);
    void println(uint8_t, uint8_t, long, int = DEC);
    void println(uint8_t, uint8_t, unsigned long, int = DEC);
    void println(uint8_t, uint8_t, double, int = 2);
    void println(uint8_t, uint8_t);

    void println(const char[]);
    void println(char, int = BYTE);
    void println(unsigned char, int = BYTE);
    void println(int, int = DEC);
    void println(unsigned int, int = DEC);
    void println(long, int = DEC);
    void println(unsigned long, int = DEC);
    void println(double, int = 2);
    void println(void);
	
	void printPGM(const char[]);
	void printPGM(uint8_t, uint8_t, const char[]);
  private:
    int _pin;
    int _bitnum;
    unsigned char dwg_buff[108];   // Ram used as a drawing buffer
    uint8_t cursor_x,cursor_y;
    const unsigned char * font;

    void inc_txtline();
    void screenkey_write(uint8_t data, uint8_t parity);
    void screenkey_start();
    void screenkey_stop();
    void screenkey_reg_1(uint8_t reg, uint8_t val);
    void screenkey_reg_2(uint8_t reg, uint8_t val1, uint8_t val2);
    void screenkey_write_img(uint8_t * data);
    void printNumber(unsigned long, uint8_t);
    void printFloat(double, uint8_t);



};




#endif
