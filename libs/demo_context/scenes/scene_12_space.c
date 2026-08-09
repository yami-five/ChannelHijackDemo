#include "scene_12_space.h"

#include "../demo_context.h"
#include "../demo_scene.h"
#include "engineApi.h"
#include "scene_12_subscenes/scene_12_subscene1.h"
#include "scene_12_subscenes/scene_12_subscene2.h"
#include "scene_12_subscenes/scene_12_subscene3.h"
#include "scene_12_subscenes/scene_12_subscene4.h"
#include "scene_12_subscenes/scene_12_subscene5.h"
#include "scene_12_subscenes/scene_12_subscene6.h"
#include "scene_12_subscenes/scene_12_subscene7.h"
#include "storage/gfx_indices.h"

#define BUG_SCALE 1.5f

static void run_scene(DemoContext *demo_ctx);

static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 0u,
    .run_scene = run_scene,
};

static SpaceSceneAssets assets;

static bool load_assets(e3d_EngineContext *engine_ctx) {
  if (assets.camera != NULL) {
    return true;
  }

  assets.bug_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BUG_SHIP, 0.0f, 0.0f, true);
  assets.bug =
      e3d_Mesh_CreateMesh(engine_ctx, assets.bug_material, GFX_MODEL_BUG_SHIP);
  if (assets.bug_material == NULL || assets.bug == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.bug, 0.0f, BUG_SCALE, BUG_SCALE,
                                  BUG_SCALE, MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.bug, 0.0f, 0.0f, 0.0f,
                                  0.0f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.bug, 0.0f, 0.0f, 0.0f,
                                  0.0f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.bug, 0.0f, 0.0f, 0.0f,
                                  0.0f, MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  assets.earth_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_EARTH, 0.0f, 0.0f, false);
  assets.earth =
      e3d_Mesh_CreateMesh(engine_ctx, assets.earth_material, GFX_MODEL_SPHERE);
  if (assets.earth_material == NULL || assets.earth == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.earth, 0.0f, 0.8f, 0.8f,
                                  0.8f, MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.earth, 0.0f, 0.0f, 0.0f,
                                  0.0f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.earth, 0.0f, 0.0f, -2.0f,
                                  0.0f, MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  assets.moon_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_MOON, 0.0f, 0.0f, false);
  assets.moon =
      e3d_Mesh_CreateMesh(engine_ctx, assets.moon_material, GFX_MODEL_SPHERE);
  if (assets.moon_material == NULL || assets.moon == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, 0.8f, 0.8f,
                                  0.8f, MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, 0.273f, 0.273f,
                                  0.273f, MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.6993408f, 0.0f,
                                  -1.0f, -0.0000001f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, -7.0f, -2.0f,
                                  0.0f,
                                  MODEL_TRANSFORM_TRANSLATE)) { //||
    // !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, 0.0f, 0.0f,
    //                             0.0f, MODEL_TRANSFORM_ROTATE) ||
    // !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, 0.0f, -2.0f,
    //                             0.0f, MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  assets.light = e3d_Light_CreateDirectionalLight(engine_ctx, 10.0f, -3.0f,
                                                  -15.0f, 15.0f, 0xffff);
  assets.camera = e3d_Camera_CreateCamera(engine_ctx, 0.0f, 75.0f, 100.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  if (assets.light == NULL || assets.camera == NULL) {
    return false;
  }

  return true;
}

static void unload_assets(e3d_EngineContext *engine_ctx) {
  e3d_Mesh_DeleteMesh(engine_ctx, &assets.bug);
  e3d_Mesh_DeleteMesh(engine_ctx, &assets.earth);
  e3d_Mesh_DeleteMesh(engine_ctx, &assets.moon);

  e3d_Material_DeleteMat(engine_ctx, &assets.bug_material);
  e3d_Material_DeleteMat(engine_ctx, &assets.earth_material);
  e3d_Material_DeleteMat(engine_ctx, &assets.moon_material);

  e3d_Camera_DeleteCamera(engine_ctx, &assets.camera);
  e3d_Light_DeleteLight(engine_ctx, &assets.light);
}

static void run_scene(DemoContext *demo_ctx) {
  if (demo_ctx == NULL || demo_ctx->engine_ctx == NULL) {
    return;
  }

  if (!load_assets(demo_ctx->engine_ctx)) {
    unload_assets(demo_ctx->engine_ctx);
    return;
  }

  scene_12_subscene1_run_scene(demo_ctx, &scene_ctx, &assets);
  scene_12_subscene2_run_scene(demo_ctx, &scene_ctx, &assets);
  scene_12_subscene3_run_scene(demo_ctx, &scene_ctx, &assets);
  scene_12_subscene4_run_scene(demo_ctx, &scene_ctx, &assets);
  scene_12_subscene5_run_scene(demo_ctx, &scene_ctx, &assets);
  scene_12_subscene6_run_scene(demo_ctx, &scene_ctx, &assets);
  scene_12_subscene7_run_scene(demo_ctx, &scene_ctx, &assets);
  unload_assets(demo_ctx->engine_ctx);
}

DemoSceneContext *scene_12_space_get_context(void) { return &scene_ctx; }
