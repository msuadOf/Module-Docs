#ifndef STM32_API_H
#define STM32_API_H

#include "gpio.h"

/**********************************************************
                    位带操作相关宏定义
              参考《CM3权威指南》第87 ~ 92页
**********************************************************/

#define BITBAND(addr, bitnum)			((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)					*((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)			MEM_ADDR(BITBAND(addr, bitnum)) 

/**********************************************************
                       GPIO地址映射
              基地址加上寄存器偏移地址组成
**********************************************************/

#define GPIOA_ODR_Addr    GPIOA->ODR    //    (GPIOA_BASE+12)	//0x4001080C 
#define GPIOB_ODR_Addr    GPIOB->ODR    //    (GPIOB_BASE+12)	//0x40010C0C 
#define GPIOC_ODR_Addr    GPIOC->ODR    //    (GPIOC_BASE+12)	//0x4001100C 
#define GPIOD_ODR_Addr    GPIOD->ODR    //    (GPIOD_BASE+12)	//0x4001140C 
#define GPIOE_ODR_Addr    GPIOE->ODR    //    (GPIOE_BASE+12)	//0x4001180C 
#define GPIOF_ODR_Addr    GPIOF->ODR    //    (GPIOF_BASE+12)	//0x40011A0C    
#define GPIOG_ODR_Addr    GPIOG->ODR    //    (GPIOG_BASE+12)	//0x40011E0C    

#define GPIOA_IDR_Addr    GPIOA->IDR    //    (GPIOA_BASE+8)	//0x40010808 
#define GPIOB_IDR_Addr    GPIOB->IDR    //    (GPIOB_BASE+8)	//0x40010C08 
#define GPIOC_IDR_Addr    GPIOC->IDR    //    (GPIOC_BASE+8)	//0x40011008 
#define GPIOD_IDR_Addr    GPIOD->IDR    //    (GPIOD_BASE+8)	//0x40011408 
#define GPIOE_IDR_Addr    GPIOE->IDR    //    (GPIOE_BASE+8)	//0x40011808 
#define GPIOF_IDR_Addr    GPIOF->IDR    //    (GPIOF_BASE+8)	//0x40011A08 
#define GPIOG_IDR_Addr    GPIOG->IDR    //    (GPIOG_BASE+8)	//0x40011E08 

/**********************************************************
             实现单一IO操作，类似于51的IO操作
                   n值要小于IO具体数目
**********************************************************/ 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


#endif // !STM32_API_H
