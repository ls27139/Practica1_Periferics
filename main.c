/**
  ******************************************************************************
  * @file    main.c
  * @author	Sergio Rodrigo Briones
  * @author
  * @version V1.0
  * @date
  * @brief   Default main function.
  ******************************************************************************
*/

/*********************Variables globales*********************/
#define ITM_Port32(n) (*((volatile unsigned long *)(0xE0000000+4*n)))

#include "stm32f4xx.h"

int periodoTiempo = 0;
float temps;
int ticks = 0; //cada 5 milisegundos un tick
int Tiempo_RPM;


void init_Ports(void)
{
	//1er Paso - Crear variable/estructura de lo que quieras modificar en concreto
	GPIO_InitTypeDef GPIO_InitStruct;

	//2o paso - Modificas el contenido que te interese
	//Led verde
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;

	//3er Paso - Inicializar/Activar los cambios
	GPIO_Init(GPIOG, &GPIO_InitStruct);

	//Pulsador Azul
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//Salida PWM puerto PD12
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

	//Salida PWM puerto PC12
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//Entrada PWM PC0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//Entrada PWM PC0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/*******************Generacion del PWM SENSOR*******************/

void initPin12_D(void){

	GPIO_InitTypeDef gpio12;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	gpio12.GPIO_Pin = GPIO_Pin_12;
	gpio12.GPIO_OType = GPIO_OType_PP;
	gpio12.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio12.GPIO_Mode = GPIO_Mode_AF;

	gpio12.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &gpio12);
}

void init_TIM4(void){
	TIM_TimeBaseInitTypeDef t4;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//t4.TIM_Prescaler = 13;

	t4.TIM_CounterMode = TIM_CounterMode_Up;
    t4.TIM_Period = periodoTiempo;
    t4.TIM_ClockDivision = TIM_CKD_DIV1;
    t4.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &t4);
}

void init_PWM_SENSOR(void){
    TIM_OCInitTypeDef pwm;

    pwm.TIM_OCMode = TIM_OCMode_PWM2;
    pwm.TIM_OutputState = TIM_OutputState_Enable;
    pwm.TIM_OCPolarity = TIM_OCPolarity_Low;

    //pwm.TIM_Pulse = ?;

    TIM_OC1Init(TIM4, &pwm);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

/*******************Generacion del PWM INJECCIO*******************/

void initPin12_C(void){

	GPIO_InitTypeDef gpio12;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_TIM3);
	gpio12.GPIO_Pin = GPIO_Pin_12;
	gpio12.GPIO_OType = GPIO_OType_PP;
	gpio12.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio12.GPIO_Mode = GPIO_Mode_AF;

	gpio12.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &gpio12);
}

void init_TIM3(void){
	TIM_TimeBaseInitTypeDef t4;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//t4.TIM_Prescaler = ?;

	t4.TIM_CounterMode = TIM_CounterMode_Up;
    t4.TIM_Period = periodoTiempo;
    t4.TIM_ClockDivision = TIM_CKD_DIV1;
    t4.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &t4);
}

void init_PWM_INJ(void){
    TIM_OCInitTypeDef pwm;

    pwm.TIM_OCMode = TIM_OCMode_PWM2;
    pwm.TIM_OutputState = TIM_OutputState_Enable;
    pwm.TIM_OCPolarity = TIM_OCPolarity_Low;
    //pwm.TIM_Pulse = ?;

    TIM_OC1Init(TIM4, &pwm);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
}


void inicialitza_sistema(void)
{


}

void calcula_temps_injeccio(void)
{

	//Estudiar los 8 casos posibles

	//Tinj(ms)

	/*
	  if(!pin2 && !pin1 && !pin0){
	  	  Tinj = 0.25;
	  }
	  if(!pin2 && !pin1 && pin0){
	  	  Tinj = 0.5;
	  }
	  if(!pin2 && pin1 && !pin0){
	  	  Tinj = 1.0;
	  }
	  if(!pin2 && pin1 && pin0){
	  	  Tinj = 2.0;
	  }
	  if(pin2 && !pin1 && !pin0){
	  	  Tinj = 4.0;
	  }
	  if(pin2 && !pin1 && pin0){
	  	  Tinj = 8.0;
	  }
	  if(pin2 && pin1 && !pin0){
	  	  Tinj = 16.0;
	  }
	  if(pin2 && pin1 && pin0){
	  	  Tinj = 32.0;
	  }

	*/

}

void espera_interrupcio(void)
{

}

int main(void)
{

	inicialitza_sistema();

	while(1){

		calcula_temps_injeccio();

		espera_interrupcio();

	}

	return 0;
}
