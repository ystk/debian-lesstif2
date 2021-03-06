/* $Id: test50.c,v 1.9 2000/08/29 21:59:21 dannybackx Exp $ */

#include <stdlib.h>
#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/CascadeB.h>
#include <Xm/MenuShell.h>
#include <Xm/MessageB.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/PushBP.h>
#include <Xm/RepType.h>
#include <Xm/RowColumn.h>
#include <Xm/SelectioB.h>
#include <Xm/DrawingA.h>
#include "../../common/Test.h"

Widget toplevel, box, push;
XtAppContext	app_context;
int DialogDone=False,DialogReturn;

char *fallback[] = {
	"*tearOffModel:				tear_off_enabled",
	NULL	/* The end */
};

typedef struct _CallbackResultStruct {
	char * WidgetName;
	int CallbackId;
	int Sequence;
	int Reason;
} _CallbackResult;

int StartLine = __LINE__;
_CallbackResult Results[] = { /* insert results of Motif run after this */
	"cascade1",             124,   0, XmCR_CASCADING                ,
	"pane1",                111,   1, XmCR_MAP                      ,
	"popup_pane1",          115,   2, XmCR_HELP                     ,
	"popup_pane1",          116,   3, XmCR_HELP                     ,
	"pane1",                114,   4, XmCR_UNMAP                    ,
	"pane1",                112,   5, XmCR_TEAR_OFF_ACTIVATE        ,
	"pane1",                111,   6, XmCR_MAP                      ,
	NULL, 0, 0, 0,  /* This must stay here */
};
int StopLine = __LINE__;
int ResultIndex = 0;


static void
GenericCallback(Widget w, int id, XmAnyCallbackStruct *cbs)
{
String Name = w ? XtName(w) : "";
int Reason = cbs ? cbs->reason : XmCR_NONE;

    if (XtNumber(Results) == 1 || ResultIndex + 1 >= XtNumber(Results))
    {
    static Boolean FirstCall = True;

	if (FirstCall)
	{
		printf("/^_CallbackResult/a");
	}
	printf("\\\n\t\"%s\", %*s%3i, %3i, %-30s,", 
	    w ? XtName(w) : "",
	    20 - strlen(Name),
	    "",
	    id, ResultIndex, 
	    cbs ? XdbReason2String(cbs->reason) : "XmCR_NONE /* cbs is NULL */");
	GlobalErrors = 1;
	FirstCall = False;
    }
    else
    {
	printf("%s, %*s%3i(%3i), %3i(%3i), %-30s ", 
	    Name,
	    20 - strlen(Name),
	    "",
	    id, Results[ResultIndex].CallbackId + (StopLine - StartLine - 4),
	    ResultIndex, Results[ResultIndex].Sequence,
	    XdbReason2String(Reason));

    	if (strcmp(Name, Results[ResultIndex].WidgetName) == 0 &&
    	    Results[ResultIndex].CallbackId == id - (StopLine - StartLine - 4) &&
    	    Results[ResultIndex].Sequence == ResultIndex &&
    	    Results[ResultIndex].Reason == Reason
    	    )
    	{
	    printf("okay\n");
    	}
    	else
    	{
	    printf("bad\n");
	    GlobalErrors++;
    	}
    }
    ResultIndex++;
}

int
main(int argc, char **argv)
{
    Widget menubar;
    Widget cascade1;
    Widget pane1;
    Widget button1;
    Widget pane1_tear_off;

/* Install converter to make the command line indicated above work */
    XmRepTypeInstallTearOffModelConverter();

/* Toplevel and Menu Bar */
    toplevel = XtVaAppInitialize(&app_context, "test50", NULL, 0, 
    	&argc, argv, fallback, NULL);
    menubar = XmCreateMenuBar(toplevel, "menubar", NULL, 0);


/* First Menu */
    pane1 = XmCreatePulldownMenu(menubar, "pane1", NULL, 0);
    XtAddCallback(pane1, XmNmapCallback, (XtCallbackProc)GenericCallback, (XtPointer)__LINE__);
    XtAddCallback(pane1, XmNtearOffMenuActivateCallback, (XtCallbackProc)GenericCallback, (XtPointer)__LINE__);
    XtAddCallback(pane1, XmNtearOffMenuDeactivateCallback, (XtCallbackProc)GenericCallback, (XtPointer)__LINE__);
    XtAddCallback(pane1, XmNunmapCallback, (XtCallbackProc)GenericCallback, (XtPointer)__LINE__);
    XtAddCallback(XtParent(pane1), XmNpopupCallback, (XtCallbackProc)GenericCallback, (XtPointer)__LINE__);
    XtAddCallback(XtParent(pane1), XmNpopdownCallback, (XtCallbackProc)GenericCallback, (XtPointer)__LINE__);
    pane1_tear_off = XmGetTearOffControl(pane1);

    cascade1 = XtVaCreateManagedWidget("cascade1",
				       xmCascadeButtonWidgetClass, menubar, 
				       XmNsubMenuId,	pane1,
				       NULL);
    XtAddCallback(cascade1, XmNactivateCallback, (XtCallbackProc)GenericCallback, (XtPointer)__LINE__);
    XtAddCallback(cascade1, XmNcascadingCallback, (XtCallbackProc)GenericCallback, (XtPointer)__LINE__);

    button1 = XtVaCreateManagedWidget("button1", 
    				xmPushButtonWidgetClass, pane1,
				NULL);

    XtManageChild(menubar);
    XtRealizeWidget(toplevel);
    LessTifTestWaitForIt(toplevel);

    LessTifTestBtn1Down(cascade1);
    LessTifTestBtn1Up(pane1_tear_off);

    printf("\n"); /* need final \n for output file or sed doesn't work on
		     SGI */
    LessTifTestMainLoop(toplevel);
    exit(0);
}
