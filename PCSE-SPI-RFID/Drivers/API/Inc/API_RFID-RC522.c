/*
 * API_RFID-RC522.c
 *
 *  Created on: 18/07/2023
 *      Author: joseluis
 */

#include "API_RFID-RC522.h"
#include "stm32f429ZI.h"


SPI_HandleTypeDef hspi4;


#define SPI4_CS_Pin       GPIO_PIN_4
#define SPI4_CS_GPIO_Port GPIOE

typedef enum
{
  RFID_ENABLED = 0,
  RFID_DISABLED=1
}Estate_RFID;


//funcciones privadas que solo seran accesibles desde la libreria
void    rc522_antennaON(void);
void    rc522_setBit(uint8_t reg, uint8_t mask);
void    rc522_clearBit(uint8_t reg, uint8_t mask);
void    rc522_halt(void);
uint8_t rc522_regRead8(uint8_t reg);
void    rc522_regWrite8(uint8_t reg, uint8_t data8);
void    spi_cs_rfid_write(bool_t state);
void    SPI4_Init(void);
bool_t  rc522_request(uint8_t reqMode, uint8_t *tagType);
bool_t  rc522_antiColl(uint8_t* serNum);
void    rc522_calculateCRC(uint8_t*  pIndata, uint8_t len, uint8_t* pOutData);
bool_t  rc522_toCard(  uint8_t command,
		   			   uint8_t* sendData,
					   uint8_t sendLen,
					   uint8_t* backData,
					   uint16_t* backLen);




bool rc522_checkCard(uint8_t *id)
{
  bool status=false;
  //Find cards, return card type
    status = rc522_request(PICC_REQIDL, id);
    if (status == true) {
      //Card detected
      //Anti-collision, return card serial number 4 bytes
      status = rc522_antiColl(id);
    }
    rc522_halt();      //Command card into hibernation

    return status;
}


void rc522_halt(void)
{
  uint16_t unLen;
  uint8_t buff[4];

  buff[0] = PICC_HALT;
  buff[1] = 0;
  rc522_calculateCRC(buff, 2, &buff[2]);

  rc522_toCard(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}


void rc522_calculateCRC(uint8_t*  pIndata, uint8_t len, uint8_t* pOutData)
{
  uint8_t i, n;

  rc522_clearBit(MFRC522_REG_DIV_IRQ, 0x04);     //CRCIrq = 0
  rc522_setBit(MFRC522_REG_FIFO_LEVEL, 0x80);      //Clear the FIFO pointer
  //Write_MFRC522(CommandReg, PCD_IDLE);

  //Writing data to the FIFO
  for (i = 0; i < len; i++) {
    rc522_regWrite8(MFRC522_REG_FIFO_DATA, *(pIndata+i));
  }
  rc522_regWrite8(MFRC522_REG_CMD, PCD_CALCCRC);

  //Wait CRC calculation is complete
  i = 0xFF;
  do {
    n = rc522_regRead8(MFRC522_REG_DIV_IRQ);
    i--;
  } while ((i!=0) && !(n&0x04));      //CRCIrq = 1

  //Read CRC calculation result
  pOutData[0] = rc522_regRead8(MFRC522_REG_CRC_RESULT_L);
  pOutData[1] = rc522_regRead8(MFRC522_REG_CRC_RESULT_M);
}


bool_t rc522_request(uint8_t reqMode, uint8_t *tagType)
{
  bool status=false;
  uint16_t backBits;
  rc522_regWrite8(MFRC522_REG_BIT_FRAMING, MFRC522_VAL_7DATABITS); // TRANSFERENCIA DE 8 BITS
  tagType[0] = reqMode;
  status = rc522_toCard(PCD_TRANSCEIVE, tagType, 1, tagType, &backBits);
  if ((status != true) || (backBits != 0x10)) {
    status = false;
  }
  return status;
}


bool rc522_antiColl(uint8_t* serNum)
{
  bool status;
  uint8_t i;
  uint8_t serNumCheck = 0;
  uint16_t unLen;
  //for (i = 0; i < 4; i++)
//    printf("Anticoll In %d: 0x%02x\r\n", i, serNum[i]);


  rc522_regWrite8(MFRC522_REG_BIT_FRAMING, 0x00);    //TxLastBists = BitFramingReg[2..0]

  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
  status = rc522_toCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

  //for (i = 0; i < 4; i++)
//      printf("Anticoll ToCard %d: 0x%02x\r\n", i, serNum[i]);

  if (status == true) {
    //Check card serial number
    for (i = 0; i < 4; i++) {
      serNumCheck ^= serNum[i];
    }
    if (serNumCheck != serNum[i]) {
      status = false;
    }
  }
  return status;
}





bool rc522_toCard(  uint8_t command,
					uint8_t* sendData,
					uint8_t sendLen,
					uint8_t* backData,
					uint16_t* backLen)
{
  bool status = false;
  uint8_t irqEn = 0x00;
  uint8_t waitIRq = 0x00;
  uint8_t lastBits;
  uint8_t n;
  uint16_t i;

  switch (command)
  {
	case PCD_AUTHENT:
	{
		irqEn = 0x12;
		waitIRq = 0x10;
		break;
	}
	case PCD_TRANSCEIVE:
	{
		irqEn = MFRC522_VAL_IRQPINCONFIG;
		waitIRq = 0x30;
		break;
	}
	default:
		break;
  }

  rc522_regWrite8(MFRC522_REG_COMM_IE_N, irqEn | 0x80);
  rc522_clearBit(MFRC522_REG_COMM_IRQ, 0x80);   // MSB lo pone en 0
  rc522_setBit(MFRC522_REG_FIFO_LEVEL, 0x80);   // MSB lo pone en 1

  rc522_regWrite8(MFRC522_REG_CMD, PCD_IDLE);  // ponerlo en modo reposo o cancelar comando

  //Writing data to the FIFO
  for (i = 0; i < sendLen; i++)
  {
    rc522_regWrite8(MFRC522_REG_FIFO_DATA, sendData[i]);
  }

  //Execute the command
  rc522_regWrite8(MFRC522_REG_CMD, command);
  if (command == PCD_TRANSCEIVE) {
    rc522_setBit(MFRC522_REG_BIT_FRAMING, 0x80);   //StartSend=1,transmission of data starts
  }

  //Waiting to receive data to complete
  i = 100;  //i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
  do {
    //CommIrqReg[7..0]
    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = rc522_regRead8(MFRC522_REG_COMM_IRQ);
    i--;
  } while ((i!=0) && !(n&0x01) && !(n&waitIRq));

  rc522_clearBit(MFRC522_REG_BIT_FRAMING, 0x80);     //StartSend=0

  if (i != 0)  {
    if (!(rc522_regRead8(MFRC522_REG_ERROR) & 0x1B)) {
      status = true;
      if (n & irqEn & 0x01) {
        status = false;
      }

      if (command == PCD_TRANSCEIVE) {
        n = rc522_regRead8(MFRC522_REG_FIFO_LEVEL);
        uint8_t l = n;
        lastBits = rc522_regRead8(MFRC522_REG_CONTROL) & 0x07;
        if (lastBits) {
          *backLen = (n - 1) * 8 + lastBits;
        } else {
          *backLen = n * 8;
        }

        if (n == 0) {
          n = 1;
        }
        if (n > MFRC522_MAX_LEN) {
          n = MFRC522_MAX_LEN;
        }

        //Reading the received data in FIFO
        for (i = 0; i < n; i++) {
          uint8_t d = rc522_regRead8(MFRC522_REG_FIFO_DATA);
          if (l == 4)
            printf("%02x ", d);
          backData[i] = d;
        }
        if (l==4)
          printf("\r\n");
        return status;
      }
    } else {
      printf("error\r\n");
      status = false;
    }
  }

  return status;
}

//**************************************************************************************
//**************************************************************************************
void rc522_init(void)
{
	SPI4_Init();

	/*Configure GPIO pin : SPI4_CS_Pin */
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  HAL_GPIO_WritePin(SPI4_CS_GPIO_Port, SPI4_CS_Pin, GPIO_PIN_RESET);
	  GPIO_InitStruct.Pin = SPI4_CS_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(SPI4_CS_GPIO_Port, &GPIO_InitStruct);



	rc522_regWrite8(MFRC522_REG_CMD        , MFRC522_VAL_RST); // reset
	rc522_regWrite8(MFRC522_REG_T_MODE     , MFRC522_VAL_INTCLK);   // habilita todos los relojes internos
	rc522_regWrite8(MFRC522_REG_CMD        , MFRC522_VAL_RST); // reset
	rc522_regWrite8(MFRC522_REG_T_MODE     , MFRC522_VAL_INTCLK);   // habilita todos los relojes internos
	rc522_regWrite8(MFRC522_REG_T_PRESCALER, MFRC522_VAL_PRESCALER);
	rc522_regWrite8(MFRC522_REG_T_RELOAD_L , MFRC522_VAL_RELOADL );
	rc522_regWrite8(MFRC522_REG_T_RELOAD_H , MFRC522_VAL_RELOADH );
	rc522_regWrite8(MFRC522_REG_TX_AUTO    , MFRC522_VAL_MODULATION_ASK);
	rc522_regWrite8(MFRC522_REG_MODE       , MFRC522_VAL_MODE_TXRX );
	rc522_antennaON();   //Open the antenna
}


void rc522_antennaON(void)
{
  uint8_t temp;

  temp = rc522_regRead8(MFRC522_REG_TX_CONTROL);
  if (!(temp & MFRC522_VAL_ANTENA_ON)) // solo asegura que bits de antena_on esten en 1 0x03
  {
    rc522_setBit(MFRC522_REG_TX_CONTROL, MFRC522_VAL_ANTENA_ON); // en caso que no los enciende
  }
}


void rc522_setBit(uint8_t reg, uint8_t mask)
{
  rc522_regWrite8(reg, rc522_regRead8(reg)|mask);   // lee el registro, hace mascara y vuelve a escribir
}

void rc522_clearBit(uint8_t reg, uint8_t mask)
{
  rc522_regWrite8(reg, rc522_regRead8(reg)&(~mask)); // lee el registro, hace mascara y vuelve a escribir
}



uint8_t rc522_regRead8(uint8_t reg)
{
  spi_cs_rfid_write(0);

  reg = ((reg << 1) & 0x7E) | 0x80;

  HAL_SPI_Transmit(&hspi4, &reg, 1, 2000);
  uint8_t dataRd=0;

  HAL_SPI_Receive(&hspi4, &dataRd, 1, 2000);

  spi_cs_rfid_write(1);
  return dataRd;
}





void rc522_regWrite8(uint8_t reg, uint8_t data8)
{
	spi_cs_rfid_write(0);  // HABILITA MODULO
	uint8_t txData[2] = {0x7E&(reg << 1), data8};
    HAL_SPI_Transmit(&hspi4, txData, 2, 2000);
    spi_cs_rfid_write(1);  // DES-HABILITA MODULO
}



void spi_cs_rfid_write(bool_t state)
{
	if(!state)
	  HAL_GPIO_WritePin(SPI4_CS_GPIO_Port, SPI4_CS_Pin , RFID_ENABLED );
	else
	  HAL_GPIO_WritePin(SPI4_CS_GPIO_Port, SPI4_CS_Pin , RFID_DISABLED  );
}


void SPI4_Init(void)
{



	  hspi4.Instance = SPI4;
	  hspi4.Init.Mode = SPI_MODE_MASTER;
	  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
	  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
	  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
	  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
	  hspi4.Init.NSS = SPI_NSS_SOFT;
	  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
	  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
	  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	  hspi4.Init.CRCPolynomial = 10;
	  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    while(1);
  }

}

