#include "draw.h"
#include "tim.h"
#include "gpio.h"

/*
从左往右逐行打印

*/



//标记笔的上一个状态
char lastpen = 2;

//调试用函数
void x_move(int dir,int len)
{
	int step = len*SCALE;
	X_DIR(dir);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_Delay(step);
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
}
void y_move(int dir,int len)
{
	int step = len*SCALE;
	Y_DIR(dir);
	HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
	HAL_Delay(step);
	HAL_TIM_PWM_Stop(&htim15,TIM_CHANNEL_1);
}
void z_move(int dir,int len)
{
	int step = len*SCALE;
	Z_DIR(dir);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_Delay(step);
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
}

//提笔下笔
//ctrl = 1:提笔
//     = 0:下笔
void pen(char ctrl)
{
	Z_DIR(ctrl);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_Delay(300);
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
}

//划一横
//len	:	长度,单位为像素
//dir = 0 左到右
//dir = 1 右到左
void drawrow(int dir,int len)
{
	int step = len*SCALE;

	X_DIR(dir);
	Y_DIR(dir);
	HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_Delay(step);
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim15,TIM_CHANNEL_1);

}

//划一竖
//len	:	长度,单位为像素
//dir = 0 上到下
//dir = 1 下到上
void drawcol(int dir,int len)
{
	int step = len*SCALE;

	X_DIR(dir);
	Y_DIR(!dir);
	HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_Delay(step);
	HAL_TIM_PWM_Stop(&htim15,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);

}

//画位图
void drawbitline(unsigned char *line)
{
	unsigned char bit = 0b10000000;
	unsigned char temp = 0;
	X_DIR(0);
	Y_DIR(0);
	for(int i = 0; i < CANVAS_L/8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			temp = bit&line[i];
			bit = bit>>1;
			if(bit == 0) bit = 0b10000000;
			if(temp!=lastpen)
			{
				pen(!temp);
				lastpen = temp;
			}		
			HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
			HAL_Delay(SCALE);
			HAL_TIM_PWM_Stop(&htim15,TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
	//		HAL_Delay(50);
		}
	}
}

void drawbitpage(unsigned char *page)
{
	for(int i = 0; i < 5; i++)
	{
		pen(0);
	}
	for(int i = 0; i < CANVAS_H/8; i++)
	{
		drawbitline(&page[i*CANVAS_L/8]);
		pen(1);
		drawcol(0,1);
		drawrow(1,CANVAS_L);
		pen(0);
	}
}
/*
	画一张图，图片为320*240的位图，
	储存方式为char数组，共9600个元素，每个元素8位
	img为输入的数组
*/
void drawbitimg(unsigned char *img)
{
//	变量
	unsigned char bit = 0b10000000;
	unsigned char temp = 0;
	unsigned char flg = 1;		//若为1则是空行，为0则不是空行
	unsigned char flg2 = 1;		//为1则改变笔刷状态
	unsigned int count = 1;		//统计下移的行数
	unsigned int count2 = 1;  //统计横移的长度	
//	每一列
	for(int i = 0; i < CANVAS_H; i++)
	{
		flg = 1;

//	判断这行是不是空行，若是则跳过
		for(int j = 0; j < CANVAS_L/8; j++)
		{
//		不是空行
			if(img[i*CANVAS_L/8+j]!=0)
			{
				flg = 0;
				break;
			}
		}
//		是空行则跳过本次循环
		if(flg)
		{
			count++;
			continue;
		}
		
		pen(1);
//	下移n格
		drawcol(1,count);
		count = 1;
		pen(0);
		
//	每一行	
		for(int j = 0; j < CANVAS_L/8; j++)
		{	
//		每一位
			for(int k = 0; k < 8; k++)
			{
				temp = ((bit&img[i*CANVAS_L/8+j])>0)?1:0;
				bit = bit>>1;
				if(bit == 0) bit = 0b10000000;
				if(temp!=lastpen)
				{
					pen(temp);
					lastpen = temp;
					drawrow(0,count2);
					count2 = 0;
				}		
				count2++;
			}
//			for(int k = 0; k < 8; k++)
//			{
//				temp = bit&img[i*CANVAS_L/8+j];
//				bit = bit>>1;
//				if(bit == 0) bit = 0b10000000;
//				if(temp!=lastpen)
//				{
//					pen(!temp);
//					lastpen = temp;
//				}		
//				HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
//				HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
//				HAL_Delay(SCALE);
//				HAL_TIM_PWM_Stop(&htim15,TIM_CHANNEL_1);
//				HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
//			}
		}
/*
		左移回下一行
		因惯性、延迟等因素有一个偏移量
		因此此处手动校准
		SCALE为8,CANVAS_L为320时 约是88
*/
		pen(1);
	  drawrow(1,CANVAS_L+3);	
		pen(0);		
	}
}

//绘制灰度图
void drawline(char *line)
{
	X_DIR(0);
	Y_DIR(0);
	for(int i = 0; i < CANVAS_L; i++)
	{
		if(line[i]!=lastpen)
		{
			pen(line[i]);
			lastpen = line[i];
		}		
		HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
		HAL_Delay(SCALE);
		HAL_TIM_PWM_Stop(&htim15,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
//		HAL_Delay(50);
	}
}

void drawpage(char *page)
{
	for(int i = 0; i < 5; i++)
	{
		pen(0);
	}
	for(int i = 0; i < CANVAS_H; i++)
	{
		drawline(page+i*CANVAS_L);
		pen(1);
		drawcol(0,1);
		drawrow(1,CANVAS_L);
		pen(0);
	}
}