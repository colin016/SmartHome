/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      driver.c
 * Description:   ����ʵ���ļ�
 * Author:        Q.Bryan	2013/06/30 9:55:57
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/06/30 9:55:57->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/

#include "driver.h"
#include "util.h"
#include "framework.h"

static void io_port_init(void);
static void time_init(void);
static void UART_Init(void);


/******************************************************************************
 * FUNCTION NAME:	driver_init
 * DESCRIPTION:	    �豸��ʼ��
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
void driver_init(void)
{
  io_port_init();
  time_init();
  UART_Init();
  INTERUPT_ENABLE();
}


static void UART_Init(void)
{
  	PC_DDR |= 0x20;             // C7CXC5C4 C3CXCXCX 	 1111 1000
	PC_CR1 |= 0x20;              	
	PC_CR2 &= ~0x20;             // Output speed up to 10MHz
#if defined(QB_MCU_S105C6)
    //uart 2
    //tx
    PD_DDR |= 0x20;
    PD_CR1 |= 0x20;
    PD_CR1 &= ~0x20;
    //rx
    PD_DDR &= ~0x40;
    PD_CR1 |= 0x40;
    PD_CR1 &= ~0x40;
#endif
    UART1_SR  = 0;
    UART1_CR1 = 0;    		//M[4]=0��1����ʼλ��8������λ
	UART1_CR3 = 0;    		//STOP: ֹͣλ 00��1��ֹͣλ�� 
	UART1_BRR2 = 0x02;      // BaudRate=9600, 00d0	��дUART1_BRR2��дUART1_BRR1
	UART1_BRR1 = 0x68;      //16M/9600=1666=0x0682
    UART1_CR2 = 0x2e;
  
}

#ifdef MY_STM8S003F
_Bool   lamb1Led  @PD_ODR:3;//ά��
_Bool   lamb2Led  @PD_ODR:2;//�޸�
_Bool   cdPlayer  @PD_ODR:1;//����
#else
_Bool   lamb1Led  @PC_ODR:4;//ά��
_Bool   lamb2Led  @PC_ODR:5;//�޸�
_Bool   cdPlayer  @PC_ODR:3;//����
#endif

#if defined(QB_MCU_S105C6)
_Bool led_CLK @PB_ODR:3;
_Bool led_DAT @PB_ODR:5;
_Bool led_CS  @PB_ODR:1;
_Bool led_GND @PB_ODR:7;
#endif

void drive_device(void)
{
#if defined( QB_MCU_S003S3P)
  lamb1Led = sys.lamb1_state;
  lamb2Led = sys.lamb2_state;
  cdPlayer = sys.cdplayer_state;
#elif defined(MY_STM8S003F)
  PD_DDR |= 0x0c;
  PD_CR1 |= 0x0c;
  PD_CR1 &= ~0x0c;
  lamb1Led = sys.lamb1_state;
  lamb2Led = sys.lamb2_state;
  cdPlayer = sys.cdplayer_state;
#elif defined(QB_MCU_S105C6)
  PB_DDR |= 0xff;
  PB_CR1 |= 0xff;   	
  PB_CR2 &= ~0xff;
    
  led_GND   =   0;
  led_CS    =   0;
  led_CLK=0;

  led_DAT=1;delay(10);led_CLK=1;led_CLK=0;//low temp
  led_DAT=0;delay(10);led_CLK=1;led_CLK=0;//mid temp
  led_DAT=0;delay(10);led_CLK=1;led_CLK=0;//charge blue
  led_DAT=sys.lamb1_state;delay(10);led_CLK=1;led_CLK=0;//care
  led_DAT=0;delay(10);led_CLK=1;led_CLK=0;//charge green
  led_DAT=sys.lamb2_state;delay(10);led_CLK=1;led_CLK=0;//repair
  led_DAT=sys.cdplayer_state;delay(10);led_CLK=1;led_CLK=0;//warning
  led_DAT=0;delay(10);led_CLK=1;led_CLK=0;//high temp

  led_CLK=0;
  led_CS    =   1;
#endif
}

static void io_port_init(void)
{
    CLK_CKDIVR = 0x00;    			// 16MHz +16M CPU
    
    PA_DDR |= 0x04;
    PA_CR1 |= 0x04;
    PA_CR2 &= ~0x04;
    
    PA_DDR |= 0x02;
    PA_CR1 |= 0x02;
    PA_CR2 &= ~0x02;
    
    PB_DDR |= 0x20;
    PB_CR1 |= 0x20;
    PB_CR1 &= ~0x20;
    
    PD_DDR |= 0x1C;
    PD_CR1 |= 0x1C;
    PD_CR1 &= ~0x1C;
    
	PC_DDR |= 0xF8;             // C7CXC5C4 C3CXCXCX 	 1111 1000
	PC_CR1 |= 0xF8;              	
	PC_CR2 &= ~0xF8;             // Output speed up to 10MHz
}

static void time_init(void)
{
  	TIM1_CR1 &= ~0x01; //�ر�TIM1 
#if defined(QB_MCU_S003S3P)
    TIM4_SR     = 0x00;                       // ������±�־
	TIM4_PSCR   = 0x07;//��ʱ��=��ʱ��>>7=16MHZ>>7=2^14kHz>>7=128KHZ t=8us
	TIM4_ARR    = 124;//t=125*8=1000us=1ms
	TIM4_CR1    = 0x01;// b0=1,�������������b1=0,����� ��
    TIM4_IER    = 0x01;
#elif defined(MY_STM8S003F)
    TIM4_SR     = 0x00;                       // ������±�־
    TIM4_CR1    = 0x00;
    TIM4_IER    = 0x00;
    TIM4_PSCR   = 0x07;
    TIM4_ARR    = 123;//124
    TIM4_CR1    |= 0x01;// ���ÿ�������������ʱ��
    TIM4_IER    = 0x01;
#elif defined(QB_MCU_S105C6)
	TIM3_PSCR   = 0x07;//��ʱ��=��ʱ��>>7=16MHZ>>7=2^14kHz>>7=128KHZ t=8us
	TIM3_ARR    = 124;//t=125*8=1000us=1ms
	TIM3_CR1    = 0x01;// b0=1,�������������b1=0,����� ��
    TIM3_IER    = 0x01;
#endif
}


#define SYNC_UART
#if 1//ndef SYNC_UART
@far @interrupt void UART1_RX_DATA_FULL(void)
{
  unsigned char c;
  static RECV_DATA_T recv_data;
  
  c = UART1_DR; // ���յ�������
  while((UART1_SR & 0x80) == 0x00);
  //change to upper
  if(c>'a' && c<'z')c=c+32;
  
  if(MAX_RECV_BUFFER_SIZE>recv_data.recved_size)
  {
     recv_data.recv_buffer[recv_data.recved_size++]=c;
  }else{
    //something error!!!!!!!!!!!
    recv_data.recved_size = 0;
  }
  {///////use the data/////begin////////////
    S8 used_len = recived_data_handler(&recv_data);
    U8 i = 0;
    ASSERT(used_len>=0);
    //////////////copy data///////////////
    if(used_len<recv_data.recved_size)
    {
      U8 move_len = recv_data.recved_size - used_len;
      for(i=0;i<move_len;i++){
        recv_data.recv_buffer[i] = recv_data.recv_buffer[used_len+i];
      }
      recv_data.recved_size = move_len;
    }else{
      recv_data.recved_size = 0;
    }
    //////////////copy data///////////////    
  }///////use the data/////begin////////////

}
#else
S16 recv(U8 *buffer,U8 size)
{
  U8 n=0;
  U8 c=0;
  while(n<size){
    while(UART1_SR & 0x20);
    c = UART1_DR; // ���յ�������
    buffer[n++]=c;
    UART1_SR &= ~0x20;
    //UART1_DR = c;
  }
  return n;
}
#endif


/******************************************************************************
 * FUNCTION NAME:	sendData
 * DESCRIPTION:	    ��������
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
void sendData(U8* data,U8 size)
{
    U8 i=0;
    static U8 isSending = 0;
    //////////////////////important///////!!!!!//begin//////////
    if(isSending)return;//!!!!!!!!!ignor current send!!!!!!!!!!
    //////////////////////important///////!!!!!//end////////////
    
    isSending = 1;            
    // b3 = 1,������;  b2 = 1,�������; b5 = 1,������������ж�
    //UART1_CR2 |= 0x08;  		//���ͱ�ʹ��
    //UART1_CR2 &= ~0x04;  		//���ܱ���ֹ

    for(i=0;i<size;i++){
    	while((UART1_SR & 0x80) == 0x00);			
        // �����ͼĴ����� �գ���ȴ�
        //λ7  ������ʱ�򣬸�λ��Ӳ����λ�����UART_CR2��
        //UART_DR��д������ʹ��λ���㡣
        //ת�Ƶ���λ�Ĵ�����
        //TXE:�������ݼĴ�����
        //��TDR�Ĵ����е����ݱ�Ӳ��ת�Ƶ���λ��
        //�����е�TIENλΪ1��������жϡ���
        //0�����ݻ�û�б�
        //1�������Ѿ���ת�Ƶ���λ�Ĵ�����
    	UART1_DR = *data++;
    	while((UART1_SR & 0x80) == 0x00);
    }
    isSending = 0;
    
    //UART1_CR2 &= ~0x08;  		//���ͱ���ֹ
    //UART1_CR2 |= 0x04;  		//���ܱ�ʹ��

}

/******************************************************************************
 * FUNCTION NAME:	operate
 * DESCRIPTION:	    ִ�в���
 *
 * PARAMETER:	    
                dev_id  :   Ŀ���豸id
                op_type :   ��������
                op_data :   ������������
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
void operate(DEVICE_ID dev_id,OP_TYPE op_type,void* op_data)
{
  if(ID_INDICATOR == dev_id){
    indicator = ~indicator;
  }else 
  switch(op_type)
  {
    case OP_OPEN:
      switch(dev_id){
        case ID_LAMB1:
        sys.lamb1_state = 1;
        break;
        case ID_LAMB2:
        sys.lamb2_state = 1;
        break;
        case ID_CDPLAYER:
        sys.cdplayer_state = 1;
       break;
      }
      break;
    case OP_CLOSE:
      switch(dev_id){
        case ID_LAMB1:
        sys.lamb1_state = 0;
        break;
        case ID_LAMB2:
        sys.lamb2_state = 0;
        break;
        case ID_CDPLAYER:
        sys.cdplayer_state = 0;
        break;
      }
      break;
    case OP_NOT:
      switch(dev_id){
        case ID_LAMB1:
        sys.lamb1_state = ~lamb1Led;
        break;
        case ID_LAMB2:
        sys.lamb2_state = ~lamb2Led;
        break;
        case ID_CDPLAYER:
        sys.cdplayer_state = ~sys.cdplayer_state;
        break;
      }
      break;
  }
}

/******************************************************************************
 * FUNCTION NAME:	generate_state_buffer
 * DESCRIPTION:	    ʹ��JSON��ʽʱ�����溯��
 *
 * PARAMETER:	    dest  Ŀ��ռ��׵�ַ
 * RETURNS:         �������ɵ�buffer�ĳ���
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
U8 generate_state_buffer(U8* dest)
{
  U8* p = dest;
  U8 temperature_str[5];
  myitoa(temperature_str,sys.current_temp,10);
  *p= '\0';
  p = mystrcat(p,"{CMD:TICK");
  p = mystrcat(p,",TEMP:");
  p = mystrcat(p,temperature_str);
  p = mystrcat(p,",LEMB1:");
  p = mystrcat(p,lamb1Led?"ON":"OFF");
  p = mystrcat(p,",LEMB2:");
  p = mystrcat(p,lamb2Led?"ON":"OFF");
  p = mystrcat(p,",CDPLAYER:");
  p = mystrcat(p,cdPlayer?"ON":"OFF");
  p = mystrcat(p,",}");
  return p-dest;
}

#define	ENVIROMENT_TEMP_CHANEL          0x02//�����¶�ͨ��//A2  

/******************************************************************************
 * FUNCTION NAME:	GetADC
 * DESCRIPTION:	    �õ�ĳһ��ͨ����DCֵ,ע����뷽ʽ
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
U16 GetADC(U8 chanel)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16	x  = 0;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
    PC_DDR &= ~0x10;
    PC_CR1 &= ~0x10;
    PC_CR2 &= ~0x10;
    ADC_CSR = chanel;
	ADC_CR1 |= 0x01;
    
	delay(100);
	ADC_CR1 |= 0x01;
	while( (ADC_CSR&0x80)==0 );
    
	//x = ADC_DRL;x+=(ADC_DRH<<8);
    x=ADC_DRH;x<<=2;x+=ADC_DRL;
	ADC_CSR  &=  ~0x80;//�����־
    
    return x;    
}

static const U16 TEMP_TABLE[]={
  127,132,138,144,150,157,163,170,177,184,//
  191,199,206,214,222,230,238,246,255,263,//
  272,281,290,299,308,317,327,336,346,355,//
  365,375,384,394,404,414,424,434,443,453,//
  463,473,483,493,502,512,522,531,541,550,//
  559,569,578,587,596,605,613,622,631,639,//
  647,655,664,671,679,687,694,702,709,716,//
  723,730,737,744,750,756,763,769,775,781,//
  786,792,797,803,808,813,818,823,828,832,//
  837,841,846,850,854,858,862,866,870,873,//
  877,880,884,887,890,893,897,900,902,905,//
  908,911,913,916,918,921,923,926,928,930,//
  932,934,936,938,940,942,944,946,948,949,//
  951,953,954,956,957,959,960,962,963,964,//
};


static S16 calc_temperature_by_adc(U32 adc)//�����¶�ֵ
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
    S16 high = sizeof(TEMP_TABLE)/sizeof(TEMP_TABLE[0]);
    S16 low = 0;
    S16 mid = 0;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
    //binary sereach
    while(low<=high)
    {
        mid = (high + low)>>1;
        if(TEMP_TABLE[mid]>adc)
            high = mid - 1;
        else if(TEMP_TABLE[mid]<adc)
            low = mid +1;
        else 
            break;
    }
    return (mid-20);
}

S16 GetT(void)
{
  return calc_temperature_by_adc(GetADC(ENVIROMENT_TEMP_CHANEL));
}

/******************************************************************************
 * FUNCTION NAME:	report_system_state
 * DESCRIPTION:	    ����ϵͳ��ǰ״̬����������������
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
void report_system_state(void)
{
  SYSTEM_RUNNING_IND();

  sys.current_temp = GetT();
  //send to network
  sendData((U8*)&sys,sizeof(sys));
}
