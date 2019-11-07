#include "../files/files.h"

#define GL_GLEXT_PROTOTYPES
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <errno.h>

#include <poll.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <libudev.h>
#include <libinput.h>

#define key_input_buf __dirpath__ "/key_input_buf"
#define keybuf_size 256*4

#define mouse_input_buf __dirpath__ "/mouse_input_buf"
#define mousebuf_size 256*4

int *keybuf=NULL;
int *mousebuf=NULL;

typedef struct {
    struct gbm_bo *bo;
    unsigned int fb_id;
} DRM_FB;

typedef struct {
	int setx;
	int sety;

	int fd;
	drmModeModeInfo *minfo;
	unsigned int crtc_id;
	unsigned int con_id;

	//gbm
	struct gbm_device *gbm_dev;
	struct gbm_surface *gbm_surface;

	//drm_fb
	DRM_FB *drm_fb;

	//drm_fb_main
	DRM_FB *drm_fb_main;

	//gl
	EGLDisplay gl_display;
	EGLConfig gl_config;
	EGLContext gl_context;
	EGLSurface gl_surface;

	struct gbm_bo *bo_main;

} DRM;

DRM drm;
