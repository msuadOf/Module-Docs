#ifndef __AD9910_H__
#define __AD9910_H__

#include "STM32_API.h"
#include "stdint.h"

#define uchar unsigned char
#define uint  unsigned int	
#define ulong  unsigned long int

#define AD9910_PWR	 PCout(6)//	PCout(13)//in
#define AD9910_SDIO	 PAout(8)//	PAout(5)//inout
#define DRHOLD		 PAout(9)//		PCout(1)//in
#define DROVER		 PAout(10)//		PCout(2)//out
#define UP_DAT		 PAout(15)//		PCout(3)//in
#define PROFILE1	 PCout(10)//		PCout(10)//in
#define MAS_REST	 PCout(11)//		PAout(6)//in
#define SCLK		 PBout(3)//			PAout(2)//in
#define DRCTL		 PBout(4)//			PAout(4)//in
#define OSK			 PBout(5)//			PCout(8)//in
#define PROFILE0	 PBout(6)//		PCout(4)//in
#define PROFILE2	 PBout(7)//		PCout(5)//in
#define CS			 PBout(9)//			PBout(10)//in

void AD9110_IOInit(void);
void Init_AD9910(void);
void AD9910_FreWrite(ulong Freq);										//写频锟斤拷
void AD9910_AmpWrite(uint16_t Amp);

#endif


