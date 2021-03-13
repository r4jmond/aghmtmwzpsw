#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

struct LedParam {
	unsigned char ucBlinkingFreq;
	unsigned char ucLedIndex;
};

void LedBlink( void *pvParameters ){
	struct LedParam *sLedParam = (struct LedParam*)pvParameters;
	while(1){		
		LedToggle(sLedParam -> ucLedIndex);
		vTaskDelay((1000/(sLedParam -> ucBlinkingFreq)) / 2);
	}
}

void LedCtrl(void *pvParameters) {
	struct LedParam *sLedParam = (struct LedParam*)pvParameters;
	unsigned char uc2divider = 0;
	while(1) {
		vTaskDelay(1000);
		(sLedParam -> ucBlinkingFreq) = ((sLedParam -> ucBlinkingFreq) + 1) % 25;
		if(uc2divider % 2) {
			(sLedParam -> ucLedIndex) = ((sLedParam -> ucLedIndex) + 1) % 4;
		}
		uc2divider++;
	}
}

void TaskSuspend(void *pvParameters) {
	xTaskHandle *xBlinkHandle = (xTaskHandle*)pvParameters;
	while(1) {
		vTaskDelay(1000);
		vTaskSuspend(*xBlinkHandle);
		vTaskDelay(1000);
		vTaskResume(*xBlinkHandle);
	}
}

int main(void) {
	struct LedParam sLedParam;
	xTaskHandle xBlinkHandle;
	
	sLedParam.ucBlinkingFreq = 5;
	sLedParam.ucLedIndex = 0;
	LedInit();
	
	xTaskCreate(LedBlink, NULL , 100 , &sLedParam, 2 , &xBlinkHandle );
	xTaskCreate(TaskSuspend, NULL , 100 , &xBlinkHandle, 2 , NULL );
	vTaskStartScheduler();
	while(1);
}
