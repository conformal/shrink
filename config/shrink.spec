# $shrink$

%define name		shrink
%define version		0.2.1
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
* Tue Jul 03 2011 - davec 0.2.1-1
- Create
