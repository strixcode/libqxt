# RPM SPEC file to generate libqxt packages
#

Name:	    libqxt
Summary:    Qxt extention library for Qt 4.x
Version:    0.7.0
Release:    %{_release}%{?dist}
Group:	    System Environment/Libraries
License:    CPL or LGPLv2
Buildroot:  %{_tmppath}/%{name}-root
URL:	    http://www.libqxt.org
Source0:    %{name}-%{version}.tar.bz2

Prereq: fileutils

BuildRequires: desktop-file-utils
BuildRequires: avahi-compat-libdns_sd-devel
BuildRequires: avahi-devel
BuildRequires: db4-devel
BuildRequires: libXrandr-devel
BuildRequires: openssl-devel
BuildRequires: qt4-devel
BuildRequires: sed

%{?_qt4_version:Requires: qt4%{?_isa} >= %{_qt4_version}}
Requires: openssl

%description
LibQxt is an extension library for Qt providing a suite of cross-platform
utility classes to add functionality not readily available in the Qt toolkit
by Qt Development Frameworks, Nokia.

%prep
%setup -q -c

# Remove "rpath"
sed -i '/-rpath/d' src/qxtlibs.pri

%build
# Custom configure (not GNU)
./configure -verbose \
	-qmake-bin %{_qt4_qmake} \
	-prefix %{_prefix} \
	-libdir %{_libdir}

make %{?_smp_mflags}
make %{?_smp_mflags} docs

%install
rm -rf $RPM_BUILD_ROOT
make install INSTALL_ROOT=$RPM_BUILD_ROOT
# Strip docs -- we want them somewhere else
rm -rf $RPM_BUILD_ROOT%{_prefix}/doc/

#%clean
#rm -rf $RPM_BUILD_ROOT
#rm -rf $RPM_BUILD_DIR/%{name}-%{version}

%files
%defattr(-,root,root,-)
%{_qt4_libdir}/libQxt*.so*
%doc AUTHORS CHANGES *.txt LICENSE README

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%package devel
##############################################################################
# Development files

Summary:	Development files for %{name}
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release}
Requires:	avahi-compat-libdns_sd-devel
Requires:	avahi-devel
Requires:	db4-devel
Requires:	qt4-devel

%description devel
This package provides libraries, header files and documentation for developing
applications using LibQxt.

%files devel
%defattr(-,root,root,-)
%doc examples/ doc/html/
%{_qt4_headerdir}/*
%{_qt4_plugindir}/designer/*.so
%{_qt4_datadir}/mkspecs/features/qxt*.prf

%changelog
* Thu Sep 01 2011 Dee Holtsclaw <dholtsclaw@libqxt.org> - 0.7.0-1
- Initial SPEC setup for inclusion in Qxt builds
