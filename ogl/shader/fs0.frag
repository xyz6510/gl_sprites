#version 300 es

precision mediump float;

uniform sampler2D tex_rgba8;
uniform sampler2D tex_sprites;

in vec2 tpos;
in vec2 tsize;
in float repeat_f;
in vec2 r_scale;
in vec2 r_offset;
in vec3 uv;
in vec4 alpha_light_pallete_ckey_f;
in vec3 alpha_light_pallete_val;
in vec4 color_key_val;
in vec4 filter_val;
in float zero_color_f;
in float draw_hole_f;

out vec4 frag;

void main()
{

	vec2 uv2=(uv.xy)/uv.z;
	if (repeat_f>0.0) {
		uv2=tpos+mod((uv2.xy*r_scale)-r_offset,tsize);
	}
	ivec2 st=ivec2(uv2);

	vec4 color=texelFetch(tex_sprites,st,0);
	vec4 color1=texture(tex_sprites,uv2/512.0);
	color.a=clamp(color1.a,0.0,1.0*filter_val.r)+clamp(color.a,0.0,1.0-filter_val.r);
	color.rgb=mix(color1.rgb,color.rgb,1.0-filter_val.r);
	if (filter_val.w>0.0) {
		float st1=3.0;
		color.r=round(color.r*st1)/st1;
		color.g=round(color.g*st1)/st1;
		color.b=round(color.b*st1)/st1;
	}

	if (color.a<filter_val.g) color.a=0.0;
	else if (filter_val.b>0.0) color.a=1.0;

	if (zero_color_f>0.0) color.rgb=vec3(0.0);

	if (draw_hole_f>0.0) {
		if (color.a>0.0) color.a=0.0;
		else color.a=1.0;
	}

	if (alpha_light_pallete_ckey_f.w<0.0) {
		if (color.rgb==color_key_val.rgb) color.a=0.0;
	}
	if (alpha_light_pallete_ckey_f.w>0.0) {
		if (color.rgba==color_key_val) color.a=0.0;
		else color.a=1.0;
	}

	if (alpha_light_pallete_ckey_f.z>0.0) {
		ivec2 stp=ivec2(color.r*255.0,alpha_light_pallete_val.z);
		color.xyz=texelFetch(tex_rgba8,stp,0).xyz;
	}

	if (alpha_light_pallete_ckey_f.y>0.0) {
		color.xyz=color.xyz+alpha_light_pallete_val.y;
	}
	if (alpha_light_pallete_ckey_f.x>0.0) {
		color.a=color.a*alpha_light_pallete_val.x;
	}

	frag=color;
}
