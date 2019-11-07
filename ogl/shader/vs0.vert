#version 300 es

layout(location=0) in vec4 vertex;

precision highp isampler2D;

uniform mat4 projection;

uniform isampler2D tex_rg16i;
uniform isampler2D tex_r16i;
uniform sampler2D tex_rgba8;

out vec2 tpos;
out vec2 tsize;
out float repeat_f;
out vec2 r_scale;
out vec2 r_offset;
out vec4 alpha_light_pallete_ckey_f;
out vec3 alpha_light_pallete_val;
out vec4 color_key_val;
out vec4 filter_val;
out float zero_color_f;
out float draw_hole_f;

out vec3 uv;

vec2 center_spot(vec2 v0,vec2 v1,vec2 v2,vec2 v3)
{
	float a1=v2.y-v0.y;
	float b1=v0.x-v2.x;
	float c1=a1*v0.x+b1*v0.y;

	float a2=v3.y-v1.y;
	float b2=v1.x-v3.x;
	float c2=a2*v1.x+b2*v1.y;

	float d=a1*b2-a2*b1;
	if (d<0.0001) d=1.0;

	float xc=(b2*c1-b1*c2)/d;
	float yc=(a1*c2-a2*c1)/d;

	return vec2(xc,yc);
}
float distance_spot(vec2 v1,vec2 v2)
{
	vec2 sq=(v1-v2)*(v1-v2);
	return sqrt(sq.x+sq.y);
}
float qcalc(float d1,float d2)
{
	if (d2<0.0001) d2=1.0;
	return (d1+d2)/d2;
}

void main()
{
	const float pi=3.1415926535897932384626433832795;

	ivec2 idx=ivec2(vertex.zw);
	ivec2 order=ivec2(texelFetch(			tex_rg16i,idx,0).rg);
	vec2 v_xy=vec2(texelFetch(				tex_rg16i,ivec2(256,0)	+order,0).rg);
	vec2 v_size=vec2(texelFetch(			tex_rg16i,ivec2(512,0)	+order,0).rg);
	vec2 t_xy=vec2(texelFetch(				tex_rg16i,ivec2(768,0)	+order,0).rg);
	vec2 t_size=vec2(texelFetch(			tex_rg16i,ivec2(0,  256)+order,0).rg);
	vec2 rot_center_xy=vec2(texelFetch(		tex_rg16i,ivec2(256,256)+order,0).rg);
	vec2 orbit_center_xy=vec2(texelFetch(	tex_rg16i,ivec2(512,256)+order,0).rg);
	vec2 repeat_scale=vec2(texelFetch(		tex_rg16i,ivec2(768,256)+order,0).rg);
	vec2 repeat_offset=vec2(texelFetch(		tex_rg16i,ivec2(0,512)	+order,0).rg);

	int settings=texelFetch(	tex_r16i,order,0).r;
	int rot_angle=texelFetch(	tex_r16i,ivec2(256,	0)	+order,0).r;
	int orbit_angle=texelFetch(	tex_r16i,ivec2(512,	0)	+order,0).r;
	int light_v=texelFetch(		tex_r16i,ivec2(768,	0)	+order,0).r;
	int alpha_v=texelFetch(		tex_r16i,ivec2(0,	256)+order,0).r;
	int pal_idx=texelFetch(		tex_r16i,ivec2(256,	256)+order,0).r;

	filter_val=texelFetch(tex_rgba8,ivec2(512,0)+order,0).rgba;

	int rot=settings 			& 0x01;
	int rot_center=settings		& 0x02;
	int orbit=settings			& 0x04;
	int mirror_x=settings		& 0x08;
	int mirror_y=settings		& 0x10;
	int alpha=settings			& 0x20;
	int light=settings			& 0x40;
	int pallete=settings 		& 0x80;
	int color_key=settings		& 0x100;
	int color_key_inv=settings	& 0x200;
	int polygon=settings		& 0x400;
	int repeat=settings			& 0x800;
	int zero_color=settings		& 0x1000;
	int draw_hole=settings		& 0x2000;
	int linear=settings			& 0x4000;

	float qf=1.0;
	float qslow=1.0;
	if (polygon!=0) {
		vec2 pos=vec2(0,0);

		//http://reedbeta.com/blog/quadrilateral-interpolation-part-1/
		vec2 v0=vec2(texelFetch(tex_rg16i,ivec2(256,512)+order,0).rg);
		vec2 v1=vec2(texelFetch(tex_rg16i,ivec2(512,512)+order,0).rg);
		vec2 v2=vec2(texelFetch(tex_rg16i,ivec2(768,512)+order,0).rg);
		vec2 v3=vec2(texelFetch(tex_rg16i,ivec2(0,	768)+order,0).rg);

		vec2 cn=center_spot(v0,v1,v2,v3);

		float d0=distance_spot(v0,cn);
		float d1=distance_spot(v1,cn);
		float d2=distance_spot(v2,cn);
		float d3=distance_spot(v3,cn);

		float q0=qcalc(d0,d2);
		float q1=qcalc(d1,d3);
		float q2=qcalc(d2,d0);
		float q3=qcalc(d3,d1);

		if (vertex.xy==vec2(0,0)) {pos=v0;qf=q0;}
		if (vertex.xy==vec2(1,0)) {pos=v1;qf=q1;}
		if (vertex.xy==vec2(1,1)) {pos=v2;qf=q2;}
		if (vertex.xy==vec2(0,1)) {pos=v3;qf=q3;}

		gl_Position=projection*vec4(pos,0.0,1.0);

		qslow=q0+q1+q2+q3;
	} else {
		vec2 vr=vertex.xy*v_size;
		vec2 pos=v_xy+vr;

		vec2 pnew=vec2(0,0);
		if (orbit!=0) {
			const float one_d=pi/32767.0;
			float ang=float(orbit_angle)*one_d;
			float ca=cos(ang);
			float sa=sin(ang);
			pnew.x=ca*(v_xy.x-orbit_center_xy.x)-sa*(v_xy.y-orbit_center_xy.y)+orbit_center_xy.x;
			pnew.y=sa*(v_xy.x-orbit_center_xy.x)+ca*(v_xy.y-orbit_center_xy.y)+orbit_center_xy.y;
			vr=(vertex.xy-0.5)*v_size;
			pos=pnew+vr-0.5;
		}
		if (rot!=0) {
			const float one_d=pi/32767.0;
			float ang=float(rot_angle)*one_d;
			float ca=cos(ang);
			float sa=sin(ang);
			if (orbit!=0) v_xy=pnew-(v_size/2.0);
			if (rot_center!=0) {
				vec2 cxy=(1.0/32767.0)*rot_center_xy;
				vr=vertex.xy-cxy;
				vr=vec2( (vr.x*ca)+(vr.y*sa),(vr.y*ca)-(vr.x*sa) );
				pos=v_xy+(vr*v_size)+(v_size/vec2(1.0/cxy.x,1.0/cxy.y))-0.5;
			} else {
				vr=(vertex.xy-0.5)*v_size;
				vr=vec2( (vr.x*ca)+(vr.y*sa),(vr.y*ca)-(vr.x*sa) );
				pos=v_xy+vr+(v_size/2.0)-0.5;
			}
		}
		gl_Position=projection*vec4(pos,0.0,1.0);
	}

	vec2 vert=vertex.xy;
	if (mirror_x!=0) vert.x=1.0-vert.x;
	if (mirror_y!=0) vert.y=1.0-vert.y;

	vec2 tr=vert*t_size;
	if (linear!=0) {//if this and filter_val.r=255 thin letters comment tr.y+-0.5 no rescale
		if (vert.x>0.0) tr.x-=0.5;
		else tr.x+=0.5;
		if (vert.y>0.0) tr.y-=0.5;
		else tr.y+=0.5;
	}
	uv.xy=t_xy.xy+tr;
	uv=vec3(uv.xy,1.0)*qf;

	tpos=t_xy;
	tsize=t_size;
	repeat_f=0.0;
	if (repeat!=0) repeat_f=1.0;
	r_scale=repeat_scale.xy;
	if (r_scale.x<1.0) r_scale.x=1.0;
	if (r_scale.y<1.0) r_scale.y=1.0;
	if (tsize.x<1.0) tsize.x=1.0;
	if (tsize.y<1.0) tsize.y=1.0;
	if (v_size.x<1.0) v_size.x=1.0;
	if (v_size.y<1.0) v_size.y=1.0;
	r_offset=(vec2((1.0/(tsize))*(tsize/r_scale))*repeat_offset.rg)/qslow;
	r_scale=(v_size/r_scale/tsize);

	float alpha_if=0.0;
	float light_if=0.0;
	float pallete_if=0.0;
	float color_key_if=0.0;
	if (alpha!=0) alpha_if=1.0;
	if (light!=0) light_if=1.0;
	if (pallete!=0) pallete_if=1.0;
	if (color_key!=0) color_key_if=-1.0;
	if (color_key_inv!=0) color_key_if=1.0;
	alpha_light_pallete_ckey_f=vec4(alpha_if,light_if,pallete_if,color_key_if);

	float alpha_val=0.0;
	if (alpha!=0) alpha_val=(1.0/255.0)*float(alpha_v);

	float light_val=0.0;
	if (light!=0) light_val=(1.0/255.0)*float(light_v);

	color_key_val=vec4(0.0);
	if (color_key_if!=0.0) color_key_val=texelFetch(tex_rgba8,ivec2(256,0)+order,0).rgba;

	float pallete_val=0.0;
	if (pallete!=0) pallete_val=float(pal_idx);

	zero_color_f=0.0;
	if (zero_color!=0) zero_color_f=1.0;

	draw_hole_f=0.0;
	if (draw_hole!=0) draw_hole_f=1.0;

	alpha_light_pallete_val=vec3(alpha_val,light_val,pallete_val);
}

/*
	1  2  1  2
	4  3  4  3
*/