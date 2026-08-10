#ifndef SCENE_TV_H
#define SCENE_TV_H

#include "engineApi.h"
#include "storage/sprites._indices.h"

typedef struct {
  const e3d_Sprite *right;
  const e3d_Sprite *top;
  const e3d_Sprite *left;
} SceneTvFrame;

static inline void scene_tv_frame_load(SceneTvFrame *frame,
                                       e3d_EngineContext *engine_ctx) {
  if (frame->right != NULL) {
    return;
  }
  frame->right = engine_ctx->storage->get_sprite(SPRITE_TV_BIG_FRAME1);
  frame->top = engine_ctx->storage->get_sprite(SPRITE_TV_BIG_FRAME2);
  frame->left = engine_ctx->storage->get_sprite(SPRITE_TV_BIG_FRAME3);
}

static inline void scene_tv_frame_draw(const SceneTvFrame *frame,
                                       e3d_EngineContext *engine_ctx) {
  e3d_Painter_DrawSprite(engine_ctx, frame->right, 272, 20, 0, 1);
  e3d_Painter_DrawSprite(engine_ctx, frame->top, 44, 30, 0, 1);
  e3d_Painter_DrawSprite(engine_ctx, frame->left, 0, 169, 0, 1);
}

#endif
