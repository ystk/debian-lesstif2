/* $Header: /cvsroot/lesstif/lesstif/test/Xm-2.0/combobox/test10.c,v 1.5 2002/05/01 15:47:31 amai Exp $ */
/* Purpose: test resizing of combobox inside a form */

#include <stdio.h>
#include <stdlib.h>

#include <Xm/Form.h>
#include <Xm/ComboBoxP.h>
#include <Xm/List.h>

#include "../../common/Test.h"

#if !defined(CB_List)
#define CB_List(w) \
        (((XmComboBoxWidget)(w))->combo_box.list)
#endif

void
check_geometry(Widget w)
{
   static int result_index = 0;

static XtWidgetGeometry Expected[] = {
/* result test 0 */
{  CWWidth | CWHeight            ,    0,    0,  386,   94, 0,0,0 }, /* combo */
{  CWWidth | CWHeight | CWX | CWY,    6,    6,  374,   50, 0,0,0 }, /* Text */
{  CWWidth | CWHeight | CWX | CWY,    6,   56,  374,   32, 0,0,0 }, /* ListSW */
{  CWWidth | CWHeight | CWX | CWY,  359,    0,   15,   32, 0,0,0 }, /* VertScrollBar */
{  CWWidth | CWHeight | CWX | CWY,    0,    0,  359,   32, 0,0,0 }, /* List */
};

#if 0
   PrintDetails2(w, NULL);
#else
  if (result_index <= 0)
  {
     PrintDetails2(w, Expected);
     fflush(stdout);
     result_index ++;
  }
#endif
}

int
main(int argc, char **argv)
{
  Widget toplevel, widget, bb, list;
  XtAppContext app;
  XmString item;
  Arg args[10];
  int n = 0;

  XtSetLanguageProc(NULL, NULL, NULL);

  toplevel = XtVaAppInitialize(&app, "ComBox", NULL, 0, &argc, argv, NULL, NULL);

  bb = XmCreateForm(toplevel, "bb", NULL, 0);
  XtManageChild(bb);

  widget = XmCreateComboBox(bb, "combo", args, n);
  XtVaSetValues(widget,
  	XmNtopAttachment, XmATTACH_FORM,
  	XmNbottomAttachment, XmATTACH_FORM,
  	XmNleftAttachment, XmATTACH_FORM,
  	XmNrightAttachment, XmATTACH_FORM,
  	NULL);

  /* use list routine to add items */
  list = CB_List(widget);

  item = XmStringCreateSimple("Item 1");
  XmListAddItem(list,item,0);
  item = XmStringCreateSimple("Item 2");
  XmListAddItem(list,item,0);
  item = XmStringCreateSimple("Item 3");
  XmListAddItem(list,item,0);
  item = XmStringCreateSimple("Item 4");
  XmListAddItem(list,item,0);
  item = XmStringCreateSimple("Item 5");
  XmListAddItem(list,item,0);

  XmComboBoxUpdate(widget);

  XtManageChild(widget);

  XtRealizeWidget(toplevel);

#if 0
  XdbPrintResources(CB_ListShell(widget));
  XdbPrintResources(widget);
#endif

  check_geometry(widget);

  LessTifTestMainLoop(toplevel);

  exit(0);
}

