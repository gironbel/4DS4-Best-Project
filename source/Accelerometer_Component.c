#include "Accelerometer_Component.h"

static fxos_handle_t fxosHandle;
static uint8_t data_scale;

void setupAccelerometerComponent()
{
	setupAccelerometerPins();
	voltageRegulator_enable();
	accelerometer_enable();

	spi_init();

    /*************** Accelerometer Task ***************/
	//Create Accelerometer Task
}

void setupAccelerometerPins()
{
	//Initialize Accelerometer Pins //assuming clocks are set in pin_mux.c
    PORT_SetPinMux(PORTB,10U,kPORT_MuxAlt2); //SPI1 PCS0
    PORT_SetPinMux(PORTB,11U,kPORT_MuxAlt2); //SPI1 SCK
    PORT_SetPinMux(PORTB,16U,kPORT_MuxAlt2); //SPI1 MOSI
    PORT_SetPinMux(PORTB,17U,kPORT_MuxAlt2); //SPI1 MISO

    PORT_SetPinMux(PORTA,25U, kPORT_MuxAsGpio); //SPI RESET
    PORT_SetPinMux(PORTB,8U, kPORT_MuxAsGpio); // SENSORS ENABLE
}

void voltageRegulator_enable()
{
	//Enable voltage Regulator
	gpio_pin_config_t pin_config = {.pinDirection = kGPIO_DigitalOutput,.outputLogic = 0U};
	GPIO_PinInit(GPIOB, 8, &pin_config);
	GPIO_PinWrite(GPIOB, 8, 1U);
}

void accelerometer_enable()
{
	//Enable accelerometer
	gpio_pin_config_t pin_config = {.pinDirection = kGPIO_DigitalOutput,.outputLogic = 0U};
	GPIO_PinInit(GPIOA, 25, &pin_config);
	GPIO_PinWrite(GPIOA, 25, 0U);
}

void spi_init()     //Initialize SPI
{
	dspi_master_config_t masterConfig;

	// Master config
	masterConfig.whichCtar = kDSPI_Ctar0;
	masterConfig.ctarConfig.baudRate = 500000;
	masterConfig.ctarConfig.bitsPerFrame = 8U;
	masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
	masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
	masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / 500000;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / 500000;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / 500000;

	masterConfig.whichPcs = kDSPI_Pcs0;
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig.enableContinuousSCK = false;
	masterConfig.enableRxFifoOverWrite = false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint = kDSPI_SckToSin0Clock;

	DSPI_MasterInit(SPI1, &masterConfig, BUS_CLK);
}

status_t SPI_Send(uint8_t regAddress, uint8_t insert_val) //SPI_write
{

	uint8_t rxBuffSize = 1;
	dspi_transfer_t masterXfer;
	uint8_t *masterTxData = (uint8_t*)malloc(rxBuffSize + 2);
	uint8_t *masterRxData = (uint8_t*)malloc(rxBuffSize + 2);
	//uint8_t *value;
	uint8_t *value = (uint8_t*)malloc(rxBuffSize);


	// Currently this setup gives us 0xC7
	masterTxData[0] = regAddress | 0x80; //Clear the most significant bit (
	masterTxData[1] = regAddress & 0x80; //Clear the least significant 7 bits (
	masterTxData[2] = insert_val; // Data being written

	printf("Writing 0x%X to register address 0x%X now.\n",  insert_val, regAddress);
	//printf("Register address 0x%X\n", regAddress);
	printf("MasterTX0 : 0x%X\n", masterTxData[0]); // 0x8D = 10001101 which is expected
	printf("MasterTX1 : 0x%X\n", masterTxData[1]); // zero because ADDR[7] will be zero for given address
	printf("MasterTX2 : 0x%X\n", masterTxData[2]);

	masterXfer.txData = masterTxData;
	masterXfer.rxData = masterRxData;

	masterXfer.dataSize = rxBuffSize + 2;

	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

	status_t ret = DSPI_MasterTransferBlocking(SPI1, &masterXfer); //returns the value from the master block which is // what is printed //
	memcpy(value, &masterTxData[2], rxBuffSize);
	//printf("Expected value is 0x52 and the transmitted and read value is: 0x%X\n", ret);
	if (ret != kStatus_Success)
	{
		printf("SPI write failed with error code: %d\n", ret);
	}

	else
	{
		printf("SPI write was successful.\n\n");
	}

	free(masterTxData);
	free(masterRxData);

	//printf("Finished writing.\n\n");

	return ret;
}

status_t SPI_receive(uint8_t regAddress, uint8_t *rxBuff, uint8_t rxBuffSize) //SPI_read
{
	dspi_transfer_t masterXfer;

	uint8_t *masterTxData = (uint8_t*)malloc(rxBuffSize + 2); //add two for the address byte and the dont care byte
	uint8_t *masterRxData = (uint8_t*)malloc(rxBuffSize + 2);

	masterTxData[0] = regAddress & 0x7F; //Clear the most significant bit
	masterTxData[1] = regAddress & 0x80; //Clear the least significant 7 bits

	masterXfer.txData = masterTxData;
	masterXfer.rxData = masterRxData;

	masterXfer.dataSize = rxBuffSize + 2; //3 bytes total

	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous; //configure flags

	status_t ret = DSPI_MasterTransferBlocking(SPI1, &masterXfer);
	memcpy(rxBuff, &masterRxData[2], rxBuffSize);

	free(masterTxData);
	free(masterRxData);

	return ret;
}

void accelerometerTask(void* pvParameters)
{
	//Accelerometer task implementation
}
