/* test for multi line labels */

#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <stdio.h>

static char *FallBack[] = {
		"*.geometrySlop: 1",
		NULL
};

void
cb(Widget w, XtPointer data, XtPointer cbs)
{
    printf("Activated\n");
}

int
main(int argc, char **argv)
{
  Widget toplevel, one;
  XtAppContext app;
  XmFontList fontlist;
  XmString xmstr1 = XmStringCreateLtoR("Here Is A\nLabel", "MY_FONT");

  XtSetLanguageProc(NULL, NULL, NULL);

  toplevel = XtVaAppInitialize(&app, "Label", NULL, 0, &argc, argv, FallBack, NULL);

  fontlist = XmFontListAppendEntry(NULL,
			   XmFontListEntryCreate("MY_FONT",
						 XmFONT_IS_FONT,
						 XLoadQueryFont(XtDisplay(toplevel), 
 	                                         "-adobe-helvetica-bold-o-normal--17-0-75-75-p-*-iso8859-1")));

  one = XtVaCreateManagedWidget("One", 
                                xmPushButtonWidgetClass, 
                                toplevel, XmNfontList, fontlist, 
				XmNlabelString, xmstr1,
				XtNborderWidth, 20,
				NULL);

  XtAddCallback(one, XmNactivateCallback, cb, NULL);

  XtRealizeWidget(toplevel);

  {
  static XtWidgetGeometry Expected[] = {
  	CWWidth | CWHeight,		0,	0,	83,	48,	0,0,0,	/* Form */
  	CWWidth | CWHeight | CWX | CWY,	45,	30,	30,	25,	0,0,0,	/* two */
};

  PrintDetails(toplevel, Expected);
  }
  LessTifTestMainLoop(toplevel);
  /*
  XtAppMainLoop(app);
  */

  exit(0);
}
