%{!?directory:%define directory /usr}

%define buildroot %{_tmppath}/%{name}
%define packckname opencv

Name:          tcl-opencv
Summary:       Tcl extension for OpenCV library
Version:       0.8
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
%if 0%{?sle_version} >= 150300 && 0%{?is_opensuse}
BuildRequires: opencv-devel >= 4.4
%else
BuildRequires: opencv-devel >= 3.3
%endif
Requires:      tcl >= 8.6
%if 0%{?sle_version} >= 150300 && 0%{?is_opensuse}
Requires:      opencv >= 4.4
%else
Requires:      opencv >= 3.3
%endif
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
%if 0%{?sle_version} < 150300 && 0%{?is_opensuse}
    --with-opencv4=no --with-sift=no \
%endif
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
