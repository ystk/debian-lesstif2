/**
 *
 * $Id: test39.c,v 1.6 2001/05/15 14:46:10 amai Exp $
 *
 **/

#include <stdlib.h>
#include <stdio.h>

#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>

#include "../../common/Test.h"

Boolean GlobalResult = True;

static String FallBack[] = {
	"*geometry: +500+500",
	"*borderWidth: 0",
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
	/*
	printf("original return (%s) ",
		XdbWidgetGeometry2String(request));
		*/

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
	WidgetList children;
	Cardinal numKids;

		Expect.geo.request_mode = CWWidth | CWHeight;
		XtVaGetValues(Form,
			XmNchildren, &children,
			XmNnumChildren, &numKids,
			NULL);
		if (numKids == 0 || !XtIsManaged(children[0]))
		{
			Expect.geo.width = 16;
			Expect.geo.height = 16;
			Expect.result = XtGeometryNo;
		}
		else
		{
		Dimension bw;

			XtVaGetValues(children[0],
				XmNborderWidth, &bw,
				XmNwidth, &Expect.geo.width,
				XmNheight, &Expect.geo.height,
				NULL);
			Expect.geo.width += 2 * bw + 6;
			Expect.geo.height += 2 * bw + 6;

			if (XtIsRealized(Form))
			{
			    Expect.result = XtGeometryNo;
			}
			else
			{
				Expect.result = XtGeometryAlmost;
			}
		}

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
		Expect.geo.width = Preferred.width + 1;
		Expect.geo.height = Preferred.height;
		if (XtIsRealized(Form))
		{
		    Expect.result = XtGeometryAlmost;
		}
		else
		{
		    Expect.result = XtGeometryAlmost;
		}

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
		Expect.geo.width = Preferred.width + 1;
		Expect.geo.height = Preferred.height + 1;
		if (XtIsRealized(Form))
		{
		    Expect.result = XtGeometryYes;
		}
		else
		{
		    Expect.result = XtGeometryYes;
		}

		Proposed.request_mode = CWWidth | CWHeight;
		Proposed.width = Preferred.width + 1;
		Proposed.height = Preferred.height + 1;
		GlobalResult &= QueryGeometry(Form, &Proposed, &Expect, &desired);
	}
	{ /* propose something smaller */
	QueryResult Expect;
	XtWidgetGeometry Proposed;
	XtWidgetGeometry desired;
	WidgetList children;
	Cardinal numKids;

		Expect.geo.request_mode = CWWidth | CWHeight;
		XtVaGetValues(Form,
			XmNchildren, &children,
			XmNnumChildren, &numKids,
			NULL);
		if (numKids == 0 || !XtIsManaged(children[0]))
		{
		    Expect.geo.width = Preferred.width;
		    Expect.geo.height = Preferred.height;
		    Expect.result = XtGeometryNo;
		}
		else
		{
		    Expect.geo.width = Preferred.width - 1;
		    Expect.geo.height = Preferred.height - 1;
		    Expect.result = XtGeometryYes;
		}
		/*
		if (XtIsRealized(Form))
		{
			Expect.result = XtGeometryNo;
		}
		else
		{
			Expect.result = XtGeometryNo;
		}
		*/

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
  Widget label;
  XtAppContext app;

  XtSetLanguageProc(NULL, NULL, NULL);

  toplevel = XtVaAppInitialize(&app, "test23", NULL, 0, &argc, argv, FallBack, NULL);

  printf("Create a RowColumn\n");
  Form = XmCreateRowColumn(toplevel, "TestWidget", NULL, 0);
  
  DoTests(Form);

  printf("\nCreate a label in the RowColumn\n");
  label = XmCreateLabel(Form, "TestWidget", NULL, 0);
  
  DoTests(Form);

  printf("\nManage the label\n");
  XtManageChild(label);
  
  DoTests(Form);

  printf("\nManage the RowColumn\n");
  XtManageChild(Form);
  
  DoTests(Form);

  printf("\nRealize the Shell\n");
  XtRealizeWidget(toplevel);
  
  DoTests(Form);

  printf("%s\n",GlobalResult ? "All Passed" : "One or more Failed");
  {
  static XtWidgetGeometry Expected[] = {
  	CWWidth | CWHeight,		500,	500,	70,	23,	0,0,0,
  	CWWidth | CWHeight | CWX | CWY,	3,	3,	64,	17,	0,0,0,
};

  PrintDetails(toplevel, Expected);
  }
    LessTifTestMainLoop(toplevel);
    /*
  XtAppMainLoop(app);
  */

  exit(GlobalResult ? 0 : 1);
}