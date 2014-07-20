/**
 * (c) Michael Amadio, 2014, Gold Coast Australia.
 * 01micko@gmail.com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "cairo-msg.h"

float red;
float green;
float blue;
const char *prog;
char msgStr1[40];
char msgStr2[40];
char *icon;
unsigned int timeout;

struct { /* allows an icon */
	cairo_surface_t *image;  
}	 glob;
void usage(){
	printf("%s-%s\n\n", prog , THIS_VERSION);
	printf("\tA simple splash window based on Cairo and Xlib.\n\n");
	printf("Usage :\n");
	printf("\tcairo-msg [-u, -t, -p, -f] -s \"some message to display\"\n");
	printf("\t-u\t:urgency - l is low, n is normal or u is urgent\n");
	printf("\t-t\t:timeout - value in seconds, default is 10, 0 is forever\n");
	printf("\t-p\t:position - tl=top left, tc=top centre, tr=top right\n");
	printf("\tcx=centre, bl=bottom left, bc=bottom centre, br=bottom right\n");
	printf("\t-f\t:font - a font name\n");
	printf("\tA limit of 36 chars per line is to keep it simple.\n");
	printf("\tBy design %s is for very simple notifications.\n\n", prog);
	printf("\tLicensed under the GPL version 2. All rights reserved.\n");
}
void urgency(char *urge) {
	
	if (strncmp(urge, "urgent", strlen(urge)) == 0 ) {
		red = 0.9;
		green = 0.2;
		blue = 0.2;
		icon = "/usr/share/pixmaps/dialog-warning.png";
	}
	else if (strncmp(urge, "normal", strlen(urge)) == 0 ) {
		red = 0.1;
		green = 0.3;
		blue = 0.5;
		icon = "/usr/share/pixmaps/dialog-complete.png";
	}
	else if (strncmp(urge, "low", strlen(urge)) == 0 ) {
		red = 0.3;
		green = 0.3;
		blue = 0.3;
		icon = "/usr/share/pixmaps/dialog-info.png";
	}
	else {
		usage();
		exit(1);
	}
}
void format_input(char *message) {
	/* sets the number of lines diplayed */
	int i;int index;
	for (i=36; i > 15; i--) {
		if (message[i] == ' ') {
			index = i; /* get index of space before 36 char limit */
			break;
		}
		else if (i == 16) {
			printf("Illegal entry!\n\n");
			usage();
			exit(1);
		}
	}
	char str1[index+1];
	strncpy(str1, &message[0], index);
	str1[index] = '\0';	/* ensure \0 termination */
	msgStr1[40] = sprintf(msgStr1, "%s", str1);
	char str2[strlen(message)+1];
	strncpy(str2, &message[index+1], strlen(message));
	str2[strlen(message)] = '\0';	/* ensure \0 termination */
	msgStr2[40] = sprintf(msgStr2, "%s", str2);
}
void paint(cairo_surface_t *cs, char *input, 
			char *inputExtra, int sizex, int sizey, char *font) {
	
	cairo_t *c;
	c = cairo_create(cs);
	cairo_select_font_face(c, font, CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL);

	cairo_set_font_size(c, 14.0);
	cairo_rectangle(c, 0.0, 0.0, sizex, sizey);
	/* urgent = 0.9, 0.2, 0.2 ; normal = 0.1, 0.3, 0.5 ; low = 0.3, 0.3, 0.3*/
	cairo_set_source_rgba(c, red, green, blue, 0.85);
	cairo_fill(c);
	int icony;
	if (sizey == 40) {
		cairo_move_to(c, 51.0, 24.0);
		cairo_set_source_rgb(c, 0.0, 0.0, 0.0);
		cairo_show_text(c, input);
		cairo_move_to(c, 50.0, 25.0);
		cairo_set_source_rgb(c, 0.8, 0.8, 0.8);
		cairo_show_text(c, input);
		icony = 8;
	}
	else { /* 2 lines */
		cairo_move_to(c, 51.0, 19.0);
		cairo_set_source_rgb(c, 0.0, 0.0, 0.0);
		cairo_show_text(c, input);
		cairo_move_to(c, 50.0, 20.0);
		cairo_set_source_rgb(c, 0.8, 0.8, 0.8);
		cairo_show_text(c, input);
		cairo_move_to(c, 51.0, 44.0);
		cairo_set_source_rgb(c, 0.0, 0.0, 0.0);
		cairo_show_text(c, inputExtra);
		cairo_move_to(c, 50.0, 45.0);
		cairo_set_source_rgb(c, 0.8, 0.8, 0.8);
		cairo_show_text(c, inputExtra);
		icony=15;
	}
	glob.image = cairo_image_surface_create_from_png(icon);
	cairo_set_source_surface(c, glob.image, 10, icony);
	cairo_paint(c);
	cairo_show_page(c);
	cairo_surface_destroy(glob.image);
	cairo_destroy(c);
	/* cairo_surface_write_to_png (cs, "test.png"); */
}
static void alarm_handler() {
	exit(0); /* rough but works */
}
void showxlib(char *in, char *inExtra, int width, 
				int height, unsigned int t, char *posi, char *fontype) {
	
	Display *dpy;
	Window rootwin;
	Window win;
	XEvent e;
	int scr;
	cairo_surface_t *cs;
	Visual *myvisual;
	int mydepth;
    int override = 1;
    signal (SIGALRM, alarm_handler);
	alarm(t); /* sets timeout */

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}
	scr = DefaultScreen(dpy);
    myvisual = DefaultVisual(dpy, scr);
    mydepth  = DefaultDepth(dpy, scr);
    
    unsigned int dpyWidth, dpyHeight;
    dpyWidth = DisplayWidth(dpy, scr);
    dpyHeight = DisplayHeight(dpy, scr);
	
	rootwin = RootWindow(dpy, scr);
	XSetWindowAttributes attr;
	attr.override_redirect = override ? True : False; /* no deco */
	int posx, posy;
	int ctrx = ((dpyWidth / 2) - 160);
	int ctry = ((dpyHeight / 2) - 25); /* close enough */
	if (strcmp(posi,"tl") == 0) {
		posx = 40;
		posy = 50;
	}
	else if (strcmp(posi,"tc") == 0) {
		posx = ctrx;
		posy = 50;
	}
	else if (strcmp(posi,"tr") == 0) {
		posx = dpyWidth - (width + 40);
		posy = 50;
	}
	else if (strcmp(posi,"cx") == 0) {
		posx = ctrx;
		posy = ctry;
	}
	else if (strcmp(posi,"bl") == 0) {
		posx = 50;
		posy = dpyHeight - (height + 50);
	}
	else if (strcmp(posi,"bc") == 0) {
		posx = ctrx;
		posy = dpyHeight - (height + 50);
	}
	else if (strcmp(posi,"br") == 0) { /* default */
		posx = dpyWidth - (width + 40);
		posy = dpyHeight - (height + 50);
	}

	win = XCreateWindow(dpy, rootwin, posx, posy, width, height, 1, mydepth, 
			InputOutput, myvisual, CWOverrideRedirect, &attr);

	XStoreName(dpy, win, prog);
	XSelectInput(dpy, win, ExposureMask|ButtonPressMask);
	XMapWindow(dpy, win);
	cs = cairo_xlib_surface_create(dpy, win, 
				DefaultVisual(dpy, 0), width, height);
	while (1) {
		XNextEvent(dpy, &e);
		if(e.type == Expose && e.xexpose.count < 1) {
		paint(cs, in, inExtra, width, height, fontype);
		} 
		else if (e.type == ButtonPress) break;
	}
	cairo_surface_destroy(cs);
	XCloseDisplay(dpy);
}

/* main */
int main(int argc, char **argv) {
	prog = argv[0];
	
	int secs = 10;
	int hsize = 320; int vsize = 40;
	
	char *uvalue = "n";
	char *tvalue = NULL; /* timeout */
	char *svalue = "cairo-msg: a splash based on Xlib and Cairo. (GPL2)";
	char *pvalue = "br"; /* default pos is bottom right */
	char *fvalue = "Dejavu Sans";
	int c;
     
	opterr = 0;
     
	while ((c = getopt (argc, argv, "u:t:s:p:f:")) != -1) {
		switch (c)
		{
			case 'u':
				uvalue = optarg;
				break;
			case 't':
				tvalue = optarg;
				secs = atoi(tvalue);
				break;
			case 's':
				svalue = optarg;
				break;
			case 'p':
				pvalue = optarg;
				break;
			case 'f':
				fvalue = optarg;
				break;
			default:
				usage();
				exit(1);
		}
	}
	urgency(uvalue);	
	if ((((strlen(svalue) * 8) + 10) > 320) &&
							(((strlen(svalue) * 8) + 10) <= 640)) {
		vsize = 60;
	}
	else if (strlen(svalue) > 70) {
		printf("The message input is too long.\n");
		usage();
		return 1;
	}
	if (vsize == 40) {
		showxlib(svalue, NULL, hsize, vsize, secs, pvalue, fvalue);
	}
	else {
		format_input(svalue);
		showxlib(msgStr1, msgStr2, hsize, vsize, secs, pvalue, fvalue);
	}
	return 0;
}
	