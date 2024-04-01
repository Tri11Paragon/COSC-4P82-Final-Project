PACC_XML_SRC_VAR = \
Document.cpp \
Finder.cpp \
Node.cpp \
Streamer.cpp
paccxmldir = $(includedir)/XML
paccxml_HEADERS = \
Attribute.hpp \
Document.hpp \
Finder.hpp \
Iterator.hpp \
Node.hpp \
Streamer.hpp
lib_LTLIBRARIES = libpacc-xml.la
INCLUDES = -I$(top_srcdir)/PACC
libpacc_xml_la_LIBADD  = $(top_srcdir)/PACC/Util/libpacc-util.la
libpacc_xml_la_LDFLAGS = -no-undefined -release @PACC_VERSION@
if FAST_COMPILATION
libpacc-xml.cpp:
	rm -f libpacc-xml.cpp
	echo "// Generated automatically by make" > libpacc-xml.cpp
	echo >> libpacc-xml.cpp
	echo $(PACC_XML_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libpacc-xml.cpp

clean-generic:
	rm -f libpacc-xml.cpp

nodist_libpacc_xml_la_SOURCES = libpacc-xml.cpp
else
libpacc_xml_la_SOURCES = $(PACC_XML_SRC_VAR)
endif