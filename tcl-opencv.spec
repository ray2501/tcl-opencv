%{!?directory:%define directory /usr}

%define buildroot %{_tmppath}/%{name}
%define packckname opencv

Name:          tcl-opencv
Summary:       Tcl extension for OpenCV library
Version:       0.12
Release:       0
License:       MIT
Group:         Development/Libraries/Tcl
Source:        %{name}-%{version}.tar.gz
URL:           https://github.com/ray2501/tcl-opencv
BuildRequires: pkg-config
BuildRequires: autoconf
BuildRequires: make
BuildRequires: gcc-c++
BuildRequires: tcl-devel >= 8.6
BuildRequires: opencv-devel >= 4.5
Requires:      tcl >= 8.6
Requires:      opencv >= 4.5
BuildRoot:     %{buildroot}

%description
It is a Tcl extension for OpenCV library.

OpenCV (Open Source Computer Vision Library) is a library of programming
functions mainly aimed at real-time computer vision.

%prep
%setup -q -n %{name}-%{version}

%build
export CC=g++
CFLAGS="%optflags" ./configure \
	--prefix=%{directory} \
	--exec-prefix=%{directory} \
	--libdir=%{directory}/%{_lib}
make

%install
make DESTDIR=%{buildroot} pkglibdir=%{tcl_archdir}/%{packckname}%{version} install

%clean
rm -rf %buildroot

%files
%defattr(-,root,root)
%doc README.md BUILDING.md LICENSE
%{tcl_archdir}
