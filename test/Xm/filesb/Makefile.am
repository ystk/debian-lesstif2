#
# $Header: /cvsroot/lesstif/lesstif/test/Xm/filesb/Makefile.am,v 1.43 2008/01/02 19:48:21 dannybackx Exp $
#
MAINTAINERCLEANFILES=Makefile.in

EXTRA_SOURCE = mkdirtree.c mkdirtree.h

EXTRA_DIST = $(EXTRA_SOURCE)

AUX_OBJS = mkdirtree.o
noinst_PROGRAMS= test1  test2  test3  test4  test5  test6  test7  test8 \
                 test9  test10 test11 test12 test13 test14 test15 \
		 test16 test17 test18 test19 test20 test21

LTTEST  = $(top_builddir)/common/libLtTest.a


LDADD	= $(AUX_OBJS)  \
	  $(LTTEST)  \
	  $(top_builddir)/../lib/Xm$(version_suffix)/libXm.la $(XPLIB) \
	  ${X_LIBS} -lXt ${X_PRE_LIBS} -lX11 ${X_EXTRA_LIBS} 

AM_CPPFLAGS= -I$(top_srcdir)/../include/Motif$(include_version_suffix)  \
	  -I$(top_builddir)/../include/Motif$(include_version_suffix)  \
	  -I$(top_srcdir)/../include/Motif-1.2  \
	  -I$(top_builddir)/../include/Motif-1.2  \
	  $(X_CFLAGS) 

$(LTTEST): 
	( cd $(top_builddir)/common ; $(MAKE) )

SUFFIXES = .motif .mo

clean-local:
	rm -rf *.motif *.mo LessTif LessTif.sum LessTif.log site.exp xlab/*.p*

if MotifTests

motif_TESTS = $(OBJECTS:.o=.motif)

.c.mo: 
	$(CC) $(DEFS) $(MOTIF_CFLAGS) $(X_CFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o $@ $< 

.mo.motif: 
	${LINK} $< $(AUX_OBJS) @MOTIF_LIBS@ $(LTTEST) ${X_LIBS} -lXm -lXt ${X_PRE_LIBS} -lX11 ${X_EXTRA_LIBS} 

motif-tests: ${motif_TESTS}

${motif_TESTS}: $(AUX_OBJS)
endif
