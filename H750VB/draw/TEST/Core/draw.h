#ifndef __DRAW_H__
#define __DRAW_H__



//方向
#define X_DIR(n) n?HAL_GPIO_WritePin(X_DIR_GPIO_Port,X_DIR_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(X_DIR_GPIO_Port,X_DIR_Pin,GPIO_PIN_RESET)
#define Y_DIR(n) n?HAL_GPIO_WritePin(Y_DIR_GPIO_Port,Y_DIR_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(Y_DIR_GPIO_Port,Y_DIR_Pin,GPIO_PIN_RESET)
#define Z_DIR(n) n?HAL_GPIO_WritePin(Z_DIR_GPIO_Port,Z_DIR_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(Z_DIR_GPIO_Port,Z_DIR_Pin,GPIO_PIN_RESET)

//#define X_STEP(n) n?HAL_GPIO_WritePin(X_STEP_GPIO_Port,X_STEP_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(X_STEP_GPIO_Port,X_STEP_Pin,GPIO_PIN_RESET)
//#define Y_STEP(n) n?HAL_GPIO_WritePin(Y_STEP_GPIO_Port,Y_STEP_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(Y_STEP_GPIO_Port,Y_STEP_Pin,GPIO_PIN_RESET)
//#define Z_STEP(n) n?HAL_GPIO_WritePin(Z_STEP_GPIO_Port,Z_STEP_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(Z_STEP_GPIO_Port,Z_STEP_Pin,GPIO_PIN_RESET)

/*
	***step为100时线长1cm
	
	画面为320*240
	256*192mm的纸
	每个像素为0.8mm
	此时SCALE = 8
*/
#define SCALE 8
#define CANVAS_L 		64
#define CANVAS_H		64

//调试用
void x_move(int dir,int len);
void y_move(int dir,int len);
void z_move(int dir,int len);

void drawrow(int dir,int len);
void drawcol(int dir,int len);

//正式函数
void pen(char ctrl);
void drawbitline(unsigned char *line);
void drawbitpage(unsigned char *page);

void drawline(char *line);
void drawpage(char *page);
void drawbitimg(unsigned char *img);
#endif /*__ DRAW_H__ */