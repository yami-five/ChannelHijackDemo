#include "scene_12_space.h"
#include "../demo_context.h"
#include "../demo_scene.h"

#include "engineApi.h"
#include "mesh.h"
#include "scene_12_star_field.inc"
#include "storage/gfx_indices.h"

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

static bool add_mesh_transformation(e3d_EngineContext *engine_ctx,
                                    e3d_Mesh *mesh, float w, float x, float y,
                                    float z, e3d_ModelTransformType type) {
  mesh->transformations =
      e3d_Mesh_AddTransformation(engine_ctx, mesh->transformations,
                                 &mesh->transformationsNum, w, x, y, z, type);
  return mesh->transformations != NULL;
}

static bool load_assets(e3d_EngineContext *engine_ctx) {
  if (assets.camera != NULL) {
    return true;
  }

  e3d_Material *bug_material =
      e3d_Material_CreateTexturedMat(engine_ctx, GFX_TEXTURE_BUG_SHIP, 0.0f,
                                     0.0f, true);
  assets.bug =
      e3d_Mesh_CreateMesh(engine_ctx, bug_material, GFX_MODEL_BUG_SHIP);
  if (assets.bug == NULL ||
      !add_mesh_transformation(engine_ctx, assets.bug, 0.0f, 0.8f, 0.8f, 0.8f,
                               MODEL_TRANSFORM_SCALE) ||
      !add_mesh_transformation(engine_ctx, assets.bug, 0.0f, 0.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_ROTATE) ||
      !add_mesh_transformation(engine_ctx, assets.bug, 0.0f, 5.0f, -1.0f, -5.0f,
                               MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  e3d_Material *earth_material =
      e3d_Material_CreateTexturedMat(engine_ctx, GFX_TEXTURE_EARTH, 0.0f, 0.0f,
                                     false);
  assets.earth =
      e3d_Mesh_CreateMesh(engine_ctx, earth_material, GFX_MODEL_SPHERE);
  if (assets.earth == NULL ||
      !add_mesh_transformation(engine_ctx, assets.earth, 0.0f, 0.8f, 0.8f, 0.8f,
                               MODEL_TRANSFORM_SCALE) ||
      !add_mesh_transformation(engine_ctx, assets.earth, 0.0f, 0.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_ROTATE) ||
      !add_mesh_transformation(engine_ctx, assets.earth, 0.0f, 0.0f, -2.0f,
                               0.0f, MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  e3d_Material *moon_material =
      e3d_Material_CreateTexturedMat(engine_ctx, GFX_TEXTURE_MOON, 0.0f, 0.0f,
                                     false);
  assets.moon =
      e3d_Mesh_CreateMesh(engine_ctx, moon_material, GFX_MODEL_SPHERE);
  if (assets.moon == NULL ||
      !add_mesh_transformation(engine_ctx, assets.moon, 0.0f, 0.8f, 0.8f, 0.8f,
                               MODEL_TRANSFORM_SCALE) ||
      !add_mesh_transformation(engine_ctx, assets.moon, 0.0f, 0.273f, 0.273f,
                               0.273f, MODEL_TRANSFORM_SCALE) ||
      !add_mesh_transformation(engine_ctx, assets.moon, 0.0f, 0.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_ROTATE) ||
      !add_mesh_transformation(engine_ctx, assets.moon, 0.0f, -7.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_TRANSLATE) ||
      !add_mesh_transformation(engine_ctx, assets.moon, 0.0f, 0.0f, 0.0f, 0.0f,
                               MODEL_TRANSFORM_ROTATE) ||
      !add_mesh_transformation(engine_ctx, assets.moon, 0.0f, 0.0f, -2.0f, 0.0f,
                               MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  e3d_Material *maggot_material =
      e3d_Material_CreateTexturedMat(engine_ctx, GFX_TEXTURE_MAGGOT, 0.0f, 0.0f,
                                     false);
  assets.maggot1 =
      e3d_Mesh_CreateMesh(engine_ctx, maggot_material, GFX_MODEL_MAGGOT);
  if (assets.maggot1 == NULL ||
      !add_mesh_transformation(engine_ctx, assets.maggot1, 0.3251409f,
                               0.4939093f, 0.5721112f, -0.6547843f,
                               MODEL_TRANSFORM_ROTATE) ||
      !add_mesh_transformation(engine_ctx, assets.maggot1, 0.0f, -1.8569f,
                               0.12f, 5.1299f, MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  e3d_Material *satelite_material =
      e3d_Material_CreateTexturedMat(engine_ctx, GFX_TEXTURE_SATELLITE, 0.0f,
                                     0.0f, false);
  assets.satelite =
      e3d_Mesh_CreateMesh(engine_ctx, satelite_material, GFX_MODEL_SATELLITE);
  if (assets.satelite == NULL ||
      !add_mesh_transformation(engine_ctx, assets.satelite, 0.4346947f,
                               0.2080906f, -0.6916279f, 0.6916279f,
                               MODEL_TRANSFORM_ROTATE) ||
      !add_mesh_transformation(engine_ctx, assets.satelite, 0.0f, -1.8314f,
                               0.062086f, 5.0865f, MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  assets.light = e3d_Light_CreateDirectionalLight(engine_ctx, 10.0f, -3.0f,
                                                  -15.0f, 15.0f, 0xffff);

  assets.camera1 =
      e3d_Camera_CreateCamera(engine_ctx, -2.1803f, 0.54145f, 6.0279f, -1.8314f,
                              0.062086f, 5.0865f, 0.0f, 1.0f, 0.0f);
  assets.camera = e3d_Camera_CreateCamera(engine_ctx, 0.0f, 75.0f, 100.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  if (assets.light == NULL || assets.camera == NULL) {
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

static void run_scene(DemoContext *demo_ctx) {
  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  if (!load_assets(engine_ctx)) {
    return;
  }

  while (demo_is_running() &&
         (scene_ctx.scene_duration == 0u ||
          scene_ctx.scene_frame < scene_ctx.scene_duration)) {
    if (scene_ctx.scene_frame == 0u) {
      scene_ctx.scene_start_time_ms = demo_platform_time_ms();
    }

    demo_begin_frame();
    e3d_Renderer_CleanScene(engine_ctx);
    e3d_Renderer_AddModelToScene(engine_ctx, assets.earth);
    e3d_Renderer_AddModelToScene(engine_ctx, assets.moon);
    // e3d_Renderer_AddModelToScene(engine_ctx, assets.bug);
    e3d_Renderer_AddModelToScene(engine_ctx, assets.maggot1);
    e3d_Renderer_AddModelToScene(engine_ctx, assets.satelite);
    for (uint32_t i = 0u; i < sizeof(starField) / sizeof(starField[0]); i++) {
      e3d_Renderer_AddPointToScene(engine_ctx, &starField[i]);
    }
    e3d_Renderer_RenderScene(engine_ctx);
    end_scene_frame();
    demo_end_frame();
  }
}

DemoSceneContext *scene_12_space_get_context(void) { return &scene_ctx; }
