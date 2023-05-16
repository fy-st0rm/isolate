#ifndef __ISO_GL_RENDER_PIPELINE_H__
#define __ISO_GL_RENDER_PIPELINE_H__

#include "iso_graphics/iso_graphics.h"

ISO_API_INTERNAL iso_render_pipeline* iso_gl_render_pipeline_new(iso_graphics* graphics, iso_render_pipeline_def def);
ISO_API_INTERNAL void iso_gl_render_pipeline_delete(iso_graphics* graphics, iso_render_pipeline* pip);
ISO_API_INTERNAL void iso_gl_render_pipeline_begin(iso_graphics* graphics, iso_render_pipeline* pip);
ISO_API_INTERNAL void iso_gl_render_pipeline_end(iso_graphics* graphics, iso_render_pipeline* pip, i32 indices_cnt);

#endif //__ISO_GL_RENDER_PIPELINE_H__
