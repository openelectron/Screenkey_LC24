/*

    This file is part of Arduino Screenkey Library.   

    Copyright (c) 2011, 2012  Shaw Systems Ltd, UK.    

    Arduino Screenkey Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Arduino Screenkey Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

   

*/
#include <TimerOne.h>
#include <Screenkey.h>

// Include bitmaps contained in an array
#include "exit.h"

// define pins connected to SkreenKey
// Multipule screenkeys can share a Clock pin but require ind
#define CLK_PIN    8
#define DATA_PIN   5



// Global Variables for writing to screenkeys
volatile uint8_t  phase;
volatile uint16_t bits;
volatile uint8_t  cnt;


Screenkey key8(DATA_PIN);


/* Setup phase: configure and enable timer2 overflow interrupt */
void setup() {  
  
  // setup clock pin as output
  pinMode(CLK_PIN, OUTPUT);

  // These two lines setup Timer1 to generate the clock
  Timer1.initialize(1);
  Timer1.attachInterrupt(my_ISR);  

  key8.init();
  
}  



void my_ISR() {  
  // Generate Clock

  //digitalWrite(CLK_PIN, phase == 0 ? HIGH : LOW);  
  PORTB = (PORTB & 0XFE) | (phase?1:0 );
  phase = ~phase;

  if (phase)
  {
    if (!cnt)
    {

      digitalWrite(DATA_PIN, 1);
      return;
    }


    // and write the number that will give high on the required pin.
    digitalWrite(DATA_PIN, (bits & 0x1) ? HIGH : LOW);
    cnt --;
    bits = bits >> 1;
  } 
}  





/* Main loop. */
void loop() {  

  key8.set_color(BR_GREEN);
  key8.load_img(exit_img);
  key8.refresh();
  delay(1000); 
  
  key8.clear();
  key8.refresh();
  delay(1000);
  
  key8.fill();
  key8.refresh();
  delay(1000);
  
  key8.clear();
  key8.set_pixel(0,23);
  key8.set_pixel(35,23);
  key8.set_pixel(35,0);
  key8.set_pixel(0,0);
  key8.refresh();
  delay(1000);
  
 // key8.draw_line(10,10,20,10);
 key8.clear();
  key8.draw_line(0,0,0,23);
  key8.draw_line(35,0,35,23);
  key8.draw_line(35,23,0,23);
  key8.draw_line(35,0,0,0);
  key8.draw_line(0,0,35,24);
  key8.draw_line(0,24,35,0);
  key8.refresh();
  delay(1000);
  
  key8.clear();
  key8.select_font(font4x6);
  key8.print_char(1,16,'A');
  key8.print_char(10,16,'B');
  key8.print_char(20,16,'C');
  key8.refresh();
  delay(1000);
  
  key8.clear();
  key8.set_cursor(0,20);
  key8.println("SMALL");
  key8.println("TEXT");
  key8.refresh();
  delay(5000); 
  
  key8.clear();
  key8.select_font(font8x8ext);
  key8.set_cursor(0,23);
  key8.println("BIG"); 
 key8.select_font(font8x8ext); 
  key8.println("TEXT");
  key8.refresh();
  delay(5000); 
 
  key8.load_img(exit_img);
  key8.set_color(BR_RED);
  key8.refresh();
  delay(1000);
}  




