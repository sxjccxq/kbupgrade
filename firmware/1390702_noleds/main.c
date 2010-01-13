/*
 * Keyboard Upgrade -- Firmware for homebrew computer keyboard controllers.
 * Copyright (C) 2010  Robert Homann
 *
 * This file is part of the Keyboard Upgrade package.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Keyboard Upgrade package; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

#include <inttypes.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>

#define ROWS_PORT1  PORTC
#define ROWS_DDR1   DDRC

#define ROWS_PORT2  PORTA
#define ROWS_DDR2   DDRA

#define COLS_PORT   PORTB
#define COLS_DDR    DDRB
#define COLS_PIN    PINB

#define CFG_KEYMAP0_PORT  PIND
#define CFG_KEYMAP0_PIN   PIND3

#include "usbkeycodes.h"
#include "keyboard.h"
#include "stdmap.h"
#include "keymapdecoder.h"

static Map current_keymap;
static Columnstate column_valid_mask[NUM_OF_ROWS];
static Columnstate column_states[NUM_OF_ROWS];

static inline void activate_high_row(uint8_t row)
{
  /* lower 4 bits of temp are always 0 */
  uint8_t temp=_BV(4+(row&0x0f));

  DDRD=(DDRD&0x0f)|temp;
  PORTD=(PORTD|0xf0)&~temp;
}

static inline void deactivate_high_rows(void)
{
  /* enable pull-ups on all four rows on port D */
  DDRD&=0x0f;
  PORTD|=0xf0;
}

#include "keymapdecode.c"
#include "usbfuns.c"
#include "scanrows.c"
#include "processcolumns.c"
#include "scankeys.c"

static void setup(void)
{
  /* port A through C are all inputs, enable pull-ups */
  ROWS_PORT1=0xff;
  ROWS_DDR1=0x00;
  ROWS_PORT2=0xff;
  ROWS_DDR2=0x00;
  COLS_PORT=0xff;
  COLS_DDR=0x00;

  /* port D has 2 inputs for jumper headers (pins 1 and 3), 4 inputs for
   * some of the rows (pins 4, 5, 6, and 7), and two ports for USB data
   * (pins 0 and 2) */
  PORTD=0xfa;  /* 1111 1010: inputs on 7, 6, 5, 4, 3, 1; outputs on 2, 0 */
  DDRD=0x00;   /* 0000 0000 */

  /* initialize USB stuff, force enumeration */
  usbInit();
  usbDeviceDisconnect();
  wdt_reset();

  set_current_keymap(get_current_keymap_index());

  _delay_ms(400);

  /* that was a delay of at least 400 ms, now connect again */
  wdt_reset();
  usbDeviceConnect();

  /* set timer 0 to CTC mode, prescaling to 1024, and a value for the Output
   * Compare Register so that the timer triggers an interrupt every 16 ms */
#if F_CPU == 16*1000*1000
  OCR0=249;
#elif F_CPU == 12*1000*1000
  OCR0=187;
#else
#error Unsupported value of F_CPU.
#endif
  TCNT0=OCR0-1;
  TIMSK|=_BV(OCIE0);
  TCCR0=_BV(WGM01)|_BV(CS02)|_BV(CS00);

  sei();
}

volatile static uint8_t update_needed;

#include "standard_timer0_isr.c"
#include "standard_main.c"