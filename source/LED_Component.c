#include "LED_Component.h"

QueueHandle_t led_queue;

void setupLEDComponent()
{
	setupLEDPins();

	setupLEDs();

    /*************** LED Task ***************/
	//Create LED Queue
	led_queue = xQueueCreate(1,sizeof(int)); //queue holding speed data, used to determine LED colour

	//Create LED Task
	BaseType_t status= xTaskCreate(ledTask, "ledTask",200,(void*)led_queue,2,NULL);
		if (status !=pdPASS){
			printf("ledTask Task creation failed!\r\n");
			while(1);

	}
}
void pwm_setupB() //configuring FTM for blue LED
{
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;

	ftmParam.chnlNumber = kFTM_Chnl_4;
	ftmParam.level = kFTM_HighTrue;
	ftmParam.dutyCyclePercent = 0;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary = false;
	ftmParam.enableDeadtime = false;
	FTM_GetDefaultConfig(&ftmInfo);

	FTM_Init(FTM3, &ftmInfo); //initializes FTM module 3
	FTM_SetupPwm(FTM3, &ftmParam, 1U, kFTM_EdgeAlignedPwm, 5000U, CLOCK_GetFreq(kCLOCK_BusClk)); //5000U is PWM frequency
	FTM_StartTimer(FTM3, kFTM_SystemClock);
}

void pwm_setupG() //configuring FTM for green LED
{
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;

	ftmParam.chnlNumber = kFTM_Chnl_5;
	ftmParam.level = kFTM_HighTrue;
	ftmParam.dutyCyclePercent = 0;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary = false;
	ftmParam.enableDeadtime = false;
	FTM_GetDefaultConfig(&ftmInfo);

	FTM_Init(FTM3, &ftmInfo); //initializes FTM module 3
	FTM_SetupPwm(FTM3, &ftmParam, 1U, kFTM_EdgeAlignedPwm, 5000U, CLOCK_GetFreq(kCLOCK_BusClk)); //5000U is PWM frequency
	FTM_StartTimer(FTM3, kFTM_SystemClock);
}

void pwm_setupR() //configuring FTM for red LED
{
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;

	ftmParam.chnlNumber = kFTM_Chnl_1;
	ftmParam.level = kFTM_HighTrue;
	ftmParam.dutyCyclePercent = 0;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary = false;
	ftmParam.enableDeadtime = false;
	FTM_GetDefaultConfig(&ftmInfo);

	FTM_Init(FTM3, &ftmInfo); //initializes FTM module 3
	FTM_SetupPwm(FTM3, &ftmParam, 1U, kFTM_EdgeAlignedPwm, 5000U, CLOCK_GetFreq(kCLOCK_BusClk)); //5000U is PWM frequency
	FTM_StartTimer(FTM3, kFTM_SystemClock);
}

void setupLEDPins()
{
	//Configure LED pins
	PORT_SetPinMux(PORTD,1U,kPORT_MuxAlt4); //red LED, alternative function 4
	PORT_SetPinMux(PORTC,9U,kPORT_MuxAlt3); //green LED, alternative function 3
	PORT_SetPinMux(PORTC,8U,kPORT_MuxAlt3); //blue LED, alternative function 3 , FTM3 for PWM
}


void setupLEDs()
{
	//Initialize PWM for the LEDs
	pwm_setupR();
	pwm_setupG();
	pwm_setupB();

}

void activateGPIOPin(GPIO_MODULE *modulex, uint32_t pin) //sets the pins we want to be output (hardcoded, no input option, but maybe later)
{
	modulex -> PDDR |= (1UL << pin); //will set the chosen pin to 1
}

void portToggle(GPIO_MODULE *modulex, uint32_t pin)
{
	modulex -> PTOR |= (1UL<<pin); //toggle the pin inputed by setting that bit to 1 in the toggle register
}

void longDelay(void) //Eight times lower frequency when used for toggling LEDs compared to delay()
{
	volatile uint32_t i = 0;
	for (i = 0; i < 6400000; ++i)
	{
		__asm("NOP"); /* delay */
	}
}

void ledTask(void* pvParameters) //LED task implementation
{
	int led_mode;
	QueueHandle_t speed_data = (QueueHandle_t)pvParameters; //convert back to queue
	BaseType_t status;

	while(1)
	{
		status = xQueueReceive(speed_data,(void*)&led_mode,portMAX_DELAY);
		if (status !=pdPASS){
			printf("Queue receive for led speed data failed!\r\n");
		    while(1);
		}
		switch (led_mode){
		case 1500: //yellow
			 FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_1, kFTM_EdgeAlignedPwm, 100); //R
			 FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_5, kFTM_EdgeAlignedPwm, 100); //G
			 FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_4, kFTM_EdgeAlignedPwm, 0); //B
			 FTM_SetSoftwareTrigger(FTM3, true);
			 break;
		case 2000: //red
			 FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_1, kFTM_EdgeAlignedPwm, 100); //R
			 FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_5, kFTM_EdgeAlignedPwm, 0); //G
			 FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_4, kFTM_EdgeAlignedPwm, 0); //B
			 FTM_SetSoftwareTrigger(FTM3, true);
			 break;
		default: //green
			 FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_1, kFTM_EdgeAlignedPwm, 0); //R
			 FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_5, kFTM_EdgeAlignedPwm, 100); //G
			 FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_4, kFTM_EdgeAlignedPwm, 0); //B
			 FTM_SetSoftwareTrigger(FTM3, true);

		}

	}


}
