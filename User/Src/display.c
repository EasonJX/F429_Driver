#include "display.h"

struct	
{
	uint32_t Color;  //LCD��ǰ������ɫ
	uint32_t BackColor;	//����ɫ
	uint32_t LayerMemoryAdd;	//���Դ��ַ
	uint8_t  Layer; //��ǰ��
	uint8_t  Direction;	//��ʾ����
	uint8_t  BufferFlag;  //�����ʹ�ñ�־
	uint8_t  ShowNum_Mode;
}LCD;


//	������������
//	������layer - Ҫ���õĲ������ڴ˶�Ϊ0�Ǳ����㣬1��ǰ����
//
void LCD_SetLayer(uint8_t layer)
{
#if LCD_NUM_LAYERS == 2		//�������˫��
	if (layer == 0)
	{
		LCD.LayerMemoryAdd = LCD_MemoryAdd; 	//���ò�0���Դ��ַ
	}
	else if(layer == 1)
	{
		LCD.LayerMemoryAdd = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET;	//���ò�1���Դ��ַ
	}
	LCD.Layer = layer;	//��¼��ǰ���ڵĲ�
#else
	LCD.LayerMemoryAdd = LCD_MemoryAdd;	//������ʾ��״̬�£�ֻ���ò�0�ĵ�ַ
	LCD.Layer = 0;		 //����Ĭ��Ϊ0
#endif

	LCD.BufferFlag = 0;	//�����ʹ�ñ��
}  

// ������������ɫɫ
// ˵��������ɫ������ʾ�ַ������㻭�ߡ�ͼ�ε���ɫ
//
//void LCD_Set_Color(uint32_t Color)
//{
//	uint32_t Display_ColorMode = 0;		//��ǰ�����ɫ��ʽ
//	uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0; //������ɫͨ����ֵ

//#if(LCD_NUM_LAYERS == 2)		//�������˫��
//	if(LCD.Layer == 0)  			
//		Display_ColorMode = ColorMode_0;		//��ȡ��0����ɫ��ʽ
//	else
//		Display_ColorMode = ColorMode_1;		//��ȡ��1����ɫ��ʽ
//#else
//	Display_ColorMode = ColorMode_0;	//������ʾ��Ĭ��Ϊ��0����ɫ��ʽ
//#endif

//	if( Display_ColorMode == LCD_RGB565	)	//��32λɫת��Ϊ16λɫ
//	{
//		Red_Value   = (uint16_t)((Color&0x00F80000)>>8);
//		Green_Value = (uint16_t)((Color&0x0000FC00)>>5);
//		Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);
//		LCD.Color = (uint16_t)(Red_Value | Green_Value | Blue_Value);		
//	}
//	else if( Display_ColorMode == LCD_ARGB1555 )	//��32λɫת��ΪARGB1555��ɫ
//	{
//		if( (Color & 0xFF000000) == 0 )	//�ж��Ƿ�ʹ��͸��ɫ
//			Alpha_Value = 0x0000;
//		else
//			Alpha_Value = 0x8000;

//		Red_Value   = (uint16_t)((Color&0x00F80000)>>9);	
//		Green_Value = (uint16_t)((Color&0x0000F800)>>6);
//		Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);
//		LCD.Color = (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
//	}
//	else
//		LCD.Color = Color;	//24λɫ��32λɫ����Ҫת��
//}

void LCD_Set_Color(uint32_t Color)
{
//		uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0; //������ɫͨ����ֵ
//		Red_Value   = (uint16_t)((Color&0x00F80000)>>8);
//		Green_Value = (uint16_t)((Color&0x0000FC00)>>5);
//		Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);
//		LCD.Color = (uint16_t)(Red_Value | Green_Value | Blue_Value);		
	LCD.Color = Color;
}

// ���������ñ���ɫ
// ˵���������������ʾ�ı�����ɫ������LCD����������ɫ
//
//void LCD_SetBackColor(uint32_t Color)
//{
//	uint32_t Display_ColorMode = 0;		//��ǰ�����ɫ��ʽ
//	uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0;  //������ɫͨ����ֵ

//#if(LCD_NUM_LAYERS == 2)	//�������˫��
//	if(LCD.Layer == 0)  
//		Display_ColorMode = ColorMode_0;	//��ȡ��0����ɫ��ʽ
//	else
//		Display_ColorMode = ColorMode_1;	//��ȡ��1����ɫ��ʽ
//#else
//	Display_ColorMode = ColorMode_0;	//������ʾ��Ĭ��Ϊ��0����ɫ��ʽ
//#endif

//	if( Display_ColorMode == LCD_RGB565	)	//��32λɫת��Ϊ16λɫ
//	{
//		Red_Value   = (uint16_t)((Color&0x00F80000)>>8);
//		Green_Value = (uint16_t)((Color&0x0000FC00)>>5);
//		Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);
//		LCD.BackColor = (uint16_t)(Red_Value | Green_Value | Blue_Value);	
//	}
//	else if( Display_ColorMode == LCD_ARGB1555 )	//��32λɫת��ΪARGB1555��ɫ
//	{
//		if( (Color & 0xFF000000) == 0 )	//�ж��Ƿ�ʹ��͸��ɫ
//			Alpha_Value = 0x0000;
//		else
//			Alpha_Value = 0x8000;

//		Red_Value   = (uint16_t)((Color&0x00F80000)>>9);
//		Green_Value = (uint16_t)((Color&0x0000F800)>>6);
//		Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);
//		LCD.BackColor = (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
//	}
//	else	
//		LCD.BackColor = Color;	//24λɫ��32λɫ����Ҫת��
//}
void LCD_SetBackColor(uint32_t Color)
{
		uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;  //������ɫͨ����ֵ
		Red_Value   = (uint16_t)((Color&0x00F80000)>>8);
		Green_Value = (uint16_t)((Color&0x0000FC00)>>5);
		Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);
		LCD.BackColor = (uint16_t)(Red_Value | Green_Value | Blue_Value);	
}

//__weak void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
//{
//	uint32_t Display_Add = 0;			//��ǰ����Դ��ַ
//	uint32_t Display_ColorMode = 0;	//��ǰ�����ɫ��ʽ

//	/*************** ���ݱ�־λ�л��Դ��ַ ****************/	
//	
//	if(LCD.BufferFlag == 0)	
//		Display_Add = LCD_Buffer;	
//	else
//		Display_Add = LCD.LayerMemoryAdd;	

//	/**************** ѡ����Ӧ�����ɫģʽ **************/
//	
//#if(LCD_NUM_LAYERS == 2)
//		if(LCD.Layer == 0)  //���ݲ�ͬ�Ĳ�ѡ��ͬ����ɫ��ʽ
//			Display_ColorMode = ColorMode_0;
//		else
//			Display_ColorMode = ColorMode_1;
//#else
//		Display_ColorMode = ColorMode_0;
//#endif
//	
//	/****************** 32λɫģʽ ***********************/
//		
//	if( Display_ColorMode == LCD_ARGB8888 ) 
//	{
//		if (LCD.Direction == Mode_H) //ˮƽ����
//		{
//			*(__IO uint32_t*)( Display_Add + 4*(x + y*LCD_Width) ) = color ; 	
//		}
//		else if(LCD.Direction == Mode_V)	//��ֱ����
//		{
//			*(__IO uint32_t*)( Display_Add + 4*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
//		}
//	}
//	
//	/****************** 24λɫģʽ ***********************/	
//	
//	else if ( Display_ColorMode == LCD_RGB888 )
//	{
//		if (LCD.Direction == Mode_H) //ˮƽ����
//		{
//			*(__IO uint16_t*)( Display_Add + 3*(x + y*LCD_Width) ) = color ; 
//			*(__IO  uint8_t*)( Display_Add + 3*(x + y*LCD_Width) + 2 ) = color>>16 ; 	
//		}
//		else if(LCD.Direction == Mode_V)	//��ֱ����
//		{
//			*(__IO uint16_t*)( Display_Add + 3*((LCD_Height - x - 1)*LCD_Width + y) ) = color ; 
//			*(__IO  uint8_t*)( Display_Add + 3*((LCD_Height - x - 1)*LCD_Width + y) +2) = color>>16 ; 	
//		}	
//	}
//	
//	/****************** 16λɫģʽ ***********************/	
//	else		
//	{
//		if (LCD.Direction == Mode_H) //ˮƽ����
//		{
//			*(__IO uint16_t*)( Display_Add + 2*(x + y*LCD_Width) ) = color ; 	
//		}
//		else if(LCD.Direction == Mode_V)	//��ֱ����
//		{
//			*(__IO uint16_t*)( Display_Add + 2*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
//		}	
//	}
//}  

void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
{
	*(__IO uint16_t*)( LCD_MemoryAdd + 2*(x + y*LCD_Width) ) = color ; 
}
//	����������
//	������x-ˮƽ���꣬y-��ֱ����
//	���أ���ȡ����ɫ
// ˵������ʹ��16��24λɫģʽʱ������������ɫ������16λ��24λ��
//
//__weak uint32_t LCD_ReadPoint(uint16_t x,uint16_t y)
//{
//	uint32_t color = 0;
//	uint32_t Display_Add = 0;		//��ǰ����Դ��ַ
//	uint32_t Display_ColorMode = 0;	//��ǰ�����ɫ��ʽ

//	/*************** ���ݱ�־λ�л��Դ��ַ ****************/	

//	if(LCD.BufferFlag == 0)	
//		Display_Add = LCD_Buffer;	
//	else
//		Display_Add = LCD.LayerMemoryAdd;	

//	/**************** ѡ����Ӧ�����ɫģʽ **************/

//#if(LCD_NUM_LAYERS == 2)
//	if(LCD.Layer == 0)  //���ݲ�ͬ�Ĳ�ѡ��ͬ����ɫ��ʽ
//		Display_ColorMode = ColorMode_0;
//	else
//		Display_ColorMode = ColorMode_1;
//#else
//	Display_ColorMode = ColorMode_0;
//#endif

//	/****************** 32λɫģʽ ***********************/
//	if( Display_ColorMode == LCD_ARGB8888 ) 
//	{
//		if (LCD.Direction == Mode_H) //ˮƽ����
//		{
//			color = *(__IO uint32_t*)( Display_Add + 4*(x + y*LCD_Width) ); 	
//		}
//		else if(LCD.Direction == Mode_V)	//��ֱ����
//		{
//			color = *(__IO uint32_t*)( Display_Add + 4*((LCD_Height - x - 1)*LCD_Width + y) );
//		}
//	}
//	/****************** 24λɫģʽ ***********************/	
//	else if ( Display_ColorMode == LCD_RGB888 )
//	{
//		if (LCD.Direction == Mode_H) //ˮƽ����
//		{
//			color = *(__IO uint32_t*)( Display_Add + 3*(x + y*LCD_Width) ) &0x00ffffff; 	
//		}
//		else if(LCD.Direction == Mode_V)	//��ֱ����
//		{
//			color = *(__IO uint32_t*)( Display_Add + 3*((LCD_Height - x - 1)*LCD_Width + y) ) &0x00ffffff; 	
//		}	
//	}
//	/****************** 16λɫģʽ ***********************/	
//	else		
//	{
//		if (LCD.Direction == Mode_H) //ˮƽ����
//		{
//			color = *(__IO uint32_t*)( Display_Add + 2*(x + y*LCD_Width) ); 	
//		}
//		else if(LCD.Direction == Mode_V)	//��ֱ����
//		{
//			color = *(__IO uint32_t*)( Display_Add + 2*((LCD_Height - x - 1)*LCD_Width + y) );
//		}	
//	}
//	return color;
//}  
uint32_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
	return *(__IO uint32_t*)( LCD_MemoryAdd + 2*(x + y*LCD_Width) ); 
}

#define ABS(X)  ((X) > 0 ? (X) : -(X))    

//	����������
//	������x1��y1Ϊ������꣬x2��y2Ϊ�յ�����
//
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;

	deltax = ABS(x2 - x1);        /* The difference between the x's */
	deltay = ABS(y2 - y1);        /* The difference between the y's */
	x = x1;                       /* Start x off at the first pixel */
	y = y1;                       /* Start y off at the first pixel */

	if (x2 >= x1)                 /* The x-values are increasing */
	{
	 xinc1 = 1;
	 xinc2 = 1;
	}
	else                          /* The x-values are decreasing */
	{
	 xinc1 = -1;
	 xinc2 = -1;
	}

	if (y2 >= y1)                 /* The y-values are increasing */
	{
	 yinc1 = 1;
	 yinc2 = 1;
	}
	else                          /* The y-values are decreasing */
	{
	 yinc1 = -1;
	 yinc2 = -1;
	}

	if (deltax >= deltay)         /* There is at least one x-value for every y-value */
	{
	 xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
	 yinc2 = 0;                  /* Don't change the y for every iteration */
	 den = deltax;
	 num = deltax / 2;
	 numadd = deltay;
	 numpixels = deltax;         /* There are more x-values than y-values */
	}
	else                          /* There is at least one y-value for every x-value */
	{
	 xinc2 = 0;                  /* Don't change the x for every iteration */
	 yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
	 den = deltay;
	 num = deltay / 2;
	 numadd = deltax;
	 numpixels = deltay;         /* There are more y-values than x-values */
	}
	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
	 LCD_DrawPoint(x,y,LCD.Color);             /* Draw the current pixel */
	 num += numadd;              /* Increase the numerator by the top of the fraction */
	 if (num >= den)             /* Check if numerator >= denominator */
	 {
		num -= den;               /* Calculate the new numerator value */
		x += xinc1;               /* Change the x as appropriate */
		y += yinc1;               /* Change the y as appropriate */
	 }
	 x += xinc2;                 /* Change the x as appropriate */
	 y += yinc2;                 /* Change the y as appropriate */
	}

}
//	������������
//	������x��yΪ������꣬width-���εĳ��ȣ�height-���
//
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	/* draw horizontal lines */
	LCD_DrawLine(x, y, x+width, y);
	LCD_DrawLine(x, y+height, x+width, y+height);

	/* draw vertical lines */
	LCD_DrawLine(x, y, x, y+height);
	LCD_DrawLine(x+width, y, x+width, y+height);
}
//	��������Բ
//	������x��yΪ������꣬r-�뾶
//	˵�����뾶���ܴ���xy�����ֵ
//
void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r)
{
	int Xadd = -r, Yadd = 0, err = 2-2*r, e2;
	do {   

		LCD_DrawPoint(x-Xadd,y+Yadd,LCD.Color);
		LCD_DrawPoint(x+Xadd,y+Yadd,LCD.Color);
		LCD_DrawPoint(x+Xadd,y-Yadd,LCD.Color);
		LCD_DrawPoint(x-Xadd,y-Yadd,LCD.Color);
		
		e2 = err;
		if (e2 <= Yadd) {
			err += ++Yadd*2+1;
			if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
		}
		if (e2 > Xadd) err += ++Xadd*2+1;
    }
    while (Xadd <= 0);
    
}
//	����������Բ
//	������x��yΪ������꣬r1��r2�ֱ�Ϊˮƽ�ʹ�ֱ����ĳ���
//
void LCD_DrawEllipse(int x, int y, int r1, int r2)
{
  int Xadd = -r1, Yadd = 0, err = 2-2*r1, e2;
  float K = 0, rad1 = 0, rad2 = 0;
   
  rad1 = r1;
  rad2 = r2;
  
  if (r1 > r2)
  { 
    do {
      K = (float)(rad1/rad2);
		 
		LCD_DrawPoint(x-Xadd,y+(uint16_t)(Yadd/K),LCD.Color);
		LCD_DrawPoint(x+Xadd,y+(uint16_t)(Yadd/K),LCD.Color);
		LCD_DrawPoint(x+Xadd,y-(uint16_t)(Yadd/K),LCD.Color);
		LCD_DrawPoint(x-Xadd,y-(uint16_t)(Yadd/K),LCD.Color);     
		 
      e2 = err;
      if (e2 <= Yadd) {
        err += ++Yadd*2+1;
        if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
      }
      if (e2 > Xadd) err += ++Xadd*2+1;
    }
    while (Xadd <= 0);
  }
  else
  {
    Yadd = -r2; 
    Xadd = 0;
    do { 
      K = (float)(rad2/rad1);

		LCD_DrawPoint(x-(uint16_t)(Xadd/K),y+Yadd,LCD.Color);
		LCD_DrawPoint(x+(uint16_t)(Xadd/K),y+Yadd,LCD.Color);
		LCD_DrawPoint(x+(uint16_t)(Xadd/K),y-Yadd,LCD.Color);
		LCD_DrawPoint(x-(uint16_t)(Xadd/K),y-Yadd,LCD.Color);  
		 
      e2 = err;
      if (e2 <= Xadd) {
        err += ++Xadd*3+1;
        if (-Yadd == Xadd && e2 <= Yadd) e2 = 0;
      }
      if (e2 > Yadd) err += ++Yadd*3+1;     
    }
    while (Yadd <= 0);
  }
}
//	���������ʵ��Բ
//	������x��yΪ������꣬r - �뾶
// ˵�����뾶Ҫ����y����
//
void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r)
{
  int32_t  D;    /* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (r << 1);
  
  CurX = 0;
  CurY = r;
  
  while (CurX <= CurY)
  {
    if(CurY > 0) 
    { 
		LCD_DrawLine(x - CurX, y - CurY,x - CurX,y - CurY + 2*CurY);
		LCD_DrawLine(x + CurX, y - CurY,x + CurX,y - CurY + 2*CurY); 
    }
    
    if(CurX > 0) 
    {
		LCD_DrawLine(x - CurY, y - CurX,x - CurY,y - CurX + 2*CurX);
		LCD_DrawLine(x + CurY, y - CurX,x + CurY,y - CurX + 2*CurX); 		 
    }
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
  
  LCD_DrawCircle(x, y, r);  
}
void Display_Init(void)
{	
	uint32_t i;
	for (i = 0;i < 2*800*480;i+=4)
	*(__IO uint32_t*)( LCD_MemoryAdd +i) = 0;
	
}
void Display_Test(void)
{
	uint32_t i;
	
		 for(i=0;i<2500;i++)
	{
		*(__IO uint16_t*)( 0XD0030000+i*2) = 0xf800 ;
	} 
//	HAL_Delay(2000);
//	LCD_DrawPoint(200,201,0x7e0);
//	LCD_DrawPoint(200,203,0x7e0);		
//	HAL_Delay(2000);LCD_DrawPoint(200,200,LCD_GREEN);
 	LCD_Set_Color(0x7e0);

	LCD_Set_Color(0x1f);
	for (i = 0;i<800;i++)
	{
LCD_DrawPoint(i,50,0x7e0);HAL_Delay(3);	
	}
		for (i = 0;i<800;i++)
	{
LCD_DrawPoint(800-i,60,0x1f);HAL_Delay(3);	
	}
	HAL_Delay(2000);
//	LCD_FillCircle(120,100,80);
//	LCD_Set_Color(LCD_RED);    
//	HAL_Delay(2000);
//	LCD_Set_Color(LCD_GREEN);  LCD_FillCircle(170,100,80); 
//	LCD_Set_Color(LCD_BLUE);   LCD_FillCircle(220,100,80);  	
	
}





