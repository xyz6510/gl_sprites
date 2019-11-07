
int xerr(Display *d,XErrorEvent *e)
{
	return 0;
}

void open_display(char *name)
{
	XInitThreads();
	XSetErrorHandler(xerr);
	screen.d=XOpenDisplay(NULL);

	screen.wx=640;
	screen.wy=360;

	screen.root=DefaultRootWindow(screen.d);
	screen.w=XCreateSimpleWindow(screen.d,screen.root,0,0,screen.wx,screen.wy,0,0,0);
	Display *d=screen.d;
	Window w=screen.w;

	XSelectInput(d,w,ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask);

	XMapWindow(d,w);
	XStoreName(d,w,name);

	screen.a=XInternAtom(d,"WM_DELETE_WINDOW",False);
	XSetWMProtocols(d,w,&screen.a,1);

	XSizeHints *sizehints=XAllocSizeHints();
	long userhints;
	XGetWMNormalHints(screen.d,screen.w,sizehints,&userhints);
	sizehints->min_width=sizehints->max_width=screen.wx;
	sizehints->min_height=sizehints->max_height=screen.wy;
	sizehints->flags|=PMinSize|PMaxSize;
	XSetWMNormalHints(screen.d,screen.w,sizehints);
	XFree(sizehints);

	XSync(d,False);
	XFlush(d);

	GLint att[]={GLX_RGBA,GLX_DEPTH_SIZE,0,GLX_DOUBLEBUFFER,None};
	XVisualInfo *vi=glXChooseVisual(d,0,att);
	screen.g=glXCreateContext(d,vi,NULL,GL_TRUE);
	glXMakeCurrent(d,w,screen.g);

	if (compare_file_size(key_input_buf,keybuf_size)) {
		if (create_file(key_input_buf,keybuf_size,0660)) exit(-1);
	}
	int f=open(key_input_buf,O_RDWR);
	if (map_file(f,(void**)&keybuf,keybuf_size,1)) exit(-1);
	close(f);
	memset(keybuf,0,keybuf_size);

	if (compare_file_size(mouse_input_buf,mousebuf_size)) {
		if (create_file(mouse_input_buf,mousebuf_size,0660)) exit(-1);
	}
	f=open(mouse_input_buf,O_RDWR);
	if (map_file(f,(void**)&mousebuf,mousebuf_size,1)) exit(-1);
	close(f);
	memset(mousebuf,0,mousebuf_size);
}

void close_display()
{
	Display *d=screen.d;
	Window w=screen.w;
	GLXContext g=screen.g;

	glXDestroyContext(d,g);
	XUnmapWindow(d,w);
	XDestroyWindow(d,w);
	XSync(d,False);
	XFlush(d);
	XCloseDisplay(d);
}
