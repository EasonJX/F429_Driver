#include "xbffont.h"
#include "ff.h"


//XBF����  �洢��SD�����ⲿFLASH��
//��������
GUI_FONT XBF20_Font;

GUI_XBF_DATA	XBF20_Data;

FIL XBF20FontFile;

//�ص�������������ȡ��������
//������Off:		��XBF��ƫ��(λ��)
//		NumBytes:	Ҫ��ȡ���ֽ���
//		pVoid:	Ҫ��ȡ���ļ�
//		pBuff:	��ȡ�������ݵĻ�����
//����ֵ:0 �ɹ���1 ʧ��
static int _cbGetData(U32 Off, U16 NumBytes, void * pVoid, void * pBuffer) 
{
	int result;
	uint16_t bread; 
	FIL *hFile;

	hFile = (FIL*)pVoid; 
	
	//�������ļ��е�ƫ��(λ��)
	result = f_lseek(hFile,Off);
	if(result != FR_OK)	return 1; //���ش���

	//��ȡ��������
		
	result = f_read(hFile,pBuffer,NumBytes,(UINT *)&bread); //��ȡ����
	
	if(result != FR_OK) return 1; //���ش���
	return 0; 
}

//����XBF20���壬��EMWINʹ��
//fxpath:XBF�����ļ�·��
//����ֵ:0���ɹ���1��ʧ��
uint8_t Create_XBF20(uint8_t *fxpath) 
{
	int result;
	result = f_open(&XBF20FontFile,(const TCHAR*)fxpath,FA_READ);	//���ֿ��ļ�
	if(result != FR_OK) return 1;
	//����XBF16����
	GUI_XBF_CreateFont(	&XBF20_Font,    //ָ��GUI_FONT�ṹ
						&XBF20_Data, 	//ָ��GUI_XBF_DATA�ṹ
						GUI_XBF_TYPE_PROP_AA2_EXT,//Ҫ��������������
						_cbGetData,   	//�ص�����
						&XBF20FontFile);  //������ص�����_cbGetData�Ĳ���
	return 0;
}

uint8_t Create_Font(void)
{
	Create_XBF20("0:/SYS/FONT/XBF20.xbf");
}

