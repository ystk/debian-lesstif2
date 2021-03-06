/*
 * Simple test for print-to-file with XmNpageSetupCallback.
 *	Currently works with OpenMotif, not with LessTif.
 */
static const char rcsid[] = "$Header: /cvsroot/lesstif/lesstif/test/Xm-2.1/print/test9.c,v 1.3 2001/01/27 16:36:09 dannybackx Exp $";

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <Xm/XmAll.h>

Widget		shell, tw;
Display		*pdpy;
XPContext	pctxt;
short		nrows = 80;
int		nlines = 481, npages, page, i;

/*
 * Use fallback resources - for now - to tro to get a reasonable size text font.
 * This means use a BIG font for printing.
 */
static char *fallback[] = {
  "*XmPrintShell*fontList: -*-courier-medium-r-normal--*-120-300-300-*-*-iso8859-1",
  "*XmText.foreground: black",
  "*XmText.background: white",
  "*XmText.marginWidth: 100",
  "*XmText.marginHeight: 300",
  "*XmForm.background: green",
  "*XmArrowButton.background: red",
  "*XmArrowButton.foreground: yellow",
  "*XmPushButton.background: black",
  "*XmPushButton.foreground: green",
  NULL
};

void EndJobCB(Widget w, XtPointer client, XtPointer call)
{
	XtDestroyWidget(shell);
	fprintf(stderr, "EndJobCB:XpDestroyContext\n");
	XpDestroyContext(XtDisplay(shell), pctxt);
	XtCloseDisplay(XtDisplay(shell));
}

void PrintPageCB(Widget w, XtPointer client, XtPointer call)
{
	XmPrintShellCallbackStruct *cbp = (XmPrintShellCallbackStruct *)call;

	if (! XmIsText(tw)) {
		cbp->last_page = True;
		return;
	}

	/* Next page */
	page ++;

	if (page < npages) {
		fprintf(stderr, "PrintPageCB page %d/%d (top is %d)\n",
			page, npages, (int)XmTextGetTopCharacter(tw));

		XmTextScroll(tw, nrows);
	} else {
		fprintf(stderr, "PrintPageCB last pg (top is %d)\n",
			(int)XmTextGetTopCharacter(tw));
		cbp->last_page = True;
	}
}

void Redraw(Widget w, XtPointer client, XtPointer call)
{
	fprintf(stderr, "Redraw()\n");
}

void FinishPrintToFile(Display		*display,
			XPContext	context,
			XPGetDocStatus	status,
			XPointer	client)

{
	fprintf(stderr, "FinishPrintToFile()\n");
	exit(0);
}

void CreateWindow(Widget top, Display *pdpy, XPContext pctxt, Screen *scr)
{
	Widget				form;
	FILE				*f;
	static char			buffer[64000];
	Arg				al[20];
	int				ac;
	unsigned short			wid, ht;
	XRectangle			rect;
	Dimension			width, height;

	int				save_data = XPGetData;

	/*
	 * Here we are sure that printer name, print display, screen are
	 * initialized, and a print context has been created.
	 */

	if (XpGetPageDimensions(pdpy, pctxt, &wid, &ht, &rect)) {
		fprintf(stderr, "Paper size is %d %d\n", wid, ht);
	} else {
		fprintf(stderr, "test9: failed to get Page Dimensions\n");
		exit(1);
	}

	ac = 0;
	/* Size from paper size */
	XtSetArg(al[ac], XmNheight, ht); ac++;
	XtSetArg(al[ac], XmNwidth, wid); ac++;
	shell = XmPrintSetup(top, scr, "Print", al, ac);

	XtAddCallback(shell, XmNendJobCallback, EndJobCB, NULL);
	XtAddCallback(shell, XmNpageSetupCallback, PrintPageCB, NULL);

	/* start job must precede XpGetDocumentData in XmPrintToFile */
	fprintf(stderr, "test9: XpStartJob\n");
	XpStartJob(XtDisplay(shell), save_data);

	/* Maintain sequence StartJob - GetDocument */
	fprintf(stderr, "test9: XFlush\n");
	XFlush(XtDisplay(shell));

	fprintf(stderr, "test9: XmPrintToFile()\n");
	XmPrintToFile(XtDisplay(shell), "test9.out", FinishPrintToFile, NULL);
	/*
	 * Now we can get the size of the shell, create widgets, etc.
	 */
	XtVaGetValues(shell,
			XmNheight,	&height,
			XmNwidth,	&width,
		NULL);
	/*
	 * Create a text widget as big as the page
	 */
	fprintf(stderr, "test9: XmCreateText(wid %d ht %d)\n",
		width, height);

	ac=0;
	XtSetArg(al[ac], XmNheight, height); ac++;
	XtSetArg(al[ac], XmNwidth, width); ac++;
	form = XmCreateForm(shell, "form", al, ac);
	XtManageChild(form);

	/* Re-use settings from above */
	XtSetArg(al[ac], XmNeditMode, XmMULTI_LINE_EDIT); ac++;
	XtSetArg(al[ac], XmNtopAttachment, XmATTACH_FORM); ac++;
	XtSetArg(al[ac], XmNbottomAttachment, XmATTACH_FORM); ac++;
	XtSetArg(al[ac], XmNleftAttachment, XmATTACH_FORM); ac++;
	XtSetArg(al[ac], XmNrightAttachment, XmATTACH_FORM); ac++;

#if 0
	/* Text */
	tw = XmCreateText(form, "tw", al, ac);
	XtManageChild(tw);

	/*
	 * Get some data in here - the LGPL text (you probably
	 * need to be in the LessTif source tree for this to work).
	 */
	for (i=0; i<10; i++) {
		f = fopen("COPYING.LIB", "r");
		if (f)
			break;
		chdir("..");
	}
	if (f) {
		fread(buffer, 1, sizeof(buffer), f);
		fclose(f);
	}
	XmTextSetString(tw, buffer);
	XmTextShowPosition(tw, 0);
#else
	tw = XmCreateArrowButton(form, "tw", al, ac);
	XtManageChild(tw);
#endif

	fprintf(stderr, "test9: XtRealizeWidget()\n");
	XtRealizeWidget(shell);

	if (XmIsText(tw)) {
		/* Get data to calculate number of pages to print */
		XtVaGetValues(tw,
				XmNrows,		&nrows,
				XmNtotalLines,		&nlines,
			NULL);

		/* Calculate number of pages to print */
		npages = nlines / nrows + 1;

		fprintf(stderr, "Text lines %d rows %d pages %d\n",
			nlines, nrows, npages);
	} else {
		npages=1;
	}
}

int
main(int argc, char *argv[])
{
	XtAppContext	appc;
	Widget		top, arrow, print, fsb;
	XPPrinterList	plist;
	int		nlist, i;
	Screen		*pscreen;
	Window		win;
	Arg		al[10];
	int		ac;

	top = XtVaAppInitialize(&appc, "drawingArea", NULL, 0,
                               &argc, argv, fallback, NULL);

	arrow = XtVaCreateManagedWidget("Button1", xmArrowButtonWidgetClass, top, 
			XmNarrowDirection,	XmARROW_UP,
		NULL);

	XtRealizeWidget(top);

	pdpy = XtOpenDisplay(appc,
		/* Display String */	":1",
		"print",
		"Print",
		NULL,
		0,
		&argc, argv);
	if (!pdpy) {
		fprintf(stderr, "Cannot connect to :1\n");
		exit(1);
	}

	plist = XpGetPrinterList(pdpy, "", &nlist);

	if (nlist == 0) {
		fprintf(stderr, "XpGetPrinterList() : No printers\n");
		exit(1);
	}

	pctxt = XpCreateContext(pdpy, plist[0].name);
	XpFreePrinterList(plist);

	XpSetContext(pdpy, pctxt);
	pscreen = XpGetScreenOfContext(pdpy, pctxt);

	CreateWindow(top, pdpy, pctxt, pscreen);

	LessTifTestMainLoop(top);
	exit(0);
}
