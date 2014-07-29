CC = gcc
PREFIX = /usr
LIBDIR = /usr/lib
# LIBDIR = /usr/lib64
CFLAGS=-Wall -pedantic -std=gnu99 -g -I/usr/include `pkg-config --cflags cairo`
LDFLAGS=-Wall -g `pkg-config --libs cairo` -L${LIBDIR} -lX11
MANDIR = /usr/share/man
PIXMAPS = {dialog-complete.png,dialog-info.png,dialog-warning.png}

all: cairo-msg

msg: cairo-msg.o
	${CC} -o cairo-msg ${LDFLAGS} cairo-msg.o

msg.o: cairo-msg.c
	${CC} -o cairo-msg.o ${CFLAGS} -c cairo-msg.c

install: cairo-msg
	-mkdir -p ${DESTDIR}/${PREFIX}/bin ${DESTDIR}/usr/share/pixmaps ${DESTDIR}/${MANDIR}/man1/
	install -m 0755 -s cairo-msg ${DESTDIR}/${PREFIX}/bin
	install -m 0644 pixmaps/${PIXMAPS} ${DESTDIR}/usr/share/pixmaps
	install -m 0644 cairo-msg.1 ${DESTDIR}/${MANDIR}/man1/cairo-msg.1

uninstall: cairo-msg
	-rm ${PREFIX}/bin/cairo-msg
	-rm ${MANDIR}/man1/cairo-msg.1
	-rm /usr/share/pixmaps/${PIXMAPS}

clean:
	-rm -f *.o cairo-msg
	