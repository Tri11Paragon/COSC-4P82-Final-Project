PACC_SOCKET_SRC_VAR = \
Address.cpp \
Cafe.cpp \
ConnectedUDP.cpp \
Exception.cpp \
Port.cpp \
TCP.cpp \
TCPServer.cpp \
UDP.cpp \
UDPServer.cpp
paccsocketdir = $(includedir)/Socket
paccsocket_HEADERS = \
Address.hpp \
Cafe.hpp \
ConnectedUDP.hpp \
Exception.hpp \
Port.hpp \
TCP.hpp \
TCPServer.hpp \
UDP.hpp \
UDPServer.hpp
lib_LTLIBRARIES = libpacc-socket.la
INCLUDES = -I$(top_srcdir)/PACC
libpacc_socket_la_LIBADD  = \
$(top_srcdir)/PACC/Threading/libpacc-threading.la \
$(top_srcdir)/PACC/Util/libpacc-util.la
libpacc_socket_la_LDFLAGS = -no-undefined -release @PACC_VERSION@
if FAST_COMPILATION
libpacc-socket.cpp:
	rm -f libpacc-socket.cpp
	echo "// Generated automatically by make" > libpacc-socket.cpp
	echo >> libpacc-socket.cpp
	echo $(PACC_SOCKET_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libpacc-socket.cpp

clean-generic:
	rm -f libpacc-socket.cpp

nodist_libpacc_socket_la_SOURCES = libpacc-socket.cpp
else
libpacc_socket_la_SOURCES = $(PACC_SOCKET_SRC_VAR)
endif
