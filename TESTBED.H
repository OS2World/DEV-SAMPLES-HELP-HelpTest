/*************************
 testbed.h 
 *************************/

/*
  BASIC NUMBERING SCHEME is as follows:

  The RESOURCE ID is used for all resources for a given window, including
  the Help stuff.  The resource ID also doubles as the window ID for the
  Frame of all windows who call the DESKTOP their parent.

  The resource ID's start at 1 and go up to 29.

  In general:
  The Menu selection ID's are the Resouce ID * 1000, plus the number
  of the top menu bar * 100, plus the number of the submenu item.  The HELP 
  selection should always considered '0'.  Examples:  The 4th item on the 2nd
  menu for the window using resource id 5 would be: 5204.  The id of the 
  menu for this is 5200.  The Help Menu would be 5000.  But see below
  for special considerations regarding items.

  The Help menu has some "standard" stuff, and possibly some non-standard 
  stuff.  The Standard stuff has FIXED ID's for the menus, as follows:

    Help For help:   #define   IDM_HELPFORHELP       30001
    Extended help:   #define   IDM_EXTENDEDHELP      30002
    Keys help:       #define   IDM_KEYSHELP          30003
    Help index:      #define   IDM_HELPINDEX         30004
    Tutorial:        #define   IDM_TUTORIAL          30005 (optional)
    About:           #define   IDM_ABOUT             30010 (only on main)

  The basic concept here is that the "main" window will handle help requests
  for these.  So, any other window receiving these should relay the message
  to the main window with a WinSendMsg() call.

  These menu ID's also double as "help panel ID's", so it is important not
  to have a collision, unless the same help panel can be used for both.
  (This rule is only for convention, and to keep the complexity and the
  typing to a minimum.)

  Special cases, like menu items on different windows that generate the 
  same functions, are handled by ID's in the 31000 to 32500 range.

*/


#define   IDM_HELPFORHELP       30001
#define   IDM_EXTENDEDHELP      30002
#define   IDM_KEYSHELP          30003
#define   IDM_HELPINDEX         30004
#define   IDM_TUTORIAL          30005
#define   IDM_ABOUT             30010



#define ID_BASE_RESOURCE         1
#define IDM_BASE_FILE            1100
#define   IDM_BFI_EXIT           1101
#define   IDM_BFI_ABOUT          1102
#define IDM_BASE_HELP            1000




#define ID_OTHER_RESOURCE        2
#define IDM_OTHER_HELP           2000
#define IDC_BUTTON               2001
#define IDC_DIALOGBOX2           2002
#define IDH_OTHER_KEYSHELP       2003
#define ID_OTHER_EXT_HELP        2004


#define IDM_OTHER_THINGS         2100
#define   IDM_OT_1               2101
#define   IDM_OT_2               2102
#define   IDM_OT_3               2103
#define IDM_OTHER_STUFF          2200
#define   IDM_OS_1               2201
#define   IDM_OS_2               2202
#define   IDM_OS_3               2203
#define IDM_DIALOGBOX1           2300


#define IDD_DIALOGBOX1           3
#define DID_DIALOGBOX1_HELP      3000
#define DID_DIALOGBOX1_OK        3001
#define DID_SOSO                 3002



