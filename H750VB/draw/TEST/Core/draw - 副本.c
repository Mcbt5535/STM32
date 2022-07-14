#include "draw.h"
#include "tim.h"
#include "gpio.h"

/*
�����������д�ӡ

*/



//��Ǳʵ���һ��״̬
char lastpen = 2;

//�����ú���
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

//����±�
//ctrl = 1:���
//     = 0:�±�
void pen(char ctrl)
{
	Z_DIR(ctrl);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_Delay(300);
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
}

//��һ��
//len	:	����,��λΪ����
//dir = 0 ����
//dir = 1 �ҵ���
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

//��һ��
//len	:	����,��λΪ����
//dir = 0 �ϵ���
//dir = 1 �µ���
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

//��λͼ
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
	��һ��ͼ��ͼƬΪ320*240��λͼ��
	���淽ʽΪchar���飬��9600��Ԫ�أ�ÿ��Ԫ��8λ
	imgΪ���������
*/
void drawbitimg(unsigned char *img)
{
//	����
	unsigned char bit = 0b10000000;
	unsigned char temp = 0;
	unsigned char flg = 1;		//��Ϊ1���ǿ��У�Ϊ0���ǿ���
	unsigned char flg2 = 1;		//Ϊ1��ı��ˢ״̬
	unsigned int count = 1;		//ͳ�����Ƶ�����
	unsigned int count2 = 1;  //ͳ�ƺ��Ƶĳ���	
//	ÿһ��
	for(int i = 0; i < CANVAS_H; i++)
	{
		flg = 1;

//	�ж������ǲ��ǿ��У�����������
		for(int j = 0; j < CANVAS_L/8; j++)
		{
//		���ǿ���
			if(img[i*CANVAS_L/8+j]!=0)
			{
				flg = 0;
				break;
			}
		}
//		�ǿ�������������ѭ��
		if(flg)
		{
			count++;
			continue;
		}
		
		pen(1);
//	����n��
		drawcol(1,count);
		count = 1;
		pen(0);
		
//	ÿһ��	
		for(int j = 0; j < CANVAS_L/8; j++)
		{	
//		ÿһλ
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
		���ƻ���һ��
		����ԡ��ӳٵ�������һ��ƫ����
		��˴˴��ֶ�У׼
		SCALEΪ8,CANVAS_LΪ320ʱ Լ��88
*/
		pen(1);
	  drawrow(1,CANVAS_L+3);	
		pen(0);		
	}
}

//���ƻҶ�ͼ
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