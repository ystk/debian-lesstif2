/* $Header: /cvsroot/lesstif/lesstif/test/Xm/scrolledwindow/test8.c,v 1.4 2001/05/23 14:30:30 amai Exp $
** Generated by X-Designer 
** linescroll
*/
/*
**LIBS: -lXm -lXt -lX11
*/

#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>

#include <Xm/Xm.h>
#include <stdio.h>


#include <Xm/DialogS.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrolledW.h>

typedef struct canvas_s {
	Widget          da;
	Widget          hsb;
	Widget          vsb;
	int             width, height, x_offset, y_offset;
	GC              gc;
}               canvas_t, *canvas_p;

extern void     expand();
extern void     expose();
extern void     graphics_expose();
extern void     resize();
extern void     hsb_changed();
extern void     vsb_changed();
extern void adjust_size();
extern void create_gc();


static void
set_no_clip(canvas)
	canvas_p        canvas;
{
	XSetClipMask(XtDisplay(canvas->da), canvas->gc, None);
}

/*
 * XmScrollBar callbacks copy an area from the window to itself.
 */

void
hsb_changed(w, client_data, call_data)
	Widget          w;
	XtPointer       client_data;
	XtPointer       call_data;
{
	canvas_p        canvas = (canvas_p) client_data;
	int             slider_size;
	int             pixels_to_lose;
	Boolean         positive;
	Display        *display = XtDisplay(canvas->da);
	Window          window = XtWindow(canvas->da);
	int             new_value =
	((XmScrollBarCallbackStruct *) call_data)->value;
	XEvent          event;

	XSync(display, False);
	while (XCheckWindowEvent(display, window, ExposureMask, &event))
		XtDispatchEvent(&event);

	if (canvas->x_offset == new_value)
		return;
	XtVaGetValues(canvas->hsb, XmNsliderSize, &slider_size, NULL);
	/* Work out how far (and in which direction) we need to scroll */
	if ((positive = (new_value > canvas->x_offset)))
		pixels_to_lose = new_value - canvas->x_offset;
	else
		pixels_to_lose = canvas->x_offset - new_value;
	canvas->x_offset = new_value;
	if (pixels_to_lose >= slider_size)
		/* Scrolling more than a page, nothing worth copying */
		XClearArea(display, window, 0, 0, 0, 0, True);
	else {
		/* Copy useful section, and expose revealed section */
		Dimension       width, height;

		XtVaGetValues(canvas->da, XmNwidth, &width,
			      XmNheight, &height, NULL);
		set_no_clip(canvas);
		if (positive) {
			XCopyArea(display, window, window, canvas->gc,
				  pixels_to_lose, 0, width - pixels_to_lose, height, 0, 0);
			XClearArea(display, window,
				   width - pixels_to_lose, 0, pixels_to_lose, height, True);
		} else {
			XCopyArea(display, window, window, canvas->gc,
				  0, 0, width - pixels_to_lose, height, pixels_to_lose, 0);
			XClearArea(display, window, 0, 0, pixels_to_lose, height, True);
		}
	}
}

void 
vsb_changed(w, client_data, call_data)
	Widget          w;
	XtPointer       client_data;
	XtPointer       call_data;
{
	canvas_p        canvas = (canvas_p) client_data;
	int             slider_size;
	int             pixels_to_lose;
	Boolean         positive;
	XEvent          event;
	Display        *display = XtDisplay(canvas->da);
	Window          window = XtWindow(canvas->da);
	int             new_offset =
	((XmScrollBarCallbackStruct *) call_data)->value * 20;

	/*
	 * Basically the same code as before, but converts from
	 * scrollbar units (lines) to pixels.
	 */
	if (canvas->y_offset == new_offset)
		return;
	XSync(display, False);
	while (XCheckWindowEvent(display, window, ExposureMask, &event))
		XtDispatchEvent(&event);
	XtVaGetValues(canvas->vsb, XmNsliderSize, &slider_size, NULL);
	if ((positive = (new_offset > canvas->y_offset)))
		pixels_to_lose = new_offset - canvas->y_offset;
	else
		pixels_to_lose = canvas->y_offset - new_offset;
	canvas->y_offset = new_offset;
	if (pixels_to_lose >= slider_size * 20)
		/* Scrolling more than a page, nothing worth copying */
		XClearArea(display, window, 0, 0, 0, 0, True);
	else {
		/* Copy useful section, and expose revealed section */
		Dimension       width, height;

		XtVaGetValues(canvas->da, XmNwidth, &width,
			      XmNheight, &height, NULL);
		set_no_clip(canvas);
		if (positive) {
			XCopyArea(display, window, window, canvas->gc,
				  0, pixels_to_lose, width, height - pixels_to_lose, 0, 0);
			XClearArea(display, window,
				   0, height - pixels_to_lose, width, pixels_to_lose, True);
		} else {
			XCopyArea(display, window, window, canvas->gc,
				  0, 0, width, height - pixels_to_lose, 0, pixels_to_lose);
			XClearArea(display, window, 0, 0, width, pixels_to_lose, True);
		}
	}
}

/*
 * Adjust one XmScrollBar to suit proportions of XmDrawingArea * and the
 * notional canvas. adjust_scrollbar() is as before. adjust_v_scrollbar()
 * uses line units.
 */

void
adjust_scrollbar(sbar, size, visible_size, value)
	Widget          sbar;
	int             size, *value;
	Dimension       visible_size;
{
	/* Assume that minimum is 0 */
	int             slider_size = visible_size;
	int             maximum = size;

	XtVaGetValues(sbar, XmNvalue, value, NULL);
	if (slider_size > maximum)
		slider_size = maximum;
	if (*value > maximum - slider_size)
		*value = maximum - slider_size;
	XtVaSetValues(sbar, XmNvalue, *value,
		      XmNsliderSize, slider_size,
		      XmNmaximum, maximum, NULL);
}

void
adjust_v_scrollbar(sbar, size, visible_size, offset)
	Widget          sbar;
	int             size, *offset;
	Dimension       visible_size;
{
	int             slider_size = (visible_size + 19) / 20;
	int             maximum = (size + 19) / 20;
	int             value;

	XtVaGetValues(sbar, XmNvalue, &value, NULL);
	if (slider_size > maximum)
		slider_size = maximum;
	if (value > maximum - slider_size)
		value = maximum - slider_size;
	XtVaSetValues(sbar, XmNvalue, value,
		      XmNsliderSize, slider_size,
		      XmNmaximum, maximum, NULL);
	*offset = value * 20;
}

/*
 * Set XmScrollBars to suit canvas and XmDrawingArea
 */
void
adjust_size(canvas)
	canvas_p        canvas;
{
	Dimension       width, height;

	XtVaGetValues(canvas->da, XmNwidth, &width,
		      XmNheight, &height, NULL);
	adjust_v_scrollbar(canvas->vsb, canvas->height, height,
			   &canvas->y_offset);
	adjust_scrollbar(canvas->hsb, canvas->width, width,
			 &canvas->x_offset);
}

/*
 * XmDrawingArea resize callback
 */

void
resize(w, client_data, call_data)
	Widget          w;
	XtPointer       client_data;
	XtPointer       call_data;
{
	canvas_p        canvas = (canvas_p) client_data;
	adjust_size(canvas);
}
static void
set_clip(canvas, x, y, width, height)
	canvas_p        canvas;
	int             x, y, width, height;
{
	XRectangle      rect;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	XSetClipRectangles(XtDisplay(canvas->da), canvas->gc, 0, 0, &rect, 1,
			   YSorted);
}

void
expose(w, client_data, call_data)
	Widget          w;
	XtPointer       client_data;
	XtPointer       call_data;
{
	int             x, y;
	XExposeEvent   *e =
	&((XmDrawingAreaCallbackStruct *) call_data)->event->xexpose;
	canvas_p        canvas = (canvas_p) client_data;
	Display        *display = XtDisplay(canvas->da);

	if (!canvas->gc)
		adjust_size(canvas);
	create_gc(canvas);
	set_clip(canvas, e->x, e->y, e->width, e->height);
	x = ((canvas->x_offset + e->x) / 100) * 100;
	while (x < canvas->width
	       && x < canvas->x_offset + e->x + e->width) {
		y = ((canvas->y_offset + e->y) / 20) * 20;
		while (y < canvas->height
		       && y < canvas->y_offset + e->y + e->height + 10) {
			char            buf[24];
			sprintf(buf, "%d,%d", x, y);
			XDrawString(display, XtWindow(canvas->da), canvas->gc,
			    x - canvas->x_offset, y - canvas->y_offset + 10,
				    buf, strlen(buf));
			y = y + 20;
		}
		x = x + 100;
	}
}

void 
graphics_expose(w, client_data, event, continue_to_dispatch)
	Widget          w;
	XtPointer       client_data;
	XEvent         *event;
	Boolean        *continue_to_dispatch;
{
	if (event->type == GraphicsExpose) {
		XmDrawingAreaCallbackStruct cb;
		cb.reason = XmCR_EXPOSE;
		cb.event = event;
		cb.window = XtWindow(w);
		XtCallCallbacks(w, XmNexposeCallback, &cb);
	}
}

void
expand(w, client_data, call_data)
	Widget          w;
	XtPointer       client_data;
	XtPointer       call_data;
{
	/* Double the size of the (notional) canvas */
	canvas_p canvas = (canvas_p)client_data;
	canvas->width=2*canvas->width;
	canvas->height=2*canvas->height;
	adjust_size(canvas);
	/*
	* Generate expose event for entire window (strictly
	* only needed if XmDrawingArea is larger than canvas.
	*/
	XClearArea(XtDisplay(canvas->da), XtWindow(canvas->da),
		   0, 0, 0, 0, True);
}
void 
create_gc(canvas)
	canvas_p        canvas;
{
	if (canvas->gc == 0) {
		XGCValues       values;
		/* Create GC with foreground taken from the XmDrawingArea */
		XtVaGetValues(canvas->da,
			      XmNforeground, &values.foreground, NULL);
		canvas->gc = XCreateGC(XtDisplay(canvas->da),
				       XtWindow(canvas->da),
				       GCForeground, &values);
	}
}
Widget
create_appshell(display, app_name, app_argc, app_argv, canvas)
	Display        *display;
	char           *app_name;
	int             app_argc;
	char          **app_argv;
	canvas_p        canvas;
{
	Widget          appshell = (Widget) NULL;
	Widget          form = (Widget) NULL;
	Widget          bigger = (Widget) NULL;
	Widget          scrollw = (Widget) NULL;
	Arg             al[64];	/* Arg List */
	register int    ac = 0;	/* Arg Count */
	XmString        xmstring;
	Widget          children[3];

	XtSetArg(al[ac], XmNallowShellResize, TRUE); ac++;
	XtSetArg(al[ac], XmNtitle, "Application Scrolling"); ac++;
	XtSetArg(al[ac], XmNargc, app_argc); ac++;
	XtSetArg(al[ac], XmNargv, app_argv); ac++;
	XtSetArg(al[ac], XmNwidth, 200); ac++;
	XtSetArg(al[ac], XmNheight, 200); ac++;
	appshell = XtAppCreateShell(app_name, "XApplication", applicationShellWidgetClass, display, al, ac);
	ac = 0;
	XtSetArg(al[ac], XmNautoUnmanage, FALSE); ac++;
	form = XmCreateForm(appshell, "form", al, ac);
	ac = 0;
	xmstring = XmStringCreateLtoR("Expand drawing", (XmStringCharSet) XmFONTLIST_DEFAULT_TAG);
	XtSetArg(al[ac], XmNlabelString, xmstring); ac++;
	bigger = XmCreatePushButton(form, "bigger", al, ac);
	XmStringFree(xmstring);

	XtAddCallback(bigger, XmNactivateCallback, expand,
		      (XtPointer) canvas);
	canvas->width = 200;
	canvas->height = 200;

	ac = 0;
	XtSetArg(al[ac], XmNscrollingPolicy, XmAPPLICATION_DEFINED); ac++;
	scrollw = XmCreateScrolledWindow(form, "scrollw", al, ac);
	ac = 0;
	XtSetArg(al[ac], XmNorientation, XmHORIZONTAL); ac++;
	canvas->hsb = XmCreateScrollBar(scrollw, "horscrollbar", al, ac);
	ac = 0;
	XtSetArg(al[ac], XmNorientation, XmVERTICAL); ac++;
	canvas->vsb = XmCreateScrollBar(scrollw, "vertscrollbar", al, ac);
	ac = 0;
	canvas->da = XmCreateDrawingArea(scrollw, "da", al, ac);

	XtAddCallback (canvas->hsb, XmNdragCallback, hsb_changed,
		      (XtPointer)canvas);
	XtAddCallback (canvas->vsb, XmNdragCallback, vsb_changed,
		      (XtPointer)canvas);

	XtAddEventHandler (canvas->da, NoEventMask, True,
		      graphics_expose, NULL);

	XtAddCallback(canvas->hsb, XmNvalueChangedCallback, hsb_changed,
		      (XtPointer) canvas);
	XtAddCallback(canvas->vsb, XmNvalueChangedCallback, vsb_changed,
		      (XtPointer) canvas);
	XtAddCallback(canvas->da, XmNexposeCallback, expose,
		      (XtPointer) canvas);
	XtAddCallback(canvas->da, XmNresizeCallback, resize,
		      (XtPointer) canvas);

	ac = 0;
	XtSetArg(al[ac], XmNtopAttachment, XmATTACH_FORM); ac++;
	XtSetArg(al[ac], XmNleftAttachment, XmATTACH_FORM); ac++;
	XtSetValues(bigger, al, ac);

	ac = 0;
	XtSetArg(al[ac], XmNtopAttachment, XmATTACH_WIDGET); ac++;
	XtSetArg(al[ac], XmNtopWidget, bigger); ac++;
	XtSetArg(al[ac], XmNleftAttachment, XmATTACH_FORM); ac++;
	XtSetArg(al[ac], XmNrightAttachment, XmATTACH_FORM); ac++;
	XtSetArg(al[ac], XmNbottomAttachment, XmATTACH_FORM); ac++;
	XtSetValues(scrollw, al, ac);
	ac = 0;
	children[ac++] = canvas->da;
	children[ac++] = canvas->hsb;
	children[ac++] = canvas->vsb;
	XtManageChildren(children, ac);
	ac = 0;
	children[ac++] = bigger;
	children[ac++] = scrollw;
	XtManageChildren(children, ac);
	ac = 0;
	XtManageChild(form);
	return appshell;
}
XtAppContext app_context;
Display *display;       /*  Display             */

int main (argc,argv)
int    argc;
char            **argv;
{
	canvas_t canvas;
	Widget appshell;
	XtSetLanguageProc ( (XtAppContext) NULL, (XtLanguageProc) NULL, (XtPointer) NULL );
	XtToolkitInitialize ();
	app_context = XtCreateApplicationContext ();
	display = XtOpenDisplay (app_context, NULL, argv[0], "XApplication",
				 NULL, 0, &argc, argv);
	if (!display)
	{
	    printf("%s: can't open display, exiting...\n", argv[0]);
	    exit (-1);
	}
	canvas.gc = 0;
	appshell = create_appshell(display, "XApplication", argc, &argv, &canvas);
	XtRealizeWidget(appshell);

  
{
    static XtWidgetGeometry Expected[] = {
   CWWidth | CWHeight            ,  500,  500,  200,  200, 0,0,0, /* form */
   CWWidth | CWHeight | CWX | CWY,    0,    0,   96,   25, 0,0,0, /* bigger */
   CWWidth | CWHeight | CWX | CWY,    0,   25,  200,  175, 0,0,0, /* scrollw */
   CWWidth | CWHeight | CWX | CWY,    0,  160,  181,   15, 0,0,0, /* horscrollbar */
   CWWidth | CWHeight | CWX | CWY,  185,    0,   15,  156, 0,0,0, /* vertscrollbar */
   CWWidth | CWHeight | CWX | CWY,    0,    0,  181,  156, 0,0,0, /* da */ 
    };
    PrintDetails(appshell,Expected);
};
  LessTifTestMainLoop(appshell);

	exit (0);
}

