# Stuff for precompiled headers
INCLUDES = -I$(top_srcdir)/PACC -I$(top_srcdir)/beagle/include -I$(top_srcdir)/beagle/GA/include -I$(top_srcdir)/beagle/GP/include
pch: $(HEADERS)
	rm -f GP.hpp.gch
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) -x c++-header $(CXXFLAGS) -c -o GP.hpp.gch GP.hpp
install-data-hook:
	if test -f GP.hpp.gch; then $(INSTALL_DATA) GP.hpp.gch $(DESTDIR)$(includedir)/GP.hpp.gch; fi
clean-generic:
	if test -f GP.hpp.gch; then rm -f GP.hpp.gch; fi

beagledir = $(includedir)/beagle
beagle_HEADERS = GP.hpp
SUBDIRS = GP
