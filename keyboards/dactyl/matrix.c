/*
Copyright 2012 Jun Wako
Copyright 2014 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "wait.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "timer.h"
#include "split_util.h"
#include "pro_micro.h"
#include "config.h"

#ifdef USE_I2C
#  include "i2c.h"
#else // USE_SERIAL
#  include "serial.h"
#endif

/* Set 0 if debouncing isn't needed */

#ifndef DEBOUNCING_DELAY
#   define DEBOUNCING_DELAY 0
#endif

#define ERROR_DISCONNECT_COUNT 5

#if (DEBOUNCING_DELAY > 0)
    static uint16_t debouncing_time;
    static bool debouncing = false;
#endif

static const int ROWS_PER_HAND = MATRIX_ROWS/2;
static uint8_t error_count = 0;

#if (MATRIX_COLS <= 8)
#    define print_matrix_header()  print("\nr/c 01234567\n")
#    define print_matrix_row(row)  print_bin_reverse8(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop(matrix[i])
#    define ROW_SHIFTER ((uint8_t)1)
#elif (MATRIX_COLS <= 16)
#    define print_matrix_header()  print("\nr/c 0123456789ABCDEF\n")
#    define print_matrix_row(row)  print_bin_reverse16(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop16(matrix[i])
#    define ROW_SHIFTER ((uint16_t)1)
#elif (MATRIX_COLS <= 32)
#    define print_matrix_header()  print("\nr/c 0123456789ABCDEF0123456789ABCDEF\n")
#    define print_matrix_row(row)  print_bin_reverse32(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop32(matrix[i])
#    define ROW_SHIFTER  ((uint32_t)1)
#endif

#ifdef MATRIX_MASKED
    extern const matrix_row_t matrix_mask[];
#endif

#if (DIODE_DIRECTION == ROW2COL) || (DIODE_DIRECTION == COL2ROW)
static const uint8_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const uint8_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
#endif

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

static matrix_row_t matrix_debouncing[MATRIX_ROWS];


#if (DIODE_DIRECTION == COL2ROW)
    static void init_cols(void);
    static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row, uint8_t offset);
    static void unselect_rows(void);
    static void select_row(uint8_t row);
    static void unselect_row(uint8_t row);
#elif (DIODE_DIRECTION == ROW2COL)
    static void init_rows(void);
    static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, uint8_t offset);
    static void unselect_cols(void);
    static void unselect_col(uint8_t col);
    static void select_col(uint8_t col);
#endif

__attribute__ ((weak))
void matrix_init_quantum(void) {
    matrix_init_kb();
}

__attribute__ ((weak))
void matrix_scan_quantum(void) {
    matrix_scan_kb();
}

__attribute__ ((weak))
void matrix_init_kb(void) {
    matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
    matrix_scan_user();
}

__attribute__ ((weak))
void matrix_init_user(void) {
}

__attribute__ ((weak))
void matrix_scan_user(void) {
}

inline
uint8_t matrix_rows(void) {
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void) {
    return MATRIX_COLS;
}

// void matrix_power_up(void) {
// #if (DIODE_DIRECTION == COL2ROW)
//     for (int8_t r = MATRIX_ROWS - 1; r >= 0; --r) {
//         /* DDRxn */
//         _SFR_IO8((row_pins[r] >> 4) + 1) |= _BV(row_pins[r] & 0xF);
//         toggle_row(r);
//     }
//     for (int8_t c = MATRIX_COLS - 1; c >= 0; --c) {
//         /* PORTxn */
//         _SFR_IO8((col_pins[c] >> 4) + 2) |= _BV(col_pins[c] & 0xF);
//     }
// #elif (DIODE_DIRECTION == ROW2COL)
//     for (int8_t c = MATRIX_COLS - 1; c >= 0; --c) {
//         /* DDRxn */
//         _SFR_IO8((col_pins[c] >> 4) + 1) |= _BV(col_pins[c] & 0xF);
//         toggle_col(c);
//     }
//     for (int8_t r = MATRIX_ROWS - 1; r >= 0; --r) {
//         /* PORTxn */
//         _SFR_IO8((row_pins[r] >> 4) + 2) |= _BV(row_pins[r] & 0xF);
//     }
// #endif
// }

void matrix_init(void) {
	debug_enable=true;
	debug_matrix = true;
	debug_mouse = true;

    TX_RX_LED_INIT;

    // To use PORTF disable JTAG with writing JTD bit twice within four cycles.
    #if  (defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__) || defined(__AVR_ATmega32U4__))
        MCUCR |= _BV(JTD);
        MCUCR |= _BV(JTD);
    #endif

    // initialize row and col
#if (DIODE_DIRECTION == COL2ROW)
    unselect_rows();
    init_cols();
#elif (DIODE_DIRECTION == ROW2COL)
    unselect_cols();
    init_rows();
#endif

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    matrix_init_quantum();
}

uint8_t _matrix_scan(void)
{

    // Right hand is stored after the left in the matirx so, we need to offset it
    uint8_t offset = isLeftHand ? 0 : (ROWS_PER_HAND);

#if (DIODE_DIRECTION == COL2ROW)

    // Set row, read cols
    for (uint8_t current_row = 0; current_row < ROWS_PER_HAND; ++current_row) {
#       if (DEBOUNCING_DELAY > 0)
            bool matrix_changed = read_cols_on_row(matrix_debouncing, current_row, offset);

            if (matrix_changed) {
                debouncing = true;
                debouncing_time = timer_read();
            }

#       else
            read_cols_on_row(matrix, current_row, offset);
#       endif

    }

#elif (DIODE_DIRECTION == ROW2COL)

    // Set col, read rows
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; ++current_col) {
#       if (DEBOUNCING_DELAY > 0)
            bool matrix_changed = read_rows_on_col(matrix_debouncing, current_col, offset);
            if (matrix_changed) {
                debouncing = true;
                debouncing_time = timer_read();
            }
#       else
             read_rows_on_col(matrix, current_col, offset);
#       endif

    }

#endif

#   if (DEBOUNCING_DELAY > 0)
        if (debouncing && (timer_elapsed(debouncing_time) > DEBOUNCING_DELAY)) {
            for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
                matrix[i+offset] = matrix_debouncing[i+offset];
            }
            debouncing = false;
        }
#   endif

    matrix_scan_quantum();
    return 1;
}

bool matrix_is_modified(void)
{
#if (DEBOUNCING_DELAY > 0)
    if (debouncing) return false;
#endif
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
	// Removed matrix mask for ghosting for debugging reasons
    return matrix[row];
}

void matrix_print(void)
{
    print_matrix_header();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        print_matrix_row(row);
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += matrix_bitpop(i);
    }
    return count;
}



#if (DIODE_DIRECTION == COL2ROW)

static void init_cols(void)
{
    for(uint8_t x = 0; x < MATRIX_COLS; x++) {
        uint8_t pin = col_pins[x];
        _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // IN
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // HI
    }
}

static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row, uint8_t offset)
{
    // Store last value of row prior to reading
    matrix_row_t last_row_value = current_matrix[current_row];

    // Clear data in matrix row
    current_matrix[current_row] = 0;

    // Select row and wait for row selecton to stabilize
    select_row(current_row);
    wait_us(30);

    // For each col...
    for(uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {

        // Select the col pin to read (active low)
        uint8_t pin = col_pins[col_index];
        uint8_t pin_state = (_SFR_IO8(pin >> 4) & _BV(pin & 0xF));

        // Populate the matrix row with the state of the col pin
        current_matrix[current_row] |=  pin_state ? 0 : (ROW_SHIFTER << col_index);
    }

    // Unselect row
    unselect_row(current_row);

    return (last_row_value != current_matrix[current_row]);
}

static void select_row(uint8_t row)
{
    uint8_t pin = row_pins[row];
    _SFR_IO8((pin >> 4) + 1) |=  _BV(pin & 0xF); // OUT
    _SFR_IO8((pin >> 4) + 2) &= ~_BV(pin & 0xF); // LOW
}

static void unselect_row(uint8_t row)
{
    uint8_t pin = row_pins[row];
    _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // IN
    _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // HI
}

static void unselect_rows(void)
{
    for(uint8_t x = 0; x < MATRIX_ROWS; x++) {
        uint8_t pin = row_pins[x];
        _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // IN
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // HI
    }
}

#elif (DIODE_DIRECTION == ROW2COL)

static void init_rows(void)
{
    for(uint8_t x = 0; x < MATRIX_ROWS; x++) {
        uint8_t pin = row_pins[x];
        _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // IN
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // HI
    }
}

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, uint8_t offset)
{
    bool matrix_changed = false;

    // Select col and wait for col selecton to stabilize
    select_col(current_col);
    wait_us(30);

    // For each row...
    for(uint8_t row_index = offset; row_index < (ROWS_PER_HAND + offset); ++row_index)
    {

        // Store last value of row prior to reading
        matrix_row_t last_row_value = current_matrix[row_index];

        // Check row pin state
        if ((_SFR_IO8(row_pins[row_index] >> 4) & _BV(row_pins[row_index] & 0xF)) == 0)
        {
            // Pin LO, set col bit
            current_matrix[row_index] |= (ROW_SHIFTER << current_col);
        }
        else
        {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~(ROW_SHIFTER << current_col);
        }

        // Determine if the matrix changed state
        if ((last_row_value != current_matrix[row_index]) && !(matrix_changed))
        {
            matrix_changed = true;
        }
    }

    // Unselect col
    unselect_col(current_col);

    return matrix_changed;
}

static void select_col(uint8_t col)
{
    uint8_t pin = col_pins[col];
    _SFR_IO8((pin >> 4) + 1) |=  _BV(pin & 0xF); // OUT
    _SFR_IO8((pin >> 4) + 2) &= ~_BV(pin & 0xF); // LOW
}

static void unselect_col(uint8_t col)
{
    uint8_t pin = col_pins[col];
    _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // IN
    _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // HI
}

static void unselect_cols(void)
{
    for(uint8_t x = 0; x < MATRIX_COLS; x++) {
        uint8_t pin = col_pins[x];
        _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // IN
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // HI
    }
}

#endif

#ifdef USE_I2C

// Get rows from other half over i2c
int i2c_transaction(void) {
    int slaveOffset = (isLeftHand) ? (ROWS_PER_HAND) : 0;

    int err = i2c_master_start(SLAVE_I2C_ADDRESS + I2C_WRITE);
    if (err) goto i2c_error;

    // start of matrix stored at 0x00
    err = i2c_master_write(0x00);
    if (err) goto i2c_error;

    // Start read
    err = i2c_master_start(SLAVE_I2C_ADDRESS + I2C_READ);
    if (err) goto i2c_error;

    if (!err) {
        int i;
        for (i = 0; i < ROWS_PER_HAND-1; ++i) {
            matrix[slaveOffset+i] = i2c_master_read(I2C_ACK);
	    int temp = matrix[slaveOffset+i];
	    print_val_hex16(temp);
        }
        matrix[slaveOffset+i] = i2c_master_read(I2C_NACK);
	print("i2c read\n");
        i2c_master_stop();
    } else {
i2c_error: // the cable is disconnceted, or something else went wrong
        i2c_reset_state();
        return err;
    }

    return 0;
}

#else // USE_SERIAL

int serial_transaction(void) {
    int slaveOffset = (isLeftHand) ? (ROWS_PER_HAND) : 0;

    if (serial_update_buffers()) {
        return 1;
    }

    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        matrix[slaveOffset+i] = serial_slave_buffer[i];
    }
    return 0;
}
#endif

uint8_t matrix_scan(void)
{
    int ret = _matrix_scan();

#ifdef USE_I2C
    if( i2c_transaction() ) {
#else // USE_SERIAL
    if( serial_transaction() ) {
#endif
        // turn on the indicator led when halves are disconnected
        TXLED1;

        error_count++;

        if (error_count > ERROR_DISCONNECT_COUNT) {
            // reset other half if disconnected
	    int slaveOffset = (isLeftHand) ? (ROWS_PER_HAND) : 0;

            for (int i = 0; i < ROWS_PER_HAND; ++i) {
                matrix[slaveOffset+i] = 0;
            }
        }

    } else {
        // turn off the indicator led on no error
        TXLED0;
        error_count = 0;
    }

    matrix_scan_quantum();

    return ret;
}

void matrix_slave_scan(void) {
    _matrix_scan();

        int offset = (isLeftHand) ? 0 : (ROWS_PER_HAND);

#ifdef USE_I2C
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        /* i2c_slave_buffer[i] = matrix[offset+i]; */
        i2c_slave_buffer[i] = matrix[offset+i];
    }
#else // USE_SERIAL
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        serial_slave_buffer[i] = matrix[offset+i];
    }
#endif
}

