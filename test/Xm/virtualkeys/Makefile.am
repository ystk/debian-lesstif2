#
# $Header: /cvsroot/lesstif/lesstif/test/Xm/virtualkeys/Makefile.am,v 1.27 2008/01/02 19:48:23 dannybackx Exp $
#
MAINTAINERCLEANFILES=Makefile.in

AUX_OBJS =
noinst_PROGRAMS= test1

LTTEST= $(top_builddir)/common/libLtTest.a

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

if MotifTests

motif_TESTS = $(OBJECTS:.o=.motif)

.c.mo: 
	$(CC) $(DEFS) $(MOTIF_CFLAGS) $(X_CFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o $@ $< 

.mo.motif: 
	${LINK} $< $(AUX_OBJS) @MOTIF_LIBS@ $(LTTEST) ${X_LIBS} -lXm -lXt ${X_PRE_LIBS} -lX11 ${X_EXTRA_LIBS} 

motif-tests: ${motif_TESTS}

${motif_TESTS}: $(AUX_OBJS)

endif
clean-local:
	rm -rf *.motif *.mo LessTif LessTif.sum LessTif.log site.exp xlab/*.p*
