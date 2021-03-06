#include "touch.h"


#include "funopts.h"


TouchStructure touchInfo;	//结构体声明

static void T_Delay(uint32_t a)
{
	int i;
	while (a --)
	{
		for (i = 0; i < 5; i++);
	}
}
#define Touch_INT_CLK     RCC_AHB1Periph_GPIOA			// INT
#define Touch_INT_PORT    GPIOA                   		
#define Touch_INT_PIN     GPIO_Pin_15             		

#define Touch_RST_CLK     RCC_AHB1Periph_GPIOI			// RST
#define Touch_RST_PORT    GPIOI                   		
#define Touch_RST_PIN     GPIO_Pin_11            		

//-----------------------------------------------------------------
//推挽输出 切换方向
//函数：初始化IIC的GPIO口
//PB2	:SDA
//PC13:SCL
// 函数: 触摸屏初始化
//
void Touch_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();  
  
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
}
#define LIB 1
#if LIB
//	函数：配置IIC的数据脚为输出模式

void IIC_Touch_SDA_Out(void)
{
//	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_2);
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

//	函数：配置IIC的数据脚为输入模式
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
GPIOB->MODER|=1<<2*2;//output
}

void IIC_Touch_SDA_In(void)
{
GPIOB->MODER&=~(3<<(2*2));
GPIOB->MODER|=0<<2*2;//input
}
#endif
//-----------------------------------------------------------------
// IO口操作
/*
#define SCL(a) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, a)
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

//	函数：IIC起始信号
void IIC_Touch_Start(void)
{
	//IIC_Touch_SDA_Out();
	SDA(1);
	SCL(1);
	T_Delay( Touch_DelayVaule );
	
	SDA(0);
	T_Delay( Touch_DelayVaule );
	SCL(0);
	T_Delay( Touch_DelayVaule );
}

//	函数：IIC停止信号
void IIC_Touch_Stop(void)
{
    SCL(0);
    T_Delay( Touch_DelayVaule );
    SDA(0);
    T_Delay( Touch_DelayVaule );
    SCL(1);
    T_Delay( Touch_DelayVaule );
    SDA(1);
    T_Delay( Touch_DelayVaule );
}

//	函数：IIC应答信号
void IIC_Touch_Response(void)
{
	//IIC_Touch_SDA_Out();
	SDA(0);
	T_Delay( Touch_DelayVaule );	
	SCL(1);
	T_Delay( Touch_DelayVaule );
	SCL(0);
	T_Delay( Touch_DelayVaule );
}

//	函数：IIC非应答信号
void IIC_Touch_NoResponse(void)
{
//	IIC_Touch_SDA_Out();
	SCL(0);	
	T_Delay( Touch_DelayVaule );
	SDA(1);
	T_Delay( Touch_DelayVaule );
	SCL(1);
	T_Delay( Touch_DelayVaule );
	SCL(0);
	T_Delay( Touch_DelayVaule );
}

//	函数：等待设备发出回应型号
uint8_t IIC_Touch_WaitResponse(void)
{

	SCL(0);
	T_Delay( Touch_DelayVaule );
	SDA(1);
	T_Delay( Touch_DelayVaule );
	SCL(1);
//	IIC_Touch_SDA_In();	//配置为输入模式
	T_Delay( Touch_DelayVaule );
	SCL(0);	
	if( HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) != 0) //判断设备是否有做出响应
	{		
		return (Response_ERR);
	}
	else
	{
		return (Response_OK);
	}
}

// 函数：IIC写字节
//	参数：IIC_Data - 要写入的8位数据
//	返回：设备有响应则返回 1，否则为0
//
uint8_t IIC_Touch_WriteByte(uint8_t IIC_Data)
{
	uint8_t i;
	//IIC_Touch_SDA_Out(); //数据脚为输出模式
	for (i = 0; i < 8; i++)
	{
		SDA(IIC_Data & 0x80);
		T_Delay( Touch_DelayVaule );
		SCL(1);
		T_Delay( Touch_DelayVaule );
		SCL(0);		
		IIC_Data <<= 1;
	}
	return (IIC_Touch_WaitResponse()); //等待设备响应
}

// 函数：IIC读字节
//	参数：ResponseMode - 应答模式选择
//       ResponseMode = 1 时，CPU发出响应信号；为 0 时，CPU发出非应答信号
//	返回：读出的数据
uint8_t IIC_Touch_ReadByte(uint8_t ResponseMode)
{
	uint8_t IIC_Data;
	uint8_t i;
	
	SDA(1);
	SCL(0);
	//IIC_Touch_SDA_In(); //输入模式
	//读一字节数据
	for (i = 0; i < 8; i++)
	{
		IIC_Data <<= 1;
		SCL(1);
		T_Delay( Touch_DelayVaule );
		IIC_Data |= (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) & 0x01);
		SCL(0);
		T_Delay( Touch_DelayVaule );
	}
	//	做出相应信号
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



/*---------------------- GT9XX相关函数 ------------------------*/
#if LCD7
// 函数：GT9XX 写操作
//	参数：addr - 要操作的寄存器
//			
uint8_t GT9XX_WriteHandle (uint16_t addr)
{
	uint8_t status;

	IIC_Touch_Start();
	if( IIC_Touch_WriteByte(GT9XX_IIC_WADDR) == Response_OK ) //写数据指令
	{
		if( IIC_Touch_WriteByte((uint8_t)(addr >> 8)) == Response_OK ) //写入16地址
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

// 函数：GT9XX 写数据
//	参数：addr - 要写数据的地址
//			value - 写入的数据
//
uint8_t GT9XX_WriteData (uint16_t addr,uint8_t value)
{
	uint8_t status;
	
	IIC_Touch_Start(); //启动IIC通讯

	if( GT9XX_WriteHandle(addr) == SUCCESS)	//写入要操作的寄存器
	{
		if (IIC_Touch_WriteByte(value) != Response_OK) //写数据
		{
			status = ERROR;						
		}
	}
	
	IIC_Touch_Stop(); //停止通讯
	
	status = SUCCESS;
	return status;
}

// 函数：GT9XX 读数据
//	参数：addr - 要读数据的地址
//			num - 读出的字节数
//			*value - 用于获取存储数据的首地址
//
uint8_t GT9XX_ReadData (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status;
	uint8_t i;

	status = ERROR;
	IIC_Touch_Start();

	if( GT9XX_WriteHandle(addr) == SUCCESS) //写入要操作的寄存器
	{
		IIC_Touch_Start(); //重新启动IIC通讯

		if (IIC_Touch_WriteByte(GT9XX_IIC_RADDR) == Response_OK)
		{	
			for(i = 0 ; i < cnt; i++)
			{
				if (i == (cnt - 1))
				{
					value[i] = IIC_Touch_ReadByte(0);//读到最后一个数据时发送 非应答信号
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


#else
//4.3LCD
// 触摸参数配置数组，在函数 GT9XX_SendCfg() 里调用，用于配置触摸IC的相关参数
//	由于GT9147可以固化保存这些参数，所以用户一般情况下无需再进行配置
//	详细的寄存器功能请参考《GT9147编程指南》
//
const uint8_t GT9XX_CFG_DATA[] = 	
{                              	 
	0XAA,			// 寄存器地址：0x8047，功能：配置版本号
	
	0XE0,0X01,	// 寄存器地址：0x8048~0x8049，功能：X坐标最大值，低位在前
	0X10,0X01,	// 寄存器地址：0x804A~0x804B，功能：Y坐标最大值，低位在前
	
	0X05,			// 寄存器地址：0x804C，功能：设置最大触摸点数，1~5点
	0X0E,			// 寄存器地址：0x804D，功能：设置INT触发方式、XY坐标交换
	0X00,			// 该寄存器无需配置
	0X88,			// 寄存器地址：0x804F，功能：按下或松开去抖次数
	0X0B,			// 寄存器地址：0x8050，功能：原始坐标窗口滤波值
	0X80,0X08,0X50,0X3C,0X0F,0X00,0X00,0X00,0XFF,0X67,		// 0X8051 ~ 0X805A
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,		// 0X805B ~ 0X8064
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,		// 0X8065 ~ 0X806E
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,		// 0X806F ~ 0X8078
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,		// 0X8079 ~ 0X8082
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,		// 0X8083 ~ 0X808C
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,		// 0X808D ~ 0X8096
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,		// 0X8097 ~ 0X80A0
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,		// 0X80A1 ~ 0X80AA
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,		// 0X80AB ~ 0X80B4
	0X00,0X00,

/******************************************************************************************
*	寄存器地址:	0x80B7~0X80C4
*	功能说明  :	修改感应通道对应的芯片通道号，可以改变触摸面板的垂直扫描方向
*******************************************************************************************/

	0X08,0X0A,0X0C,0X0E,0X10,0X12,0X14,0X16,0X18,0X1A,		// 扫描方向从 上 到 下
//	0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,0X0A,0X08,		// 扫描方向从 下 到 上	
	0X00,0X00,0X00,0X00,												// 未使用的感应通道，无需更改设置
	
/******************************************************************************************/

	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,		// 无作用寄存器，无需配置
	0X00,0X00,0X00,0X00,0x00,0x00, 								// 无作用寄存器，无需配置
	
/*******************************************************************************************
*	寄存器地址:	0x80D5~0X80EE
*	功能说明  :	修改驱动通道对应的芯片通道号，可以改变触摸面板的水平扫描方向
********************************************************************************************/

	0x00,0x02,0x04,0x05,0x06,0x08,0x0a,0x0c, 				// 扫描方向从 左 到 右
	0x0e,0x1d,0x1e,0x1f,0x20,0x22,0x24,0x28,0x29,      

// 0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,				// 扫描方向从 右 到 左
//	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,	
	
	0xff, 	// 未使用的驱动通道，无需更改设置
	
/*******************************************************************************************/

	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,		// 通道调整系数寄存器，无需修改
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,		// 通道调整系数寄存器，无需修改
	0XFF,0XFF,0XFF,0XFF,												// 通道调整系数寄存器，无需修改
};


/*****************************************************************************************
*	函 数 名:	GT9XX_Reset
*	入口参数:	无
*	返 回 值:	无
*	函数功能:	复位GT9147
*	说    明:复位GT9147，并将芯片的IIC地址配置为0xBA/0xBB
******************************************************************************************/

void GT9XX_Reset(void)
{
	//Touch_INT_Out();	//	将INT引脚配置为输出
	
	// 初始化引脚状态
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0); 	// 将INT拉低
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_11,1);	// 将RST拉高
	T_Delay(10000);	
	
	// 开始执行复位
	//	INT引脚保持低电平不变，将器件地址设置为0XBA/0XBB
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_11,0);			// 拉低复位引脚，此时芯片执行复位
	T_Delay(350000);										// 延时
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_11,1);			// 拉高复位引脚，复位结束
	T_Delay(350000);										// 延时
	//Touch_INT_In();													// INT引脚转为浮空输入
	T_Delay(350000);										// 延时
}

/*****************************************************************************************
*	函 数 名:	GT9XX_WriteHandle
*	入口参数:	addr - 要操作的寄存器
*	返 回 值:	SUCCESS - 操作成功
*				ERROR	  - 操作失败
*	函数功能:	GT9XX 写操作
*	说    明:对指定的寄存器执行写操作
******************************************************************************************/

uint8_t GT9XX_WriteHandle (uint16_t addr)
{
	uint8_t status;	// 状态标志位

	IIC_Touch_Start();	// 启动IIC通信
	
	if( IIC_Touch_WriteByte(GT9XX_IIC_WADDR) == 1 ) //写数据指令
	{
		if( IIC_Touch_WriteByte((uint8_t)(addr >> 8)) == 1 ) //写入16位地址
		{
			if( IIC_Touch_WriteByte((uint8_t)(addr)) != 1 )
			{
				status = ERROR;	// 操作失败
			}			
		}
	}
	status = SUCCESS;	// 操作成功
	return status;	
}

/*****************************************************************************************
*	函 数 名:	GT9XX_WriteData
*	入口参数:	addr - 要写入的寄存器
*				value - 要写入的数据
*	返 回 值:	SUCCESS - 操作成功
*				ERROR	  - 操作失败
*	函数功能:	GT9XX 写一字节数据
*	说    明:对指定的寄存器写入一字节数据
******************************************************************************************/

uint8_t GT9XX_WriteData (uint16_t addr,uint8_t value)
{
	uint8_t status;
	
	IIC_Touch_Start(); //启动IIC通讯

	if( GT9XX_WriteHandle(addr) == SUCCESS)	//写入要操作的寄存器
	{
		if (IIC_Touch_WriteByte(value) != 1) //写数据
		{
			status = ERROR;	// 写入失败					
		}
	}	
	IIC_Touch_Stop(); // 停止通讯
	
	status = SUCCESS;	// 写入成功
	return status;
}

/*****************************************************************************************
*	函 数 名:	GT9XX_WriteReg
*	入口参数:	addr - 要写入的寄存器区域首地址
*				cnt  - 数据长度
*				value - 要写入的数据区
*	返 回 值:	SUCCESS - 操作成功
*				ERROR	  - 操作失败
*	函数功能:	GT9XX 写寄存器
*	说    明:往芯片的寄存器区写入指定长度的数据
******************************************************************************************/

uint8_t GT9XX_WriteReg (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status = 0;
	uint8_t i = 0;

	IIC_Touch_Start();	// 启动IIC通信

	if( GT9XX_WriteHandle(addr) == SUCCESS) //写入要操作的寄存器
	{
		for(i = 0 ; i < cnt; i++)		// 计数
		{
			IIC_Touch_WriteByte(value[i]);	// 写入数据
		}					
		IIC_Touch_Stop();		// 停止IIC通信
		status = SUCCESS;		// 写入成功
	}
	else
	{
		IIC_Touch_Stop();		// 停止IIC通信
		status = ERROR;		// 写入失败
	}

	return status;	
}

/*****************************************************************************************
*	函 数 名:	GT9XX_ReadReg
*	入口参数:	addr - 要读取的寄存器区域首地址
*				cnt  - 数据长度
*				value - 要读取的数据区
*	返 回 值:	SUCCESS - 操作成功
*				ERROR	  - 操作失败
*	函数功能:	GT9XX 读寄存器
*	说    明:从芯片的寄存器区读取指定长度的数据
******************************************************************************************/

uint8_t GT9XX_ReadData (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status = 0;
	uint8_t i = 0;

	status = ERROR;
	IIC_Touch_Start();	// 启动IIC通信

	if( GT9XX_WriteHandle(addr) == SUCCESS) 	//	写入要操作的寄存器
	{
		IIC_Touch_Start(); // 重新启动IIC通讯

		if (IIC_Touch_WriteByte(GT9XX_IIC_RADDR) == 1)	// 发送读命令
		{	
			for(i = 0 ; i < cnt; i++)	// 计数
			{
				if (i == (cnt - 1))
				{
					value[i] = IIC_Touch_ReadByte(0);	//	读到最后一个数据时发送 非应答信号
				}
				else
				{
					value[i] = IIC_Touch_ReadByte(1);	// 发送应答信号
				}
			}					
			IIC_Touch_Stop();	// 停止IIC通信
			status = SUCCESS;
		}
	}
	IIC_Touch_Stop();
	return (status);	
}

/*****************************************************************************************
*	函 数 名:	GT9XX_SendCfg
*	入口参数:	无
*	返 回 值:无
*	函数功能:	发送GT9147配置参数
*	说    明:由于GT9147可以掉电保存配置参数，并且出厂时已配置好，一般情况下用户无需更改，
*				用户修改参数之后，需要将该函数屏蔽掉，不然频繁的写入会将触摸芯片的Flash写坏
******************************************************************************************/

void GT9XX_SendCfg(void)
{
	uint8_t GT9XX_Check[2];
	uint8_t i=0;
	
	GT9XX_Check[1] = 1;		// 配置更新标志
	
	for(i=0;i<sizeof(GT9XX_CFG_DATA);i++)
	{
		GT9XX_Check[0] += GT9XX_CFG_DATA[i];	//计算校验和
	}
   GT9XX_Check [0] = (~GT9XX_Check[0])+1;		
	
	GT9XX_WriteReg(0X8047,sizeof(GT9XX_CFG_DATA),(uint8_t*)GT9XX_CFG_DATA);	//	发送寄存器配置
	GT9XX_WriteReg(0X80FF,2,GT9XX_Check); // 写入校验和，并更新配置
} 

/*****************************************************************************************
*	函 数 名:	GT9XX_ReadCfg
*	入口参数:	无
*	返 回 值:无
*	函数功能:	读取GT9147配置参数
*	说    明:通过串口打印数据，在修改数据之前，需要将原厂的参数读取并做好备份，以免改动关键的参数		
******************************************************************************************/

void GT9XX_ReadCfg(void)
{
	uint8_t GT9XX_Cfg[184];	// 数组长度取决于实际的芯片的寄存器个数
	uint16_t i = 0;

	printf("-----------------------------------------\r\n");	
	printf("读取芯片配置信息，串口打印输出\r\n");
	
	GT9XX_ReadData (GT9XX_CFG_ADDR,184,GT9XX_Cfg);	// 读配置信息
	for(i=0;i<184;i++)
	{	
		if( (i%10 == 0) && (i>0) )			
		{
			printf("\r\n");
		}
		printf("0X%.2x,",GT9XX_Cfg[i]);
	}
	printf("\r\n-----------------------------------------\r\n");	
}

/*****************************************************************************************
*	函 数 名: Touch_Init
*	入口参数: 无
*	返 回 值:	 SUCCESS  - 初始化成功
*            ERROR 	 - 错误，未检测到触摸屏	
*	函数功能: 触摸IC初始化，并读取相应信息发送到串口
*	说    明: 在程序里周期性的调用该函数，用以检测触摸操作，触摸信息存储在 touchInfo 结构体
******************************************************************************************/

uint8_t Touch43_Init(void)
{
	uint8_t GT9XX_Info[11];	// 触摸屏IC信息
	uint8_t cfgVersion = 0;	// 触摸配置版本
	
//	Touch_IIC_GPIO_Config(); 	// 初始化IIC引脚
	GT9XX_Reset();					// GT9147 复位
	
//	 //读取GT9147配置参数，通过串口打印输出	
//	//	通过串口打印数据，在修改数据之前，需要将原厂的参数读取并做好备份，以免改动关键的参数
//	GT9XX_ReadCfg();	
	
//	// 发送配置参数，由于GT9147可以掉电保存配置参数，一般情况下用户无需更改
//	//用户修改参数之后，需要将该函数屏蔽掉，不然频繁的写入会将触摸芯片的Flash写坏	
//	GT9XX_SendCfg();	
	
	GT9XX_ReadData (GT9XX_ID_ADDR,11,GT9XX_Info);		// 读触摸屏IC信息
	GT9XX_ReadData (GT9XX_CFG_ADDR,1,&cfgVersion);	// 读触摸配置版本
	
	if( GT9XX_Info[0] == '9' )		//	判断第一个字符是否为 ‘9’
	{
//		printf("Touch ID： GT%.4s \r\n",GT9XX_Info);									// 打印触摸芯片的ID
//		printf("固件版本： 0X%.4x\r\n",(GT9XX_Info[5]<<8) + GT9XX_Info[4]);	// 芯片固件版本
//		printf("触摸分辨率：%d * %d\r\n",(GT9XX_Info[7]<<8) + GT9XX_Info[6],(GT9XX_Info[9]<<8) +GT9XX_Info[8]);	// 当前触摸分辨率		
//		printf("触摸参数配置版本： 0X%.2x \r\n",cfgVersion);	// 触摸配置版本	
		return SUCCESS;
	}
	else
	{
//		printf("未检测到触摸IC\r\n");			//错误，未检测到触摸屏
		return ERROR;
	}
}

/*

void Touch_Scan(void)
{
 	uint8_t  touchData[2 + 8 * TOUCH_MAX ]; //用于存储触摸数据
	uint8_t  i = 0;	
	
	GT9XX_ReadReg (GT9XX_READ_ADDR,2 + 8 * TOUCH_MAX ,touchData);	//读数据
	GT9XX_WriteData (GT9XX_READ_ADDR,0);	//	清除触摸芯片的寄存器标志位
	touchInfo.num = touchData[0] & 0x0f;	// 取当前的触摸点数，若为0则代表无触摸数据发生
	
	if ( (touchInfo.num >= 1) && (touchInfo.num <=5) ) 	//	当触摸数在 1-5 之间时
	{
		for(i=0;i<touchInfo.num;i++)		// 取相应的触摸坐标
		{
			touchInfo.y[i] = (touchData[5+8*i]<<8) | touchData[4+8*i];	// 获取Y坐标
			touchInfo.x[i] = (touchData[3+8*i]<<8) | touchData[2+8*i];	//	获取X坐标			
		}
		touchInfo.flag = 1;	// 触摸标志位置1，代表有触摸动作发生
	}
	else                       
	{
		touchInfo.flag = 0;	// 触摸标志位置0，无触摸动作
	}
}

*/
#endif


// 函数：触摸扫描

uint8_t	Touch_Scan(void)
{
 	uint8_t  touchData[2 + 8 * TOUCH_MAX ]; //用于存储触摸数据
	memset(touchData,0,10);
	GT9XX_ReadData (GT9XX_READ_ADDR,2 + 8 * TOUCH_MAX ,touchData);	//读数据
	GT9XX_WriteData (GT9XX_READ_ADDR,0);	//	清除触摸芯片的寄存器标志位
	touchInfo.num = touchData[0] & 0x0f;	//取当前的触摸点数
	
	if ( (touchInfo.num ) &&touchData[5]<200 && touchData[3]<200 ) //当触摸数在 1-5 之间时>= 1) && (touchInfo.num <=5
	{
		// 取相应的触摸坐标
		#if LCD7	
		touchInfo.y[0] = ((touchData[5]<<8) | touchData[4])*0.81+1;
		touchInfo.x[0] = ((touchData[3]<<8) | touchData[2])*0.78+1;	
		#else
		touchInfo.y[0] = (touchData[5]<<8) | touchData[4];
		touchInfo.x[0] = (touchData[3]<<8) | touchData[2];	
		#endif
		return	SUCCESS ;	
	}
	else                       
	{	
		touchInfo.x[0] = 0;
		touchInfo.y[0] = 0;
		return	ERROR ;		
	}
}






#if TOUCH_DEBUG
void	GUI_TouchScan(void)
{
	Touch_Scan();
	printf("x:%d\ty:%d\r\n",touchInfo.x[0],touchInfo.y[0]);
}
#else
void	GUI_TouchScan(void)
{
	GUI_PID_STATE State;
	Touch_Scan(); //触摸扫描
	State.x = touchInfo.x[0];
	State.y = touchInfo.y[0];
	if (touchInfo.x[0]) {
	State.Pressed = 1;
	} else {
	State.Pressed = 0;
	}
	GUI_TOUCH_StoreStateEx(&State);
}

#endif

