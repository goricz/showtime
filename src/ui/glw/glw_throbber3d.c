/*
 *  GL Widgets, 3D Throbber
 *  Copyright (C) 2010 Andreas Öman
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "glw.h"
#include "glw_renderer.h"

typedef struct glw_throbber3d {
  glw_t w;

  float angle;
  
  glw_renderer_t renderer;

} glw_throbber3d_t;



#define PINWIDTH  0.05
#define PINBOTTOM 0.2
#define PINTOP    1.0

static const struct {
  float x, y, z;
} pin[] = {
  {-PINWIDTH,  PINBOTTOM,   PINWIDTH},
  { PINWIDTH,  PINBOTTOM,   PINWIDTH},
  { PINWIDTH,  PINBOTTOM,  -PINWIDTH},
  {-PINWIDTH,  PINBOTTOM,  -PINWIDTH},

  {-PINWIDTH,  PINTOP,      PINWIDTH},
  { PINWIDTH,  PINTOP,      PINWIDTH},
  { PINWIDTH,  PINTOP,     -PINWIDTH},
  {-PINWIDTH,  PINTOP,     -PINWIDTH},
};

#define pinvtx(n) pin[n].x, pin[n].y, pin[n].z

static uint16_t surfaces[] = {
  0,1,5,  0,5,4,
  2,3,7,  2,7,6,

  8+3,8+0,8+4,8+3,8+4,8+7,
  8+1,8+2,8+6,8+1,8+6,8+5,
  8+3,8+2,8+1,8+3,8+1,8+0,
  8+4,8+5,8+6,8+4,8+6,8+7,
};

/**
 *
 */
static int
glw_throbber_callback(glw_t *w, void *opaque, glw_signal_t signal, void *extra)
{
  glw_throbber3d_t *gt = (glw_throbber3d_t *)w;

  if(signal == GLW_SIGNAL_LAYOUT)
    gt->angle += 2;

  return 0;
}

/**
 *
 */
static void
glw_throbber_render(glw_t *w, glw_rctx_t *rc)
{
  glw_throbber3d_t *gt = (glw_throbber3d_t *)w;
  glw_rctx_t rc0, rc1;
  int i;
  glw_root_t *gr = w->glw_root;
  float a0 = w->glw_alpha * rc->rc_alpha;
  if(a0 < 0.01)
    return;

  if(!glw_renderer_initialized(&gt->renderer)) {
    glw_renderer_init(&gt->renderer, 16, 12, surfaces);

    for(i = 0; i < 16; i++) {
      glw_renderer_vtx_pos(&gt->renderer, i, pinvtx(i & 7));
      if(i < 8)
	glw_renderer_vtx_col(&gt->renderer, i, 1,1,1,1);
      else
	glw_renderer_vtx_col(&gt->renderer, i, 0.25, 0.25, 0.25, 1);
    }
  }

  rc0 = *rc;
  glw_scale_to_aspect(&rc0, 1.0);

  glw_blendmode(GLW_BLEND_ADDITIVE);

#define NUMPINS 15

  for(i = 1; i < NUMPINS; i++) {
    
    float alpha = (1 - ((float)i / NUMPINS)) * rc->rc_alpha * w->glw_alpha;

    rc1 = rc0;
    glw_Rotatef(&rc1, 0.1 * gt->angle - i * ((360 / NUMPINS) / 3), 0, 1, 0);
    glw_Rotatef(&rc1,       gt->angle - i *  (360 / NUMPINS),      0, 0, 1);

    glw_renderer_draw(&gt->renderer, gr, &rc1, NULL, NULL, alpha);
  }
  glw_blendmode(GLW_BLEND_NORMAL);
}


static void
glw_throbber_dtor(glw_t *w)
{
  glw_throbber3d_t *gt = (glw_throbber3d_t *)w;
  glw_renderer_free(&gt->renderer);

}

/**
 *
 */
static glw_class_t glw_throbber3d = {
  .gc_name = "throbber3d",
  .gc_instance_size = sizeof(glw_throbber3d_t),
  .gc_render = glw_throbber_render,
  .gc_signal_handler = glw_throbber_callback,
  .gc_dtor = glw_throbber_dtor,
};

GLW_REGISTER_CLASS(glw_throbber3d);
