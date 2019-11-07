
void ortho2d(float x,float y,float x1,float y1,GLfloat *out)
{
	float zn=-1.0;
	float zf=1.0;
	float iz=1.0/(zf-zn);
	float iy=1.0/(y1-y);//y flip
	float ix=1.0/(x1-x);

	GLfloat pr[]={
		2.0*ix,     0.0,        0.0,            0.0,
		0.0,        2.0*iy,     0.0,            0.0,
		0.0,        0.0,        -2.0*iz,        0.0,
		-(x1+x)*ix,-(y+y1)*iy,  -(zf+zn)*iz,    1.0
	};
	memcpy(out,pr,sizeof(pr));
}
