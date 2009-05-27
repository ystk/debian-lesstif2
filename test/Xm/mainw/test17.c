#if 0
To:          lesstif@lesstif.org
From:        Steven G. Kargl <kargl@troutmask.apl.washington.edu>
Subject:     MainW.c is killing me.
Date:        Tue, 7 Sep 1999 14:43:31 -0700 (PDT)

Its me, again.

Well, I spent all weekend trying to understand MainW.c, testall,
and why my patch from last week failed to fix dynapix.  I forgot
a MainWindow could be a child widget to some other manager widget.
Doh!

So, I download lesstif-current with a date of 990907.  I do not
alter any sources.

The following program when compiled and linked against Lesstifs
1.2 library produces a segmentation fault.  It compiles and runs
with OSF Motif 1.2.
#endif


#include <Xm/MainW.h>

int main(int argc, char *argv[]) {

	Widget toplevel, main1, main2, menu1, menu2;
	XtAppContext app;
	XmString text;

	XtSetLanguageProc(NULL, NULL, NULL);

	toplevel = XtVaAppInitialize(&app, "t", NULL, 0, &argc, argv,
        NULL, NULL);

	main1 = XtVaCreateManagedWidget("main1",
        xmMainWindowWidgetClass, toplevel,
        XmNscrollBarDisplayPolicy, XmAS_NEEDED,
		XmNscrollingPolicy, XmAUTOMATIC,
        NULL);

	main2 = XtVaCreateManagedWidget("main2",
        xmMainWindowWidgetClass, main1,
        NULL);

    text = XmStringCreateLocalized("Menu2");
    menu2 = XmVaCreateSimpleMenuBar(main2, "menu2",
		XmVaCASCADEBUTTON, text, 'e', NULL);
    XmStringFree(text);
    XtManageChild(menu2);

	XtVaSetValues(main2, XmNmenuBar, menu2, XmNwidth, 150, XmNheight, 130,
        NULL);

    text = XmStringCreateLocalized("Menu1");
    menu1 = XmVaCreateSimpleMenuBar(main1, "menu1",
		XmVaCASCADEBUTTON, text, 'M', NULL);
    XmStringFree(text);
    XtManageChild(menu1);

#if 1
	XtVaSetValues(main1, 
		XmNmenuBar, menu1, 
		XmNworkWindow, main2, 
		NULL);
#else
	XmMainWindowSetAreas(main1, menu1, NULL, NULL, NULL, main2);
#endif

	XtRealizeWidget(toplevel);

/* Note: the following values are the result of
 * querying the current geometry.
 */
{
static XtWidgetGeometry Expected[] = {
   CWWidth | CWHeight            ,  232,  387,  131,  162, 0,0,0, /* main1 */
   CWWidth | CWHeight | CWX | CWY,    4,   35,  100,  100, 0,0,0, /* ClipWindow */
   CWWidth | CWHeight | CWX | CWY,    0,    0,  150,  130, 0,0,0, /* main2 */
   CWWidth | CWHeight | CWX | CWY,    0,    0,  150,   31, 0,0,0, /* menu2 */
   CWWidth | CWHeight | CWX | CWY,    5,    5,   46,   21, 0,0,0, /* button_0 */
   CWWidth | CWHeight | CWX | CWY,  112,   31,   19,  108, 0,0,0, /* VertScrollBar */
   CWWidth | CWHeight | CWX | CWY,    0,  143,  108,   19, 0,0,0, /* HorScrollBar */
   CWWidth | CWHeight | CWX | CWY,    0,    0,  131,   31, 0,0,0, /* menu1 */
   CWWidth | CWHeight | CWX | CWY,    5,    5,   46,   21, 0,0,0, /* button_0 */
};
/* toplevel should be replaced with to correct applicationShell */
PrintDetails(toplevel, Expected);
}
LessTifTestMainLoop(toplevel);
    return 0;
}


#if 0
troutmask:kargl[211] t -sync
Segmentation fault (core dumped)
troutmask:kargl[212] gdb t t.core
GNU gdb 4.18
Copyright 1998 Free Software Foundation, Inc.
GDB is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type "show copying" to see the conditions.
There is absolutely no warranty for GDB.  Type "show warranty" for details.
This GDB was configured as "i386-unknown-freebsd"...
Core was generated by t.
Program terminated with signal 11, Segmentation fault.
#0  0x80485f1 in set_values (old=0xbfbfd2bc, request=0xbfbfcf9c, 
    new_w=0x81e0000, args=0x81ddfa0, num_args=0xbfbfcdb0) at MainW.c:470
470             if (SW_VisualPolicy(new_w) == XmCONSTANT &&
(gdb) bt
#0  0x80485f1 in set_values (old=0xbfbfd2bc, request=0xbfbfcf9c, 
    new_w=0x81e0000, args=0x81ddfa0, num_args=0xbfbfcdb0) at MainW.c:470
#1  0x80caf1d in CallSetValues ()
#2  0x80cb303 in XtSetValues ()
#3  0x80d7d8c in XtVaSetValues ()
#4  0x804826b in main (argc=1, argv=0xbfbfd674) at t.c:39
#5  0x80480ec in _start ()
(gdb) quit

-- 
Steve
#endif