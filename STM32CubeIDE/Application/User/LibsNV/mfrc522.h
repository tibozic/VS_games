/*
 * mfrc522.h
 *
 *  Created on: Feb 2, 2024
 *      Author: nejcv
 */

#ifndef INC_MFRC522_H_
#define INC_MFRC522_H_

#include "main.h" // include everything
#include <stdio.h>
#include <string.h>

/*
 * Made using docs.:
 * 	- version: Rev. 3.9 — 27 April 2016
 */

#define DUMMY 0x00 // this is idiotic

// commands
#define CMD_IDLE		0x00 // no action, cancels current command execution
#define CMD_MEM			0x01 // stores 25 bytes into the internal buffer
#define CMD_GEN_RND_ID	0x02 // generates a 10-byte random ID number
#define CMD_CALC_CRC	0x03 // activates the CRC coprocessor or performs a self test
#define CMD_TRANSMIT	0x04 // transmits data from the FIFO buffer
#define CMD_NO_CHANGE	0x07 // no command change, can be used to modify the CommandReg register bits without affecting the command
#define CMD_RECEIVE		0x08 // activates the receiver circuits
#define CMD_TRANSCEIVE	0x0C // transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
#define CMD_MF_AUTHENT	0x0E // performs the MIFARE standard authentication as a reader
#define CMD_SOFT_RESET	0x0F // resets the MFRC522

// mifare one card command word
#define CMD_PICC_REQIDL		0x26   // find the antenna area does not enter hibernation
#define CMD_PICC_REQALL		0x52   // find all the cards antenna area
#define CMD_PICC_ANTICOLL	0x93   // anti-collision
#define CMD_PICC_SElECTTAG	0x93   // election card
#define CMD_PICC_AUTHENT1A	0x60   // authentication key A
#define CMD_PICC_AUTHENT1B	0x61   // authentication key B
#define CMD_PICC_READ		0x30   // Read Block
#define CMD_PICC_WRITE		0xA0   // write block
#define CMD_PICC_DECREMENT	0xC0   // debit
#define CMD_PICC_INCREMENT	0xC1   // recharge
#define CMD_PICC_RESTORE	0xC2   // transfer block data to the buffer
#define CMD_PICC_TRANSFER	0xB0   // save the data in the buffer
#define CMD_PICC_HALT		0x50   // sleep

// registers (reserved values are not included)
// page 0: command and status
#define REG_CMD					0x01 // starts and stops command execution
#define REG_EN_IRQ				0x02 // enable and disable interrupt request control bits
#define REG_EN_IRQ2				0x03 // enable and disable interrupt request control bits
#define REG_IRQ_BITS			0x04 // interrupt request bits
#define REG_IRQ_BITS2			0x05 // interrupt request bits
#define REG_ERROR				0x06 // error bits showing the error status of the last command executed
#define REG_STATUS1				0x07 // communication status bits
#define REG_STATUS2				0x08 // receiver and transmitter status bits
#define REG_FIFO_DATA			0x09 // input and output of 64 byte FIFO buffeR
#define REG_FIFO_LEVEL			0x0A // number of bytes stored in the FIFO buffer
#define REG_WATER_LEVEL			0x0B // level for FIFO underflow and overflow warning
#define REG_CONTROL				0x0C // miscellaneous control registers
#define REG_BIT_FRAMING			0x0D // adjustments for bit-oriented frames
#define REG_COLL				0x0E // bit position of the first bit-collision detected on the RF interface

// page 1: command
#define REG_MODE				0x11 // defines general modes for transmitting and receiving
#define REG_TX_MODE				0x12 // defines transmission data rate and framing
#define REG_RX_MODE				0x13 // defines reception data rate and framing
#define REG_TX_CONTROL			0x14 // controls the logical behavior of the antenna driver pins TX1 and TX2
#define REG_TX_ASK				0x15 // controls the setting of the transmission modulation
#define REG_TX_SEL				0x16 // selects the internal sources for the antenna driver
#define REG_RX_SEL				0x17 // selects internal receiver settings
#define REG_RX_THRESHOLD		0x18 // selects thresholds for the bit decoder
#define REG_DEMOD_SETT			0x19 // defines demodulator settings
#define REG_MIFARE_TX			0x1C // controls some MIFARE communication transmit parameters
#define REG_MIFARE_RX			0x1D // controls some MIFARE communication receive parameters
#define REG_SERIAL_SPEED		0x1F // selects the speed of the serial UART interface

// page 2: configuration
#define REG_CRC_RESULT1			0x21 // shows the MSB and LSB values of the CRC calculation (high)
#define REG_CRC_RESULT2			0x22 // shows the MSB and LSB values of the CRC calculation (low)
#define REG_MOD_WIDTH_SETT		0x24 // controls the ModWidth setting
#define REG_RF_GAIN				0x26 // configures the receiver gain
#define REG_CON_ANT				0x27 // selects the conductance of the antenna driver pins TX1 and TX2 for modulation
#define REG_CON_P_NO			0x28 // defines the conductance of the p-driver output during periods of no modulation
#define REG_CON_P_YES			0x29 // defines the conductance of the p-driver output during periods of modulation
#define REG_TIMER_SETT			0x2A // defines settings for the internal timer
#define REG_TIMER_PRESCALER		0x2B // defines settings for the internal timer
#define REG_TIMER_RELOAD1		0x2C // defines the 16-bit timer reload value
#define REG_TIMER_RELOAD2		0x2D // defines the 16-bit timer reload value
#define REG_TIMER_COUNTER_VAL1	0x2E // shows the 16-bit timer value
#define REG_TIMER_COUNTER_VAL2	0x2F // shows the 16-bit timer value

// page 3: test register
#define REG_TEST_SIG			0x31 // general test signal configuration
#define REG_TEST_SIG_PRBS		0x32 // general test signal configuration and PRBS control
#define REG_TEST_PIN_EN			0x33 // enables pin output driver on pins D1 to D7
#define REG_TEST_PIN_VAL		0x34 // defines the values for D1 to D7 when it is used as an I/O bus
#define REG_TEST_BUS_STAT		0x35 // shows the status of the internal test bus
#define REG_AUTO_TEST			0x36 // controls the digital self test
#define REG_SF_VERSION			0x37 // shows the software version
#define REG_TEST_ANALOG			0x38 // controls the pins AUX1 and AUX2
#define REG_TEST_DAC1			0x39 // defines the test value for TestDAC1
#define REG_TEST_DAC2			0x3A // defines the test value for TestDAC2
#define REG_TEST_ADC			0x3B // shows the value of ADC I and Q channels

enum MFRC522_Status {MFRC522_OK = 0, MFRC522_ERR = 1};

// user functions

void MFRC522_Init(void);
void MFRC522_Print_Terminal(uint8_t *);
enum MFRC522_Status MFRC522_Auth(uint8_t *);
enum MFRC522_Status MFRC522_Cmp(uint8_t *, uint8_t *, uint8_t);

#endif /* INC_MFRC522_H_ */
