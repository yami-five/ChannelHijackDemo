#include "scene_12_subscene1.h"

#include "../../demo_context.h"
#include "../../demo_scene.h"
#include "engineApi.h"
#include "model_animation.h"
#include "../scene_12_animations.inc"
#include "../scene_12_star_field.inc"
#include "storage/gfx_indices.h"

#define CAMERA_ZOOM_IN_FRAMES 10u
#define MAGGOT_ANIMATION_TRANSFORM_INDEX 0u

typedef struct {
  e3d_Mesh *maggot1;
  e3d_Mesh *maggot2;
  e3d_Mesh *maggot3;
  e3d_Material *maggot_material;
  e3d_Material *satelite_material;
  e3d_Mesh *satelite;
  e3d_Camera *camera;
} Scene12Subscene1Assets;

static bool load_assets_subscene1(e3d_EngineContext *engine_ctx,
                                  SpaceSceneAssets *assets,
                                  Scene12Subscene1Assets *subscene_assets) {
  if (subscene_assets->camera != NULL) {
    return true;
  }

  subscene_assets->maggot_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_MAGGOT, 0.0f, 0.0f, false);
  subscene_assets->maggot1 = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->maggot_material, GFX_MODEL_MAGGOT);
  if (subscene_assets->maggot_material == NULL ||
      subscene_assets->maggot1 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->maggot1, 0.0f,
                                  0.0f, 1.0f, 0.0f,
                                  MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->maggot1,
                                  0.3742155f,
                                  0.0957482f, -0.9931533f, 0.0669243f,
                                  MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->maggot1, 0.0f,
                                  -1.8568504f, 0.120001f, 5.1299009f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->maggot2 = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->maggot_material, GFX_MODEL_MAGGOT);
  if (subscene_assets->maggot2 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->maggot2, 0.0f,
                                  0.0f, 1.0f, 0.0f,
                                  MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->maggot2,
                                  0.6292018f,
                                  -0.2800575f, 0.1509732f, 0.9480374f,
                                  MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->maggot2, 0.0f,
                                  -1.8095753f, 0.0198577f, 5.1291432f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->maggot3 = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->maggot_material, GFX_MODEL_MAGGOT);
  if (subscene_assets->maggot3 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->maggot3, 0.0f,
                                  0.0f, 1.0f, 0.0f,
                                  MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->maggot3,
                                  0.3488846f,
                                  -0.53497f, 0.5282924f, 0.6593286f,
                                  MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->maggot3, 0.0f,
                                  -1.7636162f, 0.118544f, 5.094625f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->satelite_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_SATELLITE, 0.0f, 0.0f, false);
  subscene_assets->satelite = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->satelite_material, GFX_MODEL_SATELLITE);
  if (subscene_assets->satelite_material == NULL ||
      subscene_assets->satelite == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->satelite,
                                  0.5692173f, 0.0f, 1.0f, 0.0f,
                                  MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->satelite, 0.0f,
                                  -1.8313992f, 0.062086f, 5.086524f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->camera = e3d_Camera_CreateCamera(
      engine_ctx, -2.1803f, 0.54145f, 6.0279f, -1.8314f, 0.062086f, 5.0865f,
      0.0f, 1.0f, 0.0f);
  if (subscene_assets->camera == NULL) {
    return false;
  }
  // if (e3d_Camera_AddTransformation(engine_ctx, subscene_assets->camera, 0.0f,
  //                                  -0.152247f, 0.209179f, 0.41079f,
  //                                  CAMERA_TRANSFORM_TRANSLATE) == NULL) {
  //   return false;
  // }

  e3d_Renderer_SetLight(engine_ctx, assets->light);
  e3d_Renderer_SetCamera(engine_ctx, subscene_assets->camera);
  return true;
}

static void unload_assets_subscene1(e3d_EngineContext *engine_ctx,
                                    Scene12Subscene1Assets *subscene_assets) {
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->maggot1);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->maggot2);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->maggot3);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->satelite);

  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->maggot_material);
  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->satelite_material);

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
                               Scene12Subscene1Assets *subscene_assets) {
  if (maggot_anim.values_count > 0u) {
    const ModelAnimationValue *maggot_anim_frame = model_animation_get_value(
        &maggot_anim, scene_ctx->scene_frame % maggot_anim.values_count);
    if (maggot_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->maggot1, maggot_anim_frame->w,
          maggot_anim_frame->x, maggot_anim_frame->y, maggot_anim_frame->z,
          MAGGOT_ANIMATION_TRANSFORM_INDEX);
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->maggot2, maggot_anim_frame->w,
          maggot_anim_frame->x, maggot_anim_frame->y, maggot_anim_frame->z,
          MAGGOT_ANIMATION_TRANSFORM_INDEX);
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->maggot3, maggot_anim_frame->w,
          maggot_anim_frame->x, maggot_anim_frame->y, maggot_anim_frame->z,
          MAGGOT_ANIMATION_TRANSFORM_INDEX);
    }
  }if (maggot_anim.values_count > 0u) {
    const ModelAnimationValue *camera_maggots_anim_frame = model_animation_get_value(
        &camera_maggots_anim, scene_ctx->scene_frame % camera_maggots_anim.values_count);
    if (camera_maggots_anim_frame != NULL) {
      e3d_Camera_SetPos(
          engine_ctx, subscene_assets->camera,
          camera_maggots_anim_frame->x, camera_maggots_anim_frame->y, camera_maggots_anim_frame->z);
    }
  }

  e3d_Renderer_CleanScene(engine_ctx);
  e3d_Renderer_AddModelToScene(engine_ctx, assets->earth);
  // e3d_Renderer_AddModelToScene(engine_ctx, assets->moon);
  // e3d_Renderer_AddModelToScene(engine_ctx, assets->bug);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->maggot1);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->maggot2);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->maggot3);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->satelite);
  for (uint32_t i = 0u; i < sizeof(starField) / sizeof(starField[0]); i++) {
    e3d_Renderer_AddPointToScene(engine_ctx, &starField[i]);
  }
  e3d_Renderer_RenderScene(engine_ctx);
  end_scene_frame(scene_ctx);
  demo_end_frame();
}

void scene_12_subscene1_run_scene(DemoContext *demo_ctx,
                                  DemoSceneContext *scene_ctx,
                                  SpaceSceneAssets *assets) {
  if (demo_ctx == NULL || demo_ctx->engine_ctx == NULL || scene_ctx == NULL ||
      assets == NULL) {
    return;
  }

  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  Scene12Subscene1Assets subscene_assets = {0};
  if (!load_assets_subscene1(engine_ctx, assets, &subscene_assets)) {
    unload_assets_subscene1(engine_ctx, &subscene_assets);
    return;
  }

  if (scene_ctx->scene_frame == 0u) {
    scene_ctx->scene_start_time_ms = demo_platform_time_ms();
  }

  for (uint32_t camera_frame = scene_ctx->scene_frame;
       camera_frame < CAMERA_ZOOM_IN_FRAMES &&
       scene_should_continue(scene_ctx);
       camera_frame++) {
    demo_begin_frame();
    render_scene_frame(engine_ctx, scene_ctx, assets, &subscene_assets);
  }

  for (uint32_t camera_frame = scene_ctx->scene_frame;
       camera_frame < CAMERA_ZOOM_IN_FRAMES + 10 &&
       scene_should_continue(scene_ctx);
       camera_frame++) {
    demo_begin_frame();
    e3d_Camera_UpdateCamera(engine_ctx, subscene_assets.camera);
    render_scene_frame(engine_ctx, scene_ctx, assets, &subscene_assets);
  }

  for (uint32_t camera_frame = scene_ctx->scene_frame;
       camera_frame < CAMERA_ZOOM_IN_FRAMES + 20 &&
       scene_should_continue(scene_ctx);
       camera_frame++) {
    demo_begin_frame();
    render_scene_frame(engine_ctx, scene_ctx, assets, &subscene_assets);
  }

  unload_assets_subscene1(engine_ctx, &subscene_assets);
}
