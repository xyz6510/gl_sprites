
void draw_to_layer(LAYER *l,int clear)
{
	VIDEO_VAR *v=&video_var;

	glViewport(l->vpx,l->vpy,l->vpw,l->vph);
	glScissor(l->vpx,l->vpy,l->vpw,l->vph);
	if (clear) {
		glClearColor(l->clear_color[0],l->clear_color[1],l->clear_color[2],l->clear_color[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	glUniformMatrix4fv(v->shd0_projection,1,GL_FALSE,l->pr_ortho2d);
	glDrawArrays(GL_TRIANGLES,l->quad_start*6,(l->quad_end-l->quad_start)*6);
}
