#include "../files/files.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>


#define key_input_buf __dirpath__ "/key_input_buf"
#define keybuf_size 256*4

#define mouse_input_buf __dirpath__ "/mouse_input_buf"
#define mousebuf_size 256*4

typedef struct {
	Display *d;
	Window root;
	Window w;
	Atom a;
	GLXContext g;
	int wx;
	int wy;
	int paint;
} SCR;

SCR screen;

int *keybuf=NULL;
int *mousebuf=NULL;
