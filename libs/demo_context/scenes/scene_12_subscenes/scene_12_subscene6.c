#include "scene_12_subscene6.h"

#include "../../demo_context.h"
#include "../../demo_scene.h"
#include "../scene_12_animations.inc"
#include "../scene_12_star_field.inc"
#include "engineApi.h"
#include "model_animation.h"
#include "storage/gfx_indices.h"
 
#define BUG_ANIMATION_ROTATION_INDEX 1u
#define BUG_ANIMATION_TRANSLATION_INDEX 2u

typedef struct {
  e3d_Mesh *ant;
  e3d_Material *ant_material;
  e3d_Camera *camera;
} Scene12Subscene6Assets;

static bool load_assets_subscene6(e3d_EngineContext *engine_ctx,
                                  SpaceSceneAssets *assets,
                                  Scene12Subscene6Assets *subscene_assets) {
  if (subscene_assets->camera != NULL) {
    return true;
  }
  subscene_assets->ant_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_ANT, 0.0f, 0.0f, true);
  subscene_assets->ant = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->ant_material, GFX_MODEL_ANT);
  if (subscene_assets->ant == NULL || subscene_assets->ant == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->ant, 0.0f,
                                  0.2127071f, 0.2127071f, 0.2127071f,
                                  MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->ant, 0.6993408f,
                                  0.0f, -1.0f, 0.0f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->ant, 0.0f, -7.0f,
                                  -2.0f, 0.0f, MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->camera = e3d_Camera_CreateCamera(
      engine_ctx, -42.6213112f, -1.9740698f, 1.0076449f, -41.6213112f,
      -1.9740698f, 1.0076449f, 0.0f, 1.0f, 0.0f);
  if (subscene_assets->camera == NULL) {
    return false;
  }

  e3d_Renderer_SetLight(engine_ctx, assets->light);
  e3d_Renderer_SetCamera(engine_ctx, subscene_assets->camera);
  return true;
}

static void unload_assets_subscene6(e3d_EngineContext *engine_ctx,
                                    Scene12Subscene6Assets *subscene_assets) {
  e3d_Camera_DeleteCamera(engine_ctx, &subscene_assets->camera);
}

static void end_scene_frame(DemoSceneContext *scene_ctx) {
  scene_ctx->scene_frame++;
  scene_ctx->scene_elapsed_ms =
      demo_platform_time_ms() - scene_ctx->scene_start_time_ms;
}

static bool scene_should_continue(const DemoSceneContext *scene_ctx) {
  return demo_is_running() &&
         (scene_ctx->scene_duration == 0u ||
          scene_ctx->scene_frame < scene_ctx->scene_duration);
}

static void render_scene(e3d_EngineContext *engine_ctx,
                         DemoSceneContext *scene_ctx, SpaceSceneAssets *assets,
                         Scene12Subscene6Assets *subscene_assets,
                         uint32_t subscene_frame) {
  if (bug_anim_translate.values_count > 0u) {
    const ModelAnimationValue *bug_anim_translate_frame =
        model_animation_get_value(&bug_anim_translate, subscene_frame);
    if (bug_anim_translate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, assets->bug, bug_anim_translate_frame->w,
          bug_anim_translate_frame->x, bug_anim_translate_frame->y,
          bug_anim_translate_frame->z, BUG_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (bug_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *bug_anim_rotate_frame =
        model_animation_get_value(&bug_anim_rotate, subscene_frame);
    if (bug_anim_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, assets->bug, bug_anim_rotate_frame->w,
          bug_anim_rotate_frame->x, bug_anim_rotate_frame->y,
          bug_anim_rotate_frame->z, BUG_ANIMATION_ROTATION_INDEX);
    }
  }
  e3d_Renderer_CleanScene(engine_ctx);
  e3d_Renderer_AddModelToScene(engine_ctx, assets->bug);
  e3d_Renderer_AddModelToScene(engine_ctx, assets->earth);
  e3d_Renderer_AddModelToScene(engine_ctx, assets->moon);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->ant);
  for (uint32_t i = 0u; i < sizeof(starField) / sizeof(starField[0]); i++) {
    e3d_Renderer_AddPointToScene(engine_ctx, &starField[i]);
  }
  e3d_Renderer_RenderScene(engine_ctx);
  end_scene_frame(scene_ctx);
  demo_end_frame();
}

void scene_12_subscene6_run_scene(DemoContext *demo_ctx,
                                  DemoSceneContext *scene_ctx,
                                  SpaceSceneAssets *assets) {
  if (demo_ctx == NULL || demo_ctx->engine_ctx == NULL || scene_ctx == NULL ||
      assets == NULL) {
    return;
  }

  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  Scene12Subscene6Assets subscene_assets = {0};
  if (!load_assets_subscene6(engine_ctx, assets, &subscene_assets)) {
    unload_assets_subscene6(engine_ctx, &subscene_assets);
    return;
  }

  if (scene_ctx->scene_frame == 0u) {
    scene_ctx->scene_start_time_ms = demo_platform_time_ms();
  }

  uint32_t subscene_frame = 0u;
  for (; subscene_frame < 100 && scene_should_continue(scene_ctx);
       subscene_frame++) {
    demo_begin_frame();
    render_scene(engine_ctx, scene_ctx, assets, &subscene_assets,
                 subscene_frame);
  }

  unload_assets_subscene6(engine_ctx, &subscene_assets);
}
