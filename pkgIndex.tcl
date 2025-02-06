# -*- tcl -*-
# Tcl package index file, version 1.1
#
if {[package vsatisfies [package provide Tcl] 9.0-]} {
    package ifneeded opencv 1.0 \
	    [list load [file join $dir libtcl9opencv1.0.so] [string totitle opencv]]
} else {
    package ifneeded opencv 1.0 \
	    [list load [file join $dir libopencv1.0.so] [string totitle opencv]]
}
