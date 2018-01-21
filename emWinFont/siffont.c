#include "siffont.h"
#include "ff.h"
//���嶨��
GUI_FONT SIF12_Font;
GUI_FONT SIF16_Font;
GUI_FONT SIF24_Font;
GUI_FONT SIF36_Font;

//�����ļ�
FIL SIF12FontFile;
FIL SIF16FontFile;
FIL SIF24FontFile;
FIL SIF36FontFile;

//�ֿ��ļ�������
uint8_t SIF12FontBuff[3*1024*1024]__attribute__((at(0XC0600000)));
uint8_t SIF16FontBuff[3*1024*1024];
uint8_t SIF24FontBuff[3*1024*1024];
uint8_t SIF36FontBuff[3*1024*1024];

//����SIF12���壬��EMWINʹ��
void Create_SIF12(uint8_t *fxpath) 
{
   //����SIF��ʽ����
   GUI_SIF_CreateFont(SIF12FontBuff,&SIF12_Font,GUI_SIF_TYPE_PROP_AA2);
}
     
//����SIF16���壬��EMWINʹ��
void Create_SIF16(uint8_t *fxpath) 
{
    GUI_SIF_CreateFont(SIF16FontBuff,&SIF16_Font,GUI_SIF_TYPE_PROP_AA2);
}

//����SIF24���壬��EMWINʹ��
void Create_SIF24(uint8_t *fxpath) 
{
GUI_SIF_CreateFont(SIF24FontBuff,&SIF24_Font,GUI_SIF_TYPE_PROP_AA2);
}

//����SIF36���壬��EMWINʹ��
void Create_SIF36(uint8_t *fxpath) 
{
    //����SIF��ʽ����
    GUI_SIF_CreateFont(SIF36FontBuff,&SIF36_Font,GUI_SIF_TYPE_PROP_AA2);
}


