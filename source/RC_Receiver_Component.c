#include "RC_Receiver_Component.h"

SemaphoreHandle_t rc_hold_semaphore;
TaskHandle_t rc_task_handle;

void setupRCReceiverComponent()
{
	setupRCPins();
	setupUART_RC();

	RC_queues controller_data;
	controller_data.motor_q = &motor_queue;
	controller_data.position_q = &angle_queue;
	controller_data.led_q = &led_queue;

    /*************** RC Task ***************/
	//Create RC Semaphore


	//Create RC Task
	BaseType_t status1= xTaskCreate(rcTask, "RCInput",200,(void*)&controller_data,3,NULL);
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
	QueueHandle_t motor_q = motor_queue; //needed if we use motor_queue directly? wouldn't we need to recast pvparams?
	QueueHandle_t angle_q = angle_queue;
	QueueHandle_t led_q = led_queue;

//	QueueHandle_t motor_q = ((RC_queues*)pvParameters)->motor_q; //dereferencing
//	QueueHandle_t angle_q = ((RC_queues*)pvParameters)->position_q;

	int scaled_speed,scaled_position;
	int rc_speed, reflected_value;

	while (1)
	{
		UART_ReadBlocking(UART1, ptr, 1);
		if(*ptr != 0x20){
			continue;}

		UART_ReadBlocking(UART1, &ptr[1], sizeof(rc_values) - 1);
		if(rc_values.header == 0x4020)
		{
			scaled_speed = (rc_values.ch3 - 1000)/10; //left joystick vertical axis to control accel
			rc_speed = rc_values.ch5;
			printf("Channel 5 = %d\n", rc_speed);
			//Reverse motor speed if channel 6 is set
			if (rc_values.ch6 > 1500){
				scaled_speed *=-1;
				printf("Wheels are moving backwards\n");
			}
			else
			{
				printf("Wheels are moving forwards\n");
			}
			// Setting different speed modes. 25 power in lowest setting, half at middle and full at full setting
			switch (rc_values.ch5){
			case 1500:
				scaled_speed *= .5;
				break;
			case 2000:
				break;
			default:
				scaled_speed *= .25;
			}
			status = xQueueSendToBack(motor_queue, (void*) &scaled_speed, portMAX_DELAY);
			if (status != pdPASS) //check if sending function was executed correctly
			{
				PRINTF("Queue Send failed!.\r\n");
				while (1);
			}
			status = xQueueSendToBack(led_queue, (void*) &rc_speed, portMAX_DELAY); //sending speed_data to led queue
			if (status != pdPASS) //check if sending function was executed correctly
			{
				PRINTF("Queue Send failed!.\r\n");
				while (1);
			}

			reflected_value = (rc_values.ch1)*(-1) + 3000;
			//scaled_position = (rc_values.ch1 - 1500)/5;  //right joystick horizontal axis
			scaled_position = (reflected_value - 1500)/5;  //reflected rotation for more intuitive controls

			status = xQueueSendToBack(angle_queue, (void*) &scaled_position, portMAX_DELAY);
			if (status != pdPASS) //check if sending function was executed correctly
			{
				PRINTF("Queue Send failed!.\r\n");
				while (1);
			}

			//printf("Sent signals: %d %d\n",scaled_speed,scaled_position);
//			printf("Channel 1 = %d\t", rc_values.ch1);
//			printf("Channel 2 = %d\t", rc_values.ch2);
//			printf("Channel 3 = %d\t", rc_values.ch3);
//			printf("Channel 4 = %d\t", rc_values.ch4);
//			printf("Channel 5 = %d\t", rc_values.ch5);
//			printf("Channel 6 = %d\t", rc_values.ch6);
//			printf("Channel 7 = %d\t", rc_values.ch7);
//			printf("Channel 8 = %d\r\n", rc_values.ch8);
		}

	}
}


