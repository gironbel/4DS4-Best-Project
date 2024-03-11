#include "RC_Receiver_Component.h"

SemaphoreHandle_t rc_hold_semaphore;
TaskHandle_t rc_task_handle;

void setupRCReceiverComponent()
{
	setupRCPins();

	setupUART_RC();

	RC_queues controller_data;
	controller_data.motor_q = motor_queue;
	controller_data.position_q = angle_queue;

    /*************** RC Task ***************/
	//Create RC Semaphore


	//Create RC Task
	BaseType_t status1= xTaskCreate(rcTask, "RCInput",200,(void*)&controller_data,2,NULL);
	if (status1 !=pdPASS){
		printf("RCInput Task creation failed!\r\n");
		while(1);

	}

}

void setupRCPins()
{
	//Configure RC pins
	PORT_SetPinMux(PORTC,3U,kPORT_MuxAlt3); //rc uart input
}

void setupUART_RC()
{
	//setup UART for RC receiver
	uart_config_t config;
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 115200;
	config.enableTx = false;
	config.enableRx = true;
	UART_Init(UART1, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));
}

void rcTask(void* pvParameters)
{
	PRINTF("Starting rc task.\r\n");

	RC_Values rc_values;
	uint8_t* ptr = (uint8_t*) &rc_values;
	BaseType_t status;
	//RC task implementation

	QueueHandle_t motor_queue = ((RC_queues*)pvParameters)->motor_q; //dereferencing
	QueueHandle_t angle_queue = ((RC_queues*)pvParameters)->position_q;

	int scaled_speed;
	int scaled_position;

	while (1)
	{
		UART_ReadBlocking(UART1, ptr, 1);
		if(*ptr != 0x20)
		continue;

		PRINTF("Begin data input.\r\n");
		UART_ReadBlocking(UART1, &ptr[1], sizeof(rc_values) - 1);
		if(rc_values.header == 0x4020)
		{
			scaled_speed = rc_values.ch2 * 0.01f /100.0f - 0.015; //right joystick up and down to control accel
			status = xQueueSendToBack(motor_queue, (void*) scaled_speed, portMAX_DELAY); //will write the string at the back of the queue
			if (status != pdPASS) //check if sending function was executed correctly
			{
				PRINTF("Queue Send failed!.\r\n");
				while (1);
			}

			scaled_position = rc_values.ch3 * 0.01f /100.0f  - 0.015;  //left joystick left and right to control angle of wheels
			status = xQueueSendToBack(angle_queue, (void*) scaled_position, portMAX_DELAY); //will write the string at the back of the queue
			if (status != pdPASS) //check if sending function was executed correctly
			{
				PRINTF("Queue Send failed!.\r\n");
				while (1);
			}

			printf("Channel 1 = %d\t", rc_values.ch1);
			printf("Channel 2 = %d\t", rc_values.ch2);
			printf("Channel 3 = %d\t", rc_values.ch3);
			printf("Channel 4 = %d\t", rc_values.ch4);
			printf("Channel 5 = %d\t", rc_values.ch5);
			printf("Channel 6 = %d\t", rc_values.ch6);
			printf("Channel 7 = %d\t", rc_values.ch7);
			printf("Channel 8 = %d\r\n", rc_values.ch8);
		}

	}
}


