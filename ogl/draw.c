
void draw()
{
	DRAW_ORDER *d=&draw_order;
	SPRITE_GROUP *sg=&sprite_group;
	LAYER_GROUP *lg=&layer_group;
	VIDEO_VAR *v=&video_var;

	glBindFramebuffer(GL_FRAMEBUFFER,v->fb_gpu_mem);
	glUseProgram(v->shd0);
	glBindVertexArray(v->va0);

	glEnable(GL_SCISSOR_TEST);
	int i,j;
	for (i=0;i<8;i++) {
		if (lg->active[i]==0) continue;
		unsigned int act=1;
		for (j=0;j<32;j++) {
			if (lg->active[i]&act) draw_to_layer(&lg->l[(i<<5)+j],lg->clear[i]&act);
			if (lg->once[i]&act) {
				lg->once[i]=lg->once[i]&(~act);
				lg->clear[i]=lg->clear[i]&(~act);
				lg->active[i]=lg->active[i]&(~act);
			}
			act=act<<1;
		}
	}

	glScissor(0,0,v->screen_x,v->screen_y);

	clear_fb_screen();

	glUseProgram(v->shd0);
	glUniformMatrix4fv(v->shd0_projection,1,GL_FALSE,v->projection);

	for (i=d->start;i<d->end;i++) {
		int v=d->order[i];
		if (v<0) continue;
		int act=v&0xff;
		unsigned int idx=act>>5;
		unsigned int idb=1<<(act-(idx<<5));
		switch (v) {
			case 0 ... 255:
				if (sg->active[idx]&idb) draw_sprites(sg->start[act],sg->end[act]);
				break;
			case 256 ... 511:
				if (lg->draw[idx]&idb) draw_layer(&lg->l[act]);
				break;
			default: break;
		}
	}

	draw_fb_screen();
}
