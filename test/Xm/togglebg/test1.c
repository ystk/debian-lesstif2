#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/BulletinB.h>
#include <Xm/ToggleBG.h>

void HiCB(Widget w,XtPointer client_data,XtPointer call_data);

int
main(int argc, char **argv)
{
  XtAppContext theApp;
  Widget toplevel, rc;
  Widget butt1,butt2;

  toplevel = XtVaAppInitialize(&theApp, "toggle1", NULL, 0,
			       &argc, argv, NULL, NULL);

#if 1
  rc= XtVaCreateManagedWidget("A Simple Toggle Button",
			      xmRowColumnWidgetClass,
			      toplevel,
			      XmNentryAlignment, XmALIGNMENT_CENTER,
			      XmNorientation, XmHORIZONTAL,
			      XmNpacking, XmPACK_TIGHT,
			      XmNradioBehavior, True,
			      XmNnumColumns, 1,
			      NULL);
#else
  rc= XtVaCreateManagedWidget("A Simple Toggle Button",
			      xmBulletinBoardWidgetClass,
			      toplevel,
			      NULL);
#endif

  butt1= XtVaCreateManagedWidget("Button1", xmToggleButtonGadgetClass, rc, 
				NULL);

  butt2= XtVaCreateManagedWidget("Button1", xmToggleButtonGadgetClass, rc, 
				NULL);

  XtAddCallback(butt1,XmNvalueChangedCallback,HiCB,NULL);
  XtAddCallback(butt2,XmNvalueChangedCallback,HiCB,NULL);

  XtRealizeWidget(toplevel);

/* Note: the following values are the result of
 * querying the current geometry.
 */
{
static XtWidgetGeometry Expected[] = {
   CWWidth | CWHeight            ,    0,    0,  143,   31, 0,0,0, /* A Simple Toggle Button */
   CWWidth | CWHeight | CWX | CWY,    3,    3,   67,   25, 0,0,0, /* Button1 */
   CWWidth | CWHeight | CWX | CWY,   73,    3,   67,   25, 0,0,0, /* Button1 */
};
/* toplevel should be replaced with to correct applicationShell */
PrintDetails(toplevel, Expected);
}
LessTifTestMainLoop(toplevel);

  exit(0);
}

void HiCB(Widget w,XtPointer client_data,XtPointer call_data)
{
	XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct *)call_data;

	printf("Toggle Me and I'm Yours: %d\n", cbs->set);
}
