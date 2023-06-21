#ifndef __AD9910_H__
#define __AD9910_H__

#include "STM32_API.h"

#define uchar unsigned char
#define uint  unsigned int	
#define ulong  unsigned long int

#define AD9910_PWR	 	PCout(13)//in
#define AD9910_SDIO	 	PAout(5)//inout
#define DRHOLD		 		PCout(1)//in
#define DROVER		 		PCout(2)//out
#define UP_DAT		 		PCout(3)//in
#define PROFILE1	 		PCout(10)//in
#define MAS_REST	 		PAout(6)//in
#define SCLK		 			PAout(2)//in
#define DRCTL		 			PAout(4)//in
#define OSK			 			PCout(8)//in
#define PROFILE0	 		PCout(4)//in
#define PROFILE2	 		PCout(5)//in
#define CS			 			PBout(10)//in

void AD9110_IOInit(void);
void Init_AD9910(void);
void AD9910_FreWrite(ulong Freq);										//Ð´ÆµÂÊ
void AD9910_AmpWrite(uint16_t Amp);

#endif


