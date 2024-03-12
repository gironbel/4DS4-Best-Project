#include "Motor_Control_Component.h"

QueueHandle_t motor_queue;
QueueHandle_t angle_queue;

//prototypes
void setupPWM();
void mediumDelay();
void firstRun();

void setupMotorComponent()
{
	motor_queue = xQueueCreate(1,sizeof(int));
	angle_queue = xQueueCreate(1,sizeof(int));

	setupMotorPins();

	setupDCMotor();
	setupServo();
	firstRun();


    /*************** Motor Task ***************/
	//Create Motor Queue
	//Create Motor Task

    /*************** Position Task ***************/
	//Create Angle Queue
	//Create Position Task

	BaseType_t status1= xTaskCreate(motorTask, "MotorTask",200, (void*)motor_queue, 2,NULL);
	if (status1 !=pdPASS){
		printf("Motor Task creation failed!\r\n");
		while(1);

	}

	BaseType_t status2= xTaskCreate(positionTask, "PositionTask",200, (void*)angle_queue, 2,NULL);
	if (status2 !=pdPASS){
		printf("Motor Position Task creation failed!\r\n");
		while(1);

	}
}

void firstRun(){

    for (volatile int i = 0U; i<1000000; i++)
    	__asm("NOP"); //do nothing
	updatePWM_dutyCycle(FTM_CHANNEL_SERVO_MOTOR, 0.075);
	updatePWM_dutyCycle(FTM_CHANNEL_DC_MOTOR, 0.06);
	FTM_SetSoftwareTrigger(FTM_MOTOR,true);
}

void setupMotorPins()
{
    //Configure PWM pins for DC and Servo motors
    PORT_SetPinMux(PORTC,1U,kPORT_MuxAlt4); //DC Motor config to FTM0_CH0
    PORT_SetPinMux(PORTA,6U,kPORT_MuxAlt3); //Servo Motor config to FTM0_CH3

}

void setupDCMotor()
{
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;
	ftm_pwm_level_select_t pwmLevel = kFTM_HighTrue;
	ftmParam.chnlNumber = FTM_CHANNEL_DC_MOTOR;
	ftmParam.level = pwmLevel;
	ftmParam.dutyCyclePercent = 7;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary = false;
	ftmParam.enableDeadtime = false;
	FTM_GetDefaultConfig(&ftmInfo);
	ftmInfo.prescale = kFTM_Prescale_Divide_128;
	FTM_Init(FTM_MOTOR, &ftmInfo);
	FTM_SetupPwm(FTM_MOTOR, &ftmParam, 1U, kFTM_EdgeAlignedPwm, 50U, CLOCK_GetFreq(kCLOCK_BusClk));
	FTM_StartTimer(FTM_MOTOR, kFTM_SystemClock);

}

void setupServo()
{
	//Initialize PWM for Servo motor
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;
	ftm_pwm_level_select_t pwmLevel = kFTM_HighTrue;

	ftmParam.chnlNumber = FTM_CHANNEL_SERVO_MOTOR;
	ftmParam.level = pwmLevel;
	ftmParam.dutyCyclePercent = 7;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary = false;
	ftmParam.enableDeadtime = false;

	FTM_GetDefaultConfig(&ftmInfo);
	ftmInfo.prescale = kFTM_Prescale_Divide_128;

	FTM_Init(FTM_MOTOR, &ftmInfo);
	FTM_SetupPwm(FTM_MOTOR, &ftmParam, 1U, kFTM_EdgeAlignedPwm, 50U, CLOCK_GetFreq(kCLOCK_BusClk));
	FTM_StartTimer(FTM_MOTOR, kFTM_SystemClock);

}

void updatePWM_dutyCycle(ftm_chnl_t channel, float dutyCycle)
{
	uint32_t cnv, cnvFirstEdge = 0, mod;

	/* The CHANNEL_COUNT macro returns -1 if it cannot match the FTM instance */
	assert(-1 != FSL_FEATURE_FTM_CHANNEL_COUNTn(FTM_MOTOR));

	mod = FTM_MOTOR->MOD;
	if(dutyCycle == 0U)
	{
		/* Signal stays low */
		cnv = 0;
	}
	else
	{
		cnv = mod * dutyCycle;
		/* For 100% duty cycle */
		if (cnv >= mod)
		{
			cnv = mod + 1U;
		}
	}

	FTM_MOTOR->CONTROLS[channel].CnV = cnv;
}


void motorTask(void* pvParameters)
{
	//Motor task implementation
	int input_speed;
	float dutyCycleServoMotor;
	QueueHandle_t motor_q = (QueueHandle_t)pvParameters;
	BaseType_t status;

	updatePWM_dutyCycle(FTM_CHANNEL_DC_MOTOR, 0.0615);
	FTM_SetSoftwareTrigger(FTM_MOTOR,true);


    while (1)
    {
    	status = xQueueReceive(motor_q,(void*)&input_speed,portMAX_DELAY);
    	if (status !=pdPASS){
    		printf("Queue receive for DC motor failed!\r\n");
    		while(1);

    	}


    	dutyCycleServoMotor=input_speed*0.025f/100.0f +0.075;
    	updatePWM_dutyCycle(FTM_CHANNEL_DC_MOTOR, dutyCycleServoMotor);
    	printf("Recieved motor speed:%d\n",input_speed);
    	FTM_SetSoftwareTrigger(FTM_MOTOR,true);
    	vTaskDelay(1/ portTICK_PERIOD_MS);
    }

}

void positionTask(void* pvParameters)
{
	//Position task implementation
	int position;
	float dutyCycleServoMotor;
	QueueHandle_t position_q = (QueueHandle_t)pvParameters;
	BaseType_t status;

	updatePWM_dutyCycle(FTM_CHANNEL_SERVO_MOTOR, 0.0615);
	FTM_SetSoftwareTrigger(FTM_MOTOR,true);


    while (1)
    {
    	status = xQueueReceive(position_q,(void*)&position,portMAX_DELAY);
    	if (status !=pdPASS){
    		printf("Queue receive for position servo failed!\r\n");
    		while(1);

    	}

    	dutyCycleServoMotor=position*0.025f/100.0f +0.075;
    	updatePWM_dutyCycle(FTM_CHANNEL_SERVO_MOTOR, dutyCycleServoMotor);
    	printf("Received position :%d\n",position);
//    	FTM_SetSoftwareTrigger(FTM_MOTOR,true);
//
//    	vTaskDelay(1/ portTICK_PERIOD_MS);

    }
}
