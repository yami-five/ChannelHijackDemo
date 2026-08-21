#include "scene_12_subscene7.h"

#include "../../demo_context.h"
#include "../../demo_scene.h"
#include "../scene_12_animations.inc"
#include "../scene_12_star_field.inc"
#include "engineApi.h"
#include "mesh.h"
#include "model_animation.h"
#include "storage/gfx_indices.h"
#include <stdbool.h>

#define EARTH_ANIMATION_ROTATION_INDEX 1u
#define ANT_ANIMATION_ROTATION_INDEX 1u
#define ANT_ANIMATION_TRANSLATION_INDEX 2u
#define MOON_ANIMATION_ROTATION_INDEX 2u
#define MOON_ANIMATION_TRANSLATION_INDEX 3u
#define WASP_PARTS_ANIM_INDEX 1u

typedef struct {
  e3d_Mesh *ant;
  e3d_Material *ant_material;
  e3d_Mesh *wasp_body;
  e3d_Mesh *wasp_planet;
  e3d_Mesh *wasp_wing_left;
  e3d_Mesh *wasp_wing_right;
  e3d_Camera *camera;
} Scene12Subscene7Assets;

static bool load_assets_subscene7(e3d_EngineContext *engine_ctx,
                                  SpaceSceneAssets *assets,
                                  Scene12Subscene7Assets *subscene_assets) {
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

  subscene_assets->wasp_body = e3d_Mesh_CreateMesh(
      engine_ctx, assets->bug_material, GFX_MODEL_WASP_BODY);
  if (subscene_assets->wasp_body == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_body, 0.0f,
                                  1.5f, 1.5f, 1.5f, MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_body, 0.1942613f,
                        0.0000001f, 1.0f, -0.0000001f,
                        MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_body, 0.0f,
                        5.7514029f, 0.0f, 6.8739777f,
                        MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->wasp_planet = e3d_Mesh_CreateMesh(
      engine_ctx, assets->bug_material, GFX_MODEL_WASP_PLANET);
  if (subscene_assets->wasp_planet == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_planet, 0.0f,
                                  1.5f, 1.5f, 1.5f, MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_planet, 0.5305918f,
                        0.2915291f, 0.9468529f, 0.1359423f,
                        MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_planet, 0.0f,
                        5.5626192f, 3.8773825f, 6.6489959f,
                        MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->wasp_wing_left = e3d_Mesh_CreateMesh(
      engine_ctx, assets->bug_material, GFX_MODEL_WASP_WING_LEFT);
  if (subscene_assets->wasp_wing_left == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_wing_left, 0.0f,
                                  1.5f, 1.5f, 1.5f, MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_wing_left, 0.1942614f,
                        0.0000001f, 1.0f, 0.0f,
                        MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_wing_left, 0.0f,
                        6.6569185f, -0.7998634f, 9.9755697f,
                        MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  
  subscene_assets->wasp_wing_right = e3d_Mesh_CreateMesh(
      engine_ctx, assets->bug_material, GFX_MODEL_WASP_WING_RIGHT);
  if (subscene_assets->wasp_wing_right == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_wing_right, 0.0f,
                                  1.5f, 1.5f, 1.5f, MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_wing_right, 0.1942614f,
                        0.0000001f, 1.0f, 0.0f,
                        MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->wasp_wing_right, 0.0f,
                        8.648634f, -0.7998639f, 8.3043232f,
                        MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  
  subscene_assets->camera = e3d_Camera_CreateCamera(
      engine_ctx, -102.7094193f, 67.0399399f, 89.346962f, -102.024025f,
      66.5933228f, 88.771843f, 0.0f, 1.0f, 0.0f);
  if (subscene_assets->camera == NULL) {
    return false;
  }
  

  e3d_Renderer_SetLight(engine_ctx, assets->light);
  e3d_Renderer_SetCamera(engine_ctx, subscene_assets->camera);
  return true;
}

static void unload_assets_subscene7(e3d_EngineContext *engine_ctx,
                                    Scene12Subscene7Assets *subscene_assets) {
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
                         Scene12Subscene7Assets *subscene_assets,
                         uint32_t subscene_frame) {
  if (ant_end_anim_translate.values_count > 0u) {
    const ModelAnimationValue *ant_end_anim_translate_frame =
        model_animation_get_value(&ant_end_anim_translate, subscene_frame);
    if (ant_end_anim_translate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->ant, ant_end_anim_translate_frame->w,
          ant_end_anim_translate_frame->x, ant_end_anim_translate_frame->y,
          ant_end_anim_translate_frame->z, ANT_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (ant_end_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *ant_end_anim_rotate_frame =
        model_animation_get_value(&ant_end_anim_rotate, subscene_frame);
    if (ant_end_anim_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->ant, ant_end_anim_rotate_frame->w,
          ant_end_anim_rotate_frame->x, ant_end_anim_rotate_frame->y,
          ant_end_anim_rotate_frame->z, ANT_ANIMATION_ROTATION_INDEX);
    }
  }
  if (moon_end_anim_translate.values_count > 0u) {
    const ModelAnimationValue *moon_end_anim_translate_frame =
        model_animation_get_value(&moon_end_anim_translate, subscene_frame);
    if (moon_end_anim_translate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, assets->moon, moon_end_anim_translate_frame->w,
          moon_end_anim_translate_frame->x, moon_end_anim_translate_frame->y,
          moon_end_anim_translate_frame->z, MOON_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (moon_end_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *moon_end_anim_rotate_frame =
        model_animation_get_value(&moon_end_anim_rotate, subscene_frame);
    if (moon_end_anim_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, assets->moon, moon_end_anim_rotate_frame->w,
          moon_end_anim_rotate_frame->x, moon_end_anim_rotate_frame->y,
          moon_end_anim_rotate_frame->z, MOON_ANIMATION_ROTATION_INDEX);
    }
  }
  if (earth_end_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *earth_end_anim_rotate_frame =
        model_animation_get_value(&earth_end_anim_rotate, subscene_frame);
    if (earth_end_anim_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, assets->earth, earth_end_anim_rotate_frame->w,
          earth_end_anim_rotate_frame->x, earth_end_anim_rotate_frame->y,
          earth_end_anim_rotate_frame->z, EARTH_ANIMATION_ROTATION_INDEX);
    }
  }
  if (wasp_planet_rotate.values_count > 0u) {
    const ModelAnimationValue *wasp_planet_rotate_frame =
        model_animation_get_value(&wasp_planet_rotate, subscene_frame);
    if (wasp_planet_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->wasp_planet, wasp_planet_rotate_frame->w,
          wasp_planet_rotate_frame->x, wasp_planet_rotate_frame->y,
          wasp_planet_rotate_frame->z, WASP_PARTS_ANIM_INDEX);
    }
  }
  if (wasp_wing_left_rotate.values_count > 0u) {
    const ModelAnimationValue *wasp_wing_left_rotate_frame =
        model_animation_get_value(&wasp_wing_left_rotate, subscene_frame);
    if (wasp_wing_left_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->wasp_wing_left, wasp_wing_left_rotate_frame->w,
          wasp_wing_left_rotate_frame->x, wasp_wing_left_rotate_frame->y,
          wasp_wing_left_rotate_frame->z, WASP_PARTS_ANIM_INDEX);
    }
  }
  if (wasp_wing_right_rotate.values_count > 0u) {
    const ModelAnimationValue *wasp_wing_right_rotate_frame =
        model_animation_get_value(&wasp_wing_right_rotate, subscene_frame);
    if (wasp_wing_right_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->wasp_wing_right, wasp_wing_right_rotate_frame->w,
          wasp_wing_right_rotate_frame->x, wasp_wing_right_rotate_frame->y,
          wasp_wing_right_rotate_frame->z, WASP_PARTS_ANIM_INDEX);
    }
  }

  e3d_Renderer_CleanScene(engine_ctx);
  // e3d_Renderer_AddModelToScene(engine_ctx, assets->bug);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->wasp_body);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->wasp_planet);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->wasp_wing_left);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->wasp_wing_right);
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

void scene_12_subscene7_run_scene(DemoContext *demo_ctx,
                                  DemoSceneContext *scene_ctx,
                                  SpaceSceneAssets *assets) {
  if (demo_ctx == NULL || demo_ctx->engine_ctx == NULL || scene_ctx == NULL ||
      assets == NULL) {
    return;
  }

  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  Scene12Subscene7Assets subscene_assets = {0};
  if (!load_assets_subscene7(engine_ctx, assets, &subscene_assets)) {
    unload_assets_subscene7(engine_ctx, &subscene_assets);
    return;
  }

  if (scene_ctx->scene_frame == 0u) {
    scene_ctx->scene_start_time_ms = demo_platform_time_ms();
  }

  uint32_t subscene_frame = 0u;
  for (; subscene_frame < 80 && scene_should_continue(scene_ctx);
       subscene_frame++) {
    demo_begin_frame();
    render_scene(engine_ctx, scene_ctx, assets, &subscene_assets,
                 subscene_frame);
  }

  unload_assets_subscene7(engine_ctx, &subscene_assets);
}
