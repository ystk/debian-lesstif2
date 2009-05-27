/**
 *
 * $Header: /cvsroot/lesstif/lesstif/test/Xm/label/test9.c,v 1.4 2001/05/15 13:40:54 amai Exp $
 *
 **/

#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/Form.h>

#include "../../common/Test.h"

Boolean GlobalResult = True;
extern int GlobalErrors;

static String FallBack[] = {
	"*geometry: +500+500",
	"*borderWidth: 1",
NULL};

typedef struct {
		XtGeometryResult result;
		XtWidgetGeometry geo;
} QueryResult;

static Boolean QueryGeometry(Widget W, XtWidgetGeometry *request, QueryResult *Expected, XtWidgetGeometry *desired)
{
Boolean status = True;
XtWidgetGeometry OriginalRequest;
XtGeometryResult result;

	if (request)
		OriginalRequest = *request;

	printf("QueryGeometry asked for (%s) ",
		XdbWidgetGeometry2String(request));

	result = XtQueryGeometry(W, request, desired);

	printf("got (%s %s) ",
		XdbWidgetGeometry2String(desired),
		XdbGeometryResult2String(result));
	printf("expected (%s %s %s) ",
		XdbWidgetGeometry2String(&Expected->geo),
		XdbGeometryResult2String(Expected->result),
		XdbWidgetGeometry2String(request));
	printf("original return (%s) ",
		XdbWidgetGeometry2String(request));

	if ((desired->request_mode & Expected->geo.request_mode) == Expected->geo.request_mode &&
	    desired->width  == Expected->geo.width  &&
	    desired->height == Expected->geo.height &&
	    result == Expected->result &&
	    (!request ? True : (OriginalRequest.request_mode == request->request_mode &&
	    			OriginalRequest.width == request->width &&
	    			OriginalRequest.height == request->height))
	   )
	{
		printf("Passed\n");
	}
	else
	{
		printf("Failed\n");
		status = False;
	}
	return(status);
}

static void DoTests(Widget Form)
{
XtWidgetGeometry Preferred;

	{ /* Ask what it wants */
	QueryResult Expect;
	XtWidgetGeometry desired;
	Dimension mh, mw;
	XmFontList fontlist;
	XmString string;

		Expect.geo.request_mode = CWWidth | CWHeight;
		XtVaGetValues(Form,
			XmNfontList, &fontlist,
			XmNlabelString, &string,
			XmNmarginHeight, &mh,
			XmNmarginWidth, &mw,
			NULL);
		Expect.geo.width = 80;
		Expect.geo.height = 30;
		Expect.result = XtGeometryNo;
		

		GlobalResult &= QueryGeometry(Form, NULL, &Expect, &desired);
		Preferred = desired;
	}
	{ /* Propose what it wants */
	QueryResult Expect;
	XtWidgetGeometry Proposed;
	XtWidgetGeometry desired;

		Expect.geo.request_mode = CWWidth | CWHeight;
		Expect.geo.width = Preferred.width;
		Expect.geo.height = Preferred.height;
		Expect.result = XtGeometryYes;

		Proposed.request_mode = CWWidth | CWHeight;
		Proposed.width = Preferred.width;
		Proposed.height = Preferred.height;
		GlobalResult &= QueryGeometry(Form, &Proposed, &Expect, &desired);
	}
	{ /* propose something bigger in width */
	QueryResult Expect;
	XtWidgetGeometry Proposed;
	XtWidgetGeometry desired;

		Expect.geo.request_mode = CWWidth | CWHeight;
		Expect.geo.width = Preferred.width;
		Expect.geo.height = Preferred.height;
		Expect.result = XtGeometryNo;

		Proposed.request_mode = CWWidth;
		Proposed.width = Preferred.width + 1;
		Proposed.height = Preferred.height;
		GlobalResult &= QueryGeometry(Form, &Proposed, &Expect, &desired);
	}
	{ /* propose something bigger */
	QueryResult Expect;
	XtWidgetGeometry Proposed;
	XtWidgetGeometry desired;

		Expect.geo.request_mode = CWWidth | CWHeight;
		Expect.geo.width = Preferred.width;
		Expect.geo.height = Preferred.height;
		Expect.result = XtGeometryNo;

		Proposed.request_mode = CWWidth | CWHeight;
		Proposed.width = Preferred.width + 1;
		Proposed.height = Preferred.height + 1;
		GlobalResult &= QueryGeometry(Form, &Proposed, &Expect, &desired);
	}
	{ /* propose something smaller */
	QueryResult Expect;
	XtWidgetGeometry Proposed;
	XtWidgetGeometry desired;

		Expect.geo.request_mode = CWWidth | CWHeight;
		Expect.geo.width = Preferred.width;
		Expect.geo.height = Preferred.height;
		Expect.result = XtGeometryNo;

		Proposed.request_mode = CWWidth | CWHeight;
		Proposed.width = Preferred.width - 1;
		Proposed.height = Preferred.height - 1;
		GlobalResult &= QueryGeometry(Form, &Proposed, &Expect, &desired);
	}
}

int
main(int argc, char **argv)
{
  Widget toplevel;
  Widget Form;
  Widget Label;
  XtAppContext app;
  Arg args[10];
  int n;

  XtSetLanguageProc(NULL, NULL, NULL);

  toplevel = XtVaAppInitialize(&app, "test5", NULL, 0, &argc, argv, FallBack, NULL);

  printf("\nCreate a Label\n");
  n = 0;
  XtSetArg(args[n], XmNwidth, 80); n++;
  XtSetArg(args[n], XmNheight, 30); n++;
  XtSetArg(args[n], XmNrecomputeSize, False); n++;
  Label = XmCreateLabel(toplevel, "TestWidget", args, n);
  DoTests(Label);

  printf("\nManage the Label\n");
  XtManageChild(Label);
  DoTests(Label);

  printf("\nRealize the Shell\n");
  XtRealizeWidget(toplevel);
  DoTests(Label);

  printf("%s\n",GlobalResult ? "All Passed" : "One or more Failed");
  GlobalErrors = GlobalResult ? 0 : 1;

  {
  static XtWidgetGeometry Expected[] = {
  	CWWidth | CWHeight,		0,	0,	80,	30,	0,0,0,	/* Form */
};

  PrintDetails(toplevel, Expected);
  }
  LessTifTestMainLoop(toplevel);

  exit(GlobalResult ? 0 : 1);
}
