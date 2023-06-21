/**********************************************************
��ֲע�⣺
����������:�ο�AD9910.h�к궨��
Ȼ����ʵ��AD9110_IOInit��delay_ms����

**********************************************************/

#include "stm32f10x.h"
#include "AD9910.h"
#include "delay.h"
#include "sys.h"

const uchar cfr1[] = {0x00, 0x40, 0x00, 0x00};  //cfr1������
const uchar cfr2[] = {0x01, 0x00, 0x00, 0x00};  //cfr2������
const uchar cfr3[] = {0x05, 0x0F, 0x41, 0x32};  //cfr3������  40M����  25��Ƶ  VC0=101   ICP=001;
uchar profile11[] = {0x3f, 0xff, 0x00, 0x00, 0x25, 0x09, 0x7b, 0x42}; //profile1������ 0x25,0x09,0x7b,0x42
//01������� 23��λ���� 4567Ƶ�ʵ�г��
__attribute__((weak)) void AD9110_IOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure ;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Init(GPIOB , &GPIO_InitStructure) ;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_2;
    GPIO_Init(GPIOA , &GPIO_InitStructure) ;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All ^ (GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_Init(GPIOC , &GPIO_InitStructure) ;
}

//=====================================================================


//======================����8λ���ݳ���===================================
void txd_8bit(uchar txdat)
{
    uchar i, sbt;
    sbt = 0x80;
    SCLK = 0;
    for (i = 0; i < 8; i++)
    {
        if ((txdat & sbt) == 0) AD9910_SDIO = 0;
        else AD9910_SDIO = 1;
        SCLK = 1;
        sbt = sbt >> 1;
        SCLK = 0;
    }
}


/************************************************************
** �������� ��void Init_ad9910(void))
** �������� ����ʼ��AD9910�Ĺܽź���򵥵��ڲ��Ĵ��������ã�
** ��ڲ��� ����
** ���ڲ��� ����
** ����˵�� ����
**************************************************************/
void Init_AD9910(void)
{
    uchar k, m;

    AD9110_IOInit();//IO��ʼ��
    AD9910_PWR = 0;//��������

    PROFILE2 = PROFILE1 = PROFILE0 = 0;
    DRCTL = 0;
    DRHOLD = 0;
    MAS_REST = 1;
    delay_ms(5);
    MAS_REST = 0;

    CS = 0;
    txd_8bit(0x00);    //����CFR1�����ֵ�ַ
    for (m = 0; m < 4; m++)
        txd_8bit(cfr1[m]);
    CS = 1;
    for (k = 0; k < 10; k++);

    CS = 0;
    txd_8bit(0x01);    //����CFR2�����ֵ�ַ
    for (m = 0; m < 4; m++)
        txd_8bit(cfr2[m]);
    CS = 1;
    for (k = 0; k < 10; k++);

    CS = 0;
    txd_8bit(0x02);    //����CFR3�����ֵ�ַ
    for (m = 0; m < 4; m++)
        txd_8bit(cfr3[m]);
    CS = 1;
    for (k = 0; k < 10; k++);

    UP_DAT = 1;
    for(k = 0; k < 10; k++);
    UP_DAT = 0;
    delay_ms(1);

}
/************************************************************
** �������� ��void Txfrc(void))
** �������� ����AD9910оƬ����Ƶ�ʣ����ȵ���ؿ�������
** ��ڲ��� ����
** ���ڲ��� ����
** ����˵�� ����
**************************************************************/
void Txfrc(void)
{
    uchar m;

    CS = 0;
    txd_8bit(0x0e);    //����profile1�����ֵ�ַ
    for (m = 0; m < 8; m++)
        txd_8bit(profile11[m]);
    CS = 1;

    UP_DAT = 1;
    UP_DAT = 0;
}
/************************************************************
** �������� ��void AD9910_FreWrite(void))
** �������� ������Ҫ��Ƶ��ת��Ϊ��Ӧ�Ŀ������ݣ������profile11�����͵�оƬ
** ��ڲ��� ��Ŀ��Ƶ�ʣ���λHz����Χ0~420000000
** ���ڲ��� ����
** ����˵�� ����
**************************************************************/
void AD9910_FreWrite(ulong Freq)
{
    ulong Temp;
    Temp = (ulong)Freq * 4.294967296;	 //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�  ��Ƶ1GHz��32λ��λ�ۼ�������ÿHz�ڵĿ��������� delta =  4.294967296 = (2^32)/1000000000
    profile11[7] = (uchar)Temp;
    profile11[6] = (uchar)(Temp >> 8);
    profile11[5] = (uchar)(Temp >> 16);
    profile11[4] = (uchar)(Temp >> 24);
    Txfrc();
}

/************************************************************
** �������� ��void AD9910_AmpWrite(void))
** �������� �������ȿ������ݱ��浽profile11��д��оƬ
** ��ڲ��� �����ȿ����֣���Χ0~16383
** ���ڲ��� ����
** ����˵�� ��14λ���ȿ����֣���������0~16383��Ӧ�������0~800mV����
**************************************************************/
void AD9910_AmpWrite(uint16_t Amp)
{
    profile11[0] = (Amp % 16384) >> 8;
    profile11[1] = (Amp % 16384) & 0xff;
    Txfrc();
}
