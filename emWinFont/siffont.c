#include "siffont.h"
#include "ff.h"

//SIF���� ��ת���ڴ���

//���嶨��
GUI_FONT SIF20_Font;
//�����ļ�
FIL SIF20FontFile;

//�ֿ��ļ�������
uint8_t SIF20FontBuff[4*1024*1024]__attribute__((at(0xD0465000+0x400000)));
//uint8_t SIF16FontBuff[3*1024*1024];
//uint8_t SIF24FontBuff[3*1024*1024];
//uint8_t SIF36FontBuff[3*1024*1024];

//����SIF24���壬��EMWINʹ��
void Create_SIF20(uint8_t *fxpath) 
{
	uint16_t bread;
	f_open(&SIF20FontFile,(const TCHAR*)fxpath,FA_READ);	
	f_read(&SIF20FontFile,SIF20FontBuff,SIF20FontFile.obj.objsize,(UINT *)&bread);
	
	GUI_SIF_CreateFont(SIF20FontBuff,&SIF20_Font,GUI_SIF_TYPE_PROP_AA2);
	f_close(&SIF20FontFile);
}



