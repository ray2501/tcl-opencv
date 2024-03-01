# -*- tcl -*-
# Tcl package index file, version 1.1
#
if {[package vsatisfies [package provide Tcl] 9.0-]} {
    package ifneeded opencv 0.17 \
	    [list load [file join $dir libtcl9opencv0.17.so] [string totitle opencv]]
} else {
    package ifneeded opencv 0.17 \
	    [list load [file join $dir libopencv0.17.so] [string totitle opencv]]
}
