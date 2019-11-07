
#include "font1_x20_x7f.c"
#include "font4_x20_x7f.c"

#ifndef __kms__
	#include "x11/x11.c"
#else
	#include "drm/drm.c"
#endif

#include "ogl/ogl.c"

#include "set_data_sprites.c"

void do_stuff()
{
	static short int rot=0;
	static short int orot=0;
	static int rot_add=1024;
	update_sprite_rot(5,rot);
	update_sprite_orbit_rot(5,orot);
	update_sprite_orbit_rot(8,orot);
	update_sprite_orbit_rot(9,orot);
	update_sprite_orbit_rot(10,orot);
	update_sprite_orbit_rot(11,orot);
	update_sprite_orbit_rot(12,orot);
	orot+=32;
	rot+=rot_add;
	if (rot>8192) rot_add=-rot_add;
	if (rot<-8192) rot_add=-rot_add;
	static int ox=-2100;//overflow -2185
	static int cnt_o=0;
	update_sprite_offset(21,ox,ox);
	static short int ox1=0;
	update_sprite_offset(22,ox1,0);
	update_sprite_offset(25,-ox1,-ox1);
	update_sprite_offset(26,-ox1*1,-ox1*1);
	update_layer_offset(0,-ox1*1,-ox1*1);
	update_layer_offset(2,ox*15,0);//overflow
	update_layer_offset(3,ox*15,0);
	update_layer_rot(4,orot);
	update_layer_orbit(4,-orot);
	cnt_o++;
	if (cnt_o>1) {
		cnt_o=0;
		ox--;
		ox1-=2;
	}
}

int main(int argc, char *argv[])
{

#ifndef __kms__
	open_display("sprites");
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
			}
		} else key_f=0;
#else
		if (keybuf[1]) {
			printf(":key ESC\n");
			goto end;
		}
#endif

		do_stuff();
		ogl_draw();
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
