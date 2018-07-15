#ifndef _TOUCH_H
#define _TOUCH_H

#include "stm32f4xx_hal.h"
#include "GUI.h"


#define Response_OK 1  //IIC��Ӧ
#define Response_ERR 0

#define Touch_DelayVaule 4  //ͨѶ��ʱʱ��

// IO����غ�
#define IIC_Touch_SCL_CLK     RCC_AHB1Periph_GPIOC
#define IIC_Touch_SCL_PORT    GPIOC                 
#define IIC_Touch_SCL_PIN     GPIO_Pin_13   
         
#define IIC_Touch_SDA_CLK     RCC_AHB1Periph_GPIOB 
#define IIC_Touch_SDA_PORT    GPIOB                   
#define IIC_Touch_SDA_PIN     GPIO_Pin_2          


	
/*---------------------- GT9XXоƬ��ض��� ------------------------*/
					
#define TOUCH_MAX   1	//��������� 5

typedef struct 
{
	uint8_t  flag;	//������־λ��Ϊ1ʱ��ʾ�д�������
	uint8_t  num;		//��������
	uint16_t x[TOUCH_MAX];	//x����
	uint16_t y[TOUCH_MAX];	//y����
}TouchStructure;


#define GT9XX_CFG_ADDR 	0x8047		// �̼�������Ϣ�Ĵ�����������ʼ��ַ

#define GT9XX_IIC_RADDR 0xBB	//IIC��ʼ����ַ
#define GT9XX_IIC_WADDR 0xBA

#define GT9XX_READ_ADDR 0x814E	//������Ϣ�Ĵ���
#define GT9XX_ID_ADDR 0x8140		//�������ID�Ĵ���

void Touch_Init(void);
uint8_t Touch43_Init(void);
uint8_t Touch_Scan(void);
void	GUI_TouchScan(void);


#endif
