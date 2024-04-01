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

## Generic settings for *nix with vendors cc compiler 
 CC = cc
 CFLAGS = -O2

## Generic settings for *nix with GCC
# CC = gcc
# CFLAGS = -O2

## For Linux with GCC uncomment the next line
# CFLAGS += -mieee-fp

## For a multi-threaded kernel on most systems uncomment the next 2 lines
 CFLAGS += -DPOSIX_MT
 LIBS += -lpthread

## For POSIX style multi-threading under Solaris 2.4 or greater use:
# CFLAGS += -fast -xO4 -D_REENTRANT

## For Solaris style multi-threading under Solaris 2.x use:
# CFLAGS = -mt -fast -xO4 -DSOLARIS_MT
# LIBS += -mt

## For PSIX style multi-threading on Alphas running Digital Unix 4.0 -- Sean
# CFLAGS += -ansi
# LIBS += -lmach -lexc -lc


uobjects = function.o app.o
uheaders = appdef.h app.h function.h

include $(KERNELDIR)/GNUmakefile.kernel
