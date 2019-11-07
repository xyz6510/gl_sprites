
void draw_fb_screen()
{
	VIDEO_VAR *v=&video_var;

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(v->shd_screen);
	glUniform1f(v->idx_saturation,v->screen_saturation);
	glUniform1f(v->idx_contrast,v->screen_contrast);
	glUniform1f(v->idx_lightness,v->screen_lightness);
	glUniform1f(v->idx_gamma,v->screen_gamma);
	glScissor(v->scissor[0],v->scissor[1],v->scissor[2],v->scissor[3]);
	glBindVertexArray(v->va1);
	glDrawArrays(GL_TRIANGLES,0,6);
	glDisable(GL_SCISSOR_TEST);
}
