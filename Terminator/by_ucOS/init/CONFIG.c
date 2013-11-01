#include "stm32f10x.h"
#include "CONFIG.h"
#ifdef QB_UCOSII
#include "misc.h"
#else
#include "stm32f10x_nvic.h"
#endif
/**************
*clock config
*******************/
void RCC_Configuration(void)
{
	SystemInit();
	/*
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus==SUCCESS){
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		PCC_PCLK2Config(RCC_HCLK_Div1);
		PCC_PCLK1Config(RCC_HCLK_Div4);//18M
	}
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
													RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}


/***************
*irq config

*
************************/
void NVIC_Config(void) 
{ 
	#if 1//qbing
    NVIC_InitTypeDef NVIC_InitStructure; 				 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	 //�������ȼ����飺��ռ���ȼ��ʹ����ȼ�
    //NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQChannel;          //ͨ�� ʹ�ܻ���ʧ��ָ����IRQ ͨ��
    NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn   ;          //ͨ�� ʹ�ܻ���ʧ��ָ����IRQ ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// �ò��������˳�ԱNVIC_IRQChannel  �е���ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //   �ò��������˳�ԱNVIC_IRQChannel  �еĴ����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 		 //ͨ������
    NVIC_Init(&NVIC_InitStructure); 
	#endif
} 

#if 0
/* 
* ��������RTC_Configuration  * ����  ������RTC  * ����  ����  * ���  ���� 
* ����  ���ⲿ����  */ 
void RTC_Configuration(void) { 
	/* ʹ��PWR��BKPʱ�� */  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE); 
	/*ȡ���������д��������Ϊ�󱸼Ĵ����зŵ�����Ҫ�����ݣ�Ĭ���ǲ�����������д��ֵ�� */ 
	PWR_BackupAccessCmd(ENABLE); 
	/* ���󱳼Ĵ����ļĴ���ֵ��ΪĬ��ֵ */  
	BKP_DeInit(); 

	/* ���ⲿ���پ���RTC����ѡ���ʱ��Դ���ⲿ���ڲ�����
	�����ⲿ���پ�����������ѡ���ⲿ���پ���32768HZ */ 
	RCC_LSEConfig(RCC_LSE_ON); 
	/*�ȴ��ⲿ���پ���׼������*/ 
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)  {} 
	/*ѡ���ⲿ���پ���ΪRTC��ʱ��Դ*/  
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  
	/* Enable RTC Clock */  
	RCC_RTCCLKCmd(ENABLE); 
	/* �ȴ�RTC�Ĵ�����RTC��APBʱ��ͬ�� */
	RTC_WaitForSynchro(); 
	/* �ȴ��ϴζ�RTC�Ĵ���������� */ 
	RTC_WaitForLastTask();  /* ʹ��RTC�ж� */ 
	RTC_ITConfig(RTC_IT_SEC, ENABLE);  
	/* �ȴ��ϴζ�RTC�Ĵ���������� */ 
	RTC_WaitForLastTask(); 

	/* ����RTC��Ԥ��Ƶֵ����Ϊ�ⲿ���پ�����32768������ѡ
	�� RTC����������Ƶ��= RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */ 
	RTC_SetPrescaler(32767);  

	/* �ȴ��ϴζ�RTC�Ĵ���������� */ 
	RTC_WaitForLastTask();  
}  

/* 
* ��������RTC_IRQHandler(void)  
* ����  ��RTC�жϷ�����  
* ����  ����  
* ���  ����  
* ����  ����  
*/ 
void RTC_IRQHandler(void) { 
	/*�ж��жϱ�־λ�Ƿ���λ*/  
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)  {
		/* ����жϱ�־λ */ 
		RTC_ClearITPendingBit(RTC_IT_SEC); 


		/* TimeDisplay��һ����־λ��ֻ�е���1ʱ���ô��ڷ���ʱ
		�����ݣ����ô���һ�뷢һ��ʱ��ֵ  */ 
		//TimeDisplay = 1; 

		/* Wait until last write operation on RTC registers has finished */  
        RTC_WaitForLastTask(); 


		/* ��ʱ���ߵ�23:59:59��ʱRTC�������е�ֵ���㣬
		0x00015180=23*3600+56*60+59*/   
		if (RTC_GetCounter() == 0x00015180)   
        {    
            RTC_SetCounter(0x0); 
    		/* Wait until last write operation on RTC registers has finished */    
    		RTC_WaitForLastTask(); 
		} 
	} 
}
#endif
