#ifndef LED_COMPONENT_H
#define LED_COMPONENT_H

#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_ftm.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "Motor_Control_Component.h"
#include "Terminal_Component.h"

#include <stdarg.h>

#define FTM_LED 				FTM3
#define FTM_RED_CHANNEL			FTM3
#define FTM_GREEN_CHANNEL		FTM3
#define FTM_BLUE_CHANNEL		FTM3

#define BOARD_LED_GPIO_R_PIN 1
#define BOARD_LED_GPIO_G_PIN 9
#define BOARD_LED_GPIO_B_PIN 8

extern QueueHandle_t led_queue;
extern QueueHandle_t speed_data;

void setupLEDComponent();
void setupLEDs();
void setupLEDPins();

void ledTask(void* pvParameters);

//#define moduleA ((GPIO_MODULE*)0x400FF000) //start point for all functions of portA
//#define moduleB ((GPIO_MODULE*)0x400FF040) //start point for all functions of portB
#define moduleC ((GPIO_MODULE*)0x400FF080) //start point for all functions of portC
#define moduleD ((GPIO_MODULE*)0x400FF0C0) //start point for all functions of portD


typedef struct __attribute__((__packed__)) //from lab0
{
	//each register 4 bytes (32 bit width), all contiguous in memory
	int PDOR;
	int PSOR;
	int PCOR;
	int PTOR;
	int PDIR;
	int PDDR;

}GPIO_MODULE;

void activateGPIOPin(GPIO_MODULE *modulex, uint32_t pin);
void portToggle(GPIO_MODULE *modulex, uint32_t pin);
void longDelay();

#endif /* TERMINAL_COMPONENT_H */
