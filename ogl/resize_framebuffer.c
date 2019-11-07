
void resize_framebuffer(int screen_x,int screen_y)
{
	VIDEO_VAR *v=&video_var;

	glFinish();

	v->screen_x=screen_x;
	v->screen_y=screen_y;

	glBindFramebuffer(GL_FRAMEBUFFER,v->fb_screen);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,0,0);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glDeleteTextures(1,&v->screen_tex);

	glGenTextures(1,&v->screen_tex);
	init_texture(v->act_screen,v->screen_x,v->screen_y,v->screen_tex,GL_RGBA8,GL_RGBA,GL_UNSIGNED_BYTE,0,1);

	glBindFramebuffer(GL_FRAMEBUFFER,v->fb_screen);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,v->screen_tex,0);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	ortho2d(0,0,v->screen_x,v->screen_y,v->projection);

	glUseProgram(v->shd_layer);
	glUniformMatrix4fv(v->l_projection,1,GL_FALSE,v->projection);

	glUseProgram(v->shd_screen);
	glUniformMatrix4fv(v->idx_projection,1,GL_FALSE,v->projection);
	glUniform2f(v->idx_screen,v->screen_x,v->screen_y);

	glUseProgram(0);

	glViewport(0,0,v->screen_x,v->screen_y);

	v->scissor[0]=0;
	v->scissor[1]=0;
	v->scissor[2]=v->screen_x;
	v->scissor[3]=v->screen_y;
}
