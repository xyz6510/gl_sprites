
typedef struct {
	int scroll;
	int scrx;
	int scry;
	unsigned int screen_width;
	unsigned int screen_height;
	struct udev *udev;
	struct libinput *li;
	struct pollfd fds[2];
	sigset_t mask;
	struct libinput_interface interface;
} INPUT;

INPUT input;

int open_restricted(const char *path,int flags,void *user_data)
{
	int fd=open(path,flags);
	return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data)
{
	close(fd);
}


void print_key_event(struct libinput_event *ev)
{
	struct libinput_event_keyboard *k=libinput_event_get_keyboard_event(ev);

	int state=libinput_event_keyboard_get_key_state(k);
	int idx=libinput_event_keyboard_get_key(k);
	keybuf[idx]=state;
	printf("key:%i:%i\n",idx,state);
}

void print_absmotion_event(struct libinput_event *ev)
{
	struct libinput_event_pointer *p=libinput_event_get_pointer_event(ev);
	double x=libinput_event_pointer_get_absolute_x_transformed(p,input.screen_width);
	double y=libinput_event_pointer_get_absolute_y_transformed(p,input.screen_height);

//	int mx=x;
//	int my=y;

	double sx=(x/1000000.0)*input.scrx;
	double sy=(y/1000000.0)*input.scry;

	mousebuf[10]=sx;
	mousebuf[11]=sy;

//	printf("mouse x:%.0f y:%.0f\n",x,y);
}

void print_button_event(struct libinput_event *ev)
{
	struct libinput_event_pointer *p=libinput_event_get_pointer_event(ev);
	enum libinput_button_state state;

	state=libinput_event_pointer_get_button_state(p);
	int button=libinput_event_pointer_get_button(p)&0b11;
	mousebuf[0+button]=state;

//	printf("mouse button:%i:%i\n",libinput_event_pointer_get_button(p)&0b11,state);
}

void print_axis_event(struct libinput_event *ev)
{
//	char *ax;
	int type;

	struct libinput_event_pointer *p = libinput_event_get_pointer_event(ev);

	if (libinput_event_pointer_has_axis(p,LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL)) {
//		ax="vscroll";
		type=LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL;
	} else if (libinput_event_pointer_has_axis(p,LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL)) {
//		ax="hscroll";
		type=LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL;
	} else abort();

	double val=libinput_event_pointer_get_axis_value(p,type);
	if (val>0) input.scroll+=1;
	if (val<0) input.scroll-=1;
	mousebuf[5]=input.scroll;

//	printf("%s:%.2f\n",ax,val);
}

int handle_and_print_events(struct libinput *li)
{
	int rc=-1;
	struct libinput_event *ev;

	libinput_dispatch(li);
	while ((ev = libinput_get_event(li))) {
		int e=libinput_event_get_type(ev);
		//printf("event %i\n",e);
		switch (e) {
		case LIBINPUT_EVENT_NONE:
			abort();
		case LIBINPUT_EVENT_KEYBOARD_KEY:
			print_key_event(ev);
			break;
		case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE:
			print_absmotion_event(ev);
			break;
		case LIBINPUT_EVENT_POINTER_AXIS:
			print_axis_event(ev);
			break;
		case LIBINPUT_EVENT_POINTER_BUTTON:
			print_button_event(ev);
			break;
		}
		libinput_event_destroy(ev);
		libinput_dispatch(li);
		rc=0;
	}
	return rc;
}

int get_input()
{

	if (poll(input.fds,2,0)>-1) {
		if (input.fds[1].revents) return 0;
		handle_and_print_events(input.li);
	}

	return 0;
}

void init_input(int scrx,int scry)
{
	memset(&input,0,sizeof(INPUT));

	input.scroll=0;

	input.screen_width=1000000;
	input.screen_height=1000000;
	input.scrx=scrx;
	input.scry=scry;

	if (compare_file_size(key_input_buf,keybuf_size)) {
		if (create_file(key_input_buf,keybuf_size,0666)) exit(-1);
	}
	int kbd=open(key_input_buf,O_RDWR);
	if (map_file(kbd,(void**)&keybuf,keybuf_size,1)) exit(-1);

	if (compare_file_size(mouse_input_buf,mousebuf_size)) {
		if (create_file(mouse_input_buf,mousebuf_size,0666)) exit(-1);
	}
	int mouse=open(mouse_input_buf,O_RDWR);
	if (map_file(mouse,(void**)&mousebuf,mousebuf_size,1)) exit(-1);

	memset(keybuf,0,keybuf_size);
	memset(mousebuf,0,mousebuf_size);

	input.udev=udev_new();
	if (!input.udev) {
		fprintf(stderr, "Failed to initialize udev\n");
		exit(-1);
	}

	input.interface.open_restricted=open_restricted;
	input.interface.close_restricted=close_restricted;


	input.li=libinput_udev_create_context(&input.interface,NULL,input.udev);
	libinput_udev_assign_seat(input.li,"seat0");

	input.fds[0].fd=libinput_get_fd(input.li);
	input.fds[0].events=POLLIN;
	input.fds[0].revents=0;

	input.fds[1].fd=signalfd(-1,&input.mask,SFD_NONBLOCK);
	input.fds[1].events=POLLIN;
	input.fds[1].revents=0;

	if (input.fds[1].fd==-1 || sigprocmask(SIG_BLOCK, &input.mask, NULL) == -1) {
		printf("Failed to set up signal handling (%s)\n",strerror(errno));
		exit(-1);
	}

	if (handle_and_print_events(input.li)) {
		printf("Expected device added events on startup but got none.\n"
				"Maybe you don't have the right permissions?\n");
		exit(-1);
	}

}

void close_input()
{
	close(input.fds[1].fd);
	libinput_unref(input.li);
	udev_unref(input.udev);
}
