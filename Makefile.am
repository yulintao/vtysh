## Process this file with automake to produce Makefile.in.

SUBDIRS = lib

DIST_SUBDIRS = lib

EXTRA_DIST = aclocal.m4 SERVICES TODO REPORTING-BUGS INSTALL.quagga.txt \
	update-autotools \
	vtysh/Makefile.in vtysh/Makefile.am \

if HAVE_PANDOC

HACKING.pdf: HACKING.md
	pandoc -o $@ $<

clean-local:
	-$(LATEXMK) -C HACKING.tex

endif

ACLOCAL_AMFLAGS = -I m4
