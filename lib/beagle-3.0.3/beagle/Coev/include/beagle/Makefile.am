# Stuff for precompiled headers
INCLUDES = -I$(top_srcdir)/PACC -I$(top_srcdir)/beagle/include -I$(top_srcdir)/beagle/GA/include -I$(top_srcdir)/beagle/GP/include -I$(top_srcdir)/beagle/Coev/include
pch: $(HEADERS)
	rm -f Coev.hpp.gch
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) -x c++-header $(CXXFLAGS) -c -o Coev.hpp.gch Coev.hpp
install-data-hook:
	if test -f Coev.hpp.gch; then $(INSTALL_DATA) Coev.hpp.gch $(DESTDIR)$(includedir)/Coev.hpp.gch; fi
clean-generic:
	if test -f Coev.hpp.gch; then rm -f Coev.hpp.gch; fi

beagledir = $(includedir)/beagle
beagle_HEADERS = Coev.hpp
SUBDIRS = Coev
