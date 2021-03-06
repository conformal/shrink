%define name		shrink
%define version		0.5.4
%define release		1

Name: 		%{name}
Summary: 	Library that provides a single API into compression algorithms
Version: 	%{version}
Release: 	%{release}
License: 	ISC
Group: 		System Environment/Libraries
URL:		http://opensource.conformal.com/wiki/shrink
Source: 	%{name}-%{version}.tar.gz
Buildroot:	%{_tmppath}/%{name}-%{version}-buildroot
Prefix: 	/usr
Requires:	lzo >= 2.03, xz, libbsd

%description
The shrink library provides a single API into several compression algorithms.
It enables developers to easily add compression and decompression functionality
to an existing code base. Currently it supports LZO, LZ77, and LZMA.  All of
these fine algorithms have pros and cons. LZO is the fastest by an order of
magnitude but trades of compression ratio for speed. LZ77 is the middle of the
road on both speed and compression ration. LZMA is slow but compresses the best.
The idea of this library is to provide an app writer with the capability of
using any compression/decompression algorithm without having to understand the
intricate parts.

%prep
%setup -q

%build
make

%install
make install DESTDIR=$RPM_BUILD_ROOT LOCALBASE=/usr

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
/usr/lib/libshrink.so.*


%package devel
Summary: Libraries and header files to develop applications using shrink
Group: Development/Libraries
Requires: clens-devel >= 0.0.5, lzo-devel >= 2.03, xz-devel, libbsd-devel

%description devel
This package contains the libraries, include files, and documentation to
develop applications with shrink.

%files devel
%defattr(-,root,root)
%doc /usr/share/man/man?/*
/usr/include/shrink.h
/usr/lib/libshrink.so
/usr/lib/libshrink.a

%changelog
* Fri Jun 28 2013 - dhill 0.5.4-1
- Plug memory leak on init failure
* Fri May 31 2013 - davec 0.5.3-1
- Fix OpenBSD port Makefile for modern OpenBSD ports
* Fri Jan 04 2013 - davec 0.5.2-1
- Add support for Bitrig
- Add support for cygwin
- Remove the 'version: ' prefix from shrink_verstring
* Tue Jul 17 2012 - davec 0.5.1-1
- Support clang builds
* Tue May 08 2012 - drahn 0.5.0-1
- Allow use of shrink in a threaded environment
- Not ABI compatible, but API compat layer added
* Tue Apr 24 2012 - drahn 0.4.0-1
- Cleanup manpage
- Other minor cleanup and bug fixes
* Thu Oct 27 2011 - davec 0.3.0-1
- Convert all S_* constants to SHRINK_* to prevent conflicts
- Makefile improvements
- Add build versioning
- Other minor cleanup
* Mon Aug 15 2011 - davec 0.2.3-1
- Explicity link shared lib deps
* Tue Jul 26 2011 - davec 0.2.2-1
- Don't link against clens directly from library
* Tue Jul 03 2011 - davec 0.2.1-1
- Create
