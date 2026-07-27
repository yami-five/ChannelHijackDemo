#ifndef SCENE_TV_H
#define SCENE_TV_H

#include "engineApi.h"

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
  frame->right = engine_ctx->storage->get_sprite(1u);
  frame->top = engine_ctx->storage->get_sprite(2u);
  frame->left = engine_ctx->storage->get_sprite(3u);
}

static inline void scene_tv_frame_draw(const SceneTvFrame *frame,
                                       e3d_EngineContext *engine_ctx) {
  e3d_Painter_DrawSprite(engine_ctx, frame->right, 272, 20, 0, 1);
  e3d_Painter_DrawSprite(engine_ctx, frame->top, 44, 30, 0, 1);
  e3d_Painter_DrawSprite(engine_ctx, frame->left, 0, 169, 0, 1);
}

#endif
