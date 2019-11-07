
void draw_layer(LAYER *l)
{
	VIDEO_VAR *v=&video_var;

	glUseProgram(v->shd_layer);
	glUniform1i(v->l_layer_idx,l->layer_idx);
	glUniform1f(v->l_saturation,l->saturation);
	glUniform1f(v->l_contrast,l->contrast);
	glUniform1f(v->l_lightness,l->lightness);
	glUniform1f(v->l_gamma,l->gamma);
	glUniform1f(v->l_alpha,l->alpha);
	glBindVertexArray(v->va1);
	glDrawArrays(GL_TRIANGLES,0,6);
}
