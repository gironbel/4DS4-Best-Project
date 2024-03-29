#include "Terminal_Component.h"

EventGroupHandle_t event_group;
QueueHandle_t uart_queue;

void setupTerminalComponent()
{
	BaseType_t status;

	setupTerminalPins();

	setupUART();

    /*************** UART Task ***************/
	uart_queue = xQueueCreate(10, sizeof(char*));
	if (uart_queue == NULL)
	{
		PRINTF("Queue creation failed!.\r\n");
		while (1);
	}
    status = xTaskCreate(uartTask, "UART task", 200, NULL, 2, NULL);
    if (status != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1);
    }

    /*************** Terminal Control Task ***************/
    //Create Event groups
	//Create Terminal Control Task
}

void setupTerminalPins()
{
    //Configure UART pins
	PORT_SetPinMux(PORTC,13U,kPORT_MuxAlt3); //port C pin 13, function 3 (UART4 CTS)
    PORT_SetPinMux(PORTC,14U,kPORT_MuxAlt3); //port C pin 14, function 3 (UART4 RX)
	PORT_SetPinMux(PORTC,15U,kPORT_MuxAlt3); //port C pin 15, function 3 (UART4 TX)
	PORT_SetPinMux(PORTE,27U,kPORT_MuxAlt3); //port E pin 27, function 3 (UART4 RTS)
}

void sendMessage(const char *format, ...)
{
	va_list args;

	char* str = (char*)pvPortMalloc(250 * sizeof(char));

	va_start(args, format);
	vsprintf(str, format, args);

	if(xQueueSendToBack(uart_queue, (void *) &str, portMAX_DELAY) != pdPASS )
	{
		PRINTF("Send message to uart_queue failed!.\r\n");
		while (1);
	}

	va_end(args);
}

void setupUART()
{
	//Setup UART for sending and receiving
	uart_config_t config;
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 57600;
	config.enableTx = true;
	config.enableRx = true;
	config.enableRxRTS = true;
	config.enableTxCTS = true;

	UART_Init(TARGET_UART, &config, CLOCK_GetFreq(kCLOCK_BusClk));

	/******** Enable Interrupts *********/
	UART_EnableInterrupts(TARGET_UART, kUART_RxDataRegFullInterruptEnable);
	EnableIRQ(UART4_RX_TX_IRQn);
}

void uartTask(void* pvParameters)
{
	char* welcome_message = "UART task started\n\r";
	char* received_str;
	BaseType_t status;

	UART_WriteBlocking(TARGET_UART, welcome_message, strlen(welcome_message));

	while(1)
	{
		status = xQueueReceive(uart_queue, (void *) &received_str, portMAX_DELAY);
		if (status != pdPASS)
		{
			PRINTF("Queue Send failed!.\r\n");
			while (1);
		}
		UART_WriteBlocking(TARGET_UART, received_str, strlen(received_str));
		vPortFree(received_str);
	}
}

char ch;
int new_char=0;

void UART4_RX_TX_IRQHandler()
{
	//UART ISR
	UART_GetStatusFlags(TARGET_UART);
	ch = UART_ReadByte(TARGET_UART);
	new_char = 1;
}

void terminalControlTask(void* pvParameters)
{
	//Terminal Control Task implementation
	//bonus
}
