/*
 * mfrc522.c
 *
 *  Created on: Feb 2, 2024
 *      Author: nejcv
 */

#include "mfrc522.h"

#define Write_Pin(port, pin, state) 	HAL_GPIO_WritePin(port, pin, state);
#define __MFRC522_NSS_Low()				Write_Pin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define __MFRC522_NSS_High()			Write_Pin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
#define Convert_Write_Reg(value)		((value << 1) & 0x7E)
#define Convert_Read_Reg(value)			(Convert_Write_Reg(value) | 0x80)
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

	__MFRC522_Reset();

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

	__MFRC522_Antenna_On();

}

/**
  * @brief Try to scan and store UID of MIFARE 1K tag
  * @param *myID array of uint8_t where UID will be stored
  * @retval MFRC522_Status indicating success or error
  */
enum MFRC522_Status MFRC522_Auth(uint8_t *myID) {

	enum MFRC522_Status status;

	if ((status = __MFRC522_Req(CMD_PICC_REQIDL, myID)) == MFRC522_OK) {
		// if status is MFRC522_OK, then a card was detected
		status = __MFRC522_Col(myID);
	}
	__MFRC522_Stop();

	return status;

}

/**
  * @brief Check if two UID are the same
  * @param *myID array containing a UID
  * @param *cmp array containing a UID that will be checked with myID
  * @param size of both arrays (the size of both arrays must be the same)
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
  * @param *myID array containing the UID
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

	// if status is different than OK or received different number of bits from 16
	if (((status = __MFRC522_Compute(CMD_TRANSCEIVE, myID, 1, myID, &rcv)) != MFRC522_OK ) || (rcv != 0x10)) {
		status = MFRC522_ERR;
	}

	return status;

}

enum MFRC522_Status __MFRC522_Compute(uint8_t command, uint8_t *input, uint8_t inputLength, uint8_t *output, uint16_t *outputLength) {

	enum MFRC522_Status status = MFRC522_ERR; // assume error
	uint8_t enIrq = 0x00,
			waitIrq = 0x00,
			lastBits,
			value;
	uint16_t wait = 0; // i

	switch (command) {
		case CMD_MF_AUTHENT:
			enIrq = 0x12;
			waitIrq = 0x10;
			break;
		case CMD_TRANSCEIVE:
			enIrq = 0x77;
			waitIrq = 0x30;
			break;
		default:
			break;
	}

	__MFRC522_Write(REG_EN_IRQ, enIrq | 0x80);
	Clear_Bits(REG_IRQ_BITS, 0x80)
	Set_Bits(REG_FIFO_LEVEL, 0x80)
	__MFRC522_Write(REG_CMD, CMD_IDLE);

	while (wait < inputLength) {
		__MFRC522_Write(REG_FIFO_DATA, input[wait]);
		wait++;
	}
	__MFRC522_Write(REG_CMD, command);
	if (command == CMD_TRANSCEIVE) {
		Set_Bits(REG_BIT_FRAMING, 0x80)
	}

	wait = 2000;
	do {
		value = __MFRC522_Read(REG_IRQ_BITS);
		wait--;
	} while ((wait != 0) && !(value & 0x01) && !(value & waitIrq));

	Clear_Bits(REG_BIT_FRAMING, 0x80)

	if (wait != 0) {
		if (!(__MFRC522_Read(REG_ERROR) & 0x1B)) {
			status = MFRC522_OK;
			if (value & enIrq & 0x01) {
				status = MFRC522_ERR; // remove later
			}
			if (command == CMD_TRANSCEIVE) {
				value = __MFRC522_Read(REG_FIFO_LEVEL);
				lastBits = __MFRC522_Read(REG_CONTROL) & 0x07;
				if (lastBits) {
					*outputLength = (value - 1) * 8 + lastBits;
				} else {
					*outputLength = value * 8;
				}
				if (value == 0) {
					value = 1;
				}
				if (value > 16) {
					value = 16;
				}
				for (wait = 0; wait < value; wait++) {
					output[wait] = __MFRC522_Read(REG_FIFO_DATA);
				}
			}
		}
	}

	return status;

}

enum MFRC522_Status __MFRC522_Col(uint8_t *number) {

	enum MFRC522_Status status;
	uint8_t serNumCheck = 0, i = 0;
	uint16_t dataSize;

	// adjustments for bit-oriented frames
	__MFRC522_Write(REG_BIT_FRAMING, 0x00);
	number[0] = CMD_PICC_ANTICOLL;
	number[1] = 0x20;

	if ((status = __MFRC522_Compute(CMD_TRANSCEIVE, number, 2, number, &dataSize)) == MFRC522_OK) {
		// check card serial number
		while(i<ID_BUFFER_SIZE) {
			serNumCheck ^= number[i];
			i++;
		}
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

void __MFRC522_CRC(uint8_t *input, uint8_t *output, uint8_t lenght) {

	uint8_t wait = 0, value = 0;

	Clear_Bits(REG_IRQ_BITS2, 0x04)
	Set_Bits(REG_FIFO_LEVEL, 0x80)

	while (wait < lenght) {
		__MFRC522_Write(REG_FIFO_DATA, input[wait]);
		wait++;
	}
	__MFRC522_Write(REG_CMD, CMD_CALC_CRC);

	wait = 0xFF;
	do {
		value = __MFRC522_Read(REG_IRQ_BITS2);
		wait--;
	} while((wait != 0) && !(value & 0x04));

	output[0] = __MFRC522_Read(REG_CRC_RESULT2);
	output[1] = __MFRC522_Read(REG_CRC_RESULT1);

}

HAL_StatusTypeDef __SPI_Write_Receive(uint8_t send, uint8_t *receive, uint16_t size) {

	uint8_t txArr[]= {send};
	return HAL_SPI_TransmitReceive(&hspi2, txArr, receive, size, 10);

}

void __MFRC522_Antenna_On() {

	// register being read:
	// 	controls the logical behavior of the antenna driver pins TX1 and TX2
	uint8_t rcv;
	if (!((rcv = __MFRC522_Read(REG_TX_CONTROL)) & 0x03)) { // check if rcv is NOT 0x03 = 0011
		// set the fist two bits to 1, without effecting any other bits (they stay the same)
		Set_Bits(REG_TX_CONTROL, 0x03)
	}
	// 0x03 for this register means:
	// 	bit 1 = Tx2RFEn -> output signal on pin TX2 delivers the 13.56 MHz energy carrier modulated by the transmission data
	// 	bit 0 = Tx1RFEn -> output signal on pin TX1 delivers the 13.56 MHz energy carrier modulated by the transmission data
	// 	all of this has something todo with: Analog interface and contactless UART

}

void __MFRC522_Reset() {

	__MFRC522_Write(REG_CMD, CMD_SOFT_RESET);

}

void __MFRC522_Write(uint8_t reg_addr, uint8_t value) {

	uint8_t rcv;

	__MFRC522_NSS_Low()
	if (__SPI_Write_Receive(Convert_Write_Reg(reg_addr), &rcv, sizeof(rcv)) != HAL_OK) {
		Error_Handler();
	}
	if (__SPI_Write_Receive(value, &rcv, sizeof(rcv)) != HAL_OK) {
		Error_Handler();
	}
	__MFRC522_NSS_High()

}

uint8_t __MFRC522_Read(uint8_t reg_addr) {

	uint8_t rcv;

	__MFRC522_NSS_Low()
	if (__SPI_Write_Receive(Convert_Read_Reg(reg_addr), &rcv, sizeof(rcv)) != HAL_OK) {
		Error_Handler();
	}
	if (__SPI_Write_Receive(DUMMY, &rcv, sizeof(rcv)) != HAL_OK) {
		Error_Handler();
	}
	__MFRC522_NSS_High()

	return rcv;

}
