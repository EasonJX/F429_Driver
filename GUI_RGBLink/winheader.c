/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.32                          *
*        Compiled Oct  8 2015, 11:59:02                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "stdio.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x0A)
#define ID_MENU_0 (GUI_ID_USER + 0x0B)


#define DATE_DIS (WM_USER+1)
// USER START (Optionally insert additional defines)
// USER END
//extern GUI_CONST_STORAGE GUI_BITMAP bmlogo;
//extern GUI_CONST_STORAGE GUI_BITMAP bmQQ2;
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END
/*********************************************************************
*
*       _AddMenuItem
*/

/*********************************************************************
*
*       _cbDialog
*/
extern GUI_CONST_STORAGE GUI_BITMAP bmHlogo;

CALENDAR_DATE Date;
int Hour;
int Min;
int Sec;

WM_HWIN WIN_Header;
static void _cbDialog(WM_MESSAGE * pMsg) {
  int NCode;
  int Id;
//  WM_HWIN hWin;
//  MENU_Handle hMenu;
//  MENU_Handle hMenuFile;
//  MENU_Handle hMenuEdit;
//  MENU_Handle hMenuHelp;
//  MENU_Handle hMenuRecent;
  char p[12];
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
	  GUI_DrawBitmap(&bmHlogo, 0, 0);
	  break;
  case WM_CREATE:
	  WM_CreateTimer(pMsg->hWin, 0, 10, 0);
	  break;
  case DATE_DIS:
	  WM_InvalidateWindow(pMsg->hWin);
	  break;
  case WM_TIMER:
	  if (Sec++ > 59)
	  {
		  Min++;
		  Sec = 0;
	  }
	  if (Min == 60)
	  {
		  Hour++;
		  Min = 0;
	  }
	  if (Hour == 24)
	  {
		  Hour = 0;
		  Date.Day++;
	  }
	  WM_RestartTimer(pMsg->Data.v, 1000);
	  WM_SendMessageNoPara(WIN_Header, DATE_DIS);
	  break;
  case WM_PAINT:
	  
//  GUI_DrawGradientH(0, 0, 1023, 79, 0x0080ff80, GUI_LIGHTBLUE);

	  GUI_DrawGradientV(0, 0, 479, 15, 0xFFCCCC, GUI_LIGHTBLUE);
//	  GUI_DrawGradientV(0, 26, 800, 50, GUI_BLUE, 0xFFCCCC);
	  GUI_DrawGradientV(0, 15, 479, 30, 0xFF7070, 0x00FF2020);
//	  if (Sec%10>5)
//	  GUI_DrawBitmap(&bmlogo, 10, 8);
//	  else
//		  GUI_DrawBitmap(&bmQQlogo, 10, 2);
	  GUI_DrawBitmap(&bmHlogo, 5, 8);
	  GUI_SetColor(GUI_BLACK);
	  GUI_SetFont(&GUI_Font20B_ASCII);
	  GUI_SetTextMode(GUI_TM_TRANS);
	//  sprintf(p,"%d-%2d-%2d",Date.Year,Date.Month,Date.Day);
	//  GUI_DispStringAt(p,700,6);
	  sprintf(p, "%4d:%2d:%2d", Hour,Min,Sec);
	  GUI_DispStringAt(p,380, 5);


	  break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_MENU_0: // Notifications sent by 'Menu'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/

extern GUI_CONST_STORAGE GUI_BITMAP bmlogo;
extern GUI_CONST_STORAGE GUI_BITMAP bmbar;
extern WM_HWIN CreateWinMain(void);

WM_HWIN CreateHeaderWindow(void);
WM_HWIN CreateHeaderWindow(void) {
  WM_HWIN hWin;

  hWin = WM_CreateWindow(0, 0, 479, 30, WM_CF_SHOW, _cbDialog, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

#define START 1
WM_HWIN CreateWinMain(void);
void MainTask(void) {
	int i, j;

	GUI_RECT Rect = { 180, 220, 280, 240 };
	
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();	


	Date.Year = 2018;
	Date.Month = 7;
	Date.Day = 7;
	Hour = 16;
	Min = 24;
	Sec = 35;
	GUI_DrawGradientV(0, 0, 479, 271, GUI_LIGHTBLUE, GUI_BLUE);

	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
#if START

	GUI_DrawBitmap(&bmlogo, 120, 100);
	GUI_SetFont(&GUI_Font24B_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_DispStringHCenterAt("Build software better, together.", 240, 190);
	GUI_Delay(200);
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 65; j++)
		{
		//	GUI_DrawGradientV(0, 0, 479, 271, GUI_LIGHTBLUE, GUI_BLUE);
//			GUI_DispStringHCenterAt("Build software better, together.", 240, 190);
//			GUI_DrawBitmap(&bmlogo, 120, 100);
			GUI_FillRect(180, 222, 280, 240);
	//		GUI_SetColor(GUI_WHITE);
			GUI_DrawRoundedFrame(180, 220, 280, 240, 10, 2);
			GUI_SetClipRect(&Rect);
			GUI_DrawBitmap(&bmbar, 180 + j * 4, 225);
			GUI_SetClipRect(NULL);
			GUI_Delay(20);
		}
	}
	GUI_DrawGradientV(0, 0, 479, 271, GUI_LIGHTBLUE, GUI_BLUE);
#endif

	GUI_EnableAlpha(1);
	WIN_Header = CreateHeaderWindow();
	CreateWinMain();
	while (1) {
		GUI_Exec();

	}
}
/*************************** End of file ****************************/