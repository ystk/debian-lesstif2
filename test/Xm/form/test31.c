#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/SeparatoG.h>


static char *FallBack[] = {
		"*.borderWidth: 1",
		NULL
};

int
main(int argc, char **argv)
{
  XtAppContext	app;
  Widget Shell;
  Widget Form;
  Widget BottomLabel;

  XtSetLanguageProc(NULL, NULL, NULL);

  Shell = XtVaAppInitialize(&app, "Shell", NULL, 0, &argc, argv, FallBack, NULL);

  Form = XmCreateForm(Shell,"Form",NULL,0);

  BottomLabel = XmCreateLabel(Form,"BottomLabel",NULL,0);
  XtVaSetValues(BottomLabel,
  	XmNbottomAttachment, XmATTACH_FORM,
  	XmNleftAttachment, XmATTACH_POSITION,
  	XmNleftPosition, 0,
  	XmNleftOffset, 10,
  	NULL);
  XtManageChild(BottomLabel);

  XtManageChild(Form);

  XtRealizeWidget(Shell);
  {
  static XtWidgetGeometry Expected[] = {
  	CWWidth | CWHeight,		0,	0,	82,	19,	0,0,0,	/* Form */
  	CWWidth | CWHeight | CWX | CWY,	10,	0,	70,	17,	0,0,0,	/* two */
};

  PrintDetails(Shell, Expected);
  }
      LessTifTestMainLoop(Shell);
  exit(0);
}
