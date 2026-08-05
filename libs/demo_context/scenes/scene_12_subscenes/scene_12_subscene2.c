#include "scene_12_subscene2.h"

#include "../../demo_context.h"
#include "../../demo_scene.h"
#include "../scene_12_animations.inc"
#include "../scene_12_star_field.inc"
#include "engineApi.h"
#include "model_animation.h"
#include "storage/gfx_indices.h"

#define BEETLE1_ANIMATION_TRANSFORM_INDEX 1u
#define BEETLE2_ANIMATION_TRANSFORM_INDEX 1u
#define SUBSCENE2_FRAMES 110u

typedef struct {
  e3d_Mesh *beetle1;
  e3d_Mesh *beetle2;
  e3d_Mesh *beetle3;
  e3d_Material *beetle_material1;
  e3d_Material *beetle_material2;
  e3d_Material *beetle_material3;
  e3d_Camera *camera;
} subscene2Scene12Subscene2Assets;

static bool
load_assets_subscene2(e3d_EngineContext *engine_ctx, SpaceSceneAssets *assets,
                      subscene2Scene12Subscene2Assets *subscene_assets) {
  if (subscene_assets->camera != NULL) {
    return true;
  }

  subscene_assets->beetle_material1 = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BEETLE1, 0.0f, 0.0f, true);
  subscene_assets->beetle1 = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->beetle_material1, GFX_MODEL_BEETLE1);
  if (subscene_assets->beetle_material1 == NULL ||
      subscene_assets->beetle1 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle1,
                                  0.2610651f, -0.7223285f, -0.3626964f,
                                  -0.5888062f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle1, 0.0f,
                                  0.0f, 0.0f, 0.0f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->beetle_material2 = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BEETLE2, 0.0f, 0.0f, true);
  subscene_assets->beetle2 = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->beetle_material2, GFX_MODEL_BEETLE2);
  if (subscene_assets->beetle_material1 == NULL ||
      subscene_assets->beetle2 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle2,
                                  0.6966059f, -0.0053153f, -0.7374136f,
                                  -0.6754205f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle2, 0.0f,
                                  0.0f, 0.0f, 0.0f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->beetle_material3 = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BEETLE2, 0.0f, 0.0f, true);
  subscene_assets->beetle3 = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->beetle_material2, GFX_MODEL_BEETLE2);
  if (subscene_assets->beetle_material1 == NULL ||
      subscene_assets->beetle3 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle3, 0.1074575f,
                        -0.9937154f, -0.1087305f, 0.0265969f,
                        MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle3, 0.0f,
                                  0.0f, 0.0f, 0.0f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->camera = e3d_Camera_CreateCamera(
      engine_ctx, -1.5716472f, -1.5573962f, 21.4340439f, -1.5983772f,
      -1.6728553f, 20.4410915f, 0.0f, 1.0f, 0.0f);
  if (subscene_assets->camera == NULL) {
    return false;
  }
  if (e3d_Camera_AddTransformation(engine_ctx, subscene_assets->camera, 0.0f,
                                   -0.152247f, 0.209179f, 0.41079f,
                                   CAMERA_TRANSFORM_TRANSLATE) == NULL) {
    return false;
  }

  e3d_Renderer_SetLight(engine_ctx, assets->light);
  e3d_Renderer_SetCamera(engine_ctx, subscene_assets->camera);
  return true;
}

static void
unload_assets_subscene2(e3d_EngineContext *engine_ctx,
                        subscene2Scene12Subscene2Assets *subscene_assets) {
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->beetle1);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->beetle2);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->beetle3);

  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->beetle_material1);
  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->beetle_material2);
  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->beetle_material3);

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

static void render_scene_frame(e3d_EngineContext *engine_ctx,
                               DemoSceneContext *scene_ctx,
                               SpaceSceneAssets *assets,
                               subscene2Scene12Subscene2Assets *subscene_assets,
                               uint32_t subscene_frame) {
  if (beetle1_anim.values_count > 0u) {
    const ModelAnimationValue *beetle1_anim_frame =
        model_animation_get_value(&beetle1_anim, subscene_frame);
    if (beetle1_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx, subscene_assets->beetle1,
                                    beetle1_anim_frame->w, beetle1_anim_frame->x,
                                    beetle1_anim_frame->y, beetle1_anim_frame->z,
                                    BEETLE1_ANIMATION_TRANSFORM_INDEX);
    }
  }
  
  if (beetle2_anim.values_count > 0u) {
    const ModelAnimationValue *beetle2_anim_frame =
        model_animation_get_value(&beetle2_anim, subscene_frame-5);
    if (beetle2_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx, subscene_assets->beetle2,
                                    beetle2_anim_frame->w, beetle2_anim_frame->x,
                                    beetle2_anim_frame->y, beetle2_anim_frame->z,
                                    BEETLE1_ANIMATION_TRANSFORM_INDEX);
    }
  }
  
  if (beetle3_anim.values_count > 0u) {
    const ModelAnimationValue *beetle3_anim_frame =
        model_animation_get_value(&beetle3_anim, subscene_frame-60);
    if (beetle3_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx, subscene_assets->beetle3,
                                    beetle3_anim_frame->w, beetle3_anim_frame->x,
                                    beetle3_anim_frame->y, beetle3_anim_frame->z,
                                    BEETLE1_ANIMATION_TRANSFORM_INDEX);
    }
  }

  e3d_Renderer_CleanScene(engine_ctx);
  e3d_Renderer_AddModelToScene(engine_ctx, assets->earth);
  // e3d_Renderer_AddModelToScene(engine_ctx, assets->moon);
  // e3d_Renderer_AddModelToScene(engine_ctx, assets->bug);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->beetle1);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->beetle2);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->beetle3);
  for (uint32_t i = 0u; i < sizeof(starField) / sizeof(starField[0]); i++) {
    e3d_Renderer_AddPointToScene(engine_ctx, &starField[i]);
  }
  e3d_Renderer_RenderScene(engine_ctx);
  end_scene_frame(scene_ctx);
  demo_end_frame();
}

void scene_12_subscene2_run_scene(DemoContext *demo_ctx,
                                  DemoSceneContext *scene_ctx,
                                  SpaceSceneAssets *assets) {
  if (demo_ctx == NULL || demo_ctx->engine_ctx == NULL || scene_ctx == NULL ||
      assets == NULL) {
    return;
  }

  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  subscene2Scene12Subscene2Assets subscene_assets = {0};
  if (!load_assets_subscene2(engine_ctx, assets, &subscene_assets)) {
    unload_assets_subscene2(engine_ctx, &subscene_assets);
    return;
  }

  if (scene_ctx->scene_frame == 0u) {
    scene_ctx->scene_start_time_ms = demo_platform_time_ms();
  }

  for (uint32_t subscene_frame = 0u;
       subscene_frame < SUBSCENE2_FRAMES && scene_should_continue(scene_ctx);
       subscene_frame++) {
    demo_begin_frame();
    render_scene_frame(engine_ctx, scene_ctx, assets, &subscene_assets,
                       subscene_frame);
  }

  unload_assets_subscene2(engine_ctx, &subscene_assets);
}
