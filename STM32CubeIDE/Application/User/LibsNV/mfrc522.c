/*
 * mfrc522.c
 *
 *  Created on: Feb 2, 2024
 *      Author: nejcv
 */

#include "mfrc522.h"

/*
 * 8.1.2.3 SPI address byte
 * The address byte must meet the following format.
 * The MSB of the first byte defines the mode used. To read data from the MFRC522 the
 * MSB is set to logic 1. To write data to the MFRC522 the MSB must be set to logic 0. Bits 6
 * to 1 define the address and the LSB is set to logic 0.
 */

#define Write_Pin(port, pin, state) 	HAL_GPIO_WritePin(port, pin, state);
#define __MFRC522_NSS_Low()				Write_Pin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET) 	// pull slave select low
#define __MFRC522_NSS_High()			Write_Pin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)		// pull slave select high
#define Convert_Write_Reg(value)		((value << 1) & 0x7E)							// convert for writing
#define Convert_Read_Reg(value)			(Convert_Write_Reg(value) | 0x80)				// convert for reading
#define Set_Bits(reg_addr, mask)		__MFRC522_Write(reg_addr, __MFRC522_Read(reg_addr) | mask);
#define Clear_Bits(reg_addr, mask)		__MFRC522_Write(reg_addr, __MFRC522_Read(reg_addr) & ~(mask));
#define CRC_BUFFER_SIZE					4
#define ID_BUFFER_SIZE					CRC_BUFFER_SIZE

extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart3;
extern void Error_Handler(void);

void __MFRC522_Write(uint8_t, uint8_t);
void __MFRC522_Reset(void);
void __MFRC522_Antenna_On(void);
void __MFRC522_Stop(void);
void __MFRC522_CRC(uint8_t *, uint8_t *, uint8_t);
uint8_t __MFRC522_Read(uint8_t);
HAL_StatusTypeDef __SPI_Write_Receive(uint8_t, uint8_t *, uint16_t);
enum MFRC522_Status __MFRC522_Req(uint8_t, uint8_t *);
enum MFRC522_Status __MFRC522_Col(uint8_t *);
enum MFRC522_Status __MFRC522_Compute(uint8_t, uint8_t *, uint8_t, uint8_t *, uint16_t *);

// public functions go here

/**
  * @brief MFRC522 initialization function
  * @param None
  * @retval None
  */
void MFRC522_Init(void) {

	// Initializes the MFRC522 by reseting it and configuring its registers

	// reset
	__MFRC522_Reset();

	// set the timer mode and prescaler

	// address, value to be written to the address
	// prescaler is Where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo] = 1101 0011 1110 (12b) = 3390 = D3E
	// timer decrements to 0 and the ComIrqReg register’s TimerIRq bit is set to logic 1
	// timer starts automatically at the end of the transmission in all communication modes at all speeds
	// define settings for the internal timer
	__MFRC522_Write(REG_TIMER_SETT, 0x8D);

	// set value to TPrescaler_Low
	// define settings for the internal timer
	__MFRC522_Write(REG_TIMER_PRESCALER, 0x3E);

	// defines the 16-bit timer reload value
	// low register is set to 0x1E = 30 = 0001 1110
	// defines the lower 8 bits of the 16-bit timer reload value
	// on a start event, the timer loads the timer reload value
	// changing this register affects the timer only at the next start event
	__MFRC522_Write(REG_TIMER_RELOAD2, 0x1E);

	// High register is set to all 0
	__MFRC522_Write(REG_TIMER_RELOAD1, 0x00);

	// configures the receiver gain
	// defines the receiver’s signal voltage gain factor:
	// sets it to 0x70 = 112 = 0111 0000
	// only bits 6 to 4 [6, 4] are used, so the value written is 111
	// which sets it to 48 dB.
	// The ratio of the output signal power to the input signal power of a receiver, usually expressed in decibels
	__MFRC522_Write(REG_RF_GAIN, 0x70);

	// enable the auto timer for transmission and set mode

	// controls the setting of the transmission modulation
	// write 0x40 = 64 = 01000000
	// only bit 6 is used, which is set to 1
	// this sets the following:
	// forces a 100 % ASK modulation independent of the ModGsPReg register setting
	// Amplitude-shift keying (ASK) is a form of amplitude modulation that represents digital data as variations in the amplitude of a carrier wave.
	__MFRC522_Write(REG_TX_ASK, 0x40);

	// defines general modes for transmitting and receiving
	// writes 0x3D = 61 = 0011 1101
	// CRCPreset is set to 01, which means 0x6363
	// PolMFin = 1, which means  polarity of pin MFIN is active HIGH
	// (The most important use of pins MFIN and MFOUT is found in the active antenna concept.
	// An external active antenna circuit can be connected to the MFRC522’s digital block. )
	// TxWaitRF = 1, which means transmitter can only be started if an RF field is generated
	__MFRC522_Write(REG_MODE, 0x3D);

	// turn on the antenna
	__MFRC522_Antenna_On();

}

/**
  * @brief Try to scan and store UID of MIFARE 1K tag
  * @param *myID:	array of uint8_t where UID will be stored
  * @retval MFRC522_Status indicating success or error
  */
enum MFRC522_Status MFRC522_Auth(uint8_t *myID) {

	// myID is the array in which the card id will be stored (read from key)
	enum MFRC522_Status status;

	if ((status = __MFRC522_Req(CMD_PICC_REQIDL, myID)) == MFRC522_OK) {
		// if status is MFRC522_OK, then a card was detected
		status = __MFRC522_Col(myID);
	}
	// if no card was detected, then send MFCR522 to sleep
	__MFRC522_Stop();

	return status;

}

/**
  * @brief Check if two UID are the same
  * @param *myID:	array containing a UID
  * @param *cmp:	array containing a UID that will be checked with myID
  * @param size:	size of both arrays (the size of both arrays must be the same)
  * @retval MFRC522_Status indicating success or error
  */
enum MFRC522_Status MFRC522_Cmp(uint8_t *myID, uint8_t *cmp, uint8_t size) {

	for (int i = 0; i<size; i++) {
		if (myID[i] != cmp[i]) {
			return MFRC522_ERR;
		}
	}

	return MFRC522_OK;

}

/**
  * @brief Print UID of MIFARE 1K tag to terminal
  * @param *myID: array containing the UID
  * @retval None
  */
void MFRC522_Print_Terminal(uint8_t *myID) {

	char buffer[50] = {0};

	for (int i = 0; i<4; i++) {
		sprintf(&buffer[strlen(buffer)], "%u ", myID[i]);
	}
	strcat(buffer, "\r\n");

	if (HAL_UART_Transmit(&huart3, (const uint8_t *) buffer, 50, 10) != HAL_OK) {
		Error_Handler();
	}

}

// private functions go here

/**
  * @brief Sends a request command to a tag or card to initiate communication
  * @param command:		command to executed (usually CMD_TRANSCEIVE)
  * @param *myID:		array where ID of card will be stored
  * @retval MFRC522_Status
  */
enum MFRC522_Status __MFRC522_Req(uint8_t command, uint8_t *myID) {

	enum MFRC522_Status status;
	uint16_t rcv; // received data

	// adjustments for bit-oriented frames
	// write 0x07 = 7 = 0111
	// sets TxLastBits[2:0] to 111, which means
	// used for transmission of bit oriented frames:
	//	defines the number of bits of the last byte that will be
	//	transmitted
	__MFRC522_Write(REG_BIT_FRAMING, 0x07);
	myID[0] = command;

	// send request to card
	// if status is different than OK or received different number of bits from 16
	if (((status = __MFRC522_Compute(CMD_TRANSCEIVE, myID, 1, myID, &rcv)) != MFRC522_OK ) || (rcv != 0x10)) {
		status = MFRC522_ERR;
	}

	return status;

}

/**
  * @brief Execute a command on the MFRC522 and communicate with tag/card
  * @param command:			command to executed (usually CMD_TRANSCEIVE)
  * @param *input:			array where ID of card will be stored
  * @param inputLenght:		size of input array
  * @param *output:			array of bytes received from tag/card
  * @param *outputLength:	size of output array
  * @retval MFRC522_Status
  */
enum MFRC522_Status __MFRC522_Compute(uint8_t command, uint8_t *input, uint8_t inputLength, uint8_t *output, uint16_t *outputLength) {

	enum MFRC522_Status status = MFRC522_ERR; // assume error
	uint8_t enIrq = 0x00, // enable interrupt on MFCR522
			waitIrq = 0x00, // interrupt request enable flag
			lastBits,
			value;
	uint16_t wait = 2000; // this is according to the clock frequency adjustment

	switch (command) {
		case CMD_MF_AUTHENT:
			enIrq = 0x12;
			waitIrq = 0x10; // this value allows us to check, if a command is terminated or an unknown command is started
			break;
		case CMD_TRANSCEIVE:
			enIrq = 0x77;
			waitIrq = 0x30; // this adds checking for an end to a valid data stream
			break;
		default:
			break;
	}

	// enable interrupts and reset FIFO buffer

	// enable and disable interrupt request control bits
	// signal on pin IRQ is inverted with respect to the Status1Reg register’s IRq bit
	__MFRC522_Write(REG_EN_IRQ, enIrq | 0x80);

	// Interrupt request bits
	// indicates that the marked bits in the ComIrqReg register are set
	Clear_Bits(REG_IRQ_BITS, 0x80)

	// number of bytes stored in the FIFO buffer
	// immediately clears the internal FIFO buffer’s read
	// and write pointer and ErrorReg register’s BufferOvfl bit
	// reading this bit always returns 0
	Set_Bits(REG_FIFO_LEVEL, 0x80)

	// put MFRC522 into idle state
	// starts and stops command execution
	__MFRC522_Write(REG_CMD, CMD_IDLE);

	// write data to the FIFO
	for (int i = 0; i<inputLength; i++) {//  (wait < inputLength) {
		// input and output of 64 byte FIFO buffer
		//  data input and output port for the internal 64-byte FIFO buffer FIFO buffer acts as parallel in/parallel out converter for all serial data stream inputs and outputs
		// the register takes a byte that is to be written to the FIFO
		__MFRC522_Write(REG_FIFO_DATA, input[i]);
	}

	// execute the command
	// starts and stops command execution
	// writes [3:0] -> 4 bits that indicate a command
	__MFRC522_Write(REG_CMD, command);

	// if that command is to transmit data from
	// FIFO buffer to antenna and automatically
	// activates the receiver after transmission
	if (command == CMD_TRANSCEIVE) {
		// adjustments for bit-oriented frame
		// writing 0x80 will do the following:
		// starts the transmission of data only valid in combination with the Transceive command
		Set_Bits(REG_BIT_FRAMING, 0x80)
	}

	// wait for command execution (timeout)
	do {
		// check in the interrupt request bits register
		// check if we have detected the end of a valid data stream
		// also check if a command terminates or if an unknown command is started
		value = __MFRC522_Read(REG_IRQ_BITS);
		wait--;
	} while ((wait != 0) && !(value & TimerIRq) && !(value & waitIrq));
	// value & TimerIRq (0x01) means to check if the timer decremented its value to 0
	// break if interrupt request received or timeout

	// delete bit 7 from register adjustments for bit-oriented frame
	Clear_Bits(REG_BIT_FRAMING, 0x80)

	// check for errors and update status
	// if an "timeout" did not occur
	if (wait != 0) {
		// error bits showing the error status of the last command executed
		// 0x1B = 27 = 0001 1011
		// checks for:
		// 		ProtocolErr
		// 		ParityErr
		// 		CollErr
		// 		BufferOvfl
		// so, if no error occurred
		if (!(__MFRC522_Read(REG_ERROR) & 0x1B)) {
			status = MFRC522_OK;
			if (value & enIrq & 0x01) {
				status = MFRC522_ERR; // remove later
			}
			// read response data
			// if the command is CMD_TRANSCEIVE
			// transmits data from FIFO buffer to antenna and automatically
			// activates the receiver after transmission
			if (command == CMD_TRANSCEIVE) {
				// read from FIFO
				// get number of bytes stored in the FIFO buffer
				// FIFOLevel [6:0] indicates the number of bytes stored in the FIFO buffer
				value = __MFRC522_Read(REG_FIFO_LEVEL);
				// miscellaneous control registers
				// bit 7 is TStopNow -> timer stops immediately; reading this bit always returns it to logic 0
				lastBits = __MFRC522_Read(REG_CONTROL) & 0x07; // this is either 0 or 1
				// set the length of the output
				if (lastBits) {
					*outputLength = (value - 1) * 8 + lastBits;
				} else {
					*outputLength = value * 8;
				}
				// if no bytes are stored it the FIFO buffer
				if (value == 0) {
					value = 1;
				}
				// if more that 16 bytes are stored in the FIFO buffer
				if (value > 16) {
					value = 16; // we should read only 4 bytes, as card ID is 4 bytes long
				}
				for (int i = 0; i<value; i++) {
					// input and output of 64 byte FIFO buffer
					// whole register is FIFO data
					// read from FIFO into user specified array
					// (after this we should have ID of the scanned card)
					output[i] = __MFRC522_Read(REG_FIFO_DATA);
				}
			}
		}
	}

	return status;

}

enum MFRC522_Status __MFRC522_Col(uint8_t *number) {

	// send an anticollison command and
	// perform an anticollison algorithm
	// on tag/card to prevent multiple tags from responding

	enum MFRC522_Status status;
	uint8_t serNumCheck = 0, i = 0;
	uint16_t dataSize;

	// adjustments for bit-oriented frames
	// write all 0 to bit framing register
	// this will have the following effect:
	// 		the transmission of data wont start
	//		defines the bit position for the first bit received to be stored in the FIFO buffer
	//		(LSB of the received bit is stored at bit position 0, the second received bit is stored at bit position 1)
	//		defines the number of bits of the last byte that will be transmitted
	__MFRC522_Write(REG_BIT_FRAMING, 0x00);
	number[0] = CMD_PICC_ANTICOLL; // anti-collision
	number[1] = 0x20;

	if ((status = __MFRC522_Compute(CMD_TRANSCEIVE, number, 2, number, &dataSize)) == MFRC522_OK) {
		// check card serial number
		while (i<ID_BUFFER_SIZE) {
			// calculate the XOR checksum on the first four bytes
			serNumCheck ^= number[i];
			i++;
		}
		// check, if the calculated checksum matches the last (fifth) byte
		if (serNumCheck != number[i]) {
			status = MFRC522_ERR;
		}
	}

	return status;

}

void __MFRC522_Stop(void) {

	uint8_t crcBuffer[CRC_BUFFER_SIZE] = {0};
	uint16_t dataSize;

	crcBuffer[0] = CMD_PICC_HALT;
	__MFRC522_CRC(crcBuffer, &crcBuffer[2], 2);
	__MFRC522_Compute(CMD_TRANSCEIVE, crcBuffer, 4, crcBuffer, &dataSize);

}

/**
  * @brief Calculate CRC for the given input data
  * @param *input:		input data for which to calculate CRC
  * @param *output:		array where CRC result will be stored
  * @param	length:		length of array
  * @retval None
  */
void __MFRC522_CRC(uint8_t *input, uint8_t *output, uint8_t lenght) {

	uint8_t wait = 0xFF, value = 0;

	// write to DivIrqReg register
	// clear 0x04 = 00000100
	// effect:	MFIN is active
	//			this interrupt is set when either a rising or falling signal edge is
	//			detected
	Clear_Bits(REG_IRQ_BITS2, 0x04)

	// flush FIFO buffer
	// immediately clears the internal FIFO buffer’s read and write pointer
	// and ErrorReg register’s BufferOvfl bit
	Set_Bits(REG_FIFO_LEVEL, 0x80)

	for (int i = 0; i<lenght; i++) { //while (wait < lenght) {
		// write input to FIFO buffer
		__MFRC522_Write(REG_FIFO_DATA, input[i]);
	}
	// activate command for CRC calculation by writing to this register
	// Command[3:0]
	// activates a command based on the Command value; reading this
	// register shows which command is executed;
	__MFRC522_Write(REG_CMD, CMD_CALC_CRC);

	do {
		// read CRCIRq bit from this register
		// the CalcCRC command is active and all data is processed
		value = __MFRC522_Read(REG_IRQ_BITS2);
		wait--;
	} while((wait != 0) && !(value & 0x04)); // loop until CRCIRq bit is set

	// read CRC calculation result
	// shows the MSB and LSB values of the CRC calculation
	output[0] = __MFRC522_Read(REG_CRC_RESULT2);
	output[1] = __MFRC522_Read(REG_CRC_RESULT1);

}

/**
  * @brief Transmit or receive using HAL SPI
  * @param send:		value to send
  * @param *receive:	where received data will be stored
  * @param	size:		size of receiving data
  * @retval HAL Status
  */
HAL_StatusTypeDef __SPI_Write_Receive(uint8_t send, uint8_t *receive, uint16_t size) {

	uint8_t txArr[]= {send};
	return HAL_SPI_TransmitReceive(&hspi2, txArr, receive, size, 10);

}

/**
  * @brief Turn on the antenna of MFRC522 to start listening
  * @param None
  * @retval None
  */
void __MFRC522_Antenna_On() {

	// turns on the antenna of MFRC522 by setting the REG_TX_CONTROL to 0x03
	// if it is already set, then this does nothing

	// register being read:
	// 	controls the logical behavior of the antenna driver pins TX1 and TX2
	uint8_t rcv;
	if (!((rcv = __MFRC522_Read(REG_TX_CONTROL)) & 0x03)) { // check if rcv is NOT 0x03 = 0011
		// set the fist two bits to 1, without effecting any other bits (they stay the same)
		// set register to 0x03S
		Set_Bits(REG_TX_CONTROL, 0x03)
	}
	// 0x03 for this register means:
	// 	bit 1 = Tx2RFEn -> output signal on pin TX2 delivers the 13.56 MHz energy carrier modulated by the transmission data
	// 	bit 0 = Tx1RFEn -> output signal on pin TX1 delivers the 13.56 MHz energy carrier modulated by the transmission data
	// 	all of this has something todo with: Analog interface and contactless UART

}

/**
  * @brief Reset MFCR522
  * @param None
  * @retval None
  */
void __MFRC522_Reset() {

	// reset it by sending a specific command to a specific register
	__MFRC522_Write(REG_CMD, CMD_SOFT_RESET);

}

/**
  * @brief Write value to register
  * @param reg_addr:	register address
  * @param value:		value to be written
  * @retval None
  */
void __MFRC522_Write(uint8_t reg_addr, uint8_t value) {

	uint8_t rcv;

	// pull slave select low, indicating start of transmission
	__MFRC522_NSS_Low()
	// first send register address
	// covert register address to form requested by MFRC522 and send
	if (__SPI_Write_Receive(Convert_Write_Reg(reg_addr), &rcv, sizeof(rcv)) != HAL_OK) {
		Error_Handler();
	}
	// then write value to that register
	// value does not have to be converted to a specific form
	if (__SPI_Write_Receive(value, &rcv, sizeof(rcv)) != HAL_OK) {
		Error_Handler();
	}
	// pull slave select high, indicating end of transmission
	__MFRC522_NSS_High()

}

/**
  * @brief Read value from register
  * @param reg_addr:	register address
  * @retval register value
  */
uint8_t __MFRC522_Read(uint8_t reg_addr) {

	uint8_t rcv;

	// pull slave select low, indicating start of transmission
	__MFRC522_NSS_Low()
	// first send register address which will be read
	// covert register address to form requested by MFRC522 and send
	if (__SPI_Write_Receive(Convert_Read_Reg(reg_addr), &rcv, sizeof(rcv)) != HAL_OK) {
		Error_Handler();
	}
	// receive value from that register
	if (__SPI_Write_Receive(DUMMY, &rcv, sizeof(rcv)) != HAL_OK) {
		Error_Handler();
	}
	// pull slave select high, indicating end of transmission
	__MFRC522_NSS_High()

	// return value of register
	return rcv;

}

