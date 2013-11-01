#ifndef __DS18B20_H__
#define __DS18B20_H__
#include<Delay.h>
#include<Define.h>


/*------------------------------------------------
                  ��������
------------------------------------------------*/
extern	uint	 WD;					  //�¶�ֵ
extern	_Bool	  WD_OK;			//�¶ȿ�������
extern     _Bool	 	  WD_PIN;			//��������



/*------------------------------------------------
                  ��������
------------------------------------------------*/
unsigned int ReadTemperature(void);
_Bool Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);
extern void DelayMs(uchar t);
extern void DelayUS(uchar t);



#endif
