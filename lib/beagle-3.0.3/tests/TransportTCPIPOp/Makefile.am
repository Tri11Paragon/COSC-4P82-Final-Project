if COMPILE_TESTS
noinst_PROGRAMS = testTransportSendTCPIPOp testTransportReceiveTCPIPOp
INCLUDES        = \
-I.\
-I$(top_srcdir)/PACC\
-I$(top_srcdir)/beagle/include\
-I$(top_srcdir)/beagle/GP/include\
-I$(top_srcdir)/tests/Common
noinst_HEADERS = \
DefaultSignalHandler.hpp \
TransportReceiveOp.hpp \
TransportReceiveTCPIPOp.hpp \
TransportSendOp.hpp \
TransportSendTCPIPOp.hpp \
TransportTCPIPReceiver.hpp
testTransportSendTCPIPOp_LDADD = \
$(top_srcdir)/beagle/src/libbeagle.la \
$(top_srcdir)/beagle/GP/src/libbeagle-GP.la \
$(top_srcdir)/tests/Common/libtest.la \
$(top_srcdir)/PACC/Socket/libpacc-socket.la\
$(top_srcdir)/PACC/Threading/libpacc-threading.la\
$(top_srcdir)/PACC/XML/libpacc-xml.la\
$(top_srcdir)/PACC/Util/libpacc-util.la\
$(top_srcdir)/PACC/Math/libpacc-math.la
testTransportSendTCPIPOp_SOURCES = \
TestTransportSendTCPIPOp.cpp \
TransportSendOp.cpp \
TransportSendTCPIPOp.cpp
testTransportReceiveTCPIPOp_LDADD = \
$(top_srcdir)/beagle/src/libbeagle.la \
$(top_srcdir)/beagle/GP/src/libbeagle-GP.la \
$(top_srcdir)/tests/Common/libtest.la \
$(top_srcdir)/PACC/Socket/libpacc-socket.la\
$(top_srcdir)/PACC/Threading/libpacc-threading.la\
$(top_srcdir)/PACC/XML/libpacc-xml.la\
$(top_srcdir)/PACC/Util/libpacc-util.la\
$(top_srcdir)/PACC/Math/libpacc-math.la
testTransportReceiveTCPIPOp_SOURCES = \
TestTransportReceiveTCPIPOp.cpp \
TransportReceiveOp.cpp \
TransportReceiveTCPIPOp.cpp \
TransportTCPIPReceiver.cpp \
DefaultSignalHandler.cpp
endif COMPILE_TESTS
