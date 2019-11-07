#version 300 es

precision mediump float;

uniform float lightness;
uniform float contrast;
uniform float saturation;
uniform float gamma;
uniform float alpha;

uniform sampler2D tex_gpu;
uniform sampler2D tex_rgba8;

in vec2 tpos;
in vec2 tsize;
in float repeat_f;
in vec2 r_scale;
in vec2 r_offset;
in float color_key_f;
in vec3 color_key_val;
in vec4 filter_amount;
in float pallete_f;
in float pallete_idx;
in float use_abc_f;
in float zero_color_f;
in float draw_hole_f;

in vec3 uv;
in vec3 uv_a;
in vec3 uv_c;

out vec4 frag;

void main()
{
	vec2 uv2=uv.xy/uv.z;
	if (repeat_f>0.0) {
		uv2=tpos+mod((uv2.xy*r_scale)-r_offset,tsize);
	}
	ivec2 st=ivec2(uv2);

	vec4 color=texelFetch(tex_gpu,st,0);
	vec4 color1=texture(tex_gpu,uv2/4096.0);
	color.a=clamp(color1.a,0.0,1.0*filter_amount.r)+clamp(color.a,0.0,1.0-filter_amount.r);
	color.rgb=mix(color1.rgb,color.rgb,1.0-filter_amount.r);
	if (filter_amount.w>0.0) {
		float st1=3.0;
		color.r=round(color.r*st1)/st1;
		color.g=round(color.g*st1)/st1;
		color.b=round(color.b*st1)/st1;
	}

	if (color.a<filter_amount.g) color.a=0.0;
	else if (filter_amount.b>0.0) color.a=1.0;

	if (zero_color_f>0.0) color.rgb=vec3(0.0);

	if (zero_color_f>0.0) color.rgb=vec3(0.0);

	if (draw_hole_f>0.0) {
		if (color.a>0.0) color.a=0.0;
		else color.a=1.0;
	}

	if (color_key_f>0.0) {
		if (color.rgb==color_key_val) color.a=0.0;
	}

	if (pallete_f>0.0) {
		ivec2 stp=ivec2(color.r*255.0,pallete_idx);
		color.xyz=texelFetch(tex_rgba8,stp,0).xyz;
	}

	if (use_abc_f>0.0) {//0-skip,1-ab,2-aib,3-abc
		//color_a=color_a.rgb*color_a.a
		//color_c=color_c.rgb*color_c.a
		//inv_alpha=1-alpha_b
		//alpha=alpha_b
		//d.color.rgba=vec4(color_a.rgb*inv_alpha,inv_alpha)+vec4(color_c.rgb*alpha,alpha)

		vec2 uv2_a=uv_a.xy/uv_a.z;
		vec2 uv2_c=uv_c.xy/uv_c.z;

		vec4 color_a=texture(tex_gpu,uv2_a/4096.0);
		vec4 color_c=texture(tex_gpu,uv2_c/4096.0);
		color_a.rgb=color_a.rgb*color_a.a;
		color_c.rgb=color_c.rgb*color_c.a;

		if (filter_amount.w>0.0) {
			float st1=3.0;
			color_a.r=round(color_a.r*st1)/st1;
			color_a.g=round(color_a.g*st1)/st1;
			color_a.b=round(color_a.b*st1)/st1;
			color_c.r=round(color_c.r*st1)/st1;
			color_c.g=round(color_c.g*st1)/st1;
			color_c.b=round(color_c.b*st1)/st1;
		}


		float av=color.a;
		float i_av=1.0-av;

		if (use_abc_f==1.0) {
			color.rgba=vec4(color_a.rgb,av);
		}
		if (use_abc_f==2.0) {
			color.rgba=vec4(color_a.rgb,i_av);
		}
		if (use_abc_f==3.0) {
			color.rgba=vec4(color_a.rgb*i_av,i_av)+vec4(color_c.rgb*av,av);
		}
	}

	//saturation like in gimp -1.0,1.0
	float mn=min(min(color.r,color.g),color.b);
	float mx=max(max(color.r,color.g),color.b);
	vec3 light=vec3((mn+mx)/2.0);
	vec3 color_sat=mix(color.rgb,light,-saturation);

	//contrast -1.0,1.0
	vec3 color_contrast=((color_sat-0.5)*(1.0+contrast))+0.5;

	//lightness -1.0,1.0
	vec3 color_light=color_contrast+lightness;

	//gamma 0.0,10.0
	vec3 color_gamma=pow(color_light,vec3(1.0/gamma));

	frag=vec4(color_gamma,color.a*alpha);
}
