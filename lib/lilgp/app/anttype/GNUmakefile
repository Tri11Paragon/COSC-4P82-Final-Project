#
# GNU makefile, application portion.
#
# "make" or "make all" to build executable.
# "make clean" to delete object code.
#
# some users have reported a "high performance arithmetic trap" on DEC Alphas
# and on LINUX.  if you get this, try adding "-DTOLERANCE_ZERO" to CFLAGS --
# it apparently works on Alphas; don't know about Linux.
#

TARGET = gp
KERNELDIR = ../../kernel
#CC = gcc
#CFLAGS = -O2

# For POSIX style multi-threading under Solaris 2.4 or greater use:
#CC = cc
#CFLAGS = -fast -xO4 -DPOSIX_MT -D_REENTRANT
#LIBS = -lpthread

# For POSIX multi-threading under other OSes try:
#CC = CC
#CFLAGS = -O -DPOSIX_MT
#LIBS = -lpthread

# For Solaris style multi-threading under Solaris 2.x use:
#CC = cc
#CFLAGS = -mt -fast -xO4 -DSOLARIS_MT
#LIBS = -mt


# if you want multithreading with POSIX threads,
# this will do the trick for Alphas running Digital Unix 4.0 -- Sean
CC = cc
CFLAGS = -O3 -DPOSIX_MT -ansi
LIBS =  -lpthread -lmach -lexc -lc


uobjects = function.o app.o
uheaders = appdef.h app.h function.h

include $(KERNELDIR)/GNUmakefile.kernel
