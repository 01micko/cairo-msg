CC = gcc
CFLAGS=-Wall -pedantic -std=gnu99 -g -I/usr/include `pkg-config --cflags cairo`
LDFLAGS=-Wall -g `pkg-config --libs cairo` -L/usr/X11R6/lib -lX11
PREFIX = /usr/local
MANDIR = /usr/share/man
PIXMAPS = {dialog-complete.png,dialog-info.png,dialog-warning.png}

all: cairo-msg

msg: cairo-msg.o
	${CC} -o cairo-msg ${LDFLAGS} cairo-msg.o

msg.o: cairo-msg.c
	${CC} -o cairo-msg.o ${CFLAGS} -c cairo-msg.c

install: cairo-msg
	install -m 0755 -s cairo-msg ${PREFIX}/bin
	install -m 0644 pixmaps/${PIXMAPS} /usr/share/pixmaps
	install -m 0644 cairo-msg.1 ${MANDIR}/man1/cairo-msg.1

uninstall: cairo-msg
	-rm ${PREFIX}/bin/cairo-msg
	-rm ${MANDIR}/man1/cairo-msg.1
	-rm /usr/share/pixmaps/${PIXMAPS}

clean:
	-rm -f *.o cairo-msg
	