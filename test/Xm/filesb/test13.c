/* $Header: /cvsroot/lesstif/lesstif/test/Xm/filesb/test13.c,v 1.8 2001/06/15 09:17:36 amai Exp $
   From:        Yasushi Yamasaki <yamapu@osk3.3web.ne.jp>
   To:          lesstif@hungry.com
   Subject:     bug report
   Date:        Sun, 30 Aug 1998 23:55:47 +0900
   Cc:          yamapu@osk3.3web.ne.jp
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <Xm/Xm.h>
#include <Xm/FileSB.h>


static char *FallBack[] = {
		"*.geometrySlop: 0",
		NULL
};

int
main(int argc, char *argv[])
{
    XtAppContext app;
    Widget shell, w;
    void *before;
    void *after;
    int iter = 0;
    int diff = 0;
    int total = 0;

/*  make_tmp_dir_tree(); */
    shell = XtAppInitialize(&app, "Test", NULL, 0,
			    &argc, argv, FallBack, NULL, 0);
    while (iter < 100)
    {
	before = sbrk((ptrdiff_t) 0);
	w = XtCreateWidget("name", xmFileSelectionBoxWidgetClass, shell, NULL, 0);
	XtDestroyWidget(w);
	after = sbrk((ptrdiff_t) 0);
	if ((int)((char *)after - (char *)before) > 0)
	{
	    if (iter != 0)
	    {
		/*
		printf("%i %i %p %i\n", iter, iter - diff, after - before, (after - before) / (iter - diff));
		*/
		total += (int)((char *)after - (char *)before);
	    }
	    diff = iter;
	}
	iter++;
    }
    printf("leaking %i bytes per Create/Destroy\n", total / iter);
    exit((int)(total / iter));
}
