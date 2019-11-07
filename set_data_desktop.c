
typedef struct {
	int x;
	int y;
	int w;
	int h;
	int s;//state:0-normal,1-min,2-max
	int a;//active:0,1
	int idx;
	int sp0;
	int sp1;
	int minw;
	int min_a;
	int max_a;
	int close_a;
	int spr_min;
	int spr_max;
	int spr_close;
	char *name;
} WINDOW;

typedef struct {
	WINDOW w[128];
	int s;
	int e;
} WINLIST;

typedef struct {
	char *n;
	char *s;
	int t;
	int x;
	int y;
	int w;
	int h;
	int a;
	int spr;
} MENU_ITEM;

typedef struct {
	char *n;
	int x;
	int y;
	int w;
	int h;
	int a;
	int idx;
	int spr;
} TASK_ITEM;

typedef struct {
	WINLIST wl;
	int l[128];
	int cnt;
	int pos;
	int spr;
	int minwinw;
	int minwinh;
	int topwin;
	int resizewin;
	int resizemouse_x;
	int resizemouse_y;
	int resizewin2;
	int actwin;
	int dragwin;
	int insidewin;
	int actwinframe;
	int actwinframe_x;
	int actwinframe_y;
	int actwinframe_w;
	int actwinframe_h;
	int resize_top;
	int resize_bottom;
	int resize_left;
	int resize_right;
	int inside_top;
	int inside_min;
	int inside_max;
	int inside_close;
	int offset_x;
	int offset_y;
	int mouse_1;
	int mouse_1_press;
	int mouse_1_click;
	int mouse_x;
	int mouse_y;
	int menu_x;
	int menu_y;
	int menu_w;
	int menu_h;
	int menu1_x;
	int menu1_y;
	int menu1_w;
	int menu1_h;
	int menu2_x;
	int menu2_y;
	int menu2_w;
	int menu2_h;
	int menu2up_x;
	int menu2up_y;
	int menu2up_a;
	int menu2up_spr;
	MENU_ITEM menu1[32];
	MENU_ITEM menu2[32];
	char menu2path[1024];
	char menu2name[64];
	int menu_open;
	int spr_background;
	int spr_cursor;
	int spr_actframe;
	int spr_menu;
	int spr_menu1;
	int spr_menu2;
	int spr_task;
	TASK_ITEM task[128];
	void (*newwin)(int,int,int,int,char*);
} DESKTOP;

DESKTOP desktop;

void update_sprite_v_xy(int idx,short int px,short int py);
void update_sprite_v_size_xy(int idx,short int w,short int h);
void update_sprite_t_xy(int idx,short int tx,short int ty);
void update_sprite_t_size_xy(int idx,short int tw,short int th);
void update_sprite_rot(int idx,short int rot);
void update_sprite_settings(int idx,short int settings);
void update_sprite_filter(int idx,unsigned char filter_amount,unsigned char filter_cut,unsigned char filter_max,unsigned char filter_rgb);
void update_sprite_pal(int idx,short int pal);
void update_sprite_o_xy_wh_txy_twh_set_rot_pal_filter(int idx);
void update_sprite_all(int idx);
void set_sprite(unsigned short int sprite_idx,
				short int x,short int y,
				short int w,short int h,
				short int tex_x,short int tex_y,
				short int tex_w,short int tex_h,
				short int settings,
				short int rot,
				short int rot_x,short int rot_y,
				short int orbit_rot,
				short int orbit_x,short int orbit_y,
				short int light,
				short int alpha,
				short int pal_idx,
				unsigned char ckey_r,unsigned char ckey_g,unsigned char ckey_b,unsigned char ckey_a,
				short int repeat_scale_x,short int repeat_scale_y,
				short int repeat_offset_x,short int repeat_offset_y,
				unsigned char filter_amount,unsigned char filter_cut,unsigned char filter_max,unsigned char filter_rgb,
				short int xp0,short int yp0,
				short int xp1,short int yp1,
				short int xp2,short int yp2,
				short int xp3,short int yp3
				);
void add_str_h(int x,int y,int len,char *str,int *spidx,int pal,int font);
void add_str_v(int x,int y,char *str,int *spidx,int pal,int font);
void add_block(int x,int y,int w,int h,int *spidx,int p1,int p2,int settings);
void add_block_text(int x,int y,int w,int h,int tx,int ty,int lmax,int *spidx,int p1,int p2,char *str,int settings,int font);
int inside_menu(DESKTOP *d);
void open_menu2(DESKTOP *d);

void cursor_inside(DESKTOP *d)
{
	if (d->mouse_1_press) return;
	int i;
	int resize_hor=0;
	int resize_ver=0;
	int resize_top=0;
	int resize_bottom=0;
	int resize_left=0;
	int resize_right=0;
	d->inside_top=0;
	d->inside_min=0;
	d->inside_max=0;
	d->inside_close=0;
	int resizefound=0;
	d->resizewin2=-1;
	d->cnt++;
	if (d->cnt>9) d->cnt=0;
	for (i=0;i<d->pos;i++) {
		if (d->l[i]>=0) {
			int pos=d->l[i];
			WINDOW *w=&d->wl.w[pos];
			int x=d->mouse_x-w->x;
			int y=d->mouse_y-w->y;
			int set_top=0;
			int set_left=0;
			int set_right=0;
			if ((x>-12)&&(x<w->w+12)&&(y>0)&&(y<8)) {
				printf("inside[%i]:%i:%s:resize-top mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
				resize_ver=1;resize_top=1;set_top=1;
				resize_bottom=0;resize_hor=0;resize_right=0;resize_left=0;
				if (resizefound) d->resizewin2=d->resizewin;
				resizefound=1;
				d->resizewin=i;
			}
			if (w->min_a) {update_sprite_pal(w->spr_min,0);w->min_a=0;printf("update min a\n");}
			if (w->max_a) {update_sprite_pal(w->spr_max,0);w->max_a=0;printf("update min a\n");}
			if (w->close_a) {update_sprite_pal(w->spr_close,0);w->close_a=0;printf("update min a\n");}
			if ((x>0)&&(x<w->w)&&(y>0)&&(y<w->h)) {
				printf("inside[%i]:%i:%s mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
				d->topwin=i;
				if (d->topwin!=d->resizewin) {
					resize_ver=0;resize_top=0;
				} else {
					if (set_top==0) {
						resize_ver=0;resize_top=0;
					}
				}
				resize_bottom=0;resize_hor=0;resize_left=0;resize_right=0;
				if ((x>0)&&(x<w->w)&&(y>8)&&(y<30)) {
					printf("inside[%i]:%i:%s:top mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
					d->inside_top=1;
				}
				if ((x>w->w-62)&&(x<w->w-62+16)&&(y>8)&&(y<24)) {
					printf("inside[%i]:%i:%s:min mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
					d->inside_min=1;
				}
				if ((x>w->w-42)&&(x<w->w-42+16)&&(y>8)&&(y<24)) {
					printf("inside[%i]:%i:%s:max mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
					d->inside_max=1;
				}
				if ((x>w->w-22)&&(x<w->w-22+16)&&(y>8)&&(y<24)) {
					printf("inside[%i]:%i:%s:close mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
					d->inside_close=1;
				}
				if (d->inside_min|d->inside_max|d->inside_close) d->inside_top=0;
				if ((x>0)&&(x<w->w)&&(y>30)&&(y<w->h)) {
					//printf("inside[%i]:%s:main mx:%i my:%i\n",d->cnt,w->name,d->mouse_x,d->mouse_y);
					if ((x>w->w-5)&&(x<w->w)&&(y>30)&&(y<w->h)) {
						printf("inside[%i]:%i:%s:v-scroll mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
					}
					if ((x>0)&&(x<w->w)&&(y>w->h-5)&&(y<w->h)) {
						printf("inside[%i]:%i:%s:h-scroll mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
					}
				}
			} else {
				if ((x>-12)&&(x<0)&&(y>0)&&(y<w->h+12)) {
					if (resizefound) d->resizewin2=d->resizewin;
					resizefound=1;
					d->resizewin=i;
					if (d->resizewin==d->actwin) d->topwin=d->resizewin;
					printf("inside[%i]:%i:%s:resize-left mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
					resize_hor=1;resize_left=1;resize_right=0;
					if (set_top==0) {
						resize_ver=0;resize_top=0;resize_bottom=0;
					}
					set_left=1;
				}
				if ((x>w->w)&&(x<w->w+12)&&(y>0)&&(y<w->h+12)) {
					if (resizefound) d->resizewin2=d->resizewin;
					resizefound=1;
					d->resizewin=i;
					if (d->resizewin==d->actwin) d->topwin=d->resizewin;
					printf("inside[%i]:%i:%s:resize-right mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
					resize_hor=1;resize_right=1;
					resize_left=set_left;
					if (set_top==0) {
						resize_ver=0;resize_top=0;resize_bottom=0;
					}
					set_right=1;
				}
				if ((x>-12)&&(x<w->w+12)&&(y>w->h)&&(y<w->h+12)) {
					if (resizefound) d->resizewin2=d->resizewin;
					resizefound=1;
					d->resizewin=i;
					if (d->resizewin==d->actwin) d->topwin=d->resizewin;
					printf("inside[%i]:%i:%s:resize-bottom mx:%i my:%i\n",i,d->cnt,w->name,d->mouse_x,d->mouse_y);
					resize_ver=1;resize_bottom=1;
					resize_top=0;
					resize_left=set_left;resize_right=set_right;
					resize_hor=0;
					if (set_left|set_right) resize_hor=1;
				}
			}
		}
	}
	if (d->menu_open) {
		if (inside_menu(d)) {
			resize_hor=0;resize_ver=0;
			resize_top=0;resize_bottom=0;resize_left=0;	resize_right=0;
		}
	}
	if (resize_hor|resize_ver) {
		if (resize_hor&resize_ver) {
			d->offset_x=-9;
			d->offset_y=-9;
			if (resize_top&resize_left) update_sprite_settings(d->spr_cursor,0x80);
			if (resize_top&resize_right) {
				update_sprite_settings(d->spr_cursor,0x80|0x08);
				d->offset_x=-16;
			}
			if (resize_bottom&resize_left) {
				update_sprite_settings(d->spr_cursor,0x80|0x10);
				d->offset_y=-16;
			}
			if (resize_bottom&resize_right) {
				update_sprite_settings(d->spr_cursor,0x80|0x08|0x10);
				d->offset_x=-16;
				d->offset_y=-16;
			}
			update_sprite_v_size_xy(d->spr_cursor,24,24);
			update_sprite_t_xy(d->spr_cursor,200+48,64);
			update_sprite_t_size_xy(d->spr_cursor,24,24);
		} else {
			if (resize_hor) {
				d->offset_x=-12;d->offset_y=-6;
				update_sprite_v_size_xy(d->spr_cursor,24,24);
				update_sprite_t_xy(d->spr_cursor,200,64);
				update_sprite_t_size_xy(d->spr_cursor,24,24);
			}
			if (resize_ver) {
				d->offset_x=-6;d->offset_y=-12;
				update_sprite_v_size_xy(d->spr_cursor,24,24);
				update_sprite_t_xy(d->spr_cursor,200+24,64);
				update_sprite_t_size_xy(d->spr_cursor,24,24);
			}
			update_sprite_settings(d->spr_cursor,0x80);
		}
	} else {
		d->offset_x=0;d->offset_y=0;
		update_sprite_v_size_xy(d->spr_cursor,16,16);
		update_sprite_t_xy(d->spr_cursor,200+0,16);
		update_sprite_t_size_xy(d->spr_cursor,16,16);
		update_sprite_settings(d->spr_cursor,0x80);
		int pos=desktop.l[desktop.topwin];
		WINDOW *w=&desktop.wl.w[pos];
		int x=d->mouse_x-w->x;
		int y=d->mouse_y-w->y;
		if (d->inside_min) {
			if ((x>w->w-62)&&(x<w->w-62+16)&&(y>8)&&(y<24)) {
				if (d->mouse_1_click) {
					d->mouse_1_click=0;
					printf("click minimize:%s\n",w->name);
				} else {
					if (w->min_a==0) {
						printf("update_min_a 1\n");
						update_sprite_pal(w->spr_min,1);
						w->min_a=1;
					}
				}
			} else d->inside_min=0;
		}
		if (d->inside_max) {
			if ((x>w->w-42)&&(x<w->w-42+16)&&(y>8)&&(y<24)) {
				if (d->mouse_1_click) {
					d->mouse_1_click=0;
					printf("click max:%s\n",w->name);
				} else {
					if (w->max_a==0) {
						printf("update_max_a 1\n");
						update_sprite_pal(w->spr_max,1);
						w->max_a=1;
					}
				}
			} else d->inside_max=0;
		}
		if (d->inside_close) {
			if ((x>w->w-22)&&(x<w->w-22+16)&&(y>8)&&(y<24)) {
				if (d->mouse_1_click) {
					printf("close[%i]:%s\n",d->topwin,w->name);
					draw_order.order[32+d->topwin]=-1;
					d->l[d->topwin]=-1;
					d->mouse_1_click=0;
					cursor_inside(d);
					return;
				} else {
					if (w->close_a==0) {
						printf("update_close_a 1\n");
						update_sprite_pal(w->spr_close,1);
						w->close_a=1;
					}
				}
			} else d->inside_close=0;
		}
	}
	d->resize_top=resize_top;
	d->resize_bottom=resize_bottom;
	d->resize_left=resize_left;
	d->resize_right=resize_right;

}

void merge_list(DESKTOP *d)
{

}

void newwin(DESKTOP *d,int x,int y,int w,int h,char *name)
{
	SPRITE_GROUP *sg=&sprite_group;
	DRAW_ORDER *dr=&draw_order;

	if (d->pos>127) {
		printf(":window limit reached\n");
		return;
	}
	printf("spr:%i\n",d->spr);
	printf(":%i %i %i %i %s\n",x,y,w,h,name);
	int pos=d->pos;
	d->l[pos]=pos;
	d->wl.w[pos].x=x;
	d->wl.w[pos].y=y;
	d->wl.w[pos].w=w;
	d->wl.w[pos].h=h;
	int max=16;
	char buf[32];
	memset(buf,'.',32);
	strncpy(buf,name,max);
	if (strlen(name)>max) buf[max+3]=0;
	else buf[max]=0;
	d->wl.w[pos].name=strdup(buf);
	d->wl.w[pos].sp0=d->spr;
	d->wl.w[pos].minw=8+(strlen(buf)+1)*11+62;

	int a=d->wl.w[pos].a;
	int spr=d->spr;
	//top              0
	set_sprite(spr++, x,y,w,30,      120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 14, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//main             1
	set_sprite(spr++, x,y+30,w,h-30, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 14, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//h-line top       2
	set_sprite(spr++, x,y,w,1+(a*3), 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//h-line top title 3
	set_sprite(spr++, x,y+30,w,1,    120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//h-line bottom    4
	set_sprite(spr++, x,y+h,w,1,     120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//v-line left      5
	set_sprite(spr++, x,y,1,h,       120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//v-line right     6
	set_sprite(spr++, x+w-1,y,1,h,   120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//v-scroll         7,8,9
	set_sprite(spr++, x+w-5,y+30+1,(4)*1,(h-30-1)*1, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 14, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(spr++, x+w-6,y+30  ,(1)*1,(h-30-5)*1, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(spr++, x+w-4,y+30+8,(2)*1,(32)*1,     120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//h-scroll         10,11,12
	set_sprite(spr++, x+1  ,y+h-4 ,(w-2)*1,(4)*1,    120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 14, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(spr++, x    ,y+h-5 ,(w-5)*1,(1)*1,    120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(spr++, x+8  ,y+h-3 ,(32)*1 ,(2)*1,    120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//symbols          13,14,15
	d->wl.w[pos].spr_min=spr;
	set_sprite(spr++,x+w-62,y+8,16,16, 200+0, 0,16,16, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	d->wl.w[pos].spr_max=spr;
	set_sprite(spr++,x+w-42,y+8,16,16, 200+16,0,16,16, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	d->wl.w[pos].spr_close=spr;
	set_sprite(spr++,x+w-22,y+8,16,16, 200+48,0,16,16, 0x80, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//name 16...
	add_str_h(x+8,y+6,-1,d->wl.w[pos].name,&spr,0,0);

	int act=pos>>5;
	int ab=1<<(pos-(act<<5));
	sg->active[1+act]|=ab;
	sg->start[32+pos]=d->spr;
	sg->end[32+pos]=spr;
	dr->order[32+pos]=32+pos;
	d->wl.w[pos].sp1=spr;
	d->spr+=35;
	printf("sp0:%i sp1:%i\n",d->wl.w[pos].sp0,d->wl.w[pos].sp1);

	pos++;
	d->pos=pos;
	if (pos>127) merge_list(d);
}

int inside_menu(DESKTOP *d)
{
	int mx=mousebuf[10];
	int my=mousebuf[11];

	int ret=0;

	if ((mx>d->menu_x)&&(mx<d->menu_x+d->menu_w)&&(my>d->menu_y)&&(my<d->menu_y+d->menu_h)) {
		ret=1;
	}
	if ((mx>d->menu1_x)&&(mx<d->menu1_x+d->menu1_w)&&(my>d->menu1_y)&&(my<d->menu1_y+d->menu1_h)) {
		ret=2;
	}
	if ((mx>d->menu2_x)&&(mx<d->menu2_x+d->menu2_w)&&(my>d->menu2_y)&&(my<d->menu2_y+d->menu2_h)) {
		ret=3;
	}

	return ret;
}

void cursor_inside_menu2(DESKTOP *d)
{
	if (d->menu_open) {
		int i;
		int x=mousebuf[10]-d->menu2up_x;
		int y=mousebuf[11]-d->menu2up_y;
		if ((x>0)&&(x<24)&&(y>0)&&(y<24)) {
			if (d->menu2up_a==0) {
				printf("cursor inside menu2up\n");
				update_sprite_pal(d->menu2up_spr,1);
				d->menu2up_a=1;
			}
			if (d->mouse_1_click) {
				d->mouse_1_click=0;
				printf("mouse click menu2up\n");
				int i;
				int num_slash=0;
				for (i=0;i<strlen(d->menu2path);i++) {
					if (d->menu2path[i]=='/') num_slash++;
				}
				if (num_slash>1) {
					for (i=strlen(d->menu2path);i!=0;i--) {
						if (d->menu2path[i]=='/') {
							d->menu2path[i]=0;
							int j=i;
							for (j=i;j!=0;j--) {
								if (d->menu2path[j]=='/') break;
							}
							if (num_slash>2) {
								snprintf(d->menu2name,i-j,"..%s",&d->menu2path[j+3]);
							} else {
								snprintf(d->menu2name,i-j,"%s",&d->menu2path[j+3]);
							}
							printf("name:%s\n%s\n%i\n",d->menu2path,d->menu2name,j+1);
							open_menu2(d);
							return;
						}
					}
				}
			}
		} else {
			if (d->menu2up_a) {
				update_sprite_pal(d->menu2up_spr,0);
				d->menu2up_a=0;
			}
		}
		for (i=0;d->menu2[i].n!=NULL;i++) {
			int x=mousebuf[10]-d->menu2[i].x;
			int y=mousebuf[11]-d->menu2[i].y;
			int w=d->menu2[i].w;
			int h=d->menu2[i].h;
			if (d->menu2[i].t==0xff) continue;
			if ((x>0)&&(x<w)&&(y>0)&&(y<h)) {
				printf("cursor inside menu item:%s\n",d->menu2[i].s);
				if (d->menu2[i].a==0) {
					update_sprite_pal(d->menu2[i].spr,1);
					d->menu2[i].a=1;
				}
				if (d->mouse_1_click) {
					d->mouse_1_click=0;
					printf("menu click on ");
					if (d->menu2[i].t==1) printf("dir:");
					if (d->menu2[i].t==2) printf("file:");
					printf("%s\n",d->menu2[i].s);
					if (d->menu2[i].t==1) {
						int len=strlen(d->menu2path);
						sprintf(&d->menu2path[len],"/%s",d->menu2[i].n);
						snprintf(d->menu2name,strlen(d->menu2[i].s)-1+2,"..%s",d->menu2[i].s);
						open_menu2(d);
						return;
					}
					if (d->menu2[i].t==2) {
						d->newwin(20,20,400,250,d->menu2[i].s);
						update_sprite_o_xy_wh_txy_twh_set_rot_pal_filter(32+(d->pos-1));
						printf("create new win:%i\n",d->pos);
					}
				}
			} else {
				if (d->menu2[i].a) {
					update_sprite_pal(d->menu2[i].spr,14);
					d->menu2[i].a=0;
				}
			}
		}
	}
}

void open_menu2(DESKTOP *d)
{
	SPRITE_GROUP *sg=&sprite_group;

	printf("open menu2 path:%s name:%s\n",d->menu2path,d->menu2name);

	DIR *dir;
	struct dirent *e;

	int i;
	for (i=0;i<32;i++) {
		if (d->menu2[i].n) {
			free(d->menu2[i].n);
			free(d->menu2[i].s);
		}
		d->menu2[i].n=NULL;
		d->menu2[i].s=NULL;
		d->menu2[i].t=0;
	}
	dir=opendir(d->menu2path);
	int num=0;
	int len=0;
	if (dir!=NULL) {
		int max=16;
		int i=0;
		for (e=readdir(dir);e!=NULL;e=readdir(dir)) {
			if (strcmp(e->d_name,"..")==0) continue;
			if (strcmp(e->d_name,".")==0) continue;
			if (e->d_type==DT_DIR) {
				d->menu2[i].n=strdup(e->d_name);
				char tn[32];
				snprintf(tn,31,"%s..",e->d_name+2);
				d->menu2[i].s=strdup(tn);
				d->menu2[i].t=1;
				int l=strlen(d->menu2[i].s);
				if (l>len) len=l;
				i++;
			}
			if (e->d_type==DT_REG) {
				d->menu2[i].n=strdup(e->d_name);
				d->menu2[i].s=strdup(e->d_name+2);
				d->menu2[i].t=2;
				if (d->menu2[i].s[0]=='-') d->menu2[i].t=0xff;
				int l=strlen(d->menu2[i].s);
				if (l>len) len=l;
				i++;
			}
			num=i;
			if (i>max) break;
		}
		closedir(dir);
	}
	int comp(const void *e1,const void *e2)
	{
		MENU_ITEM *m1=(MENU_ITEM*)e1;
		MENU_ITEM *m2=(MENU_ITEM*)e2;
		return strcmp(m1->n,m2->n);
	}
	qsort(d->menu2,num,sizeof(MENU_ITEM),comp);
	for (i=0;i<num;i++) {
		printf("%s ",d->menu2[i].n);
	}
	printf("\n");

	draw_order.order[252]=252;
	int spr=d->spr_menu2;
	sg->start[252]=spr;
	int eh=22;
	int h=40+num*eh;
	if (h<80) h=80;
	int yp=d->menu_y-5;
	int x=d->menu1_x+d->menu1_w+1;
	int y=yp-h;
	int w=15+len*11;
	int wmin=24+5+10+strlen(d->menu2name)*11;
	if (w<wmin) w=wmin;
	add_block(x,y,w,h,&spr,14,0,0x10|0x08|0x04|0x02|0x01);
	add_str_h(x+5,y+5,-1,d->menu2name,&spr,0,0);
	set_sprite(spr++,x,y+30,w,1,120,40,1,1,0x80, 0, 0,0, 0, 0,0, 0, 0,  0, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	d->menu2up_x=x+w-29;
	d->menu2up_y=y+3;
	d->menu2up_a=0;
	d->menu2up_spr=spr;
	set_sprite(spr++,d->menu2up_x,d->menu2up_y,24,24,200+72,64,24,24,0x80, 0, 0,0, 0, 0,0, 0, 0,  0, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	for (i=0;i<num;i++) {
		if (d->menu2[i].t==1) printf("[%s]\n",d->menu2[i].n);
		if (d->menu2[i].t==2) printf("%s\n",d->menu2[i].n);
		d->menu2[i].spr=spr;
		int ix=x+5,iy=y+30+5+(eh*i),iw=w-10+2,ih=eh;
		add_block_text(ix,iy,iw,ih,2,0,-1,&spr,14,0,d->menu2[i].s,0x01,0);
		d->menu2[i].x=ix;
		d->menu2[i].y=iy;
		d->menu2[i].w=iw;
		d->menu2[i].h=ih;
	}

	sg->end[252]=spr;
	update_sprite_o_xy_wh_txy_twh_set_rot_pal_filter(252);
	d->menu2_x=x;
	d->menu2_y=y;
	d->menu2_w=w;
	d->menu2_h=h;
}

void cursor_inside_menu(DESKTOP *d)
{
	if (d->menu_open) {
		int i;
		for (i=0;d->menu1[i].n!=NULL;i++) {
			int x=mousebuf[10]-d->menu1[i].x;
			int y=mousebuf[11]-d->menu1[i].y;
			int w=d->menu1[i].w;
			int h=d->menu1[i].h;
			if (d->menu1[i].t==0xff) continue;
			if ((x>0)&&(x<w)&&(y>0)&&(y<h)) {
				printf("cursor inside menu item:%s\n",d->menu1[i].s);
				if (d->menu1[i].a==0) {
					update_sprite_pal(d->menu1[i].spr,1);
					d->menu1[i].a=1;
				}
				if (d->mouse_1_click) {
					d->mouse_1_click=0;
					printf("menu click on ");
					if (d->menu1[i].t==1) printf("dir:");
					if (d->menu1[i].t==2) printf("file:");
					printf("%s\n",d->menu1[i].s);
					if (d->menu1[i].t==1) {
						sprintf(d->menu2path,"menu/%s",d->menu1[i].n);
						snprintf(d->menu2name,strlen(d->menu1[i].s)-1,"%s",d->menu1[i].s);
						open_menu2(d);
						return;
					}
					if (d->menu1[i].t==2) {
						d->newwin(20,20,400,250,d->menu1[i].s);
						update_sprite_o_xy_wh_txy_twh_set_rot_pal_filter(32+(d->pos-1));
						printf("create new win:%i\n",d->pos);
					}
				}
			} else {
				if (d->menu1[i].a) {
					update_sprite_pal(d->menu1[i].spr,14);
					d->menu1[i].a=0;
				}
			}
		}
	}
}

int close_menu(DESKTOP *d)
{
	int ret=d->menu_open;
	if (d->menu_open) {
		printf("close menu\n");
		d->menu_open=0;
		draw_order.order[251]=-1;
		draw_order.order[252]=-1;
		int spr=d->spr_menu;
		update_sprite_v_size_xy(spr+3,d->menu_w,1);
	}
	return ret;
}

void open_menu(DESKTOP *d,int update)
{
	if ((inside_menu(d)==1)||update) {
		if (close_menu(d)) return;
		printf("open menu\n");
		draw_order.order[251]=251;
		d->menu_open=1;

		{int spr=d->spr_menu;
		 update_sprite_v_size_xy(spr+3,d->menu_w,4);
		}

		DIR *dir;
		struct dirent *e;

		int i;
		for (i=0;i<32;i++) {
			if (d->menu1[i].n) {
				free(d->menu1[i].n);
				free(d->menu1[i].s);
			}
			d->menu1[i].n=NULL;
			d->menu1[i].s=NULL;
			d->menu1[i].t=0;
		}
		dir=opendir("menu");
		int num=0;
		int len=0;
		if (dir!=NULL) {
			int max=16;
			int i=0;
			for (e=readdir(dir);e!=NULL;e=readdir(dir)) {
				if (strcmp(e->d_name,"..")==0) continue;
				if (strcmp(e->d_name,".")==0) continue;
				if (e->d_type==DT_DIR) {
					d->menu1[i].n=strdup(e->d_name);
					char tn[32];
					snprintf(tn,31,"%s..",e->d_name+2);
					d->menu1[i].s=strdup(tn);
					d->menu1[i].t=1;
					int l=strlen(d->menu1[i].s);
					if (l>len) len=l;
					i++;
				}
				if (e->d_type==DT_REG) {
					d->menu1[i].n=strdup(e->d_name);
					d->menu1[i].s=strdup(e->d_name+2);
					d->menu1[i].t=2;
					if (d->menu1[i].s[0]=='-') d->menu1[i].t=0xff;
					int l=strlen(d->menu1[i].s);
					if (l>len) len=l;
					i++;
				}
				num=i;
				if (i>max) break;
			}
			closedir(dir);
		}
		int comp(const void *e1,const void *e2)
		{
			MENU_ITEM *m1=(MENU_ITEM*)e1;
			MENU_ITEM *m2=(MENU_ITEM*)e2;
			return strcmp(m1->n,m2->n);
		}
		qsort(d->menu1,num,sizeof(MENU_ITEM),comp);
		for (i=0;i<num;i++) {
			printf("%s ",d->menu1[i].n);
		}
		printf("\n");
		SPRITE_GROUP *sg=&sprite_group;

		int eh=22;
		int height=num*eh;
		if (height<90) height=90;
		int spr=d->spr_menu1;
		sg->start[251]=spr;
		int yp=d->menu_y-15;
		int x=5;
		int y=yp-height;
		int w=40+len*11;if (w<80) w=80;
		int h=10+height;
		add_block(x,y,w,h,&spr,14,0,0x10|0x08|0x04|0x02|0x01);
		set_sprite(spr++,35,y,1,h,120,40,1,1,0x80, 0, 0,0, 0, 0,0, 0, 0,  0, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
		add_str_v(15,yp-10,"Desktop",&spr,0,0);
		for (i=0;i<num;i++) {
			if (d->menu1[i].t==1) printf("[%s]\n",d->menu1[i].n);
			if (d->menu1[i].t==2) printf("%s\n",d->menu1[i].n);
			d->menu1[i].spr=spr;
			int ix=40,iy=y+5+(eh*i),iw=w-40+2,ih=eh;
			add_block_text(ix,iy,iw,ih,2,0,-1,&spr,14,0,d->menu1[i].s,0x01,0);
			d->menu1[i].x=ix;
			d->menu1[i].y=iy;
			d->menu1[i].w=iw;
			d->menu1[i].h=ih;
		}
		sg->end[251]=spr;
		update_sprite_o_xy_wh_txy_twh_set_rot_pal_filter(251);
		d->menu1_x=x;
		d->menu1_y=y;
		d->menu1_w=w;
		d->menu1_h=h;
	}
}

void create_menu(DESKTOP *d)
{
	SPRITE_GROUP *sg=&sprite_group;
	DRAW_ORDER *dr=&draw_order;

	int yp=video_var.screen_y-45;
	d->menu_x=5;
	d->menu_y=yp+10;
	d->menu_w=80;
	d->menu_h=30;
	int w=video_var.screen_x;
	int spr=d->spr_menu;
	sg->start[250]=spr;
	set_sprite(spr++,0,yp,w,50,120,40,1,1,0x80, 0, 0,0, 0, 0,0, 0, 0, 14, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(spr++,0,yp,w,1,120,40,1,1,0x80, 0, 0,0, 0, 0,0, 0, 0,  0, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	add_block_text(d->menu_x,d->menu_y,d->menu_w,d->menu_h,
		(d->menu_w-strlen("MENU")*11)/2-2,5,-1,&spr,14,0,"MENU",0x10|0x08|0x04|0x02|0x01,0);
	sg->end[250]=spr;
	sg->active[7]|=1<<26;
	dr->order[250]=250;

	sg->active[7]|=1<<27;
	sg->active[7]|=1<<28;
//	dr->order[251]=-1;
//	d->menu_open=0;

}

void create_taskbar(DESKTOP *d)
{

}

__attribute__ ((noinline,optimize(0))) void *get_pc(){return __builtin_return_address(0);}

__attribute__ ((noinline,optimize(0))) void _newwin(int x,int y,int w,int h,char *name)
{
	void* (*pget_pc)()=get_pc;
	int (*pgetpagesize)()=getpagesize;
	uint64_t m=(uint64_t)pget_pc() & (~((uint64_t)pgetpagesize()-1));
	void (*pnewwin)(DESKTOP*,int,int,int,int,char*)=newwin;
	pnewwin(((DESKTOP **)m)[0],x,y,w,h,name);
}
__attribute__((noinline,optimize(0))) void _newwin_e(){};

void init_desktop(DESKTOP *d)
{
	memset(d,0,sizeof(DESKTOP));
	int align=getpagesize();
	void *m=NULL;
	posix_memalign(&m,align,align);
	printf(":bytes=%lu\n",(uint64_t)_newwin_e - (uint64_t)_newwin);
	((uint64_t *)m)[0]=(uint64_t)d;
	memcpy(m+8,_newwin,(uint64_t)_newwin_e - (uint64_t)_newwin);
	d->newwin=m+8;
	mprotect(m,align,PROT_READ|PROT_WRITE|PROT_EXEC);
	//default _values
	d->actwin=-1;
	d->minwinw=150;
	d->minwinh=150;
	d->cnt=0;
	desktop.spr=50000;
	desktop.spr_background=0;
	desktop.spr_cursor=65501;
	desktop.spr_actframe=65510;
	desktop.spr_menu=60000;
	desktop.spr_menu1=61000;
	desktop.spr_menu2=62000;
	desktop.spr_task=63000;
}

void update_tex_all()
{
	VIDEO_VAR *v=&video_var;

	update_texture(v->act_rg16i,v->gl_rg16i, 0,0,256,256,0,      GL_RG_INTEGER,GL_SHORT,tx_data_order);
	update_texture(v->act_rg16i,v->gl_rg16i, 256,0,256,256,256,  GL_RG_INTEGER,GL_SHORT,tx_data_v_xy);
	update_texture(v->act_rg16i,v->gl_rg16i, 512,0,256,256,256,  GL_RG_INTEGER,GL_SHORT,tx_data_v_size_xy);
	update_texture(v->act_rg16i,v->gl_rg16i, 768,0,256,256,256,  GL_RG_INTEGER,GL_SHORT,tx_data_t_xy);
	update_texture(v->act_rg16i,v->gl_rg16i, 0,256,256,256,256,  GL_RG_INTEGER,GL_SHORT,tx_data_t_size_xy);
	update_texture(v->act_rg16i,v->gl_rg16i, 256,256,256,256,256,GL_RG_INTEGER,GL_SHORT,tx_data_rot_center);
	update_texture(v->act_rg16i,v->gl_rg16i, 512,256,256,256,256,GL_RG_INTEGER,GL_SHORT,tx_data_orbit_center);
	update_texture(v->act_rg16i,v->gl_rg16i, 768,256,256,256,256,GL_RG_INTEGER,GL_SHORT,tx_data_repeat_scale);
	update_texture(v->act_rg16i,v->gl_rg16i, 0,512,256,256,256,  GL_RG_INTEGER,GL_SHORT,tx_data_repeat_offset);
	update_texture(v->act_rg16i,v->gl_rg16i, 256,512,256,256,256,GL_RG_INTEGER,GL_SHORT,tx_data_polygon0);
	update_texture(v->act_rg16i,v->gl_rg16i, 512,512,256,256,256,GL_RG_INTEGER,GL_SHORT,tx_data_polygon1);
	update_texture(v->act_rg16i,v->gl_rg16i, 768,512,256,256,256,GL_RG_INTEGER,GL_SHORT,tx_data_polygon2);
	update_texture(v->act_rg16i,v->gl_rg16i, 0,768,256,256,256,  GL_RG_INTEGER,GL_SHORT,tx_data_polygon3);

	update_texture(v->act_r16i,v->gl_r16i,  0,0,256,256,256,    GL_RED_INTEGER,GL_SHORT,tx_data_settings);
	update_texture(v->act_r16i,v->gl_r16i,  256,0,256,256,256,  GL_RED_INTEGER,GL_SHORT,tx_data_rot);
	update_texture(v->act_r16i,v->gl_r16i,  512,0,256,256,256,  GL_RED_INTEGER,GL_SHORT,tx_data_orbit_rot);
	update_texture(v->act_r16i,v->gl_r16i,  768,0,256,256,256,  GL_RED_INTEGER,GL_SHORT,tx_data_light);
	update_texture(v->act_r16i,v->gl_r16i,  0,256,256,256,256,  GL_RED_INTEGER,GL_SHORT,tx_data_alpha);
	update_texture(v->act_r16i,v->gl_r16i,  256,256,256,256,256,GL_RED_INTEGER,GL_SHORT,tx_data_pal_idx);
	update_texture(v->act_r16i,v->gl_r16i,  512,256,256,256,256,GL_RED_INTEGER,GL_SHORT,tx_data_layer);

	update_texture(v->act_rgba8,v->gl_rgba8, 0,0,256,256,256,  GL_RGBA,GL_UNSIGNED_BYTE,tx_data_pallete);
	update_texture(v->act_rgba8,v->gl_rgba8, 256,0,256,256,256,GL_RGBA,GL_UNSIGNED_BYTE,tx_data_color_key);
	update_texture(v->act_rgba8,v->gl_rgba8, 512,0,256,256,256,GL_RGBA,GL_UNSIGNED_BYTE,tx_data_filter);
}

void set_sprite(unsigned short int sprite_idx,
				short int x,short int y,
				short int w,short int h,
				short int tex_x,short int tex_y,
				short int tex_w,short int tex_h,
				short int settings,
				short int rot,
				short int rot_x,short int rot_y,
				short int orbit_rot,
				short int orbit_x,short int orbit_y,
				short int light,
				short int alpha,
				short int pal_idx,
				unsigned char ckey_r,unsigned char ckey_g,unsigned char ckey_b,unsigned char ckey_a,
				short int repeat_scale_x,short int repeat_scale_y,
				short int repeat_offset_x,short int repeat_offset_y,
				unsigned char filter_amount,unsigned char filter_cut,unsigned char filter_max,unsigned char filter_rgb,
				short int xp0,short int yp0,
				short int xp1,short int yp1,
				short int xp2,short int yp2,
				short int xp3,short int yp3
				)
{
	int i=sprite_idx>>8;
	int j=sprite_idx&0xff;
	{short int *p=(short int*)&tx_data_order[sprite_idx];
		p[0]=j;
		p[1]=i;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_v_xy;
		short int *p=(short int*)&buf[i][j];
		p[0]=x;
		p[1]=y;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_v_size_xy;
		short int *p=(short int*)&buf[i][j];
		p[0]=w;
		p[1]=h;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_t_xy;
		short int *p=(short int*)&buf[i][j];
		p[0]=tex_x;
		p[1]=tex_y;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_t_size_xy;
		short int *p=(short int*)&buf[i][j];
		p[0]=tex_w;
		p[1]=tex_h;
	}
	{short int (*buf)[256]=(short int(*)[256]) tx_data_settings;
		//r 16bit bits
		//  rot           0  0x0001
		//  rot_center    1  0x0002
		//  orbit         2  0x0004
		//  mirrox_x      3  0x0008
		//  mirrox_y      4  0x0010
		//  alpha         5  0x0020
		//  light         6  0x0040
		//  pallete       7  0x0080
		//  color_key     8  0x0100
		//  color_key_inv 9  0x0200
		//  polygon       10 0x0400
		//  repeat        11 0x0800
		//  zero_color    12 0x1000 color.rgb=0
		//  draw_hole     13 0x2000 if (color.a>0) color.a=0 else color.a=1
		buf[i][j]=settings;
	}
	{short int (*buf)[256]=(short int(*)[256]) tx_data_rot;
		buf[i][j]=rot;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_rot_center;
		short int *p=(short int*)&buf[i][j];
		p[0]=rot_x;
		p[1]=rot_y;
	}
	{short int (*buf)[256]=(short int(*)[256]) tx_data_orbit_rot;
		buf[i][j]=orbit_rot;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_orbit_center;
		short int *p=(short int*)&buf[i][j];
		p[0]=orbit_x;
		p[1]=orbit_y;
	}
	{short int (*buf)[256]=(short int(*)[256]) tx_data_light;
		buf[i][j]=light;
	}
	{short int (*buf)[256]=(short int(*)[256]) tx_data_alpha;
		buf[i][j]=alpha;
	}
	{short int (*buf)[256]=(short int(*)[256]) tx_data_pal_idx;
		buf[i][j]=pal_idx;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_color_key;
		unsigned char *p=(unsigned char *)&buf[i][j];
		p[0]=ckey_r;
		p[1]=ckey_g;
		p[2]=ckey_b;
		p[3]=ckey_a;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_repeat_scale;
		short int *p=(short int *)&buf[i][j];
		p[0]=repeat_scale_x;
		p[1]=repeat_scale_y;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_repeat_offset;
		short int *p=(short int *)&buf[i][j];
		p[0]=repeat_offset_x;
		p[1]=repeat_offset_y;
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_filter;
		unsigned char *p=(unsigned char *)&buf[i][j];
		p[0]=filter_amount;
		p[1]=filter_cut;
		p[2]=filter_max;
		p[3]=filter_rgb;
	}
	{
		unsigned int (*buf0)[256]=(unsigned int(*)[256]) tx_data_polygon0;
		unsigned int (*buf1)[256]=(unsigned int(*)[256]) tx_data_polygon1;
		unsigned int (*buf2)[256]=(unsigned int(*)[256]) tx_data_polygon2;
		unsigned int (*buf3)[256]=(unsigned int(*)[256]) tx_data_polygon3;
		short int *p0=(short int *)&buf0[i][j];
		short int *p1=(short int *)&buf1[i][j];
		short int *p2=(short int *)&buf2[i][j];
		short int *p3=(short int *)&buf3[i][j];
		p0[0]=xp0;p0[1]=yp0;
		p1[0]=xp1;p1[1]=yp1;
		p2[0]=xp2;p2[1]=yp2;
		p3[0]=xp3;p3[1]=yp3;
	}

}

void set_layer(	LAYER *l,
				short int idx,
				short int vpx,short int vpy,short int vpw,short int vph,
				short int x,short int y,short int w,short int h,
				short int txywh[4],
				short int repeat_scale[2],short int repeat_offset[2],
				short int pxy[4][2],
				short int rot_angle,short int rot_center_xy[2],
				short int orbit_angle,short int orbit_center_xy[2],
				short int color_key[3],
				short int filter_amount[4],
				short int pallete_idx,
				short int use_abc,short int ta_xy[2],short int tc_xy[2],
				short int settings,
				int quad_start,int quad_end,
				float clear_color[4],
				float saturation,
				float contrast,
				float lightness,
				float gamma,
				float alpha
				)
{
	short int (*data)[256]=(short int(*)[256])tx_data_layer;

	//xywh
	data[idx][0]=x;
	data[idx][1]=y;
	data[idx][2]=w;
	data[idx][3]=h;
	//txywh
	data[idx][4]=vpx;
	data[idx][5]=vpy;
	data[idx][6]=vpw;
	data[idx][7]=vph;
	if (txywh!=NULL) {
		data[idx][4]=txywh[0];
		data[idx][5]=txywh[1];
		data[idx][6]=txywh[2];
		data[idx][7]=txywh[3];
	}
	//polygon
	data[idx][8]=pxy[0][0];data[idx][9]=pxy[0][1];
	data[idx][10]=pxy[1][0];data[idx][11]=pxy[1][1];
	data[idx][12]=pxy[2][0];data[idx][13]=pxy[2][1];
	data[idx][14]=pxy[3][0];data[idx][15]=pxy[3][1];
	//scale
	data[idx][16]=repeat_scale[0];data[idx][17]=repeat_scale[1];
	//offset
	data[idx][18]=repeat_offset[0];data[idx][19]=repeat_offset[1];
	//rot
	data[idx][20]=rot_angle;
	data[idx][21]=rot_center_xy[0];data[idx][22]=rot_center_xy[1];
	//orbit
	data[idx][23]=orbit_angle;
	data[idx][24]=orbit_center_xy[0];data[idx][25]=orbit_center_xy[1];
	//color key
	data[idx][26]=color_key[0];data[idx][27]=color_key[1];data[idx][28]=color_key[2];
	//filter
	data[idx][29]=filter_amount[0];data[idx][30]=filter_amount[1];data[idx][31]=filter_amount[2];data[idx][32]=filter_amount[3];
	//pallete_idx
	data[idx][33]=pallete_idx;
	//abc
	data[idx][34]=use_abc;data[idx][35]=ta_xy[0];data[idx][36]=ta_xy[1];data[idx][37]=tc_xy[0];data[idx][38]=tc_xy[1];
	//settings
	data[idx][39]=settings;
	//  rot         0x0001
	//  rot_center  0x0002
	//  orbit       0x0004
	//  mirrox_x    0x0008
	//  mirrox_y    0x0010
	//  pallete     0x0080
	//  color_key   0x0100
	//  polygon     0x0400
	//  repeat      0x0800
	//  zero_color  0x1000 color.rgb=0
	//  draw_hole   0x2000 if (color.a>0) color.a=0 else color.a=1

	l->layer_idx=idx;
	l->vpx=vpx;
	l->vpy=vpy;
	l->vpw=vpw;
	l->vph=vph;
	ortho2d(0,0,vpw,vph,l->pr_ortho2d);

	l->quad_start=quad_start;
	l->quad_end=quad_end;
	l->clear_color[0]=clear_color[0];
	l->clear_color[1]=clear_color[1];
	l->clear_color[2]=clear_color[2];
	l->clear_color[3]=clear_color[3];
	l->saturation=saturation;
	l->contrast=contrast;
	l->lightness=lightness;
	l->gamma=gamma;
	l->alpha=alpha;
}

void add_str_h(int x,int y,int lmax,char *str,int *spidx,int pal,int font)
{
	int w=11;
	int h=22;
	int fx=0;
	int fy=48+48+108;
	if (font==1) {
		w=8;
		h=8;
		fx=0;
		fy=0;
	}
	if (font==2) {
		w=8;
		h=8;
		fx=0;
		fy=48;
	}
	if (font==3) {
		w=10;
		h=18;
		fx=0;
		fy=48+48;
	}
	int sp=*spidx;
	int len=strlen(str);
	if (lmax!=-1) len=lmax;
	int i;
	int pos=0;
	for (i=0;i<len;i++) {
		int chr=str[i]-32;
		if (chr<0) chr=95;
		if (chr>95) chr=95;
		int nx=chr&0xf;
		int ny=chr>>4;
		int tx=fx+nx*w;
		int ty=fy+ny*h;
		set_sprite(sp++,x+pos,y,w,h, tx,ty,w,h, 0x80, 0, 0,0, 0, 0,0, 0, 0, pal, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
		pos+=w;
	}
	*spidx=sp;
}
void add_str_v(int x,int y,char *str,int *spidx,int pal,int font)
{
	int w=11;
	int h=22;
	int fx=0;
	int fy=48+48+108;
	if (font==1) {
		w=8;
		h=8;
		fx=0;
		fy=0;
	}
	if (font==2) {
		w=8;
		h=8;
		fx=0;
		fy=48;
	}
	if (font==3) {
		w=10;
		h=18;
		fx=0;
		fy=48+48;
	}
	int sp=*spidx;
	int len=strlen(str);
	int i;
	int pos=0;
	for (i=0;i<len;i++) {
		int chr=str[i]-32;
		if (chr<0) chr=95;
		if (chr>95) chr=95;
		int nx=chr&0xf;
		int ny=chr>>4;
		int tx=fx+nx*w;
		int ty=fy+ny*h;
		set_sprite(sp++,x,y-pos,w,h, tx,ty,w,h, 0x80|0x01, 16384, 0,0, 0, 0,0, 0, 0, pal, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
		pos+=w;
	}
	*spidx=sp;
}

void add_block(int x,int y,int w,int h,int *spidx,int p1,int p2,int settings)
{
	int spr=*spidx;
	int tl=0,tr=0,tt=0,tb=0;
	if (settings&0x020) tt=1;
	if (settings&0x040) tb=1;
	if (settings&0x080) tl=1;
	if (settings&0x0100) tr=1;
	if (settings&0x01) set_sprite(spr++, x,    y,    w,   h,   120,40,1,1,0x80, 0, 0,0, 0, 0,0, 0, 0, p1, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	if (settings&0x02) set_sprite(spr++, x,    y,    w,   1+tt,120,40,1,1,0x80, 0, 0,0, 0, 0,0, 0, 0, p2, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	if (settings&0x04) set_sprite(spr++, x,    y+h-1,w,   1+tb,120,40,1,1,0x80, 0, 0,0, 0, 0,0, 0, 0, p2, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	if (settings&0x08) set_sprite(spr++, x,    y,    1+tl,h,   120,40,1,1,0x80, 0, 0,0, 0, 0,0, 0, 0, p2, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	if (settings&0x10) set_sprite(spr++, x+w-1,y,    1+tr,h,   120,40,1,1,0x80, 0, 0,0, 0, 0,0, 0, 0, p2, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	*spidx=spr;
}

void add_block_text(int x,int y,int w,int h,int tx,int ty,int lmax,int *spidx,int p1,int p2,char *str,int settings,int font)
{
	add_block(x,y,w,h,spidx,p1,p2,settings);
	add_str_h(x+tx,y+ty,lmax,str,spidx,p2,font);
}

void create_clock(int idx)
{
	VIDEO_VAR *v=&video_var;
	SPRITE_GROUP *sg=&sprite_group;

	int spr=65200;
	sg->start[idx]=spr;

	time_t t;
	struct tm *tinfo;
	time(&t);
	tinfo=localtime(&t);
	char str[64];
	memset(str,0,sizeof(str));
	int len=sprintf(str,"%s",asctime(tinfo));
	str[len-1]=0;
	int w=v->screen_x;
	int yp=v->screen_y-40;

	add_str_h(w-200,yp+((40-8)/2),-1,str,&spr,0,2);

	sg->end[idx]=spr;
}

void create_fps_info(int idx)
{
	VIDEO_VAR *v=&video_var;
	SPRITE_GROUP *sg=&sprite_group;

	int spr=65300;
	sg->start[idx]=spr;

	char str[64];
	sprintf(str,"%0.0f %0.0f %0.0f %i",1000000.0/v->wtime,v->rtime,v->wtime,v->usleep);

	add_str_h(100,10,-1,str,&spr,0,0);

	sg->end[idx]=spr;
}

void init_data()
{

	memset(opengl_buf,0x55,ogl_buf_size);
	memset(opengl_data,0xaa,ogl_data_size);

	{
		void create_chars(int y,int x,int w,int h,int row,int bit,int bs,void *font,int ay,void *adr)
		{
			uint8_t  (*ascii8)[h]=(uint8_t(*)[h]) font;
			uint16_t (*ascii16)[h]=(uint16_t(*)[h]) font;
			unsigned int (*buf)[row]=(unsigned int(*)[row]) adr;
			int i;
			for (i=0;i<ay;i++) {
				int j;
				for (j=0;j<h;j++) {
					int k;
					for (k=0;k<w;k++) {
						int act=bit>>k;
						if (bs==8) act= ascii8[i][j]&act;
						if (bs==16)act=ascii16[i][j]&act;
						buf[y+j][x+k]=0;
						if (act) buf[y+j][x+k]=0xff000000;
					}
				}
				x+=w;
				if (x>=row) {
					x=0;
					y+=h;
				}
			}
		}
		create_chars(0,0,8,8,128,  0b10000000,    8,font1_x20_x7f,96,opengl_buf);
		create_chars(0,0,8,8,128,  0b10000000,    8,font2_x20_x7f,96,&opengl_buf[(128*48)]);
		create_chars(0,0,10,18,160,0b1000000000, 16,font3_x20_x7f,96,&opengl_buf[(128*48)+(128*48)]);
		create_chars(0,0,11,22,176,0b10000000000,16,font4_x20_x7f,96,&opengl_buf[(128*48)+(128*48)+(160*108)]);
		{
			void load_data(char *name,void *buf,int size)
			{
				int f=open(name,O_RDONLY);
				if (f<0) {
					printf("error load file:%s\n",name);
					exit(-1);
				}
				read(f,buf,size);
				close(f);
			}
			load_data("symbols_16x16.data",&opengl_buf[(128*48)+(128*48)+(160*108)+(176*132)],64*64*4);
			load_data("symbols_24x24.data",&opengl_buf[(128*48)+(128*48)+(160*108)+(176*132)+(64*64)],96*24*4);
			load_data("symbols_32x32.data",&opengl_buf[(128*48)+(128*48)+(160*108)+(176*132)+(64*64)+(96*24)],128*32*4);
		}
	}

	VIDEO_VAR *v=&video_var;
	LAYER_GROUP *lg=&layer_group;
	SPRITE_GROUP *sg=&sprite_group;
	DRAW_ORDER *dr=&draw_order;

	{int i;for (i=0;i<255;i++) dr->order[i]=-1;}
	dr->start=0;
	dr->end=256;

	lg->active[0]=0;

	init_desktop(&desktop);

	//background
	set_sprite(desktop.spr_background, 0,0,v->screen_x,v->screen_y, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 14,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//font
	set_sprite(desktop.spr_cursor-1,1280,10,512,512, 0,0,512,512, 0x0, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//cursor
	set_sprite(desktop.spr_cursor,200,200,16,16,   200,16,16,16, 0x80, 0, 0,0, 0, 0,0, 0, 0, 14, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	//actframe
	set_sprite(desktop.spr_actframe+0, 0,0  ,100,1,   120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 1, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(desktop.spr_actframe+1, 0,100,100,1,   120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 1, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(desktop.spr_actframe+2, 0,30 ,100,1,   120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 1, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(desktop.spr_actframe+3, 0,0  ,1  ,100, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 1, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(desktop.spr_actframe+4, 99,0 ,1,  100, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 1, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);

	sg->start[0]=0;
	sg->end[0]=1;
	sg->start[255]=desktop.spr_cursor-1;
	sg->end[255]=desktop.spr_cursor+1;
	sg->start[240]=desktop.spr_actframe;
	sg->end[240]=desktop.spr_actframe+5;
	sg->active[0]=1;
	sg->active[7]=1<<31;
	sg->active[7]|=1<<16;

	dr->order[0]=0;
	dr->order[240]=-1;
	dr->order[255]=255;

	desktop.newwin(10,100,501,301,"Terminal");
	desktop.newwin(530,10,202,302,"Programs");
	desktop.newwin(750,10,503,303,"Settings");
	desktop.newwin(600,350,504,304,"Files");

	sg->active[7]|=1<<29;
	sg->active[7]|=1<<30;
	dr->order[253]=253;
	dr->order[254]=254;

	create_menu(&desktop);

		//r 16bit bits 
		//  rot           0  0x0001
		//  rot_center    1  0x0002
		//  orbit         2  0x0004
		//  mirrox_x      3  0x0008
		//  mirrox_y      4  0x0010
		//  alpha         5  0x0020
		//  light         6  0x0040
		//  pallete       7  0x0080
		//  color_key     8  0x0100
		//  color_key_inv 9  0x0200
		//  polygon       10 0x0400
		//  repeat        11 0x0800
		//  zero_color    12 0x1000 color.rgb=0
		//  draw_hole     13 0x2000 if (color.a>0) color.a=0 else color.a=1

	unsigned int pal[16]={ // a b g r texture this a r g b
		0xff000000,
		0xffffffff,
		0xffd9311a,
		0xff87d4db,
		0xffa05abf,
		0xff71be43,
		0xff4242e7,
		0xffffff00,
		0xffff8835,
		0xff624900,
		0xffffa0a0,
		0xff545454,
		0xff6c6c6c,
		0xff98ff5a,
		0xffa5a5ff,
		0xff959595,
	};
	int s=0;
	int i,j;
	for (i=0;i<16;i++) {
		unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_pallete;
		for (j=0;j<16;j++) {
			unsigned char *p0=(unsigned char*)&buf[i][j];
			unsigned char *p1=(unsigned char*)&pal[s];
			buf[i][j]=pal[s];
			p0[0]=p1[2];
			p0[2]=p1[0];
			s++;
			if (s<16) continue;
			else s=0;
		}
		s++;
	}
	update_texture(v->act_sprites,v->sprites,  0,0,128,48,128, GL_RGBA, GL_UNSIGNED_BYTE,opengl_buf);
	update_texture(v->act_sprites,v->sprites,  0,48,128,48,128, GL_RGBA, GL_UNSIGNED_BYTE,&opengl_buf[128*48]);
	update_texture(v->act_sprites,v->sprites,  0,48+48,160,108,160,GL_RGBA, GL_UNSIGNED_BYTE,&opengl_buf[(128*48)+(128*48)]);
	update_texture(v->act_sprites,v->sprites,  0,48+48+108,176,132,176,GL_RGBA, GL_UNSIGNED_BYTE,&opengl_buf[(128*48)+(128*48)+(160*108)]);
	update_texture(v->act_sprites,v->sprites,  200,0, 64,64,64,GL_RGBA, GL_UNSIGNED_BYTE,&opengl_buf[(128*48)+(128*48)+(160*108)+(176*132)]);
	update_texture(v->act_sprites,v->sprites,  200,64,96,24,96,GL_RGBA, GL_UNSIGNED_BYTE,&opengl_buf[(128*48)+(128*48)+(160*108)+(176*132)+(64*64)]);
	update_texture(v->act_sprites,v->sprites,  200,64+24,128,32,128,GL_RGBA, GL_UNSIGNED_BYTE,&opengl_buf[(128*48)+(128*48)+(160*108)+(176*132)+(64*64)+(96*24)]);

	update_tex_all();
}

void update_sprite_v_xy(int idx,short int px,short int py)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_v_xy;
	short int *p=(short int *)&buf[y][x];
	p[0]=px;
	p[1]=py;
	update_texture(v->act_rg16i,v->gl_rg16i,256+x,y,1,1,256,GL_RG_INTEGER,GL_SHORT,p);
}

void update_sprite_v_size_xy(int idx,short int w,short int h)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_v_size_xy;
	short int *p=(short int *)&buf[y][x];
	p[0]=w;
	p[1]=h;
	update_texture(v->act_rg16i,v->gl_rg16i,512+x,y,1,1,256,GL_RG_INTEGER,GL_SHORT,p);
}

void update_sprite_t_xy(int idx,short int tx,short int ty)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_t_xy;
	short int *p=(short int *)&buf[y][x];
	p[0]=tx;
	p[1]=ty;
	update_texture(v->act_rg16i,v->gl_rg16i,768+x,y,1,1,256,GL_RG_INTEGER,GL_SHORT,p);
}

void update_sprite_t_size_xy(int idx,short int tw,short int th)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_t_size_xy;
	short int *p=(short int *)&buf[y][x];
	p[0]=tw;
	p[1]=th;
	update_texture(v->act_rg16i,v->gl_rg16i,x,256+y,1,1,256,GL_RG_INTEGER,GL_SHORT,p);
}

void update_sprite_settings(int idx,short int settings)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	short int (*buf)[256]=(short int(*)[256]) tx_data_settings;
	buf[y][x]=settings;

	update_texture(v->act_r16i,v->gl_r16i,x,y,1,1,256,GL_RED_INTEGER,GL_SHORT,&buf[y][x]);
}

void update_sprite_rot(int idx,short int rot)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	short int (*buf)[256]=(short int(*)[256]) tx_data_rot;
	buf[y][x]=rot;

	update_texture(v->act_r16i,v->gl_r16i,256+x,y,1,1,256,GL_RED_INTEGER,GL_SHORT,&buf[y][x]);
}

void update_sprite_pal(int idx,short int pal)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	short int (*buf)[256]=(short int(*)[256]) tx_data_pal_idx;
	buf[y][x]=pal;

	update_texture(v->act_r16i,v->gl_r16i,256+x,256+y,1,1,256,GL_RED_INTEGER,GL_SHORT,&buf[y][x]);
}

void update_sprite_filter(int idx,unsigned char filter_amount,unsigned char filter_cut,unsigned char filter_max,unsigned char filter_rgb)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_filter;
	unsigned char *p=(unsigned char *)&buf[y][x];
	p[0]=filter_amount;
	p[1]=filter_cut;
	p[2]=filter_max;
	p[3]=filter_rgb;

	update_texture(v->act_rgba8,v->gl_rgba8,512+x,y,1,1,256,GL_RGBA,GL_UNSIGNED_BYTE,&buf[y][x]);
}

void update_layer_offset(int idx,short int ox,short int oy)
{
	VIDEO_VAR *v=&video_var;

	short int (*data)[256]=(short int(*)[256])tx_data_layer;
	data[idx][18]=ox;data[idx][19]=oy;
	update_texture(v->act_r16i,v->gl_r16i,512,256+idx,256,1,256,GL_RED_INTEGER,GL_SHORT,&data[idx][0]);
}

void update_sprite_o_xy_wh_txy_twh_set_rot_pal_filter(int idx)
{
	VIDEO_VAR *v=&video_var;
	SPRITE_GROUP *sg=&sprite_group;

	int start=sg->start[idx];
	int end=sg->end[idx];

	int y=start>>8;
	int h=((end>>8)-y)+1;
	{short int *p=(short int*)&tx_data_order[y*256];
		update_texture(v->act_rg16i,v->gl_rg16i, 0,y,256,h,0,GL_RG_INTEGER,GL_SHORT,p);
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_v_xy;
		short int *p=(short int*)&buf[y][0];
		update_texture(v->act_rg16i,v->gl_rg16i, 256,y,256,h,256,GL_RG_INTEGER,GL_SHORT,p);
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_v_size_xy;
		short int *p=(short int*)&buf[y][0];
		update_texture(v->act_rg16i,v->gl_rg16i,512,y,256,h,256,GL_RG_INTEGER,GL_SHORT,p);
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_t_xy;
		short int *p=(short int*)&buf[y][0];
		update_texture(v->act_rg16i,v->gl_rg16i,768,y,256,h,256,GL_RG_INTEGER,GL_SHORT,p);
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_t_size_xy;
		short int *p=(short int*)&buf[y][0];
		update_texture(v->act_rg16i,v->gl_rg16i,0,256+y,256,h,256,GL_RG_INTEGER,GL_SHORT,p);
	}
	{short int (*buf)[256]=(short int(*)[256]) tx_data_settings;
		short int *p=(short int*)&buf[y][0];
		update_texture(v->act_r16i,v->gl_r16i,0,y,256,h,256,GL_RED_INTEGER,GL_SHORT,p);
	}
	{short int (*buf)[256]=(short int(*)[256]) tx_data_rot;
		short int *p=(short int*)&buf[y][0];
		update_texture(v->act_r16i,v->gl_r16i,256,y,256,h,256,GL_RED_INTEGER,GL_SHORT,p);
	}
	{short int (*buf)[256]=(short int(*)[256]) tx_data_pal_idx;
		short int *p=(short int*)&buf[y][0];
		update_texture(v->act_r16i,v->gl_r16i,256,256+y,256,h,256,GL_RED_INTEGER,GL_SHORT,p);
	}
	{unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_filter;
		unsigned int *p=(unsigned int*)&buf[y][0];
		update_texture(v->act_rgba8,v->gl_rgba8, 512,y,256,h,256,GL_RGBA,GL_UNSIGNED_BYTE,p);
	}
}
