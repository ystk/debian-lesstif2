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
		"*.geometrySlop: 1",
		NULL
};

int
main(int argc, char **argv)
{
  XtAppContext	app;
  Widget Shell;
  Widget Form;
  Widget TopLabel;
  Widget BottomLabel;

  XtSetLanguageProc(NULL, NULL, NULL);

  Shell = XtVaAppInitialize(&app, "Shell", NULL, 0, &argc, argv, FallBack, NULL);

  Form = XmCreateForm(Shell,"Form",NULL,0);

  TopLabel = XmCreateLabel(Form,"TopLabel",NULL,0);

  BottomLabel = XmCreateLabel(Form,"BottomLabel",NULL,0);

  XtVaSetValues(TopLabel,
  	XmNtopAttachment, XmATTACH_POSITION,
  	XmNtopPosition, 20,
  	XmNbottomAttachment, XmATTACH_WIDGET,
  	XmNbottomWidget, BottomLabel,
  	NULL);
  XtVaSetValues(BottomLabel,
  	XmNbottomAttachment, XmATTACH_POSITION,
  	XmNbottomPosition, 80,
  	NULL);

  XtManageChild(TopLabel);
  XtManageChild(BottomLabel);

  XtManageChild(Form);

  XtRealizeWidget(Shell);
  {
  static XtWidgetGeometry Expected[] = {
  	CWWidth | CWHeight,		0,	0,	72,	62,	0,0,0,	/* Form */
  	CWWidth | CWHeight | CWX | CWY,	0,	12,	52,	17,	0,0,0,	/* two */
  	CWWidth | CWHeight | CWX | CWY,	0,	31,	70,	17,	0,0,0,	/* two */
};

  PrintDetails(Shell, Expected);
  }
      LessTifTestMainLoop(Shell);
  exit(0);
}
