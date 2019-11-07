
void init_gl(int screen_x,int screen_y)
{
	VIDEO_VAR *v=&video_var;

	struct timeval time;
	gettimeofday(&time,NULL);
	v->time=(time.tv_sec*1000000)+time.tv_usec;
	v->dtime=1;

	v->screen_x=screen_x;
	v->screen_y=screen_y;

	v->act_sprites=0;
	v->act_rg16i=1;
	v->act_r16i=2;
	v->act_rgba8=3;
	v->act_screen=4;
	v->act_gpu=5;

	//GL RG16I 13 ,GL R16I 7 ,GL RGBA8 3
	//GL RG16I 13  1024x1024 4x4
	//order         0 0
	//v_xy          1 0
	//v_size_xy     2 0
	//t_xy          3 0
	//t_size_xy     0 1
	//rot_center    1 1
	//orbit_center  2 1
	//repeat_scale  3 1
	//repeat_offset 0 2
	//polygon0      1 2 xp0yp0
	//polygon1      2 2 xp1yp1
	//polygon2      3 2 xp2yp2
	//polygon3      0 3 xp3yp3

	//GL R16I 7     1024x512 4x2
	//settings      0 0
	//rot           1 0
	//orbit_rot     2 0
	//light         3 0
	//alpha         0 1
	//pal_idx       1 1
	//layer         2 1

	//GL RGBA8 3    1024x256 4x1
	//pallete       0 0
	//color_key     1 0
	//filter        2 0

	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glGenTextures(1,&v->sprites);
	glGenTextures(1,&v->gl_rg16i);
	glGenTextures(1,&v->gl_r16i);
	glGenTextures(1,&v->gl_rgba8);
    glGenTextures(1,&v->screen_tex);
    glGenTextures(1,&v->gpu_tex);

	glGenFramebuffers(1,&v->fb_screen);
	glGenFramebuffers(1,&v->fb_gpu_mem);

	int ws=sprite_tex_width;
	int wg=gpu_ram_width;
	init_texture(v->act_sprites,ws,ws,v->sprites,GL_RGBA8,GL_RGBA,GL_UNSIGNED_BYTE,1,1);
	init_texture(v->act_rg16i, 1024,1024,v->gl_rg16i,GL_RG16I,GL_RG_INTEGER, GL_SHORT,1,0);
	init_texture(v->act_r16i,  1024,512,v->gl_r16i,GL_R16I, GL_RED_INTEGER,GL_SHORT,1,0);
	init_texture(v->act_rgba8, 1024,256,v->gl_rgba8,GL_RGBA8,GL_RGBA,GL_UNSIGNED_BYTE,1,0);
	init_texture(v->act_screen,v->screen_x,v->screen_y,v->screen_tex,GL_RGBA8,GL_RGBA,GL_UNSIGNED_BYTE,0,1);
	init_texture(v->act_gpu,   wg,wg,v->gpu_tex,GL_RGBA8,GL_RGBA,GL_UNSIGNED_BYTE,1,1);

	glBindFramebuffer(GL_FRAMEBUFFER,v->fb_gpu_mem);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,v->gpu_tex,0);

	glBindFramebuffer(GL_FRAMEBUFFER,v->fb_screen);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,v->screen_tex,0);

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	ortho2d(0,0,v->screen_x,v->screen_y,v->projection);

	//-- v->va0 --
	glGenVertexArrays(1,&v->va0);
	glBindVertexArray(v->va0);

	int size_vbo=256*256*(6*4)*4;
	float *data_vbo=malloc(size_vbo);
	memset(data_vbo,0,size_vbo);
	int m=0,i,j;
	for (i=0;i<256;i++) {
		for (j=0;j<256;j++) {
			data_vbo[m+0] =0;data_vbo[m+1] =0;data_vbo[m+2] =j;data_vbo[m+3] =i;
			data_vbo[m+4] =1;data_vbo[m+5] =0;data_vbo[m+6] =j;data_vbo[m+7] =i;
			data_vbo[m+8] =1;data_vbo[m+9] =1;data_vbo[m+10]=j;data_vbo[m+11]=i;
			data_vbo[m+12]=1;data_vbo[m+13]=1;data_vbo[m+14]=j;data_vbo[m+15]=i;
			data_vbo[m+16]=0;data_vbo[m+17]=1;data_vbo[m+18]=j;data_vbo[m+19]=i;
			data_vbo[m+20]=0;data_vbo[m+21]=0;data_vbo[m+22]=j;data_vbo[m+23]=i;
			m+=24;
		}
	}
	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,size_vbo,data_vbo,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,NULL);
	free(data_vbo);
	//------------

	//-- v->va1 --
	glGenVertexArrays(1,&v->va1);
	glBindVertexArray(v->va1);
	size_vbo=1*1*(6*2)*4;
	data_vbo=malloc(size_vbo);
	memset(data_vbo,0,size_vbo);
	data_vbo[0] =0;data_vbo[1] =0;
	data_vbo[2] =1;data_vbo[3] =0;
	data_vbo[4] =1;data_vbo[5] =1;
	data_vbo[6] =1;data_vbo[7] =1;
	data_vbo[8] =0;data_vbo[9] =1;
	data_vbo[10]=0;data_vbo[11]=0;

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,size_vbo,data_vbo,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL);
	free(data_vbo);
	//------------------

	//-- v->shd0 --
	v->shd0=glCreateProgram();
	GLuint vs0=compile_shader("ogl/shader/vs0.vert",GL_VERTEX_SHADER);
	GLuint fs0=compile_shader("ogl/shader/fs0.frag",GL_FRAGMENT_SHADER);
	glAttachShader(v->shd0,vs0);
	glAttachShader(v->shd0,fs0);
    glLinkProgram(v->shd0);
    print_link_log("shd0",v->shd0);

	glUseProgram(v->shd0);

	v->shd0_projection=glGetUniformLocation(v->shd0,"projection");glUniformMatrix4fv(v->shd0_projection,1,GL_FALSE,v->projection);
	i=glGetUniformLocation(v->shd0,"tex_sprites");glUniform1i(i,v->act_sprites);
	i=glGetUniformLocation(v->shd0,"tex_rg16i");  glUniform1i(i,v->act_rg16i);
	i=glGetUniformLocation(v->shd0,"tex_r16i");   glUniform1i(i,v->act_r16i);
	i=glGetUniformLocation(v->shd0,"tex_rgba8");  glUniform1i(i,v->act_rgba8);
	//-----------

	//-- v->shd_screen --
	v->shd_screen=glCreateProgram();
	GLuint vs=compile_shader("ogl/shader/vs_screen.vert",GL_VERTEX_SHADER);
	GLuint fs=compile_shader("ogl/shader/fs_screen.frag",GL_FRAGMENT_SHADER);
	glAttachShader(v->shd_screen,vs);
	glAttachShader(v->shd_screen,fs);
    glLinkProgram(v->shd_screen);
    print_link_log("shd_screen",v->shd_screen);

	glUseProgram(v->shd_screen);

	v->idx_projection=glGetUniformLocation(v->shd_screen,"projection");glUniformMatrix4fv(v->idx_projection,1,GL_FALSE,v->projection);
	i=glGetUniformLocation(                v->shd_screen,"tex_screen");glUniform1i(i,v->act_screen);
	v->idx_screen=glGetUniformLocation(    v->shd_screen,"screen");    glUniform2f(v->idx_screen,v->screen_x,v->screen_y);
	v->idx_saturation=glGetUniformLocation(v->shd_screen,"saturation");
	v->idx_contrast=glGetUniformLocation(  v->shd_screen,"contrast");
	v->idx_lightness=glGetUniformLocation( v->shd_screen,"lightness");
	v->idx_gamma=glGetUniformLocation(     v->shd_screen,"gamma");
	//-----------------

	//-- v->shd_layer --
	v->shd_layer=glCreateProgram();
	vs=compile_shader("ogl/shader/vs_layer.vert",GL_VERTEX_SHADER);
	fs=compile_shader("ogl/shader/fs_layer.frag",GL_FRAGMENT_SHADER);
	glAttachShader(v->shd_layer,vs);
	glAttachShader(v->shd_layer,fs);
    glLinkProgram(v->shd_layer);
    print_link_log("shd_layer",v->shd_layer);

	glUseProgram(v->shd_layer);

	v->l_layer_idx=glGetUniformLocation( v->shd_layer,"layer_idx");
	v->l_projection=glGetUniformLocation(v->shd_layer,"projection");glUniformMatrix4fv(v->l_projection,1,GL_FALSE,v->projection);
	i=glGetUniformLocation(              v->shd_layer,"tex_gpu");   glUniform1i(i,v->act_gpu);
	i=glGetUniformLocation(              v->shd_layer,"tex_rgba8"); glUniform1i(i,v->act_rgba8);
	i=glGetUniformLocation(              v->shd_layer,"tex_r16i");  glUniform1i(i,v->act_r16i);
	v->l_saturation=glGetUniformLocation(v->shd_layer,"saturation");
	v->l_contrast=glGetUniformLocation(  v->shd_layer,"contrast");
	v->l_lightness=glGetUniformLocation( v->shd_layer,"lightness");
	v->l_gamma=glGetUniformLocation(     v->shd_layer,"gamma");
	v->l_alpha=glGetUniformLocation(     v->shd_layer,"alpha");
	//----------------

	glUseProgram(0);

	v->screen_saturation=0; //-1,1
	v->screen_contrast=0;   //-1,1
	v->screen_lightness=0;  //-1,1
	v->screen_gamma=1.0;    //0,10

	v->scissor[0]=0;//left
	v->scissor[1]=0;//bottom
	v->scissor[2]=v->screen_x;
	v->scissor[3]=v->screen_y;

	glViewport(0,0,v->screen_x,v->screen_y);

	glFinish();

	if (compare_file_size(ogl_data,ogl_data_size)) {
		if (create_file(ogl_data,ogl_data_size,0666)) exit(-1);
	}
	int file=open(ogl_data,O_RDWR);
	if (map_file(file,(void**)&opengl_data,ogl_data_size,1)) exit(-1);
	close(file);

	if (compare_file_size(ogl_buf,ogl_buf_size)) {
		if (create_file(ogl_buf,ogl_buf_size,0666)) exit(-1);
	}
	file=open(ogl_buf,O_RDWR);
	if (map_file(file,(void**)&opengl_buf,ogl_buf_size,1)) exit(-1);
	close(file);

	//GL RG16I 13
	tx_data_order=&opengl_data[0];
	tx_data_v_xy=&opengl_data[256*256*1];
	tx_data_v_size_xy=&opengl_data[256*256*2];
	tx_data_t_xy=&opengl_data[256*256*3];
	tx_data_t_size_xy=&opengl_data[256*256*4];
	tx_data_rot_center=&opengl_data[256*256*5];
	tx_data_orbit_center=&opengl_data[256*256*6];
	tx_data_repeat_scale=&opengl_data[256*256*7];
	tx_data_repeat_offset=&opengl_data[256*256*8];
	tx_data_polygon0=&opengl_data[256*256*9];
	tx_data_polygon1=&opengl_data[256*256*10];
	tx_data_polygon2=&opengl_data[256*256*11];
	tx_data_polygon3=&opengl_data[256*256*12];

	//GL R16I 7
	tx_data_settings=&opengl_data[256*256*13];
	tx_data_rot=&opengl_data[256*256*14];
	tx_data_orbit_rot=&opengl_data[256*256*15];
	tx_data_light=&opengl_data[256*256*16];
	tx_data_alpha=&opengl_data[256*256*17];
	tx_data_pal_idx=&opengl_data[256*256*18];
	tx_data_layer=&opengl_data[256*256*19];

	//GL RGBA8 3
	tx_data_pallete=&opengl_data[256*256*20];
	tx_data_color_key=&opengl_data[256*256*21];
	tx_data_filter=&opengl_data[256*256*22];

	memset(&layer_group,0,sizeof(LAYER_GROUP));
	memset(&sprite_group,0,sizeof(SPRITE_GROUP));
	memset(&draw_order,0,sizeof(DRAW_ORDER));

}
