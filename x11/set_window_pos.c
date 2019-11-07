
void set_window_pos(int px,int py)//px=-1 center x ,py=-1 center y
{
	int wr,hr,wx,hy;

	get_root_size(&wr,&hr);
	get_window_size(&wx,&hy);
	if (px==-1) px=(wr-wx)/2;
	if (py==-1) py=(hr-hy)/2;
	XMoveWindow(screen.d,screen.w,px,py);

	XSync(screen.d,False);
	XFlush(screen.d);
	usleep(25000);
}
