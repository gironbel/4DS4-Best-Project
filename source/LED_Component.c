#include "LED_Component.h"

QueueHandle_t led_queue;

void setupLEDComponent()
{
	setupLEDPins();

	setupLEDs();

    /*************** LED Task ***************/
	//Create LED Queue
	//Create LED Task
}
void pwm_setupB() //configuring FTM for blue LED
{
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;

	ftmParam.chnlNumber = kFTM_Chnl_4; //changed channel 1 to channel 4
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

void pwm_setupG() //configuring FTM for blue LED
{
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;

	ftmParam.chnlNumber = kFTM_Chnl_5; //changed to channel 5
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

void pwm_setupR() //configuring FTM for blue LED
{
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;

	ftmParam.chnlNumber = kFTM_Chnl_1; //changed to channel 1
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
	PORT_SetPinMux(PORTC,8U,kPORT_MuxAlt3); //blue LED, alternative function 3 , FTM3 for PWM
	PORT_SetPinMux(PORTC,9U,kPORT_MuxAlt3); //green LED, alternative function 3
	PORT_SetPinMux(PORTD,1U,kPORT_MuxAlt4); //red LED, alternative function 4
}

void setupLEDs()
{
	//Initialize PWM for the LEDs
	pwm_setupR();
	pwm_setupG();
	pwm_setupB();

}

void ledTask(void* pvParameters)
{

	//LED task implementation
}
