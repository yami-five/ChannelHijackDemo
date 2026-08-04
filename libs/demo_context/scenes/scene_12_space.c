#include "scene_12_space.h"
#include "../demo_context.h"
#include "../demo_scene.h"

#include "engineApi.h"
#include "scene_12_star_field.inc"
#include "storage/gfx_indices.h"

#define CAMERA_PULL_BACK_FRAMES 10u

typedef struct {
  e3d_Mesh *bug;
  e3d_Mesh *earth;
  e3d_Mesh *moon;
  e3d_Mesh *maggot1;
  e3d_Mesh *maggot2;
  e3d_Mesh *maggot3;
  e3d_Mesh *satelite;
  e3d_Light *light;
  e3d_Camera *camera1;
  e3d_Camera *camera;
} SpaceSceneAssets;

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

  e3d_Material *bug_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BUG_SHIP, 0.0f, 0.0f, true);
  assets.bug =
      e3d_Mesh_CreateMesh(engine_ctx, bug_material, GFX_MODEL_BUG_SHIP);
  if (assets.bug == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.bug, 0.0f, 0.8f, 0.8f, 0.8f,
                               MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.bug, 0.0f, 0.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.bug, 0.0f, 5.0f, -1.0f, -5.0f,
                               MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  e3d_Material *earth_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_EARTH, 0.0f, 0.0f, false);
  assets.earth =
      e3d_Mesh_CreateMesh(engine_ctx, earth_material, GFX_MODEL_SPHERE);
  if (assets.earth == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.earth, 0.0f, 0.8f, 0.8f, 0.8f,
                               MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.earth, 0.0f, 0.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.earth, 0.0f, 0.0f, -2.0f,
                               0.0f, MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  e3d_Material *moon_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_MOON, 0.0f, 0.0f, false);
  assets.moon =
      e3d_Mesh_CreateMesh(engine_ctx, moon_material, GFX_MODEL_SPHERE);
  if (assets.moon == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, 0.8f, 0.8f, 0.8f,
                               MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, 0.273f, 0.273f,
                               0.273f, MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, 0.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, -7.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_TRANSLATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, 0.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.moon, 0.0f, 0.0f, -2.0f, 0.0f,
                               MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  e3d_Material *maggot_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_MAGGOT, 0.0f, 0.0f, false);
  assets.maggot1 =
      e3d_Mesh_CreateMesh(engine_ctx, maggot_material, GFX_MODEL_MAGGOT);
  if (assets.maggot1 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.maggot1, 0.3742155f,
                               0.0957482f, -0.9931533f, 0.0669243f,
                               MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.maggot1, 0.0f, -1.8568504f,
                               0.120001f, 5.1299009f,
                               MODEL_TRANSFORM_TRANSLATE) /*||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.maggot1, 0.350094f,
                               0.9980992f, -0.043578f, -0.043578f,
                               MODEL_TRANSFORM_ROTATE)*/) {
    return false;
  }
  assets.maggot2 =
      e3d_Mesh_CreateMesh(engine_ctx, maggot_material, GFX_MODEL_MAGGOT);
  if (assets.maggot2 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.maggot2, 0.6292018f,
                               -0.2800575f, 0.1509732f, 0.9480374f,
                               MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.maggot2, 0.0f, -1.8095753f,
                               0.0198577f, 5.1291432f,
                               MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  assets.maggot3 =
      e3d_Mesh_CreateMesh(engine_ctx, maggot_material, GFX_MODEL_MAGGOT);
  if (assets.maggot3 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.maggot3, 0.3488846f,
                               -0.53497f, 0.5282924f, 0.6593286f,
                               MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.maggot3, 0.0f, -1.7636162f,
                               0.118544f, 5.094625f,
                               MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  e3d_Material *satelite_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_SATELLITE, 0.0f, 0.0f, false);
  assets.satelite =
      e3d_Mesh_CreateMesh(engine_ctx, satelite_material, GFX_MODEL_SATELLITE);
  if (assets.satelite == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.satelite, 0.5692173f, 0.0f,
                               1.0f, 0.0f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, assets.satelite, 0.0f, -1.8313992f,
                               0.062086f, 5.086524f,
                               MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  assets.light = e3d_Light_CreateDirectionalLight(engine_ctx, 10.0f, -3.0f,
                                                  -15.0f, 15.0f, 0xffff);

  assets.camera1 =
      e3d_Camera_CreateCamera(engine_ctx, -2.1803f, 0.54145f, 6.0279f, -1.8314f,
                              0.062086f, 5.0865f, 0.0f, 1.0f, 0.0f);
  assets.camera = e3d_Camera_CreateCamera(engine_ctx, 0.0f, 75.0f, 100.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  if (assets.light == NULL || assets.camera1 == NULL || assets.camera == NULL) {
    return false;
  }
  if (e3d_Camera_AddTransformation(engine_ctx, assets.camera1, 0.0f,
                                   -0.152247f, 0.209179f, 0.41079f,
                                   CAMERA_TRANSFORM_TRANSLATE) == NULL) {
    return false;
  }

  e3d_Renderer_SetLight(engine_ctx, assets.light);
  e3d_Renderer_SetCamera(engine_ctx, assets.camera1);
  return true;
}

static void end_scene_frame(void) {
  scene_ctx.scene_frame++;
  scene_ctx.scene_elapsed_ms =
      demo_platform_time_ms() - scene_ctx.scene_start_time_ms;
}

static bool scene_should_continue(void) {
  return demo_is_running() &&
         (scene_ctx.scene_duration == 0u ||
          scene_ctx.scene_frame < scene_ctx.scene_duration);
}

static void render_scene_frame(e3d_EngineContext *engine_ctx) {
  e3d_Renderer_CleanScene(engine_ctx);
  e3d_Renderer_AddModelToScene(engine_ctx, assets.earth);
  e3d_Renderer_AddModelToScene(engine_ctx, assets.moon);
  // e3d_Renderer_AddModelToScene(engine_ctx, assets.bug);
  e3d_Renderer_AddModelToScene(engine_ctx, assets.maggot1);
  e3d_Renderer_AddModelToScene(engine_ctx, assets.maggot2);
  e3d_Renderer_AddModelToScene(engine_ctx, assets.maggot3);
  e3d_Renderer_AddModelToScene(engine_ctx, assets.satelite);
  for (uint32_t i = 0u; i < sizeof(starField) / sizeof(starField[0]); i++) {
    e3d_Renderer_AddPointToScene(engine_ctx, &starField[i]);
  }
  e3d_Renderer_RenderScene(engine_ctx);
  end_scene_frame();
  demo_end_frame();
}

static void run_scene(DemoContext *demo_ctx) {
  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  if (!load_assets(engine_ctx)) {
    return;
  }

  if (scene_ctx.scene_frame == 0u) {
    scene_ctx.scene_start_time_ms = demo_platform_time_ms();
  }

  for (uint32_t camera_frame = scene_ctx.scene_frame;
       camera_frame < CAMERA_PULL_BACK_FRAMES && scene_should_continue();
       camera_frame++) {
    demo_begin_frame();
    render_scene_frame(engine_ctx);
  }

  for (uint32_t camera_frame = scene_ctx.scene_frame;
       camera_frame < CAMERA_PULL_BACK_FRAMES+10 && scene_should_continue();
       camera_frame++) {
    demo_begin_frame();
    e3d_Camera_UpdateCamera(engine_ctx, assets.camera1);
    render_scene_frame(engine_ctx);
  }

  for (; scene_should_continue();) {
    demo_begin_frame();
    render_scene_frame(engine_ctx);
  }
}

DemoSceneContext *scene_12_space_get_context(void) { return &scene_ctx; }
