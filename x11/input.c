
int get_input()
{
	Display *d=screen.d;
	Window w=screen.w;

	XEvent ev;
	while ((XCheckTypedWindowEvent(d,w,ClientMessage,&ev))||
			(XCheckWindowEvent(d,w,ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask,&ev))) {
		int type=ev.type;
		if (type==Expose) {
			screen.paint++;
			//printf(":paint:%i\n",screen.paint);
		}
		if ((type==KeyPress)||(type==KeyRelease)) {
			int key=ev.xkey.keycode;
			printf(":key:%i hex:%x\n",key,key);
			if (type==KeyPress) keybuf[key]=1;
			if (type==KeyRelease) keybuf[key]=0;
		}
		if (type==ButtonPress) {
			//printf("mb:%i\n",ev.xbutton.button);
			if (ev.xbutton.button==1) mousebuf[0]=1;
			if (ev.xbutton.button==3) mousebuf[1]=1;
			if (ev.xbutton.button==2) mousebuf[2]=1;
			if (ev.xbutton.button==4) mousebuf[3]=1;//set to 0 after read;
			if (ev.xbutton.button==5) mousebuf[4]=1;//set to 0 after read;
		}
		if (type==ButtonRelease) {
			//printf("mr:%i\n",ev.xbutton.button);
			if (ev.xbutton.button==1) mousebuf[0]=0;
			if (ev.xbutton.button==3) mousebuf[1]=0;
			if (ev.xbutton.button==2) mousebuf[2]=0;
		}
		if (type==MotionNotify) {
			mousebuf[10]=ev.xmotion.x;
			mousebuf[11]=ev.xmotion.y;
		}
		if (type==ClientMessage) {
			if ((Atom)ev.xclient.data.l[0]==screen.a) {
				printf(":win close\n");
				return -1;
			}
		}
	}
	return 0;
}
