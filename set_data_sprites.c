
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


void init_data()
{
	{
		void create_chars(int y,int x,int w,int h,int row,int bit,int bs,void *font,int ay,void *adr)
		{
			uint8_t (*ascii8)[h]=(uint8_t(*)[h]) font;
			uint16_t (*ascii16)[h]=(uint16_t(*)[h]) font;
			unsigned int (*buf)[row]=(unsigned int(*)[row]) adr;
			int i;
			for (i=0;i<ay;i++) {
				int j;
				for (j=0;j<h;j++) {
					int k=0;
					for (k=0;k<w;k++) {
						int act=0;
						if (bs==8) act=ascii8[i][j]&(bit>>k);
						if (bs==16)act=ascii16[i][j]&(bit>>k);
						if (act) {
							buf[y+j][x+k]=0xff000000;
						} else {
							buf[y+j][x+k]=0;
						}
					}
				}
				x+=w;
				if (x>=row) {
					x=0;
					y+=h;
				}
			}
		}
		create_chars(0,0,8,8,128,0b10000000,8,font1_x20_x7f,96,opengl_buf);
		{
			char fn[]="spr.data";
			int f=open(fn,O_RDONLY);
			if (f<0) {
				printf("error load file:%s\n",fn);
				exit(-1);
			}
			read(f,&opengl_buf[128*48],64*80*4);
			close(f);
		}
		create_chars(0,0,11,22,176,0b10000000000,16,font4_x20_x7f,96,&opengl_buf[(128*48)+(64*80)]);
	}

	VIDEO_VAR *v=&video_var;
	LAYER_GROUP *lg=&layer_group;
	SPRITE_GROUP *sg=&sprite_group;
	DRAW_ORDER *dr=&draw_order;
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

	//background
	set_sprite(0, 0,0,1280,720, 120,40,1,1, 0x80|0x40|0x20, 0, 0,0, 0, 0,0, -20, 128, 11,  0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);

	//sprite
	set_sprite(1,  50,400,128*5*1,48*5*1, 0,0,128,48, 0x80|0x40, 0, 0,0, 0, 0,0, 0, 0, 15, 0,0,0,0, 0,0, 0,0, 255,128,1,0, 0,0,0,0,0,0,0,0);//grey

	set_sprite(2,  100,100,8*40,8*40, 8,16,8,8, 0x80, 0, 0,0, 0, 0,0, 0, 0, 1, 0,0,0,0, 0,0, 0,0, 160,217,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(3,  300,100,8*40,8*40, 16,16,8,8, 0x80|0x20, 0, 0,0, 0, 0,0, 0, 128, 2, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(4,  600,100,8*40,8*40, 16,24,8,8, 0x80|0x08, 0, 0,0, 0, 0,0, 0, 0, 3, 0,0,0, 0,0,0, 0,0, 160,217,1,0, 0,0,0,0,0,0,0,0);

	set_sprite(6,  965,400,128*1,48*1, 0,0,128,48, 0x200|0x80, 0, 0,0, 0, 0,0, 0, 0, 7, 0x00,0x00,0x00,0xff, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);//yellow

	set_sprite(5,  800,200,8*40,8*40, 8,0,8,8, 0x80|0x01|0x02|0x04, 8192, 16384,28671, 0, 600,360, 0, 0, 4, 0,0,0, 0,0,0, 0,0, 96,255,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(7,  600-4,360-4,8,8, 120,40,8,8, 0x80, 0, 0,0, 0, 0,0, 0, 0, 7, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(8,  800,200,2,2, 		 120,40,8,8, 0x80|0x01|0x02|0x04, 0, 0,0, 0, 600,360, 0, 0, 8, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(9,  800-160,200-160,8,8,  120,40,8,8, 0x80|0x01|0x02|0x04, 0, 0,0, 0, 600,360, 0, 0, 1, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(10, 800+160,200-160,8,8, 120,40,8,8, 0x80|0x01|0x02|0x04, 0, 0,0, 0, 600,360, 0, 0, 2, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(11, 800+160,200+160,8,8, 120,40,8,8, 0x80|0x01|0x02|0x04, 0, 0,0, 0, 600,360, 0, 0, 5, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(12, 800-160,200+160,8,8, 120,40,8,8, 0x80|0x01|0x02|0x04, 0, 0,0, 0, 600,360, 0, 0, 14, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);

	set_sprite(13, 20,680,1240,1, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 7, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(14, 20,682,1240,1, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 8, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(15, 20,684,1240,1, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 9, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(16, 20,686,1240,2, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 2, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);

	set_sprite(17, 1268,20,1,680, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 1, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(18, 1270,20,1,680, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 2, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(19, 1272,20,1,680, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 3, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(20, 1274,20,1,680, 120,40,1,1, 0x80, 0, 0,0, 0, 0,0, 0, 0, 4, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);

	set_sprite(21, 750,470,480,128, 0,8,128,8, 0x800|0x80, 0, 0,0, 0, 0,0, 0, 0, 13, 0,0,0,0, 2,2, 0,0, 200,0,0,0, 0,0,0,0,0,0,0,0);//green

	set_sprite(22, 750,470,128,128, 0,0,128,48, 0x800|0x400|0x80, 0, 0,0, 0, 0,0, 0, 0, 14, 0,0,0,0, 2,2, 0,0, 160,217,1,0, 1100,50,1150,50,1250,350,950,350);//blue

	set_sprite(23, 750,470,128,128, 120,40,1,1, 0x400|0x80, 0, 0,0, 0, 0,0, 0, 0, 15, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 500,100,600,200,600,201,500,101);

	set_sprite(24, 750,470,128,128, 120,40,1,1, 0x400|0x80, 0, 0,0, 0, 0,0, 0, 0, 13, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 500,100,1200,0,1200,1,500,101);

	set_sprite(25, 750,470,128,48, 0,0,128,48, 0x800|0x400|0x80, 0, 0,0, 0, 0,0, 0, 0, 2, 0,0,0,0, 1,1, 0,0, 225,0,0,0, 1100,400,1228,400,1228,464,1100,448);//red

	set_sprite(1000, 0,0,128*3,48*3, 0,0,128,48, 0x80, 0, 0,0, 0, 0,0, 0, 0, 1, 0,0,0,0, 2,2, 0,0, 0,0,0,0, 10,10,400,10,400,400,10,400);

	set_layer(&lg->l[0], 0,  0,0,500,500, 100,400,500,500, NULL, (short int[2]){0,0},(short int[2]){0,0},
				(short int[4][2]){{10,10},{500,50},{500,600},{10,400}},
				0,(short int[2]){0,0}, 0,(short int[2]){0,0}, (short int[3]){255,255,255},
				(short int[4]){0,0,0,0}, 0, 0,(short int[2]){0,0},(short int[2]){0,0},
				0x800|0x400|0x100, 1000,1001, (float[4]){0,0,0,1}, 0, 0, 0, 1, 1);

	set_layer(&lg->l[1], 1,  500,0,1280,720, 800,460,1280/3,720/3, NULL, (short int[2]){0,0},(short int[2]){0,0},
				(short int[4][2]){{560,320},{1200,320},{1200,640},{560,640}},
				0,(short int[2]){0,0}, 0,(short int[2]){0,0}, (short int[3]){255,255,255},
				(short int[4]){165,0,0,0}, 0, 0,(short int[2]){0,0},(short int[2]){0,0},
				0x0, 1,26, (float[4]){0,0,0,0}, -0.75, 0, -0.25, 1, 1);

	set_layer(&lg->l[2], 2,  500,0,1280,720, 640,360,1280,720, NULL, (short int[2]){0,0},(short int[2]){0,0},
				(short int[4][2]){{640,360},{1920,360},{1920,1080},{640,900}},
				0,(short int[2]){0,0}, 0,(short int[2]){0,0}, (short int[3]){255,255,255},
				(short int[4]){255,0,0,0}, 0, 0,(short int[2]){0,0},(short int[2]){0,0},
				0x800|0x400, 1,26, (float[4]){0,0,0,1}, 0, 0, 0, 1, 1);

	set_layer(&lg->l[3], 3,  500,0,1280,720, 640,360,1280,720, NULL, (short int[2]){0,0},(short int[2]){0,0},
				(short int[4][2]){{-640,360},{640,360},{640,900},{-640,1080}},
				0,(short int[2]){0,0}, 0,(short int[2]){0,0}, (short int[3]){255,255,255},
				(short int[4]){160,217,0,0}, 0, 0,(short int[2]){0,0},(short int[2]){0,0},
				0x800|0x400, 1,26, (float[4]){0,0,0,1}, 0, 0, 0, 1, 1);

	set_layer(&lg->l[4], 4,  1780,0,500,600, 500,150,40*5,40*5, (short int[4]){1780+50+40*8,400+40*3,40,40},(short int[2]){0,0},(short int[2]){0,0},
				(short int[4][2]){{0,0},{0,0},{0,0},{0,0}},
				0,(short int[2]){0,0}, 0,(short int[2]){640,360}, (short int[3]){255,255,255},
				(short int[4]){255,128,1,0}, 14, 0,(short int[2]){0,0},(short int[2]){0,0},
				0x4000|0x2000|0x1000|0x80|0x01|0x04, 1,2, (float[4]){0,0,0,0}, 0, 0, 0, 1, 1);

	set_layer(&lg->l[5], 5,  1780,0,500,600, 700,150,40*5,40*5, (short int[4]){1780+50+40*8,400+40*3,40,40}, (short int[2]){0,0},(short int[2]){0,0},
				(short int[4][2]){{0,0},{0,0},{0,0},{0,0}},
				0,(short int[2]){0,0}, 0,(short int[2]){0,0}, (short int[3]){255,255,255},
				(short int[4]){255,128,1,0}, 13, 1,(short int[2]){500+750,470},(short int[2]){500+850,470},
				0x1000|0x80, 1,2, (float[4]){0,0,0,0}, 0, 0, 0, 1, 1);

	int m=5;int n=10,ny=8;int xs=80,ys=0,f1=255,f2=128,f3=1,f4=1,tpos=1280;
	set_sprite(3000, tpos+n*16*0,0,16*n,16*n, 0,48+0,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3001, tpos+n*16*1,0,16*n,16*n, 0,48+0,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 8, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3002, tpos+n*16*2,0,16*n,16*n, 0,48+0,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 9, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3003, tpos+n*16*3,0,16*n,16*n, 0,48+0,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 10, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3006, tpos+n*16*0,1*16*n,16*n,16*n, 0,48+16,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3007, tpos+n*16*1,1*16*n,16*n,16*n, 0,48+16,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 8, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3008, tpos+n*16*2,1*16*n,16*n,16*n, 0,48+16,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 9, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3009, tpos+n*16*3,1*16*n,16*n,16*n, 0,48+16,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 10, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3010, tpos+n*16*0,2*16*n,16*n,16*n, 0,48+16,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3011, tpos+n*16*1,2*16*n,16*n,16*n, 0,48+16,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 8, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3012, tpos+n*16*2,2*16*n,16*n,16*n, 0,48+16,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 9, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3013, tpos+n*16*3,2*16*n,16*n,16*n, 0,48+16,16,16, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 10, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3014, tpos+n*8*0,(3*16*n),8*n,8*n, 0,48+32,8,8, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3015, tpos+n*8*1,(3*16*n),8*n,8*n, 0,48+32,8,8, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 8, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3016, tpos+n*8*2,(3*16*n),8*n,8*n, 0,48+32,8,8, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 9, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3017, tpos+n*8*3,(3*16*n),8*n,8*n, 0+8,48+32,8,8, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 10, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3018, tpos+n*8*0,(3*16*n)+(1*8*n),8*n,8*ny, 0,48+32+8,8,8, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3019, tpos+n*8*1,(3*16*n)+(1*8*n),8*n,8*ny, 0+8,48+32+8,8,8, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 8, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3020, tpos+n*8*2,(3*16*n)+(1*8*n),8*n,8*ny, 0,48+32+8,8,8, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 9, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);
	set_sprite(3021, tpos+n*8*3,(3*16*n)+(1*8*n),8*n,8*ny, 0+8,48+32+8,8,8, 0x4000, 0, 0,0, 0, 0,0, 0, 0, 10, 0,0,0,0, 0,0, 0,0, f1,f2,f3,f4, 0,0,0,0,0,0,0,0);

	set_sprite(3004, xs,ys,64*m,80*m, 0,48,64,80, 0x000, 0, 0,0, 0, 0,0, 0, 0, 0, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(3005, xs,ys+240+160,64*m,80*m, 0,48,64,80, 0x000|0x40, 0, 0,0, 0, 0,0, -20, 0, 1, 0,0,0,0, 0,0, 0,0, 255,128,1,1, 0,0,0,0,0,0,0,0);

	set_sprite(2000, 0,0,1280,720, 0,0,128,48, 0x0, 0, 0,0, 0, 0,0, 0, 0, 2, 0,0,0,0, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);

	set_sprite(4000, 0,0,40*5,40*5, 8,16,8,8, 0x80, 0, 0,0, 0, 0,0, 0, 0, 14, 0,0,0,0, 0,0, 0,0, 255,128,1,0, 0,0,0,0,0,0,0,0);

	set_sprite(5000, 700,0,176,132, 0,48+81,176,132, 0x80, 0, 0,0, 0, 0,0, 0, 0, 7, 0,0,0,0xff, 0,0, 0,0, 0,0,0,0, 0,0,0,0,0,0,0,0);
	set_sprite(5001, 700,380,176*1.5,132*1.5, 0,48+81,176,132, 0x80, 0, 0,0, 0, 0,0, 0, 0, 1, 0,0,0,0xff, 0,0, 0,0, 255,32,1,0, 0,0,0,0,0,0,0,0);
	set_sprite(5002, 270,65,128,128, 112,0,8,8, 0x4000|0x80|0x20|0x01, 8192, 0,0, 0, 0,0, 0, 225, 1, 0,0,0,0xff, 0,0, 0,0, 255,0,0,0, 0,0,0,0,0,0,0,0);

	sg->active[0]=0b111011;
	sg->start[0]=0;
	sg->end[0]=1;
	sg->start[1]=1;
	sg->end[1]=26;

	sg->start[2]=2000;
	sg->end[2]=2001;

	sg->start[3]=3000;
	sg->end[3]=3022;

	sg->start[4]=4000;
	sg->end[4]=4001;

	sg->start[5]=5000;
	sg->end[5]=5003;

	dr->order[0]=256+2;
	dr->order[1]=256+3;
	dr->order[2]=0;
	dr->order[3]=256;
	dr->order[4]=4;
	dr->order[5]=1;
	dr->order[6]=256+1;
	dr->order[7]=3;
	dr->order[8]=256+4;
	dr->order[9]=256+5;
	dr->order[10]=5;

	dr->start=0;
	dr->end=11;

	lg->active[0]=0b010011;
	lg->once[0]=  0b000000;
	lg->clear[0]= 0b110001;
	lg->draw[0]=  0b111111;

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
	update_texture(v->act_sprites,v->sprites,  0,48,64,80,64,GL_RGBA, GL_UNSIGNED_BYTE,&opengl_buf[128*48]);
	update_texture(v->act_sprites,v->sprites,  0,48+81,176,132,176,GL_RGBA, GL_UNSIGNED_BYTE,&opengl_buf[(128*48)+(64*80)]);

	update_texture(v->act_rg16i,v->gl_rg16i, 0,0,256,256,0,      GL_RG_INTEGER,GL_SHORT,tx_data_order);
	update_texture(v->act_rg16i,v->gl_rg16i, 256,0,256,256,256,    GL_RG_INTEGER,GL_SHORT,tx_data_v_xy);
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

void update_sprite_pos(int idx,short int px,short int py)
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

void update_sprite_rot(int idx,short int rot)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	short int (*buf)[256]=(short int(*)[256]) tx_data_rot;
	buf[y][x]=rot;

	update_texture(v->act_r16i,v->gl_r16i,256+x,y,1,1,256,GL_RED_INTEGER,GL_SHORT,&buf[y][x]);
}

void update_sprite_orbit_rot(int idx,short int rot)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	short int (*buf)[256]=(short int(*)[256]) tx_data_orbit_rot;
	buf[y][x]=rot;

	update_texture(v->act_r16i,v->gl_r16i,512+x,y,1,1,256,GL_RED_INTEGER,GL_SHORT,&buf[y][x]);
}

void update_sprite_pal(int idx,int pal)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	short int (*buf)[256]=(short int(*)[256]) tx_data_pal_idx;
	buf[y][x]=pal;

	update_texture(v->act_r16i,v->gl_r16i,256+x,256+y,1,1,256,GL_RED_INTEGER,GL_SHORT,&buf[y][x]);
}

void update_sprite_scale(int idx,short int sx,short int sy)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_repeat_scale;
	short int *p=(short int *)&buf[y][x];
	p[0]=sx;
	p[1]=sy;

	update_texture(v->act_rg16i,v->gl_rg16i,768+x,256+y,1,1,256,GL_RG_INTEGER,GL_SHORT,p);
}

void update_sprite_offset(int idx,short int ox,short int oy)
{
	VIDEO_VAR *v=&video_var;

	int x=idx&0xff;
	int y=idx>>8;

	unsigned int (*buf)[256]=(unsigned int(*)[256]) tx_data_repeat_offset;
	short int *p=(short int *)&buf[y][x];
	p[0]=ox;
	p[1]=oy;

	update_texture(v->act_rg16i,v->gl_rg16i,x,512+y,1,1,256,GL_RG_INTEGER,GL_SHORT,p);
}

void update_layer_offset(int idx,short int ox,short int oy)
{
	VIDEO_VAR *v=&video_var;

	short int (*data)[256]=(short int(*)[256])tx_data_layer;
	data[idx][18]=ox;data[idx][19]=oy;
	update_texture(v->act_r16i,v->gl_r16i,512,256+idx,256,1,256,GL_RED_INTEGER,GL_SHORT,&data[idx][0]);
}

void update_layer_rot(int idx,short int rot)
{
	VIDEO_VAR *v=&video_var;

	short int (*data)[256]=(short int(*)[256])tx_data_layer;
	data[idx][20]=rot;
	update_texture(v->act_r16i,v->gl_r16i,512,256+idx,256,1,256,GL_RED_INTEGER,GL_SHORT,&data[idx][0]);
}

void update_layer_orbit(int idx,short int orot)
{
	VIDEO_VAR *v=&video_var;

	short int (*data)[256]=(short int(*)[256])tx_data_layer;
	data[idx][23]=orot;
	update_texture(v->act_r16i,v->gl_r16i,512,256+idx,256,1,256,GL_RED_INTEGER,GL_SHORT,&data[idx][0]);
}
