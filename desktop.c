
#include "font1_x20_x7f.c"
#include "font2_x20_x7f.c"
#include "font3_x20_x7f.c"
#include "font4_x20_x7f.c"

#ifndef __kms__
	#include "x11/x11.c"
#else
	#include "drm/drm.c"
#endif

#include "ogl/ogl.c"

#include <time.h>
#include <stdarg.h>
#include <dirent.h>
#include <errno.h>

#include "set_data_desktop.c"

void do_stuff()
{


}

int main(int argc, char *argv[])
{

#ifndef __kms__
	open_display("desktop");
	set_window_size(1280,720);
	set_window_pos(-1,-1);
	set_cursor(0);

	int x,y;
	get_window_size(&x,&y);
	printf("WINDOW_SIZE:\t\t\t\t%ix%i\n",x,y);
#else
	int x,y;
	setup_drm(&x,&y,1920,1080);
#endif

	print_gl_info();

	init_opengl(x,y);

	init_data();

	while(get_input()==0) {

#ifndef __kms__
		int draw=0;

		if (keybuf[9]) {
			printf(":key ESC\n");
			goto end;
		}
		static int key_f=0;
		if (keybuf[41]) {
			if (key_f==0) {
				key_f=1;
				static int fullscreen=0;
				fullscreen^=1;
				set_fullscreen(fullscreen);
				int x,y;
				get_window_size(&x,&y);
				resize_framebuffer(x,y);
				update_sprite_v_size_xy(0,x,y);
				create_clock(253);
				update_sprite_o_xy_wh_txy_twh_set_rot_pal_filter(253);
				create_menu(&desktop);
				if (desktop.menu_open) {
					desktop.menu_open=0;
					open_menu(&desktop,1);
					open_menu2(&desktop);
				}
				update_tex_all();
			}
		} else key_f=0;
#else
		int draw=0;
		if (keybuf[1]) {
			printf(":key ESC\n");
			goto end;
		}
#endif

		void activate_window(WINDOW *w)
		{
			if (desktop.actwin!=desktop.topwin) {
				if (desktop.actwin!=-1) {
					int pos=desktop.l[desktop.actwin];
					WINDOW *win=&desktop.wl.w[pos];
					win->a=0;
					update_sprite_v_size_xy(win->sp0+2,win->w,1+(win->a*3));
					printf("deactivating:%s %i %i %i %i\n",win->name,win->x,win->y,win->w,win->h);
				}
				printf("activate:%s %i %i %i %i\n",w->name,w->x,w->y,w->w,w->h);
				w->a=1;
				update_sprite_v_size_xy(w->sp0+2,w->w,1+(w->a*3));
				//{int i;printf("top:%i\n",desktop.topwin);for(i=0;i<desktop.pos;i++) { printf("%i\n",desktop.l[i]);}}
				if (desktop.pos>=2) {
					int i;
					int v=desktop.l[desktop.topwin];
					int s=draw_order.order[32+desktop.topwin];
					for (i=desktop.topwin;i<=desktop.pos-2;i++) {
						desktop.l[i]=desktop.l[i+1];
						draw_order.order[32+i]=draw_order.order[32+i+1];
					}
					desktop.l[i]=v;
					draw_order.order[32+i]=s;
					desktop.topwin=i;
				}
				//{int i;printf("top:%i\n",desktop.topwin);for(i=0;i<desktop.pos;i++) { printf("%i\n",desktop.l[i]);}}
				desktop.actwin=desktop.topwin;
				printf("actwin=%i %i\n",desktop.actwin,desktop.l[desktop.actwin]);
				desktop.actwinframe_x=w->x;
				desktop.actwinframe_y=w->y;
				desktop.actwinframe_w=w->w;
				desktop.actwinframe_h=w->h;
				desktop.minwinw=w->minw;
			}
		}
		void update_actwinframe()
		{
			int spr=desktop.spr_actframe;
			int x=desktop.actwinframe_x;
			int y=desktop.actwinframe_y;
			int w=desktop.actwinframe_w;
			int h=desktop.actwinframe_h;
			update_sprite_v_xy(spr+0,x,y);    update_sprite_v_size_xy(spr+0,w,1);
			update_sprite_v_xy(spr+1,x,y+h);  update_sprite_v_size_xy(spr+1,w,1);
			update_sprite_v_xy(spr+2,x,y+30); update_sprite_v_size_xy(spr+2,w,1);
			update_sprite_v_xy(spr+3,x,y);    update_sprite_v_size_xy(spr+3,1,h);
			update_sprite_v_xy(spr+4,x+w-1,y);update_sprite_v_size_xy(spr+4,1,h);
		}
		void update_sprwin()
		{
			int pos=desktop.l[desktop.actwin];
			WINDOW *win=&desktop.wl.w[pos];
			int spr=win->sp0;
			int x=win->x;
			int y=win->y;
			int w=win->w;
			int h=win->h;
			int a=win->a;
			//top
			update_sprite_v_xy(spr+0 ,x    ,y);     update_sprite_v_size_xy(spr+0,w,30);
			//main
			update_sprite_v_xy(spr+1 ,x    ,y+30);  update_sprite_v_size_xy(spr+1,w,h-30);
			//h-line top
			update_sprite_v_xy(spr+2 ,x    ,y);     update_sprite_v_size_xy(spr+2,w,1+(a*3));
			//h-line top title
			update_sprite_v_xy(spr+3 ,x    ,y+30);  update_sprite_v_size_xy(spr+3,w,1);
			//h-line bottom
			update_sprite_v_xy(spr+4 ,x    ,y+h);   update_sprite_v_size_xy(spr+4,w,1);
			//v-line left
			update_sprite_v_xy(spr+5 ,x    ,y);     update_sprite_v_size_xy(spr+5,1,h);
			//v-line right
			update_sprite_v_xy(spr+6 ,x+w-1,y);     update_sprite_v_size_xy(spr+6,1,h);
			//v-scroll
			update_sprite_v_xy(spr+7 ,x+w-5,y+30+1);update_sprite_v_size_xy(spr+7,4*1,(h-30-1)*1);
			update_sprite_v_xy(spr+8 ,x+w-6,y+30);  update_sprite_v_size_xy(spr+8,(1)*1,(h-30-5)*1);
			update_sprite_v_xy(spr+9 ,x+w-4,y+30+8);update_sprite_v_size_xy(spr+9,(2)*1,(32)*1);
			//h-scroll
			update_sprite_v_xy(spr+10,x+1  ,y+h-4); update_sprite_v_size_xy(spr+10,(w-2)*1,(4)*1);
			update_sprite_v_xy(spr+11,x    ,y+h-5); update_sprite_v_size_xy(spr+11,(w-5)*1,(1)*1);
			update_sprite_v_xy(spr+12,x+8  ,y+h-3); update_sprite_v_size_xy(spr+12,(32)*1,(2)*1);
			//symbols
			update_sprite_v_xy(spr+13,x+w-62,y+8);
			update_sprite_v_xy(spr+14,x+w-42,y+8);
			update_sprite_v_xy(spr+15,x+w-22,y+8);
			int len=strlen(win->name);
			int i;
			for (i=0;i<len;i++) {
				update_sprite_v_xy(spr+16+i,x+8+(i*11),y+6);
			}
			printf("update sprwin:%i %i\n",win->sp0,win->sp1);
		}
		int show_actwinframe(int a)
		{
			int ret=0;
			if (desktop.actwinframe!=a) {
				ret=1;
				desktop.actwinframe=a;
				if (a) {
					draw_order.order[240]=240;
					update_actwinframe();
				} else {
					draw_order.order[240]=-1;
					if (desktop.actwin!=-1) {
						int pos=desktop.l[desktop.actwin];
						WINDOW *win=&desktop.wl.w[pos];
						int x=win->x-desktop.actwinframe_x;
						int y=win->y-desktop.actwinframe_y;
						int w=win->w-desktop.actwinframe_w;
						int h=win->h-desktop.actwinframe_h;
						if (x|y|w|h) {
							win->x=desktop.actwinframe_x;
							win->y=desktop.actwinframe_y;
							win->w=desktop.actwinframe_w;
							win->h=desktop.actwinframe_h;
							printf("actwinframe_xywh:%s x:%i y:%i w:%i h:%i\n",win->name,
								desktop.actwinframe_x,desktop.actwinframe_y,
								desktop.actwinframe_w,desktop.actwinframe_h);
						}
					}
				}
			}
			return ret;
		}
		desktop.mouse_1=mousebuf[0];
		desktop.mouse_1_click=0;
		if (desktop.mouse_1) {
			int pos=desktop.l[desktop.topwin];
			WINDOW *w=&desktop.wl.w[pos];
			int x=mousebuf[10]-w->x;
			int y=mousebuf[11]-w->y;
			if (desktop.mouse_1_press==0) {
				if (inside_menu(&desktop)==0) {
					draw=close_menu(&desktop);
				}
				if (desktop.resize_top|desktop.resize_bottom|
						desktop.resize_left|desktop.resize_right) {
							if (desktop.resizewin2!=-1) {
								int pos_r=desktop.l[desktop.resizewin];
								WINDOW *w1=&desktop.wl.w[pos_r];
								pos_r=desktop.l[desktop.resizewin2];
								WINDOW *w2=&desktop.wl.w[pos_r];
								int c1=w1->x;
								int c2=w2->x+w2->w;
								if (desktop.resize_right) {
									c1=w1->x+w1->w;
									c2=w2->x;
								}
								int dist1=abs(mousebuf[10]-c1);
								int dist2=abs(mousebuf[10]-c2);
								if (dist2<dist1) {
									desktop.resizewin=desktop.resizewin2;
									int tmp=desktop.resize_left;
									desktop.resize_left=desktop.resize_right;
									desktop.resize_right=tmp;
								}
								printf("res2:%i %i %i %i %i\n",desktop.resizewin2,dist1,dist2,c1,c2);
							}
							desktop.topwin=desktop.resizewin;
							pos=desktop.l[desktop.topwin];
							w=&desktop.wl.w[pos];
							x=mousebuf[10]-w->x;
							y=mousebuf[11]-w->y;
							printf("activate resizewin:%s\n",w->name);
							activate_window(w);
							desktop.resizemouse_x=mousebuf[10];
							desktop.resizemouse_y=mousebuf[11];
							draw=1;
				} else {
					if ((x>0)&&(x<w->w)&&(y>0)&&(y<w->h)) {
						if (inside_menu(&desktop)==0) {
							activate_window(w);
							desktop.resizemouse_x=mousebuf[10];
							desktop.resizemouse_y=mousebuf[11];
							draw=1;
						}
					} else {
						if (desktop.actwin!=-1) {
							int pos=desktop.l[desktop.actwin];
							WINDOW *w=&desktop.wl.w[pos];
							desktop.actwin=-1;
							w->a=0;
							update_sprite_v_size_xy(w->sp0+2,w->w,1+(w->a*3));
							printf("deactivating:%s %i %i %i %i\n",w->name,w->x,w->y,w->w,w->h);
							draw=1;
						}
					}
				}
			} else {
				if (!(desktop.resize_top|desktop.resize_bottom|
						desktop.resize_left|desktop.resize_right)) {
					if ((x>0)&&(x<w->w)&&(y>8)&&(y<30)) {
						if (!(desktop.inside_min|desktop.inside_max|desktop.inside_close)) {
							if (desktop.dragwin==0) {
								if ((desktop.mouse_x!=mousebuf[10])||(desktop.mouse_y!=mousebuf[11])) {
										printf("dragwin:%s\n",w->name);
										desktop.dragwin=1;
								}
							}
						}
					}
				}
			}
			if (desktop.dragwin) {
				if (show_actwinframe(1)) draw=1;
				int x=mousebuf[10]-desktop.mouse_x;
				int y=mousebuf[11]-desktop.mouse_y;
				desktop.actwinframe_x+=x;
				desktop.actwinframe_y+=y;
				if (x|y) {
					printf("drag:%i %i new:%i %i old:%i %i \n",x,y,
							desktop.actwinframe_x,desktop.actwinframe_y,w->x,w->y);
					update_actwinframe();
				}
			}
			if (desktop.resize_left) {
				int min_w=desktop.minwinw;
				if (show_actwinframe(1)) draw=1;
				int x=mousebuf[10]-desktop.resizemouse_x;
				if (x) {
					desktop.resizemouse_x=mousebuf[10];
					int x1=desktop.actwinframe_x+desktop.actwinframe_w-min_w;
					if ((desktop.resizemouse_x<x1)||(desktop.actwinframe_w!=min_w)) {
						int new_x=desktop.actwinframe_x;
						int new_w=desktop.actwinframe_w;
						new_x+=x;
						new_w-=x;
						if (new_w<min_w) {
							new_w=min_w;
							new_x=desktop.actwinframe_x+desktop.actwinframe_w-min_w;
						}
						desktop.actwinframe_x=new_x;
						desktop.actwinframe_w=new_w;
						printf("r_left:[%s] %i new_x:%i new_w:%i\n",w->name,x,new_x,new_w);
						update_actwinframe();
					}
				}
			}
			if (desktop.resize_right) {
				int min_w=desktop.minwinw;
				if (show_actwinframe(1)) draw=1;
				int x=mousebuf[10]-desktop.resizemouse_x;
				if (x) {
					desktop.resizemouse_x=mousebuf[10];
					int x1=desktop.actwinframe_x+min_w;
					if ((desktop.resizemouse_x>x1)||(desktop.actwinframe_w!=min_w)) {
						desktop.actwinframe_w+=x;
						if (desktop.actwinframe_w<min_w) desktop.actwinframe_w=min_w;
						printf("r_right:[%s] %i new_w:%i\n",w->name,x,desktop.actwinframe_w);
						update_actwinframe();
					}
				}
			}
			if (desktop.resize_top) {
				int min_h=desktop.minwinh;
				if (show_actwinframe(1)) draw=1;
				int y=mousebuf[11]-desktop.resizemouse_y;
				if (y) {
					desktop.resizemouse_y=mousebuf[11];
					int y1=desktop.actwinframe_y+desktop.actwinframe_h-min_h;
					if ((desktop.resizemouse_y<y1)||(desktop.actwinframe_h!=min_h)) {
						int new_y=desktop.actwinframe_y;
						int new_h=desktop.actwinframe_h;
						new_y+=y;
						new_h-=y;
						if (new_h<min_h) {
							new_h=min_h;
							new_y=desktop.actwinframe_y+desktop.actwinframe_h-min_h;
						}
						desktop.actwinframe_y=new_y;
						desktop.actwinframe_h=new_h;
						printf("r_top:[%s] %i new_y:%i new_h:%i\n",w->name,y,new_y,new_h);
						update_actwinframe();
					}
				}
			}
			if (desktop.resize_bottom) {
				int min_h=desktop.minwinh;
				if (show_actwinframe(1)) draw=1;
				int y=mousebuf[11]-desktop.resizemouse_y;
				if (y) {
					desktop.resizemouse_y=mousebuf[11];
					int y1=desktop.actwinframe_y+min_h;
					if ((desktop.resizemouse_y>y1)||(desktop.actwinframe_h!=min_h)) {
						desktop.actwinframe_h+=y;
						if (desktop.actwinframe_h<min_h) desktop.actwinframe_h=min_h;
						printf("r_bottom:[%s] %i new_h:%i\n",w->name,y,desktop.actwinframe_h);
						update_actwinframe();
					}
				}
			}
			desktop.mouse_1_press=1;
		} else {
			if (desktop.mouse_1_press) {
				desktop.mouse_1_press=0;
				desktop.mouse_1_click=1;
				printf("mouse:1 click\n");
				open_menu(&desktop,0);
				desktop.dragwin=0;
				int act=desktop.actwinframe;
				show_actwinframe(0);
				if (act) update_sprwin();
				draw=1;
			}
		}
		if (desktop.mouse_x!=mousebuf[10]) {
			desktop.mouse_x=mousebuf[10];
			draw=1;
		}
		if (desktop.mouse_y!=mousebuf[11]) {
			desktop.mouse_y=mousebuf[11];
			draw=1;
		}
		if (draw) cursor_inside_menu2(&desktop);
		if (draw) cursor_inside_menu(&desktop);
		if (draw) cursor_inside(&desktop);
		if (draw) update_sprite_v_xy(desktop.spr_cursor,
						desktop.mouse_x+desktop.offset_x,desktop.mouse_y+desktop.offset_y);
#ifndef __kms__
		static int p=0;
		if (p!=screen.paint) draw=1;
#endif
		static int cnt=0;cnt++;if (cnt>60) {cnt=0;draw=1;}
		if (draw) {
			create_clock(253);
			create_fps_info(254);
			update_sprite_o_xy_wh_txy_twh_set_rot_pal_filter(253);
			update_sprite_o_xy_wh_txy_twh_set_rot_pal_filter(254);
			do_stuff();
			ogl_draw();
		} else {
			usleep(16666);
		}
#ifndef __kms__
		p=screen.paint;
#endif

	}
end:

#ifdef __kms__
	close_input();
#endif

#ifndef __kms__
	close_display();
#endif

	return 42;
}
