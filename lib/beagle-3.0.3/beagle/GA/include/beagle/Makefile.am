# Stuff for precompiled headers
INCLUDES = -I$(top_srcdir)/PACC -I$(top_srcdir)/beagle/include -I$(top_srcdir)/beagle/GA/include
pch: $(HEADERS)
	rm -f GA.hpp.gch
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) -x c++-header $(CXXFLAGS) -c -o GA.hpp.gch  GA.hpp
install-data-hook:
	if test -f GA.hpp.gch; then $(INSTALL_DATA) GA.hpp.gch $(DESTDIR)$(includedir)/GA.hpp.gch; fi
clean-generic:
	if test -f GA.hpp.gch; then rm -f GA.hpp.gch; fi

beagledir = $(includedir)/beagle
beagle_HEADERS = GA.hpp
SUBDIRS = GA



