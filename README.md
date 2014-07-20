cairo-msg
=========

a small splash program for UNIX like OS

ABOUT
-----

This is a simple message splash designed for very short notifications.
It depends on Xlib and Cairo and nothing more. It should build and run
on any UNIX variant as long as build time and runtime dependencies are met.

It is designed to be "pretty" yet small and efficient and also to support
any font (see BUGS).

It supports GNU short options only from the commandline.

BUILD
-----

Just type 'make'
then to install, as root type 'make install'

By default the binary is installed into /usr/local/bin
To change install paths edit the Makefile.

Alternatively:
Strip and install the binary manually.
Install the man page to your MANPATH and the 3 png icons to /usr/share/pixmaps

BUGS
----

Possibly poor support of wide chars, particularly CJK. 

If you find one report it or fix it and queue a pull request.

TODO
----

- implement wchar_t for wide char support.
- save settings to a personal rc file.
- possibly implement use of custom icons - low priority.
- port to win32 - low priority.

REFERENCES
----------
http://www.cairographics.org/manual/cairo-cairo-t.html
http://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html
http://man7.org/linux/man-pages/man3/getopt.3.html

Some snippets were borrowed or inspired by these projects:
http://en.literateprograms.org/Hello_World_(C,_Cairo)
http://zetcode.com/gfx/cairo/cairoimages/
https://code.google.com/p/semicomplete/source/browse/codesamples/cairo-example.c
