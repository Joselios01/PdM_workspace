
//includes para la liberia completa
#include "stdbool.h"
#include "stdint.h"

// tipos de datos
typedef bool 		bool_t;



// definiciones generales del modulo

#define MFRC522_MAX_LEN         16

/* Mifare_One card command word */
#define PICC_REQIDL         0x26   // find the antenna area does not enter hibernation
#define PICC_REQALL         0x52   // find all the cards antenna area
#define PICC_ANTICOLL       0x93   // anti-collision
#define PICC_SElECTTAG      0x93   // election card
#define PICC_AUTHENT1A      0x60   // authentication key A
#define PICC_AUTHENT1B      0x61   // authentication key B
#define PICC_READ           0x30   // Read Block
#define PICC_WRITE          0xA0   // write block
#define PICC_DECREMENT      0xC0   // debit
#define PICC_INCREMENT      0xC1   // recharge
#define PICC_RESTORE        0xC2   // transfer block data to the buffer
#define PICC_TRANSFER       0xB0   // save the data in the buffer
#define PICC_HALT           0x50   // Sleep


//COMANDOS

#define PCD_IDLE            0x00   //NO action; Cancel the current command
#define PCD_AUTHENT         0x0E   //Authentication Key
#define PCD_RECEIVE         0x08   //Receive Data
#define PCD_TRANSMIT        0x04   //Transmit data
#define PCD_TRANSCEIVE      0x0C   //Transmit and receive data,
#define PCD_RESETPHASE      0x0F   //Reset
#define PCD_CALCCRC         0x03   //CRC Calculate


//PAGINA0 REGISTROS INTERNOS

#define MFRC522_REG_CMD             0x01   // Value command
#define MFRC522_REG_BIT_FRAMING     0x0D   //  define tamaño de bits a transferir
#define MFRC522_REG_COMM_IRQ        0x04   //  define como utilizar el pin-IRQ externo
#define MFRC522_REG_FIFO_LEVEL      0x0A   //  indica num de Bytes en la FIFO
#define MFRC522_REG_COMM_IE_N       0x02
#define MFRC522_REG_ERROR           0x06
#define MFRC522_REG_FIFO_DATA       0x09
#define MFRC522_REG_CONTROL         0x0C
#define MFRC522_REG_DIV_IRQ         0x05

//PAGINA1 REGISTROS INTERNOS
#define MFRC522_REG_TX_AUTO         0x15   // CMD MODULATION ASK
#define MFRC522_REG_MODE            0x11   // configuracion de tX y RX
#define MFRC522_REG_TX_CONTROL      0x14   // registro de comando antena
//PAGINA2 REGISTROS INTERNOS

#define MFRC522_REG_T_MODE          0x2A  // TModeReg and TPrescalerReg registers
#define MFRC522_REG_T_PRESCALER     0x2B  // Divisor de tiempo 13.56MHZ/ (2*Prescaler+1)
#define MFRC522_REG_T_RELOAD_H      0x2C  // registro de recarga H contador de tiempo
#define MFRC522_REG_T_RELOAD_L      0x2D  // registro de recarga L contador de tiempo
#define MFRC522_REG_CRC_RESULT_M    0x21
#define MFRC522_REG_CRC_RESULT_L    0x22

//VALORES REGISTROS INTERNOS
#define MFRC522_VAL_RST             0x0F   // valor de reseteo por software
#define MFRC522_VAL_INTCLK          0x80   // valor Habilitador de relojes internos
#define MFRC522_VAL_PRESCALER       0xA9   // valor de preescaler
#define MFRC522_VAL_RELOADH         0x03   // valor alto de recarga contador de tiempo
#define MFRC522_VAL_RELOADL         0xE8   // valor bajo de recarga contador de tiempo
#define MFRC522_VAL_MODULATION_ASK  0x40   // valor MODULACION ask 100 independiente
#define MFRC522_VAL_MODE_TXRX       0x3D   //   0     , 0 ,  1   , 1 ,   1    , 1 , 0 , 0
										   //MSB FIRST,N/A,WAITRF,N/A,pin MFIN,N/A,CRC,CRC										   //
#define MFRC522_VAL_ANTENA_ON       0x03   // antena encendida
#define MFRC522_VAL_7DATABITS       0x07   // DEFINE TRANSACCION DE 8 BITS

#define MFRC522_VAL_IRQPINCONFIG    0X77 //ENABLED IRQ_PIN(tx,rx Y MODO idle) INTERRUPCION DE ALERTA BAJA
	 //INTERRUOCION POR ERROR, ERROR DE TIEMPO

// creacion de funciones que seran accesibles desde main.

void rc522_init(void);
bool_t rc522_checkCard(uint8_t *id);


