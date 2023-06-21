/**********************************************************
                       ��������
���ܣ�stm32f103rct6����AD9910ģ��������Ҳ�
�ӿڣ��������Žӿ������AD9910.H
ʱ�䣺
�汾��0.6
���ߣ���������
������

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com/ 
**********************************************************/

#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "AD9910.h"
#include "task_manage.h"
#include "timer.h"

char str[30];	//��ʾ����
extern u8 _return;
int main(void)
{
	u16 i=0;

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//�����жϷ���
	delay_init(72);	//��ʼ����ʱ����
	LED_Init();	//��ʼ��LED�ӿ�
	key_init();//������ʼ��
	initial_lcd();//Һ����ʼ��
	LCD_Clear();
	delay_ms(300);
	LCD_Refresh_Gram();
	
	Timerx_Init(99,71);
	
	LCD_Clear();
	LCD_Show_CEStr(0,0,"5");//��ɫ orange
	
	Init_ad9910();//9910��ʼ��
	Freq_convert(1000000);//дƵ��1MHz
	AD9910_AmpWrite(16383);//0X0000~16383��Ӧ���ֵ0mv~800mv(����)
	
	//1��
	//���������漰���漰�����������ܣ�Ƶ�ʻ���Ȼ������������ܱ���д�����ܵ���������Ĳ�����Ч
	//���������ȡ������ע�ͼ��ɣ���ֱ�Ӹ���Ƶ�ʼ����ȣ�
//	Freq_convert(10000);	//дƵ��10kHz
//	AD9910_AmpWrite(16383);//0X0000~16383��Ӧ���ֵ0mv~800mv(����)
//	while(1);

	//2��
//	����ɨƵ��˵��
//	�ó����ɨƵ�������ö�ʱ���жϣ�����д���Լӵ�Ƶ��ʵ�֣�
//	��timer.C��TIM4_IRQHandler�н��ԼӺ��Ƶ��д��
	while(1)
	{
		KeyRead();//��ȡ����ֵ �� 
		Set_PointFre(Keycode, 0);//��������
		if(_return){_return=0;LCD_Refresh_Gram();}//������ʾ
		KEY_EXIT();
	}	
}

