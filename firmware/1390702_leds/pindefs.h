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

#define ROWS_PORT1  PORTC
#define ROWS_DDR1   DDRC

#define ROWS_PORT2  PORTA
#define ROWS_DDR2   DDRA

#define ROWS_PORT3  PORTB
#define ROWS_DDR3   DDRB
#define ROWS_ALL3   (_BV(PB0)|_BV(PB1)|_BV(PB2)|_BV(PB3))

#define SHIFT_PORT  PORTB
#define SHIFT_PIN   PINB
#define SHIFT_CLOCK _BV(PB5)
#define SHIFT_NLOAD _BV(PB6)
#define SHIFT_DATA  _BV(PB4)
#define SHIFT_NUM_OF_PINS  8

#define LED_PORT        PORTD
#define LED_DDR         DDRD
#define LED_SCROLL_PIN  PIND4
#define LED_CAPS_PIN    PIND5
#define LED_NUM_PIN     PIND6
#define LED_ALL_PINS    (_BV(PIND4)|_BV(PIND5)|_BV(PIND6))
