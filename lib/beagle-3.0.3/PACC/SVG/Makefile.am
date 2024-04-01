PACC_SVG_SRC_VAR = \
Canvas.cpp \
Styles.cpp \
Types.cpp
paccsvgdir = $(includedir)/SVG
paccsvg_HEADERS = \
Canvas.hpp \
Frame.hpp \
Group.hpp \
Path.hpp \
Primitives.hpp \
Styles.hpp \
Transforms.hpp \
Types.hpp
lib_LTLIBRARIES = libpacc-svg.la
INCLUDES = -I$(top_srcdir)/PACC
libpacc_svg_la_LIBADD  = \
$(top_srcdir)/PACC/Socket/libpacc-socket.la \
$(top_srcdir)/PACC/Threading/libpacc-threading.la \
$(top_srcdir)/PACC/XML/libpacc-xml.la \
$(top_srcdir)/PACC/Util/libpacc-util.la
libpacc_svg_la_LDFLAGS = -no-undefined -release @PACC_VERSION@
if FAST_COMPILATION
libpacc-svg.cpp:
	rm -f libpacc-svg.cpp
	echo "// Generated automatically by make" > libpacc-svg.cpp
	echo >> libpacc-svg.cpp
	echo $(PACC_SVG_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libpacc-svg.cpp

clean-generic:
	rm -f libpacc-svg.cpp

nodist_libpacc_svg_la_SOURCES = libpacc-svg.cpp
else
libpacc_svg_la_SOURCES = $(PACC_SVG_SRC_VAR)
endif
