
#include "types.h"
#include "input.c"

unsigned int find_crtc_for_encoder(drmModeRes *res,drmModeEncoder *enc)
{
	int i;

	for (i=0;i<res->count_crtcs;i++) {
		unsigned int mask=1<<i;
		unsigned int id=res->crtcs[i];
		if (enc->possible_crtcs&mask) return id;
	}
	return -1;
}

unsigned int find_crtc_for_connector(drmModeRes *res,drmModeConnector *con)
{
	int i;

	for (i=0;i<con->count_encoders;i++) {
		unsigned int ei=con->encoders[i];
		drmModeEncoder *enc=drmModeGetEncoder(drm.fd,ei);
		if (enc) {
			unsigned int id=find_crtc_for_encoder(res,enc);
			drmModeFreeEncoder(enc);
			if (id!=0) return id;
		}
	}
	return -1;
}

void init_drm()
{
	drmModeRes *res;
	drmModeConnector *con=NULL;
	drmModeEncoder *enc=NULL;
	int i;

	drm.fd=open("/dev/dri/card0",O_RDWR);
	if (drm.fd<0) {
		printf("could not open drm device\n");
		exit(-1);
	}

	res=drmModeGetResources(drm.fd);
	if (!res) {
		printf("drmModeGetResources failed: %s\n",strerror(errno));
		exit(-1);
	}

	/* find a connected connector: */
	for (i=0;i<res->count_connectors;i++) {
		con=drmModeGetConnector(drm.fd,res->connectors[i]);
		if (con->connection==DRM_MODE_CONNECTED) break;
		drmModeFreeConnector(con);
		con=NULL;
	}
	if (!con) {
		printf("no connected connector!\n");
		exit(-1);
	}

	for (i=0;i<con->count_modes;i++) {
		drm.minfo=&con->modes[i];
		printf("mode:%ix%i@%i\n",drm.minfo->hdisplay,drm.minfo->vdisplay,drm.minfo->vrefresh);
		drm.minfo=NULL;
	}

	for (i=0;i<con->count_modes;i++) {
		drm.minfo=&con->modes[i];
//		if ((drm.minfo->hdisplay==1920)&&(drm.minfo->vdisplay==1080)) break;
		if ((drm.setx!=0)&&(drm.sety!=0)) {
			if ((drm.minfo->hdisplay==drm.setx)&&(drm.minfo->vdisplay==drm.sety)) break;
		} else {
			if (drm.minfo->type&DRM_MODE_TYPE_PREFERRED) break;
		}
		drm.minfo=NULL;
	}
	if (!drm.minfo) {
		printf("could not find mode info!\n");
		exit(-1);
	}
	printf("SIZE:%ix%i\n",drm.minfo->hdisplay,drm.minfo->vdisplay);

	/* find encoder: */
	for (i=0;i<res->count_encoders;i++) {
		enc=drmModeGetEncoder(drm.fd,res->encoders[i]);
		if (enc->encoder_id==con->encoder_id) break;
		drmModeFreeEncoder(enc);
		enc=NULL;
	}
	if (enc) drm.crtc_id=enc->crtc_id;
	else {
		unsigned int id=find_crtc_for_connector(res,con);
		if (id==0) {
			printf("no crtc found!\n");
			exit(-1);
		}
		drm.crtc_id=id;
	}
	drm.con_id=con->connector_id;

	drm.gbm_dev=gbm_create_device(drm.fd);
	drm.gbm_surface=gbm_surface_create(drm.gbm_dev,
			drm.minfo->hdisplay,drm.minfo->vdisplay,
			GBM_FORMAT_XRGB8888,
			GBM_BO_USE_SCANOUT|GBM_BO_USE_RENDERING);
	if (!drm.gbm_surface) {
		printf("failed to create gbm surface\n");
		exit(-1);
	}

}

void egl_choose_config(EGLDisplay disp,EGLint *attr,EGLint id,EGLConfig *out)
{
	EGLint cnt=0;
	EGLint match=0;
	EGLConfig *conf;

	if (!eglGetConfigs(disp,NULL,0,&cnt)||cnt<1) {
		printf("No EGL configs to choose from.\n");
		exit(-1);
	}
	conf=malloc(cnt*sizeof(*conf));

	if (!eglChooseConfig(disp,attr,conf,cnt,&match)) {
		printf("No EGL configs with appropriate attributes.\n");
		exit(-1);
	}

	int i;
	for (i=0;i<cnt;i++) {
		EGLint vi;
		if (!eglGetConfigAttrib(disp,conf[i],EGL_NATIVE_VISUAL_ID,&vi)) continue;
		if (vi==id) break;
	}
	if (i==cnt) {
		printf("No matching config to visual.\n");
		exit(-1);
	}
	*out=conf[i];
	free(conf);
}

void init_drm_gl()
{
	EGLint major,minor;

	EGLint context_attribs[]={EGL_CONTEXT_CLIENT_VERSION,3,EGL_NONE};

	EGLint config_attribs[]={
		EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
		EGL_RED_SIZE,8,
		EGL_GREEN_SIZE,8,
		EGL_BLUE_SIZE,8,
		EGL_ALPHA_SIZE,0,
		EGL_RENDERABLE_TYPE,EGL_OPENGL_ES3_BIT,
		EGL_NONE
	};

	PFNEGLGETPLATFORMDISPLAYEXTPROC get_platform_display=NULL;
	get_platform_display=(void*)eglGetProcAddress("eglGetPlatformDisplayEXT");
	drm.gl_display=get_platform_display(EGL_PLATFORM_GBM_KHR,drm.gbm_dev,NULL);
	if (!eglInitialize(drm.gl_display,&major,&minor)) {
		printf("failed to initialize\n");
		exit(-1);
	}

	if (!eglBindAPI(EGL_OPENGL_ES_API)) {
		printf("failed to bind api EGL_OPENGL_ES_API\n");
		exit(-1);
	}

	egl_choose_config(drm.gl_display,config_attribs,GBM_FORMAT_XRGB8888,&drm.gl_config);

	drm.gl_context=eglCreateContext(drm.gl_display,drm.gl_config,EGL_NO_CONTEXT,context_attribs);
	if (drm.gl_context==NULL) {
		printf("failed to create context\n");
		exit(-1);
	}

	drm.gl_surface=eglCreateWindowSurface(drm.gl_display,drm.gl_config,drm.gbm_surface,NULL);
	if (drm.gl_surface==EGL_NO_SURFACE) {
		printf("failed to create egl surface\n");
		exit(-1);
	}

	/* connect the context to the surface */
	eglMakeCurrent(drm.gl_display,drm.gl_surface,drm.gl_surface,drm.gl_context);
}

void drm_fb_destroy_callback(struct gbm_bo *bo, void *data)
{
	DRM_FB *fb=data;
	if (fb->fb_id) drmModeRmFB(drm.fd,fb->fb_id);
	free(fb);
}

DRM_FB *drm_fb_get_from_bo(struct gbm_bo *bo)
{
	unsigned int width,height,stride,handle;
	int ret;

	DRM_FB *fb=gbm_bo_get_user_data(bo);
	if (fb)	return fb;
	fb=calloc(1,sizeof(*fb));
	fb->bo=bo;

	width=gbm_bo_get_width(bo);
	height=gbm_bo_get_height(bo);
	stride=gbm_bo_get_stride(bo);
	handle=gbm_bo_get_handle(bo).u32;

	ret=drmModeAddFB(drm.fd,width,height,24,32,stride,handle,&fb->fb_id);
	if (ret) {
		printf("failed to create fb: %s\n",strerror(errno));
		free(fb);
		return NULL;
	}
	gbm_bo_set_user_data(bo,fb,drm_fb_destroy_callback);

	return fb;
}

void init_fb()
{
	int ret;

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(drm.gl_display,drm.gl_surface);
	drm.bo_main=gbm_surface_lock_front_buffer(drm.gbm_surface);
	drm.drm_fb_main=drm_fb_get_from_bo(drm.bo_main);

	ret=drmModeSetCrtc(drm.fd,drm.crtc_id,drm.drm_fb_main->fb_id,0,0,&drm.con_id,1,drm.minfo);
	if (ret) {
		printf("failed to set mode: %s\n", strerror(errno));
		exit(-1);
	}
}

void page_flip_handler(int fd,unsigned int frame,unsigned int sec,unsigned int usec,void *data)
{
	int *wait_flip=data;
	*wait_flip=0;
}

void flip()
{
	int wait_flip=1;
	drmEventContext evctx={.version=DRM_EVENT_CONTEXT_VERSION,.page_flip_handler=page_flip_handler,};
	struct gbm_bo *next_bo;
	int ret;

	eglSwapBuffers(drm.gl_display,drm.gl_surface);

	next_bo=gbm_surface_lock_front_buffer(drm.gbm_surface);
	drm.drm_fb_main=drm_fb_get_from_bo(next_bo);

	ret=drmModePageFlip(drm.fd,drm.crtc_id,drm.drm_fb_main->fb_id,DRM_MODE_PAGE_FLIP_EVENT,&wait_flip);
	if (ret) {
		printf("failed to queue page flip: %s\n", strerror(errno));
		exit(-1);
	}

	while (wait_flip) {
		drmHandleEvent(drm.fd,&evctx);
	}
	gbm_surface_release_buffer(drm.gbm_surface,drm.bo_main);
	drm.bo_main=next_bo;
}

void setup_drm(int *sx,int *sy,int setx,int sety)
{
	drm.setx=setx;
	drm.sety=sety;
	init_drm();
	init_drm_gl();
	init_fb();
	*sx=drm.minfo->hdisplay;
	*sy=drm.minfo->vdisplay;

	init_input(*sx,*sy);
}
