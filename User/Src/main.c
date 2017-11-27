/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dma2d.h"
#include "dma.h"
#include "ltdc.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "crc.h"
#include "fatfs.h"
#include "usb_device.h"

#include "display.h"
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "timers.h"

#include "stdio.h"


/* USER CODE END Includes */
#include "GUIDEMO.h"
#include "touch.h"
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static TaskHandle_t xHandleTaskLed = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskSD = NULL;
static TaskHandle_t xHandleTaskTouch = NULL;
static xTimerHandle TouchScreenTimer = NULL;
static TaskHandle_t xHandleTaskScreen = NULL;
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
  /* write a character to the uart1 and Loop until the end of transmission */
		HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,20);
  return ch;
}

void HAL_Delay(__IO uint32_t Delay)
{
	vTaskDelay(Delay);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
static void vTaskLed(void *pvParameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
//		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_7);
				vTaskDelay(250);
	}
}
//--------------------------------------------------------------
//������ͼ
//--------------------------------------------------------------
FIL file;

void _WriteByte2File(U8 Data, void * p) 
{
	f_putc(Data,p);
}
void ScreenShot()
{	
	char buf[20] = "1:/Screenshot.bmp";
	
	f_open(&file,buf, FA_WRITE|FA_CREATE_ALWAYS);
	GUI_BMP_Serialize(_WriteByte2File,&file);
	f_close(&file);

}
//PI8  ������������   
static void vTaskScreenshot(void *pvParameters)
{
	static uint8_t temp = 1;
	uint32_t Use_Time;
	vTaskDelay(10000);//10���
	while(1)
	{	
		if((HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_8) == 0)&& temp)
		{
		vTaskDelay(20);
			if(HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_8) == 0)
			{
				Use_Time = HAL_GetTick();
				ScreenShot();
				Use_Time = HAL_GetTick() - Use_Time;
				printf("��ͼ�ɹ�! ��ʱ%dms\r\n",Use_Time);
				temp = 0;
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_8))
				temp = 1;
		vTaskDelay(20);
	}
}
//--------------------------------------------------------------
//--------------------------------------------------------------
static void vTaskMsgPro(void *pvParameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_7);
				vTaskDelay(500);
	}
}
//--------------------------------------------------------------
//--------------------------------------------------------------
static void vTimerCallback( xTimerHandle pxTimer )
{
   GUI_TouchScan();
}
static void vTaskTouch(void *pvParameters)
{
	while(1)
	{
		GUI_TouchScan();
		vTaskDelay(30);
	}
}

	FATFS fs[2];
	FIL fil;
uint8_t look = 0;
uint32_t testsram[250] __attribute__((at(0XD0000000)));//����������
uint32_t testsram2[2500] __attribute__((at(0XD0020000)));
	uint8_t ReadBuff[200];
extern void MainTask_U(void);
static void vSD_Task(void *pvParameters)
{
	uint8_t res;
	UINT brw;

	vTaskDelay(1000);
	//SD������
		printf("SD fatfs test\r\n");
	look = f_mount(&fs[0],"0:/",0);
//	scan_files("0:/");
	look =f_open (&fil,"0:/123.txt",FA_OPEN_ALWAYS|FA_WRITE);
	f_puts("fatfs test \r\n�ļ�ϵͳ����",&fil);
	look = f_close(&fil);

	
 	memset(ReadBuff,0,50);
	look = f_open (&fil,"0:/123.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);

	while(1)
	 {
	 res = f_read(&fil,ReadBuff,sizeof(ReadBuff),&brw);
	 if(res||brw==0) break;
	 }
		look = f_close(&fil);

	 printf("SD:%s\r\n",ReadBuff);	
 //----------------------------------------------------------
	printf("spi flash fatfs test\r\n");
	f_mount(&fs[1],"1:/",0);
//	f_open (&fil,"1:/1.txt",FA_OPEN_ALWAYS|FA_WRITE);
//	f_puts("spiflash fatfs test!\r\n�ļ�ϵͳ����",&fil);
//	f_close(&fil);

	f_open (&fil,"1:/1.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
	memset(ReadBuff,0,200);
	while(1)
	 {
	 res = f_read(&fil,ReadBuff,sizeof(ReadBuff),&brw);
	 if(res||brw==0) break;
	 }
	 f_close(&fil);
	 printf("�ļ�����:\r\n%s\r\n",ReadBuff);
	 printf("----------------\r\n");
	 //----------------------------------------

	 printf("SDRAM����\r\n");
	uint32_t i=0;  	  
	 for(i=0;i<2500;i++)
	{
		*(__IO uint16_t*)( 0XD0030000+i*2) = 15 ;
	} 	
	printf ("����250����\r\n");
	 for(i=0;i<250;i++)
	{
		testsram[i]=i;//*(3|3<<16)
	} 	
	printf ("��һ�ζ�ȡ\r\n");
	for(i=0;i<250;i++)
	{
		printf ("%4d",testsram[i]);
	} 
	printf ("\r\n�ڶ��ζ�ȡ\r\n");
	for(i=0;i<250;i++)
	{
		printf ("%4d",testsram[i]);
	} 
		 for(i=0;i<2500;i++)
	{
		testsram2[i]=0x7e0|0x7e0<<16;
	} 	
	HAL_Delay(1000);
	Display_Init();
	Display_Test();
/*	uint32_t temp=0;	   
	uint32_t sval=0;	//�ڵ�ַ0����������		
		for(i=0;i<16*1024*1024;i+=16*1024)
	{
		*(volatile uint32_t*)(Bank5_SDRAM_ADDR+i)=temp; 
		temp++;
	}
	//���ζ���֮ǰд�������,����У��		
  
 	for(i=0;i<16*1024*1024;i+=16*1024) 
	{	
  		temp=*(volatile uint32_t*)(Bank5_SDRAM_ADDR+i);
		if(i==0)sval=temp;
 		else if(temp<=sval)break;//�������������һ��Ҫ�ȵ�һ�ζ��������ݴ�.	 
		printf("SDRAM Capacity:%dKB\r\n",(uint16_t)(temp-sval+1)*16);//��ӡSDRAM����
 	}		*/

	GUIDEMO_Main();
//MainTask_U();
	while(1)
	{
		vTaskDelay(1000);
	}
}
static void AppTaskCreate (void)
{

//	TouchScreenTimer = xTimerCreate ("Timer", 50, pdTRUE, ( void * ) 1, vTimerCallback );
//	if( TouchScreenTimer != NULL )
//  {
//    if( xTimerStart( TouchScreenTimer, 0 ) != pdPASS )
//    {
//      /* The timer could not be set into the Active state. */
//    }
//  }
//	xTaskCreate(vTaskTouch,
//							"vTaskTouch",
//							512,
//							NULL,
//							4,
//							&xHandleTaskTouch);
	xTaskCreate(vTaskLed,
							"vTaskLed",
							512,
							NULL,
							2,
							&xHandleTaskLed);
	
	xTaskCreate(vTaskMsgPro,
							"vTaskMsgPro",
							512,
							NULL,
							3,
							&xHandleTaskMsgPro);
	
	xTaskCreate(vSD_Task,
							"SD_Task",
							1024,
							NULL,
							1,
							&xHandleTaskSD
							);
	xTaskCreate(vTaskScreenshot,
							"vTaskScreenshot",
							512,
							NULL,
							2,
							&xHandleTaskScreen);							
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
			__set_PRIMASK(1);
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
 // MX_DMA_Init();	
	MX_FMC_Init();
  MX_LTDC_Init();
  MX_SDIO_SD_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
	MX_DMA2D_Init();
  MX_CRC_Init();

	MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	MX_USB_DEVICE_Init();
	
	 GUI_Init();
	//Touch_Init();
	AppTaskCreate();
	vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 100;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
