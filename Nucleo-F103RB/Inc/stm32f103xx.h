/*
 * stm32f103xx.h
 *
 *  Created on: Jul 21, 2023
 *      Author: Kowshik
 */

#ifndef STM32F103XX_H_
#define STM32F103XX_H_

#include<stdio.h>

#define RCC_BASEADD			0x40021000U
#define GPIOA_BASEADD		0x40010800U
#define GPIOB_BASEADD		0x40010C00U
#define GPIOC_BASEADD		0x40011000U
#define TIM3_BASEADD		0x40000400U

/*.....................................................Structure of RCC Register..............................................*/

typedef struct
{
	uint32_t CR;
	uint32_t CFGR;
	uint32_t CIR;
	uint32_t APB2RSTR;
	uint32_t APB1RSTR;
	uint32_t AHBENR;
	uint32_t APB2ENR;
	uint32_t APB1ENR;
	uint32_t BDCR;
	uint32_t CSR;
	uint32_t AHBSTR;
	uint32_t CFGR2;

}RCC_RegDef_t;

/* .........................................................Structure of GPIO.......................................... */

typedef struct
{
	uint32_t CRL;
	uint32_t CRH;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t BRR;
	uint32_t LCKR;

}GPIO_RegDef_t;

/*.........................................................Structure of AFIO.............................................*/

typedef struct
{
	uint32_t EVCR;
	uint32_t MAPR;
	uint32_t EXTICR1;
	uint32_t EXTICR2;
	uint32_t EXTICR3;
	uint32_t EXTICR4;
	uint32_t MAPR2;

}AFIO_RegDef_t;

/*.......................Structure of TIM...............................*/

typedef struct
{
	uint32_t CR1;
	uint32_t CR2;
	uint32_t SMCR;
	uint32_t DIER;
	uint32_t SR;
	uint32_t EGR;
	uint32_t CCMR1;
	uint32_t CCMR2;
	uint32_t CCER;
	uint32_t CNT;
	uint32_t PSC;
	uint32_t ARR;
	uint32_t RESVD;
	uint32_t CCR1;
	uint32_t CCR2;
	uint32_t CCR3;
	uint32_t CCR4;
	uint32_t RESVD2;
	uint32_t DCR;
	uint32_t DMAR;


}TIM_RegDef_t;



/*................................................Pointer Declaration declared structures....................................*/
//#define RCC ((RCC_RegDef_t*)RCC_BASEADD)

RCC_RegDef_t *RCC =(RCC_RegDef_t*)RCC_BASEADD;
GPIO_RegDef_t *GPIOC =(GPIO_RegDef_t*)GPIOC_BASEADD;
GPIO_RegDef_t *GPIOA =(GPIO_RegDef_t*)GPIOA_BASEADD;
GPIO_RegDef_t *GPIOB =(GPIO_RegDef_t*)GPIOB_BASEADD;
TIM_RegDef_t *TIM3 =(TIM_RegDef_t*)TIM3_BASEADD;


#define RCC_CR_HSIRDY ((RCC->CR >> 1)&1)
#define RCC_CR_HSION  ((RCC->CR >> 0)&1)
#define TIM_SR_UIF    ((TIM3->SR>>0)&1)


void Tim3config();
void delay(uint32_t ms);
void SetSysClockAs8MHz();


void Tim3config()
{

	/*Enable the Clock for Tim3*/

	RCC->APB1ENR |=(1<<1);
	/*Set the Prescalar value as 7 for 1Mhz*/
	TIM3->PSC =7;
	/*Auto reload register to count upto 1000*/
	TIM3->ARR =999;
	/*Enable the Counter*/
	TIM3->CR1 |=(1<<0);

}

void delay(uint32_t ms)
{
	uint32_t i;
	for(i=0;i<=ms/2;i++)
	{
		/*Clear the counter*/
		TIM3->CNT=0;
		while((TIM3->SR &TIM_SR_UIF)==0);
		TIM3->SR &= ~(1<<0);
	}

}
void SetSysClockAs8MHz()
{
	/* Enabling the HSI clock - If not enabled and ready */
		if((RCC->CR & RCC_CR_HSIRDY)==0)
		{
			RCC->CR |=(1<<0);
			while( (RCC->CR & RCC_CR_HSIRDY) == 0);
		}

		/* Select APB2 prescaler to 1 */
		RCC->CFGR |=(4<<11);

		/* Select APB1 prescaler to 1 */
		RCC->CFGR |=(4<<8);

		/* Select AHB prescaler to 1 */
		RCC->CFGR |=(8<<4);

		/* Select the system clock source */

		RCC->CFGR &=~(3<<1);

		/* Disable  the HSE  clock source */

		RCC->CR &=~(1<<16);
}




#endif /* STM32F103XX_H_ */
