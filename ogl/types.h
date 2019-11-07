#include "../files/files.h"

#include <math.h>
#include <sys/time.h>

#ifndef __kms__
	#define GL_GLEXT_PROTOTYPES
	#define GLX_GLXEXT_PROTOTYPES

	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <GL/glxext.h>
#endif

#define ogl_buf __dirpath__ "/ogl_buf"
#define ogl_buf_size 1024*1024*4

#define ogl_data __dirpath__ "/ogl_data"
#define ogl_data_size 256*256*23*4

#define sprite_tex_width 512

#define gpu_ram_width 4096

typedef struct {
	double time;
	double dtime;
	double rtime;
	double wtime;
	int usleep;

	int screen_x;
	int screen_y;

	int act_sprites;
	int act_rg16i;
	int act_r16i;
	int act_rgba8;
	int act_screen;
	int act_gpu;

	GLfloat projection[16];

	float screen_saturation;//-1,1
	float screen_contrast;//-1,1
	float screen_lightness;//-1,1
	float screen_gamma;//0,10

	int scissor[4];

	GLuint sprites;
	GLuint gl_rg16i;
	GLuint gl_r16i;
	GLuint gl_rgba8;
	GLuint gpu_tex;
	GLuint screen_tex;

	GLuint fb_screen;
	GLuint fb_gpu_mem;

	//settings
	//r 16bit bits 
	//  rot           0   0x0001
	//  rot_center    1   0x0002
	//  orbit         2   0x0004
	//  mirrox_x      3   0x0008
	//  mirrox_y      4   0x0010
	//  alpha         5   0x0020
	//  light         6   0x0040
	//  pallete       7   0x0080
	//  color_key     8   0x0100
	//  color_key_inv 9   0x0200
	//  polygon       10  0x0400
	//  repeat        11  0x0800
	//  zero_color    12  0x1000 color.rgb=0
	//  draw_hole     13  0x2000 if (color.a>0) color.a=0 else color.a=1

	GLuint shd0;
	GLuint va0;
	GLuint shd0_projection;

	GLuint shd_screen;
	GLuint shd_layer;
	GLuint va1;

	int idx_projection;
	int idx_screen;
	int idx_saturation;
	int idx_contrast;
	int idx_lightness;
	int idx_gamma;

	int l_layer_idx;
	int l_projection;
	int l_saturation;
	int l_contrast;
	int l_lightness;
	int l_gamma;
	int l_alpha;
} VIDEO_VAR;

typedef struct {
	int layer_idx;
	//projection
	float pr_ortho2d[16];
	//location in gpu mem texture viewport
	unsigned int vpx,vpy,vpw,vph;

	//gl r16i layer 2 1
	//x,y,w,h
	//tx,ty,tw,th
	//pxy[4][2];
	//repeat_scale[2];
	//repeat_offset[2];
	//rot_angle
	//rot_center_xy[2]
	//orbit_angle
	//orbit_center_xy[2]
	//color_key[3]
	//filter_amount[4];
	//pallete_idx
	//settings
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

	//quads
	int quad_start;
	int quad_end;

	float clear_color[4];
	//post color process
	float saturation;//-1,1
	float contrast;  //-1,1
	float lightness; //-1,1
	float gamma;     //0,10
	float alpha;     //0,1
} LAYER;

typedef struct {
	unsigned int active[8];//1 bit one layer
	unsigned int once[8];//1 bit one layer
	unsigned int clear[8];//1 bit one layer
	unsigned int draw[8];//1 bit one layer
	LAYER l[256];
} LAYER_GROUP;

typedef struct {
	unsigned int active[8];//1 bit one group
	int start[256];
	int end[256];
} SPRITE_GROUP;

typedef struct {
	int order[256];
	int start;
	int end;
} DRAW_ORDER;

int *tx_data_order=NULL;
int *tx_data_v_xy=NULL;
int *tx_data_v_size_xy=NULL;
int *tx_data_t_xy=NULL;
int *tx_data_t_size_xy=NULL;
int *tx_data_settings=NULL;
int *tx_data_rot;
int	*tx_data_rot_center;
int *tx_data_orbit_rot;
int	*tx_data_orbit_center;
int	*tx_data_light;
int	*tx_data_alpha;
int	*tx_data_pal_idx;
int *tx_data_pallete=NULL;
int *tx_data_color_key=NULL;
int *tx_data_repeat_scale=NULL;
int *tx_data_repeat_offset=NULL;
int *tx_data_filter=NULL;
int *tx_data_polygon0=NULL;
int *tx_data_polygon1=NULL;
int *tx_data_polygon2=NULL;
int *tx_data_polygon3=NULL;
int *tx_data_layer=NULL;

VIDEO_VAR video_var;

LAYER_GROUP layer_group;

SPRITE_GROUP sprite_group;

DRAW_ORDER  draw_order; //value 0-255:sprite_group quad_start[],end[],
                        //value (256-511)-256 layer_group layer[]
                        //value <0 skip
int *opengl_data=NULL;
int *opengl_buf=NULL;
