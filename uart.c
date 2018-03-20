#include "uart.h"

extern volatile u32* uart0_base;

void uart_init(u8 u32 baud_rate){
	u16 mantissa;
	u16 fraction;
	*(uart0_base+BCM2835_UART0_IBRD/4)=mantissa;
	*(uart0_base+BCM2835_UART0_FBRD/4)=mantissa;
}

void uart_init(u8 pclk2,u16 bound){//参数：APB2时钟频率、波特率(小于65535bps)  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分,更为精确的是使用round()	 
    mantissa<<=4;
	mantissa+=fraction;	//合并整数和小数部分得到USART->BRR 
	RCC->APB2ENR|=0x04;   //使能PORTA口时钟  
	RCC->APB2ENR|=0x4000; //使能USART1时钟 
	GPIOA->CRH&=0XFFFFF00F; 
	GPIOA->CRH|=0x000008B0;//IO状态设置,PA9->RX,PA10->TX
		  
	RCC->APB2RSTR|=0x4000;   //复位USART1
	RCC->APB2RSTR&=0xffffbfff;//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0x200C;  //1位停止,8数据位,无校验位
#ifdef EN_USART1_RX		  //如果使能了接收
	//使能接收中断
	USART1->CR1|=0x0100;    //校验错误中断使能
	USART1->CR1|=0x20;    //接收缓冲区非空中断使能
	NVIC_Init(USART1_IRQChannel,2,1);
#endif
}