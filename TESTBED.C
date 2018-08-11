/* testbed.c  -- Test program */

/*
  This test program has been my attempt to demonstrate ways to make the
  Help manager work for me.  My needs are to have more than one "main" window,
  and to have modeless dialog boxes, plus buttons in windows and dialog
  boxes and lots of menus.

  The real challenge, and what they don't bother to say in the book, is that
  the ID's for main windows have to be PUT THERE manually.  Then things
  really begin to work well.  The ID of the FRAME window is the key to
  having the help manager figure out what HELPITEM to choose from the
  HELPTABLE, and the ID of the CONTROL window or MENUITEM is used to
  pick a HELPSUBITEM from the HELPSUBTABLE.

  I found no way around doing a WinAssociateHelpInstance() call for
  each frame window, though I'm very interested in this.  I tried setting
  the OWNER of the "other" main window to be the "base" window, but then
  I got some strange effects:  When calling up help from the "other"
  window, the help would come up, and then when exited
  the active window would be the base window, even though the focus
  was still on the "other" window.  This looked weird, and was totally
  non-functional.  So I gave in.  Anybody know what I did wrong?

  To keep complexity to a minimum, I devised a numbering scheme for
  windows, buttons, menu items, etc. that I could live with.  I re-used
  the same ID for the resources and the Window ID and the help resources.
  This seemed to limit confusion.

  The other thing that isn't obvious from what they give you is that 
  in the IPF file you can't (I think!), put constants, only "hard-coded"
  numbers (the bane of a programmer's existence).  So, my advice is to
  get over the revulsion you feel about this and just do it their way.
  (They do talk about ways to call up help panels using string identifiers,
  but that doesn't work for the "automatic" way.

*/
#define INCL_WIN
#include <os2.h>
#include "testbed.h"



void winpanic( void );


HAB hab;

HWND hwndHelp;
HELPINIT helpinit = {
    sizeof( HELPINIT),
    0L,
    NULL,
    MAKEP(0xFFFF, ID_BASE_RESOURCE),
    0,
    0,
    0,
    0,
    "Help for Testbed Program",
    CMIC_SHOW_PANEL_ID,
    "c:\\test\\testbed.hlp"
    };

HWND hwndBaseFrame;
HWND hwndBaseClient;
MRESULT EXPENTRY BaseWndProc( HWND, USHORT, MPARAM, MPARAM);
CHAR szBaseClass[] = "BASE";
ULONG flBaseFrFlags =     FCF_TITLEBAR      | FCF_SYSMENU       |
                          FCF_SIZEBORDER    | FCF_MINMAX        |
                          FCF_SHELLPOSITION | FCF_TASKLIST      |
                          FCF_ICON          | FCF_MENU          ;

HWND hwndOtherFrame;
HWND hwndOther;
MRESULT EXPENTRY OtherWndProc( HWND, USHORT, MPARAM, MPARAM);
CHAR szOtherClass[] = "OTHER";
ULONG flOtherFrFlags =    FCF_TITLEBAR      | FCF_SYSMENU       |
                          FCF_SIZEBORDER    | FCF_MINMAX        |
                          FCF_SHELLPOSITION |
                          FCF_ICON          | FCF_MENU          ;

MRESULT EXPENTRY Box1DlgProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);


void main(void)
{
  HMQ hmq;
  QMSG qmsg;
  BOOL ef;
  BOOL          winRes;

  hab = WinInitialize( 0);
  if (hab==0) winpanic();
  hmq = WinCreateMsgQueue( hab, 50);
  if (hmq==0) winpanic();

  hwndHelp = WinCreateHelpInstance( hab, &helpinit);
  if (hwndHelp == NULL) winpanic();

  ef = WinRegisterClass ( hab, szBaseClass, BaseWndProc, CS_SIZEREDRAW, 0);
  if (!ef) winpanic();
  hwndBaseFrame = WinCreateStdWindow (  HWND_DESKTOP, WS_VISIBLE,
                                    &flBaseFrFlags, szBaseClass,
                                    " - Testbed Program", 0L,
                                    0, ID_BASE_RESOURCE,
                                    &hwndBaseClient);
  if( hwndBaseFrame==0) winpanic();
  WinSetWindowUShort( hwndBaseFrame, QWS_ID, ID_BASE_RESOURCE);
  winRes = WinAssociateHelpInstance( hwndHelp, hwndBaseFrame);
  if (!winRes) winpanic();

  ef = WinRegisterClass( hab, szOtherClass, OtherWndProc, CS_SIZEREDRAW, 0);
  if (!ef) winpanic();
  hwndOtherFrame = WinCreateStdWindow (  HWND_DESKTOP, WS_VISIBLE,
                                    &flOtherFrFlags, szOtherClass,
                                    "Testbed Program -- Other Window", 0L,
                                    0, ID_OTHER_RESOURCE,
                                    &hwndOther);
  if( hwndOtherFrame==0) winpanic();
  WinSetWindowUShort( hwndOtherFrame, QWS_ID, ID_OTHER_RESOURCE);
  //  would have liked to delete these next two lines.  See comment above.
  winRes = WinAssociateHelpInstance( hwndHelp, hwndOtherFrame);
  if (!winRes) winpanic();

  while (WinGetMsg( hab, &qmsg, NULL, 0, 0))
    WinDispatchMsg( hab, &qmsg);

  ef = WinDestroyWindow( hwndBaseFrame);
  if (!ef) winpanic();

  winRes = WinDestroyHelpInstance( hwndHelp);
  if (!winRes) winpanic();

  ef = WinDestroyMsgQueue( hmq);
  if (!ef) winpanic();

  ef = WinTerminate( hab);
  if (!ef) winpanic();
}


void winpanic( void )
{
  ERRORID errid;
  ERRINFO *pe;

  errid = WinGetLastError(hab);
  pe = WinGetErrorInfo(hab);
}



MRESULT EXPENTRY BaseWndProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
{
  HPS           hps;
  RECTL         rectl;

  switch(msg)
    {
  case WM_CREATE:
    return 0;
    break;

  case WM_PAINT:
    hps = WinBeginPaint (hwnd, NULL, &rectl);
    WinQueryWindowRect( hwnd, &rectl);
    WinFillRect( hps, &rectl, CLR_WHITE);
    WinDrawText( hps, -1, "Client Window", &rectl,
        CLR_NEUTRAL, CLR_WHITE, DT_CENTER | DT_VCENTER);
    WinEndPaint (hps) ;
    break;

  case WM_COMMAND:
    switch (SHORT1FROMMP(mp1) )
      {
    case IDM_BFI_EXIT:
      WinPostMsg( hwnd, WM_CLOSE, 0, 0);
      return 0;

    case IDM_ABOUT:
    case IDM_BFI_ABOUT:
      WinMessageBox( HWND_DESKTOP, hwnd,
          "Copyright (c) 1990,  X O Technologies, Inc. ",
          "Testbed Program", 0, MB_OK | MB_ICONASTERISK);
      return 0;

    case IDM_HELPFORHELP:
      WinSendMsg( hwndHelp, HM_DISPLAY_HELP, 0L, 0L);
      break;
    case IDM_EXTENDEDHELP:
      WinSendMsg( hwndHelp, HM_EXT_HELP, 0L, 0L);
      break;
    case IDM_KEYSHELP:
      WinSendMsg( hwndHelp, HM_KEYS_HELP, 0L, 0L);
      break;
    case IDM_HELPINDEX:
      WinSendMsg( hwndHelp, HM_HELP_INDEX, 0L, 0L);
      break;
      }
    break;

  case WM_HELP:
    break;  /* let the default handle it */

  case HM_QUERY_KEYS_HELP:
    return (MRESULT) IDM_KEYSHELP;

  case HM_ERROR:
    WinMessageBox( HWND_DESKTOP, hwnd,
          "A Help Manager Error was reported, report to Product Support.",
          "Help Manager Error", 0, MB_OK | MB_ICONEXCLAMATION);
      return 0;

  case HM_ACTIONBAR_COMMAND:
    break;
  case HM_EXT_HELP_UNDEFINED:
    break;
  case HM_HELPSUBITEM_NOT_FOUND:
    break;
  case HM_INFORM:
    break;
  case HM_TUTORIAL:
    break;
  case WM_CLOSE:
    break;   /* now let the default handler do its thing */

    }

  return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
}


MRESULT EXPENTRY OtherWndProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
{
  HPS           hps;
  RECTL         rectl;

  switch(msg)
    {
  case WM_CREATE:
    WinCreateWindow( hwnd, WC_BUTTON, "Help",
      WS_VISIBLE | BS_PUSHBUTTON | BS_HELP | BS_NOPOINTERFOCUS,
        10,  10,  80,  25,
      hwnd, HWND_TOP, IDC_BUTTON, NULL, NULL);
    WinCreateWindow( hwnd, WC_BUTTON, "Boop", WS_VISIBLE | BS_PUSHBUTTON,
      110,  10,  80,  25,
      hwnd, HWND_TOP, IDC_BUTTON, NULL, NULL);
    WinCreateWindow( hwnd, WC_BUTTON, "Dialog Box 2", WS_VISIBLE | BS_PUSHBUTTON,
      210,  10,  120,  25,
      hwnd, HWND_TOP, IDC_DIALOGBOX2, NULL, NULL);
    return 0;
    break;

  case WM_PAINT:
    hps = WinBeginPaint (hwnd, NULL, &rectl);
    WinQueryWindowRect( hwnd, &rectl);
    WinFillRect( hps, &rectl, CLR_WHITE);
    WinDrawText( hps, -1, "The Other Window", &rectl,
        CLR_NEUTRAL, CLR_WHITE, DT_CENTER | DT_VCENTER);
    WinEndPaint (hps) ;
    break;

  case WM_COMMAND:
    switch (SHORT1FROMMP(mp1) )
      {
    case IDM_OT_1:
      DosBeep( 1000, 250);
      return 0;

    case IDM_OT_2:
      DosBeep( 2000, 250);
      return 0;

    case IDM_OT_3:
      DosBeep( 4000, 250);
      return 0;

    case IDM_OS_1:
      DosBeep( 1000, 1250);
      return 0;

    case IDM_OS_2:
      DosBeep( 2000, 1250);
      return 0;

    case IDM_OS_3:
      DosBeep( 4000, 1250);
      return 0;

    case IDC_BUTTON:
      DosBeep( 250, 1250);
      return 0;

    case IDM_DIALOGBOX1:
    case IDC_DIALOGBOX2:
      WinDlgBox( HWND_DESKTOP, hwnd, Box1DlgProc, 0, IDD_DIALOGBOX1, NULL);
      return 0;

      }
    WinSendMsg( hwndBaseClient, msg, mp1, mp2);  /* any mis-understood WM_COMMANDs
                                                    get bumped back to the base */
    break;

  case WM_HELP:
    break;  /* let WinDefWindowProc() handle it */

  case HM_QUERY_KEYS_HELP:
    return (MRESULT) IDH_OTHER_KEYSHELP;

  case WM_CLOSE:
    break;   /* now let the default handler do its thing */

    }

  return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
}

MRESULT EXPENTRY Box1DlgProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
{
  BOOL wr;

  switch (msg)
    {
  case WM_INITDLG:
    wr = WinAssociateHelpInstance( hwndHelp, hwnd);
    if ( !wr) winpanic();
    return 0;

  case WM_COMMAND:
    switch (COMMANDMSG(&msg)->cmd)
      {
    case DID_SOSO:
      DosBeep( 200, 500);  /* and let fall through */
    case DID_DIALOGBOX1_OK:
      DosBeep( 100, 500);  /* and let fall through */
    case DID_CANCEL:
      WinDismissDlg( hwnd, TRUE);
      return 0;
      }

  case WM_HELP:
    break;  /* let WinDefDlgProc() handle it */


  case WM_CLOSE:
    wr = WinAssociateHelpInstance( hwndHelp, NULL);
    if ( !wr) winpanic();
    break;
    }
  return WinDefDlgProc( hwnd, msg, mp1, mp2);
}

