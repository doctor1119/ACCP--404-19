﻿/*
 * main.c
 *
 * Created: 5/16/2023 11:13:39 PM
 *  Author: Дима
 */ 

#include <xc.h>
#include <util/delay.h>

int main(void)
{
	DDRD |= (1<<5);
	//int arr[139] = {252,364,396,424,454,482,514,598,630,658,690,776,806,834,866,892,924,952,984,1070,1100,1186,1218,1244,1276,1304,1336,1366,1396,1480,1512,1598,1628,1656,1688,1716,1748,1776,1808,1834,1866,1894,1926,1954,1986,2014,2046,2072,2104,2132,2164,2194,2224,2252,2284,2310,2342,2370,2402,2486,2518,2546,2578,2608,2638,2666,2698,2724,2756,2784,2816,2844,2876,2904,2936,3022,3052,3080,3112,3138,3170,3198,3230,3258,3290,3318,3350,3436,3466,3550,3582,3668,3698,3784,3816,3900,3930,4016,4048,4076,4106,4134,4166,4194,4226,4310,4342,4372,4402,4430,4462,4488,4520,4548,4580,4608,4640,4668,4700,4726,4758,4786,4818,4904,4934,5018,5050,5080,5110,5138,5170,5196,5228,5256,5288,5316,5348,5376,5410};
	int arr[3] = {252,364,396};
	int i = 0;
	//PORTD |= (1 << 5);
	
	
   while(1){
	   for(int j = 0; j < 3; j++)
	{
		if(j == 0)
		{
			i = abs(0 - arr[j]);
		}
		else
		{
			i = abs(arr[j-1] - arr[j]);
		}
		
		if(j%2 == 0)
		{
			PORTD |= (1 << 5);
			for(int k = 0; k < i/2; k++)
			{
				_delay_us(13.26); //2600
			}
		}
		else
		{
			PORTD &= ~(1 << 5);
			for(int k = 0; k < i; k++)
			{
				_delay_us(7.28); //1400
			}
		}
			
	}
		PORTD &= ~(1 << 5);
		_delay_ms(5000);
		};
}