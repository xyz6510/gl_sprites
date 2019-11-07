
void draw_sprites(int start,int end)
{
	VIDEO_VAR *v=&video_var;

	glUseProgram(v->shd0);
	glBindVertexArray(v->va0);
	glDrawArrays(GL_TRIANGLES,start*6,(end-start)*6);
}
