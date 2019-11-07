#version 300 es

layout(location=0) in vec2 vertex;

precision highp isampler2D;

uniform int layer_idx;
uniform mat4 projection;

uniform isampler2D tex_r16i;

out vec2 tpos;
out vec2 tsize;
out float repeat_f;
out vec2 r_scale;
out vec2 r_offset;
out float color_key_f;
out vec3 color_key_val;
out vec4 filter_amount;
out float pallete_f;
out float pallete_idx;
out float use_abc_f;
out float zero_color_f;
out float draw_hole_f;

out vec3 uv;
out vec3 uv_a;
out vec3 uv_c;

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
	ivec2 idx=ivec2(512+0,256+layer_idx);

	int settings=texelFetch(tex_r16i,idx+ivec2(39,0),0).r;

	int rot=settings 			& 0x01;
	int rot_center=settings		& 0x02;
	int orbit=settings			& 0x04;
	int mirror_x=settings		& 0x08;
	int mirror_y=settings		& 0x10;
	int pallete=settings		& 0x80;
	int color_key=settings		& 0x100;
	int polygon=settings		& 0x400;
	int repeat=settings			& 0x800;
	int zero_color=settings		& 0x1000;
	int draw_hole=settings		& 0x2000;
	int linear=settings			& 0x4000;

	vec2 v_xy;
	v_xy.x=float(texelFetch(tex_r16i,idx+ivec2(0,0),0).r);
	v_xy.y=float(texelFetch(tex_r16i,idx+ivec2(1,0),0).r);
	vec2 v_size;
	v_size.x=float(texelFetch(tex_r16i,idx+ivec2(2,0),0).r);
	v_size.y=float(texelFetch(tex_r16i,idx+ivec2(3,0),0).r);

	vec2 t_xy;
	t_xy.x=float(texelFetch(tex_r16i,idx+ivec2(4,0),0).r);
	t_xy.y=float(texelFetch(tex_r16i,idx+ivec2(5,0),0).r);
	vec2 t_size;
	t_size.x=float(texelFetch(tex_r16i,idx+ivec2(6,0),0).r);
	t_size.y=float(texelFetch(tex_r16i,idx+ivec2(7,0),0).r);

	float qf=1.0;
	float qslow=1.0;
	if (polygon!=0) {
		vec2 pos=vec2(0,0);

		//http://reedbeta.com/blog/quadrilateral-interpolation-part-1/
		vec2 xy0,xy1,xy2,xy3;
		xy0.x=float(texelFetch(tex_r16i,idx+ivec2(8,0),0).r);
		xy0.y=float(texelFetch(tex_r16i,idx+ivec2(9,0),0).r);
		xy1.x=float(texelFetch(tex_r16i,idx+ivec2(10,0),0).r);
		xy1.y=float(texelFetch(tex_r16i,idx+ivec2(11,0),0).r);
		xy2.x=float(texelFetch(tex_r16i,idx+ivec2(12,0),0).r);
		xy2.y=float(texelFetch(tex_r16i,idx+ivec2(13,0),0).r);
		xy3.x=float(texelFetch(tex_r16i,idx+ivec2(14,0),0).r);
		xy3.y=float(texelFetch(tex_r16i,idx+ivec2(15,0),0).r);

		vec2 v0=xy0;
		vec2 v1=xy1;
		vec2 v2=xy2;
		vec2 v3=xy3;

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
		vec2 pos=v_xy.xy+vr;

		vec2 pnew=vec2(0,0);
		if (orbit!=0) {
			float orbit_angle=float(texelFetch(tex_r16i,idx+ivec2(23,0),0).r);
			vec2 orbit_center_xy;
			orbit_center_xy.x=float(texelFetch(tex_r16i,idx+ivec2(24,0),0).r);
			orbit_center_xy.y=float(texelFetch(tex_r16i,idx+ivec2(25,0),0).r);

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
			float rot_angle=float(texelFetch(tex_r16i,idx+ivec2(20,0),0).r);
			vec2 rot_center_xy;
			rot_center_xy.x=float(texelFetch(tex_r16i,idx+ivec2(21,0),0).r);
			rot_center_xy.y=float(texelFetch(tex_r16i,idx+ivec2(22,0),0).r);

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

	vec2 tx=vertex.xy;
	vec2 vert=tx;
	if (mirror_x!=0) vert.x=1.0-vert.x;
	if (mirror_y!=0) vert.y=1.0-vert.y;

	vec2 tr=vert*t_size;
	if (linear!=0) {
		if (vert.x>0.0) tr.x-=0.5;
		else tr.x+=0.5;
		if (vert.y>0.0) tr.y-=0.5;
		else tr.y+=0.5;
	}
	vec2 ta_xy;
	ta_xy.x=float(texelFetch(tex_r16i,idx+ivec2(35,0),0).r);
	ta_xy.y=float(texelFetch(tex_r16i,idx+ivec2(36,0),0).r);
	vec2 tc_xy;
	tc_xy.x=float(texelFetch(tex_r16i,idx+ivec2(37,0),0).r);
	tc_xy.y=float(texelFetch(tex_r16i,idx+ivec2(38,0),0).r);

	uv.xy=t_xy+tr;
	uv=vec3(uv.xy,1.0)*qf;
	uv_a.xy=ta_xy+tr;
	uv_a=vec3(uv_a.xy,1.0)*qf;
	uv_c.xy=tc_xy+tr;
	uv_c=vec3(uv_c.xy,1.0)*qf;

	tpos=t_xy;
	tsize=t_size;
	repeat_f=0.0;
	if (repeat!=0) repeat_f=1.0;
	r_scale.x=float(texelFetch(tex_r16i,idx+ivec2(16,0),0).r);
	r_scale.y=float(texelFetch(tex_r16i,idx+ivec2(17,0),0).r);
	if (r_scale.x<1.0) r_scale.x=1.0;
	if (r_scale.y<1.0) r_scale.y=1.0;
	if (tsize.x<1.0) tsize.x=1.0;
	if (tsize.y<1.0) tsize.y=1.0;
	if (v_size.x<1.0) v_size.x=1.0;
	if (v_size.y<1.0) v_size.y=1.0;
	r_offset.x=float(texelFetch(tex_r16i,idx+ivec2(18,0),0).r);
	r_offset.y=float(texelFetch(tex_r16i,idx+ivec2(19,0),0).r);
	r_offset=(vec2((1.0/(tsize))*(tsize/r_scale))*r_offset.xy)/qslow;
	r_scale=(v_size/r_scale/tsize);

	color_key_f=0.0;
	color_key_val=vec3(0.0);
	if (color_key!=0) {
		color_key_f=1.0;
		color_key_val.r=float(texelFetch(tex_r16i,idx+ivec2(26,0),0).r)/255.0;
		color_key_val.g=float(texelFetch(tex_r16i,idx+ivec2(27,0),0).r)/255.0;
		color_key_val.b=float(texelFetch(tex_r16i,idx+ivec2(28,0),0).r)/255.0;
	}
	filter_amount.r=float(texelFetch(tex_r16i,idx+ivec2(29,0),0).r)/255.0;
	filter_amount.g=float(texelFetch(tex_r16i,idx+ivec2(30,0),0).r)/255.0;
	filter_amount.b=float(texelFetch(tex_r16i,idx+ivec2(31,0),0).r);
	filter_amount.a=float(texelFetch(tex_r16i,idx+ivec2(32,0),0).r);

	pallete_f=0.0;
	pallete_idx=0.0;
	if (pallete!=0) {
		pallete_f=1.0;
		pallete_idx=float(texelFetch(tex_r16i,idx+ivec2(33,0),0).r);
	}

	use_abc_f=float(texelFetch(tex_r16i,idx+ivec2(34,0),0).r);

	zero_color_f=0.0;
	if (zero_color!=0) zero_color_f=1.0;

	draw_hole_f=0.0;
	if (draw_hole!=0) draw_hole_f=1.0;

}
