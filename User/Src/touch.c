#include "touch.h"


TouchStructure touchInfo;	//�ṹ������


void IIC_Touch_Delay(uint16_t a)
{
	int i;
	while (a --)
	{
		for (i = 0; i < 5; i++);
	}
}
//	��������ʼ��IIC��GPIO��
//
//PB2	:SDA
//PC13:SCL

void IIC_Touch_GPIO_Config (void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PG3 PG7 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}
#define LIB 1
#if LIB
//	����������IIC�����ݽ�Ϊ���ģʽ
//
void IIC_Touch_SDA_Out(void)
{
//	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_2);
	GPIO_InitTypeDef GPIO_InitStruct;
  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

//	����������IIC�����ݽ�Ϊ����ģʽ
//
void IIC_Touch_SDA_In(void)
{
//	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_2);
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/*
GPIOB->MODER&=~(3<<(2*2));
GPIOB->MODER|=1<<2*2;//�?3?
GPIOB->MODER|=0<<2*2;//�?�?
*/
#else
void IIC_Touch_SDA_Out(void)
{
GPIOB->MODER&=~(3<<(2*2));
GPIOB->MODER|=1<<2*2;//�?3?
}

//	o��y?o????IIC�?�y?Y???a�?�???�?
//
void IIC_Touch_SDA_In(void)
{
GPIOB->MODER&=~(3<<(2*2));
GPIOB->MODER|=0<<2*2;//�?�?
}

#endif
// IO�ڲ���
/*#define SCL(a) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, a)

#define SDA(a) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, a)
*/
void SCL(uint8_t mode)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, mode);
}

void SDA(uint8_t mode)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, mode);
}

//	������IIC��ʼ�ź�
//
void IIC_Touch_Start(void)
{
	IIC_Touch_SDA_Out();
	
	SDA(1);
	SCL(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	
	SDA(0);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(0);
	IIC_Touch_Delay( Touch_DelayVaule );
}

//	������IICֹͣ�ź�
//
void IIC_Touch_Stop(void)
{
    SCL(0);
    IIC_Touch_Delay( Touch_DelayVaule );
    SDA(0);
    IIC_Touch_Delay( Touch_DelayVaule );
    SCL(1);
    IIC_Touch_Delay( Touch_DelayVaule );
    SDA(1);
    IIC_Touch_Delay( Touch_DelayVaule );
}

//	������IICӦ���ź�
//
void IIC_Touch_Response(void)
{
	IIC_Touch_SDA_Out();

	SDA(0);
	IIC_Touch_Delay( Touch_DelayVaule );	
	SCL(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(0);
	IIC_Touch_Delay( Touch_DelayVaule );
}

//	������IIC��Ӧ���ź�
//
void IIC_Touch_NoResponse(void)
{
	IIC_Touch_SDA_Out();
	
	SCL(0);	
	IIC_Touch_Delay( Touch_DelayVaule );
	SDA(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(0);
	IIC_Touch_Delay( Touch_DelayVaule );
}

//	�������ȴ��豸������Ӧ�ͺ�
//
uint8_t IIC_Touch_WaitResponse(void)
{

	SCL(0);
	IIC_Touch_Delay( Touch_DelayVaule );
	SDA(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(1);

	IIC_Touch_SDA_In();	//����Ϊ����ģʽ
	IIC_Touch_Delay( Touch_DelayVaule );
	
	SCL(0);	
	if( HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) != 0) //�ж��豸�Ƿ���������Ӧ
	{		
		return (Response_ERR);
	}
	else
	{
		return (Response_OK);
	}

}

// ������IICд�ֽ�
//	������IIC_Data - Ҫд���8λ����
//	���أ��豸����Ӧ�򷵻� 1������Ϊ0
//
uint8_t IIC_Touch_WriteByte(uint8_t IIC_Data)
{
	uint8_t i;

	IIC_Touch_SDA_Out(); //���ݽ�Ϊ���ģʽ
	
	for (i = 0; i < 8; i++)
	{
		SDA(IIC_Data & 0x80);
		
		IIC_Touch_Delay( Touch_DelayVaule );
		SCL(1);
		IIC_Touch_Delay( Touch_DelayVaule );
		SCL(0);		
		
		IIC_Data <<= 1;
	}

	return (IIC_Touch_WaitResponse()); //�ȴ��豸��Ӧ
}

// ������IIC���ֽ�
//	������ResponseMode - Ӧ��ģʽѡ��
//       ResponseMode = 1 ʱ��CPU������Ӧ�źţ�Ϊ 0 ʱ��CPU������Ӧ���ź�
//	���أ�����������
//
uint8_t IIC_Touch_ReadByte(uint8_t ResponseMode)
{
	uint8_t IIC_Data;
	uint8_t i;
	
	SDA(1);
	SCL(0);

	IIC_Touch_SDA_In(); //����ģʽ
	
	//��һ�ֽ�����
	for (i = 0; i < 8; i++)
	{
		IIC_Data <<= 1;
		
		SCL(1);
		IIC_Touch_Delay( Touch_DelayVaule );

		IIC_Data |= (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) & 0x01);
		
		SCL(0);
		IIC_Touch_Delay( Touch_DelayVaule );
	}

	//	������Ӧ�ź�
	if (ResponseMode)
	{
		IIC_Touch_Response();
	}
	else
	{
		IIC_Touch_NoResponse();
	}
	
	return (IIC_Data); 
}



/*---------------------- GT9XX��غ��� ------------------------*/

// ������GT9XX д����
//	������addr - Ҫ�����ļĴ���
//			
uint8_t GT9XX_WriteHandle (uint16_t addr)
{
	uint8_t status;

	IIC_Touch_Start();
	if( IIC_Touch_WriteByte(GT9XX_IIC_WADDR) == Response_OK ) //д����ָ��
	{
		if( IIC_Touch_WriteByte((uint8_t)(addr >> 8)) == Response_OK ) //д��16��ַ
		{
			if( IIC_Touch_WriteByte((uint8_t)(addr)) != Response_OK )
			{
				status = ERROR;
			}			
		}
	}
	status = SUCCESS;
	return status;	
}

// ������GT9XX д����
//	������addr - Ҫд���ݵĵ�ַ
//			value - д�������
//
uint8_t GT9XX_WriteData (uint16_t addr,uint8_t value)
{
	uint8_t status;
	
	IIC_Touch_Start(); //����IICͨѶ

	if( GT9XX_WriteHandle(addr) == SUCCESS)	//д��Ҫ�����ļĴ���
	{
		if (IIC_Touch_WriteByte(value) != Response_OK) //д����
		{
			status = ERROR;						
		}
	}
	
	IIC_Touch_Stop(); //ֹͣͨѶ
	
	status = SUCCESS;
	return status;
}

// ������GT9XX ������
//	������addr - Ҫ�����ݵĵ�ַ
//			num - �������ֽ���
//			*value - ���ڻ�ȡ�洢���ݵ��׵�ַ
//
uint8_t GT9XX_ReadData (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status;
	uint8_t i;

	status = ERROR;
	IIC_Touch_Start();

	if( GT9XX_WriteHandle(addr) == SUCCESS) //д��Ҫ�����ļĴ���
	{
		IIC_Touch_Start(); //��������IICͨѶ

		if (IIC_Touch_WriteByte(GT9XX_IIC_RADDR) == Response_OK)
		{	
			for(i = 0 ; i < cnt; i++)
			{
				if (i == (cnt - 1))
				{
					value[i] = IIC_Touch_ReadByte(0);//�������һ������ʱ���� ��Ӧ���ź�
				}
				else
				{
					value[i] = IIC_Touch_ReadByte(1);
				}

			}					
			IIC_Touch_Stop();
			status = SUCCESS;
		}
	}
	
	IIC_Touch_Stop();
	return (status);	
}

// ����: ��������ʼ��
//
void Touch_Init(void)
{
//	IIC_Touch_GPIO_Config(); //��ʼ������ͨ�ŵ�IIC����
	GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PG3 PG7 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// ����������ɨ��
//	˵�����ڳ����������Եĵ��øú��������Լ�ⴥ������
//
uint8_t	Touch_Scan(void)
{
 	uint8_t  touchData[2 + 8 * TOUCH_MAX ]; //���ڴ洢��������
	memset(touchData,0,10);
	GT9XX_ReadData (GT9XX_READ_ADDR,2 + 8 * TOUCH_MAX ,touchData);	//������
	GT9XX_WriteData (GT9XX_READ_ADDR,0);	//	�������оƬ�ļĴ�����־λ
	touchInfo.num = touchData[0] & 0x0f;	//ȡ��ǰ�Ĵ�������
	
	if ( (touchInfo.num ) ) //���������� 1-5 ֮��ʱ>= 1) && (touchInfo.num <=5
	{
		// ȡ��Ӧ�Ĵ�������
	if(touchData[5]<200)
						touchInfo.y[0] = ((touchData[5]<<8) | touchData[4])*0.81+1;

		if(touchData[3]<200)
				touchInfo.x[0] = ((touchData[3]<<8) | touchData[2])*0.78+1;	

		return	SUCCESS ;	
	}

	else                       
	{	
		
		touchInfo.x[0] = 0;
		touchInfo.y[0] = 0;

		return	ERROR ;		
	}
	
}

void	GUI_TouchScan(void)
{
	GUI_PID_STATE State;
	Touch_Scan(); //����ɨ��
	State.x = touchInfo.x[0];
	State.y = touchInfo.y[0];
	if (touchInfo.x[0]) {
	State.Pressed = 1;
	} else {
	State.Pressed = 0;
	}
	GUI_TOUCH_StoreStateEx(&State);
}
