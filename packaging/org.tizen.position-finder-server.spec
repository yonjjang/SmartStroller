Name:       org.tizen.position-finder-server
%define alias org.tizen.position-finder-server
Summary:    IoTivity Application
Group:      Applications/Core Applications
Version:    0.0.1
Release:    1
License:    Apache-2.0
Provides:   org.tizen.position-finder-server = %{version}-%{release}
Source0:    %{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  hash-signer
BuildRequires:  pkgconfig(capi-appfw-application)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(libtzplatform-config)
BuildRequires:  pkgconfig(capi-appfw-service-application)
BuildRequires:	pkgconfig(capi-system-peripheral-io)
BuildRequires:	pkgconfig(ecore)
BuildRequires:	pkgconfig(eina)
BuildRequires:	pkgconfig(iotcon)

%description
Server for Position Finder

%prep
%setup -q

%build

%define _pkg_dir %{TZ_SYS_RO_APP}/%{alias}
%define _pkg_shared_dir %{_pkg_dir}/shared
%define _pkg_data_dir %{_pkg_dir}/data
%define _sys_icons_dir %{_pkg_shared_dir}/res
%define _sys_packages_dir %{TZ_SYS_RO_PACKAGES}
%define _sys_license_dir %{TZ_SYS_SHARE}/license

%ifarch %{arm}
export CFLAGS="$CFLAGS -DTIZEN_BUILD_TARGET"
export CXXFLAGS="$CXXFLAGS -DTIZEN_BUILD_TARGET"
export FFLAGS="$FFLAGS -DTIZEN_BUILD_TARGET"
%else
export CFLAGS="$CFLAGS -DTIZEN_BUILD_EMULATOR"
export CXXFLAGS="$CXXFLAGS -DTIZEN_BUILD_EMULATOR"
export FFLAGS="$FFLAGS -DTIZEN_BUILD_EMULATOR"
%endif

cmake . -DINSTALL_PREFIX=%{_pkg_dir} \
	-DSYS_ICONS_DIR=%{_sys_icons_dir} \
	-DSYS_PACKAGES_DIR=%{_sys_packages_dir}
make %{?jobs:-j%jobs}

%install
%make_install

%define tizen_sign 1
%define tizen_sign_base %{_pkg_dir}
%define tizen_sign_level platform
%define tizen_author_sign 1
%define tizen_dist_sign 1
#%find_lang position-finder-server

%post
/sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest org.tizen.position-finder-server.manifest
%defattr(-,root,root,-)
%{_pkg_dir}/bin/position-finder-server
%{_pkg_dir}/res/*.dat
%{_sys_packages_dir}/org.tizen.position-finder-server.xml
%{_sys_icons_dir}/position_finder_server.png
%{_pkg_dir}/author-signature.xml
%{_pkg_dir}/signature1.xml