
void set_window_size(unsigned int wx,unsigned int wy)
{
	screen.wx=wx;
	screen.wy=wy;

	XSizeHints *sizehints=XAllocSizeHints();
	long userhints;
	XGetWMNormalHints(screen.d,screen.w,sizehints,&userhints);
	sizehints->min_width=sizehints->max_width=screen.wx;
	sizehints->min_height=sizehints->max_height=screen.wy;
	sizehints->flags|=PMinSize|PMaxSize;
	XSetWMNormalHints(screen.d,screen.w,sizehints);
	XFree(sizehints);

	XResizeWindow(screen.d,screen.w,screen.wx,screen.wy);

	XSync(screen.d,False);
	XFlush(screen.d);
	usleep(250000);
}
