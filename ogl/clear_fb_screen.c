
void clear_fb_screen()
{
	VIDEO_VAR *v=&video_var;

	glBindFramebuffer(GL_FRAMEBUFFER,v->fb_screen);
	glViewport(0,0,v->screen_x,v->screen_y);
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
}
