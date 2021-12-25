# Building this extension

## UNIX BUILD

Before building tcl-opencv, you need have OpenCV developmen files.

You also need a c++ compiler and `pkg-config`.

Building under most UNIX systems is easy, just run the configure script
and then run make. For more information about the build process, see
the tcl/unix/README file in the Tcl src dist. The following minimal
example will install the extension in the /opt/tcl directory.

    $ export CC=g++
    $ cd tcl-opencv
    $ ./configure --prefix=/opt/tcl
    $ make
    $ make install

If you need setup directory containing tcl configuration (tclConfig.sh),
below is an example:

    $ export CC=g++
    $ cd tcl-opencv
    $ ./configure --with-tcl=/opt/activetcl/lib
    $ make
    $ make install

Thanks for Christian's refactoring code, you can try to build this
extension by using OpenCV 3.x. This extension will use pkg-config
to detect opencv4. If opencv4 does not exist, this extension will
try to build with OpenCV 3.x.

SIFT (Scale-Invariant Feature Transform) algorithm has been moved to
the OpenCV main repository in OpenCV 4.4.0 because its patent expired.
You can use `--with-sift` flag to enable/disable SIFT related code.
Default is on, below is an exmaple to disable:

    $ export CC=g++
    $ cd tcl-opencv
    $ ./configure --with-sift=no
    $ make
    $ make install

Christian also provides the configure option `--enable-tkphoto`
to allow for optional Tk support in the cv::Mat class (toPhoto and
fromPhoto methods) and a cv::fromPhoto function to make a cv::Mat
from a Tk photo image.
Default is off, below is an example to enable:

    $ export CC=g++
    $ cd tcl-opencv
    $ ./configure --enable-tkphoto [--with-tk=dir]
    $ make
    $ make install

Christian also provides the configure option `--enable-vectcl`
to allow for optional VecTcl NumArray support in the cv::Mat class
(toNumArray and fromNumArray methods) and a cv::fromNumArray function
to make a cv::Mat from a VecTcl NumArray.
Default is off, below is an example to enable:

    $ export CC=g++
    $ cd tcl-opencv
    $ ./configure --enable-vectcl [--with-vectcl=dir]
    $ make
    $ make install

## Windows BUILD

### MSYS2

[Msys2](https://www.msys2.org/)  provides a Unix-style build while generating
native Windows binaries. Using the Msys2 build tools means that you can use
the same configure script as per the Unix build to create a Makefile.

Using below command to install x86_64 OpenCV packages for Mingw-w64:

    pacman -S mingw-w64-x86_64-opencv

If you need setup directory containing tcl configuration (tclConfig.sh),
below is an example:

    $ export CC=g++
    $ cd tcl-opencv
    $ ./configure --prefix=/opt/tcl --with-tcl=/opt/tcl/lib
    $ make
    $ make install

### Visual Studio 2017

I use [Magicsplat Tcl/Tk for Windows](https://www.magicsplat.com/tcl-installer/index.html)
to test, and install path is `c:\Tcl`.

Users require download and install
[opencv-4.5.5-vc14_vc15.exe](https://sourceforge.net/projects/opencvlibrary/files/4.5.5/),
and install path is `c:\opencv`.

Or you can modify `win\makefile.vc` file to specify OpenCV path:

    PRJ_INCLUDES = -I"C:\opencv\build\include"
    PRJ_LIBS = C:\opencv\build\x64\vc15\lib\opencv_world455.lib

Change folder to `win` folder and using below command to bulid:

    nmake /s /nologo /f makefile.vc INSTALLDIR=c:\tcl

Then install:

    nmake /s /nologo /f makefile.vc INSTALLDIR=c:\tcl install
