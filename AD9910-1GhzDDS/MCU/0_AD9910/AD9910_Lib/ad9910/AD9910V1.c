/**********************************************************
移植注意：
首先是引脚:参考AD9910.h中宏定义
然后是实现AD9110_IOInit、delay_ms函数

**********************************************************/

#include "stm32f10x.h"
#include "AD9910.h"
#include "delay.h"
#include "sys.h"

const uchar cfr1[] = {0x00, 0x40, 0x00, 0x00};  //cfr1控制字
const uchar cfr2[] = {0x01, 0x00, 0x00, 0x00};  //cfr2控制字
const uchar cfr3[] = {0x05, 0x0F, 0x41, 0x32};  //cfr3控制字  40M输入  25倍频  VC0=101   ICP=001;
uchar profile11[] = {0x3f, 0xff, 0x00, 0x00, 0x25, 0x09, 0x7b, 0x42}; //profile1控制字 0x25,0x09,0x7b,0x42
//01振幅控制 23相位控制 4567频率调谐字
__attribute__((weak)) void AD9110_IOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure ;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PE端口时钟

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


//======================发送8位数据程序===================================
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
** 函数名称 ：void Init_ad9910(void))
** 函数功能 ：初始化AD9910的管脚和最简单的内部寄存器的配置，
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Init_AD9910(void)
{
    uchar k, m;

    AD9110_IOInit();//IO初始化
    AD9910_PWR = 0;//软件拉低

    PROFILE2 = PROFILE1 = PROFILE0 = 0;
    DRCTL = 0;
    DRHOLD = 0;
    MAS_REST = 1;
    delay_ms(5);
    MAS_REST = 0;

    CS = 0;
    txd_8bit(0x00);    //发送CFR1控制字地址
    for (m = 0; m < 4; m++)
        txd_8bit(cfr1[m]);
    CS = 1;
    for (k = 0; k < 10; k++);

    CS = 0;
    txd_8bit(0x01);    //发送CFR2控制字地址
    for (m = 0; m < 4; m++)
        txd_8bit(cfr2[m]);
    CS = 1;
    for (k = 0; k < 10; k++);

    CS = 0;
    txd_8bit(0x02);    //发送CFR3控制字地址
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
** 函数名称 ：void Txfrc(void))
** 函数功能 ：向AD9910芯片发送频率，幅度等相关控制数据
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Txfrc(void)
{
    uchar m;

    CS = 0;
    txd_8bit(0x0e);    //发送profile1控制字地址
    for (m = 0; m < 8; m++)
        txd_8bit(profile11[m]);
    CS = 1;

    UP_DAT = 1;
    UP_DAT = 0;
}
/************************************************************
** 函数名称 ：void AD9910_FreWrite(void))
** 函数功能 ：将需要的频率转换为对应的控制数据，保存进profile11并发送到芯片
** 入口参数 ：目标频率，单位Hz，范围0~420000000
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9910_FreWrite(ulong Freq)
{
    ulong Temp;
    Temp = (ulong)Freq * 4.294967296;	 //将输入频率因子分为四个字节  主频1GHz，32位相位累加器，故每Hz在的控制字增量 delta =  4.294967296 = (2^32)/1000000000
    profile11[7] = (uchar)Temp;
    profile11[6] = (uchar)(Temp >> 8);
    profile11[5] = (uchar)(Temp >> 16);
    profile11[4] = (uchar)(Temp >> 24);
    Txfrc();
}

/************************************************************
** 函数名称 ：void AD9910_AmpWrite(void))
** 函数功能 ：将幅度控制数据保存到profile11并写入芯片
** 入口参数 ：幅度控制字，范围0~16383
** 出口参数 ：无
** 函数说明 ：14位幅度控制字，控制数据0~16383对应输出幅度0~800mV左右
**************************************************************/
void AD9910_AmpWrite(uint16_t Amp)
{
    profile11[0] = (Amp % 16384) >> 8;
    profile11[1] = (Amp % 16384) & 0xff;
    Txfrc();
}

