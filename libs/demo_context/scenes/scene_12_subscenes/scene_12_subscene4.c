#include "scene_12_subscene4.h"

#include "../../demo_context.h"
#include "../../demo_scene.h"
#include "../scene_12_animations.inc"
#include "../scene_12_star_field.inc"
#include "engineApi.h"
#include "mesh.h"
#include "model_animation.h"
#include "storage/gfx_indices.h"

#define BEETLE_ANIMATION_ROTATION_INDEX 1u
#define BEETLE_ANIMATION_TRANSLATION_INDEX 2u
#define BIG_BEETLE_ANIMATION_ROTATION_INDEX 0u
#define BIG_BEETLE_ANIMATION_TRANSLATION_INDEX 1u
#define BEETLE_SCALE 0.5f

typedef struct {
  e3d_Mesh *big_beetle_part1;
  e3d_Mesh *big_beetle_part2;
  e3d_Mesh *big_beetle_part3;
  e3d_Mesh *big_beetle_part4;
  e3d_Mesh *big_beetle_part5;
  e3d_Mesh *big_beetle_part6;
  e3d_Mesh *big_beetle_part7;
  e3d_Mesh *big_beetle_part8;
  e3d_Mesh *big_beetle_part9;
  e3d_Mesh *big_beetle_part10;
  e3d_Material *big_beetle_material;
  e3d_Mesh *beetle1;
  e3d_Mesh *beetle2;
  e3d_Mesh *beetle3;
  e3d_Material *beetle_material1;
  e3d_Material *beetle_material2;
  e3d_Material *beetle_material3;
  e3d_Mesh *ant;
  e3d_Material *ant_material;
  e3d_Camera *camera;
} Scene12Subscene4Assets;

static bool load_assets_subscene4(e3d_EngineContext *engine_ctx,
                                  SpaceSceneAssets *assets,
                                  Scene12Subscene4Assets *subscene_assets) {
  if (subscene_assets->camera != NULL) {
    return true;
  }
  subscene_assets->ant_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_ANT, 0.0f, 0.0f, true);
  subscene_assets->ant = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->ant_material, GFX_MODEL_ANT);
  if (subscene_assets->ant_material == NULL || subscene_assets->ant == NULL ||
          !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->ant, 0.0f,
                                      0.2127071f, 0.2127071f, 0.2127071f,
                                      MODEL_TRANSFORM_SCALE) ||
          !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->ant,
                                      0.6993408f, 0.0f, -1.0f, 0.0f,
                                      MODEL_TRANSFORM_ROTATE) ||
          !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->ant, 0.0f,
                                      -7.0f, -2.0f, 0.0f,
                                      MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->beetle_material1 = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BEETLE1, 0.0f, 0.0f, true);
  subscene_assets->beetle1 = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->beetle_material1, GFX_MODEL_BEETLE1);
  if (subscene_assets->beetle_material1 == NULL ||
      subscene_assets->beetle1 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle1, 0.0f,
                                  BEETLE_SCALE, BEETLE_SCALE, BEETLE_SCALE,
                                  MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle1,
                                  0.4662272f, -0.5773503f, -0.5773503f,
                                  -0.5773503f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle1, 0.0f,
                                  -3.4780421f, -3.1775231f, 5.8804655f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->beetle_material2 = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BEETLE2, 0.0f, 0.0f, true);
  subscene_assets->beetle2 = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->beetle_material2, GFX_MODEL_BEETLE2);
  if (subscene_assets->beetle_material2 == NULL ||
      subscene_assets->beetle2 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle2, 0.0f,
                                  BEETLE_SCALE, BEETLE_SCALE, BEETLE_SCALE,
                                  MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle2,
                                  0.4662272f, -0.5773503f, -0.5773503f,
                                  -0.5773503f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle2, 0.0f,
                                  -3.4780421f, -3.1775231f, 5.8804655f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->beetle_material3 = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BEETLE3, 0.0f, 0.0f, true);
  subscene_assets->beetle3 = e3d_Mesh_CreateMesh(
      engine_ctx, subscene_assets->beetle_material3, GFX_MODEL_BEETLE3);
  if (subscene_assets->beetle_material3 == NULL ||
      subscene_assets->beetle3 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle3, 0.0f,
                                  BEETLE_SCALE, BEETLE_SCALE, BEETLE_SCALE,
                                  MODEL_TRANSFORM_SCALE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle3,
                                  0.4662272f, -0.5773503f, -0.5773503f,
                                  -0.5773503f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->beetle3, 0.0f,
                                  -3.4780421f, -3.1775231f, 5.8804655f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }

  subscene_assets->big_beetle_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BIG_BEETLE, 0.0f, 0.0f, false);
  subscene_assets->big_beetle_part1 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART1);
  if (subscene_assets->big_beetle_material == NULL ||
      subscene_assets->big_beetle_part1 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part1,
                                  0.4662272f, -0.5773503f, -0.5773503f,
                                  -0.5773503f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part1,
                                  0.0f, -3.4780421f, -3.1775231f, 5.8804655f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->big_beetle_part2 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART2);
  if (subscene_assets->big_beetle_part2 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part2,
                                  0.4444874f, -0.6444005f, -0.5407162f,
                                  -0.5407162f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part2,
                                  0.0f, -2.9588583f, -3.1775224f, 5.9737964f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->big_beetle_part3 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART3);
  if (subscene_assets->big_beetle_part3 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part3,
                                  0.4551795f, -0.6109241f, -0.5598088f,
                                  -0.5598088f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part3,
                                  0.0f, -2.5622821f, -3.2474496f, 5.9737959f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->big_beetle_part4 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART4);
  if (subscene_assets->big_beetle_part4 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part4,
                                  0.4662272f, -0.5773502f, -0.5773502f,
                                  -0.5773503f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part4,
                                  0.0f, -2.109704f, -3.2870438f, 5.9737954f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->big_beetle_part5 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART5);
  if (subscene_assets->big_beetle_part5 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part5,
                                  0.4776013f, -0.5437748f, -0.593426f,
                                  -0.5934261f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part5,
                                  0.0f, -1.8852423f, -3.287044f, 5.972878f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->big_beetle_part6 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART6);
  if (subscene_assets->big_beetle_part6 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part6,
                                  0.4776013f, -0.5437748f, -0.593426f,
                                  -0.5934261f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part6,
                                  0.0f, -1.6736788f, -3.2685332f, 5.9678516f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->big_beetle_part7 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART7);
  if (subscene_assets->big_beetle_part7 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part7,
                                  0.4662272f, -0.5773503f, -0.5773503f,
                                  -0.5773502f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part7,
                                  0.0f, -1.4693183f, -3.2506537f, 5.9602246f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->big_beetle_part8 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART8);
  if (subscene_assets->big_beetle_part8 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part8,
                                  0.4551795f, -0.6109241f, -0.5598088f,
                                  -0.5598087f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part8,
                                  0.0f, -1.271131f, -3.2506533f, 5.9517198f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->big_beetle_part9 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART9);
  if (subscene_assets->big_beetle_part9 == NULL ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part9,
                                  0.4444874f, -0.6444005f, -0.5407163f,
                                  -0.5407161f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(engine_ctx, subscene_assets->big_beetle_part9,
                                  0.0f, -1.0773219f, -3.2676089f, 5.9436374f,
                                  MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->big_beetle_part10 =
      e3d_Mesh_CreateMesh(engine_ctx, subscene_assets->big_beetle_material,
                          GFX_MODEL_BIG_BEETLE_PART10);
  if (subscene_assets->big_beetle_part10 == NULL ||
      !e3d_Mesh_AddTransformation(
          engine_ctx, subscene_assets->big_beetle_part10, 0.4551795f,
          -0.6109241f, -0.5598088f, -0.5598087f, MODEL_TRANSFORM_ROTATE) ||
      !e3d_Mesh_AddTransformation(
          engine_ctx, subscene_assets->big_beetle_part10, 0.0f, -0.8830516f,
          -3.3018625f, 5.9370565f, MODEL_TRANSFORM_TRANSLATE)) {
    return false;
  }
  subscene_assets->camera = e3d_Camera_CreateCamera(
      engine_ctx, -2.9656358f, 2.0259302f, 19.7691593f, -2.9656358f, 2.0259302f,
      18.7691593f, 0.0f, 1.0f, 0.0f);
  if (subscene_assets->camera == NULL) {
    return false;
  }

  e3d_Renderer_SetLight(engine_ctx, assets->light);
  e3d_Renderer_SetCamera(engine_ctx, subscene_assets->camera);
  return true;
}

static void unload_assets_subscene4(e3d_EngineContext *engine_ctx,
                                    Scene12Subscene4Assets *subscene_assets) {
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->ant);
  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->ant_material);

  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->beetle1);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->beetle2);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->beetle3);

  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->beetle_material1);
  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->beetle_material2);
  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->beetle_material3);

  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part1);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part2);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part3);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part4);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part5);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part6);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part7);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part8);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part9);
  e3d_Mesh_DeleteMesh(engine_ctx, &subscene_assets->big_beetle_part10);

  e3d_Material_DeleteMat(engine_ctx, &subscene_assets->big_beetle_material);

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
                         Scene12Subscene4Assets *subscene_assets,
                         uint32_t subscene_frame) {
  if (big_beetle_part1_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part1_moon_anim_frame =
        model_animation_get_value(&big_beetle_part1_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part1_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part1,
                                    big_beetle_part1_moon_anim_frame->w,
                                    big_beetle_part1_moon_anim_frame->x,
                                    big_beetle_part1_moon_anim_frame->y,
                                    big_beetle_part1_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part1_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part1_moon_anim_frame =
        model_animation_get_value(&big_beetle_part1_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part1_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part1,
                                    big_beetle_part1_moon_anim_frame->w,
                                    big_beetle_part1_moon_anim_frame->x,
                                    big_beetle_part1_moon_anim_frame->y,
                                    big_beetle_part1_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (big_beetle_part2_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part2_moon_anim_frame =
        model_animation_get_value(&big_beetle_part2_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part2_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part2,
                                    big_beetle_part2_moon_anim_frame->w,
                                    big_beetle_part2_moon_anim_frame->x,
                                    big_beetle_part2_moon_anim_frame->y,
                                    big_beetle_part2_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part2_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part2_moon_anim_frame =
        model_animation_get_value(&big_beetle_part2_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part2_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part2,
                                    big_beetle_part2_moon_anim_frame->w,
                                    big_beetle_part2_moon_anim_frame->x,
                                    big_beetle_part2_moon_anim_frame->y,
                                    big_beetle_part2_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (big_beetle_part3_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part3_moon_anim_frame =
        model_animation_get_value(&big_beetle_part3_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part3_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part3,
                                    big_beetle_part3_moon_anim_frame->w,
                                    big_beetle_part3_moon_anim_frame->x,
                                    big_beetle_part3_moon_anim_frame->y,
                                    big_beetle_part3_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part3_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part3_moon_anim_frame =
        model_animation_get_value(&big_beetle_part3_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part3_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part3,
                                    big_beetle_part3_moon_anim_frame->w,
                                    big_beetle_part3_moon_anim_frame->x,
                                    big_beetle_part3_moon_anim_frame->y,
                                    big_beetle_part3_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (big_beetle_part4_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part4_moon_anim_frame =
        model_animation_get_value(&big_beetle_part4_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part4_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part4,
                                    big_beetle_part4_moon_anim_frame->w,
                                    big_beetle_part4_moon_anim_frame->x,
                                    big_beetle_part4_moon_anim_frame->y,
                                    big_beetle_part4_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part4_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part4_moon_anim_frame =
        model_animation_get_value(&big_beetle_part4_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part4_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part4,
                                    big_beetle_part4_moon_anim_frame->w,
                                    big_beetle_part4_moon_anim_frame->x,
                                    big_beetle_part4_moon_anim_frame->y,
                                    big_beetle_part4_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (big_beetle_part5_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part5_moon_anim_frame =
        model_animation_get_value(&big_beetle_part5_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part5_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part5,
                                    big_beetle_part5_moon_anim_frame->w,
                                    big_beetle_part5_moon_anim_frame->x,
                                    big_beetle_part5_moon_anim_frame->y,
                                    big_beetle_part5_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part5_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part5_moon_anim_frame =
        model_animation_get_value(&big_beetle_part5_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part5_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part5,
                                    big_beetle_part5_moon_anim_frame->w,
                                    big_beetle_part5_moon_anim_frame->x,
                                    big_beetle_part5_moon_anim_frame->y,
                                    big_beetle_part5_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (big_beetle_part6_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part6_moon_anim_frame =
        model_animation_get_value(&big_beetle_part6_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part6_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part6,
                                    big_beetle_part6_moon_anim_frame->w,
                                    big_beetle_part6_moon_anim_frame->x,
                                    big_beetle_part6_moon_anim_frame->y,
                                    big_beetle_part6_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part6_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part6_moon_anim_frame =
        model_animation_get_value(&big_beetle_part6_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part6_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part6,
                                    big_beetle_part6_moon_anim_frame->w,
                                    big_beetle_part6_moon_anim_frame->x,
                                    big_beetle_part6_moon_anim_frame->y,
                                    big_beetle_part6_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (big_beetle_part7_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part7_moon_anim_frame =
        model_animation_get_value(&big_beetle_part7_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part7_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part7,
                                    big_beetle_part7_moon_anim_frame->w,
                                    big_beetle_part7_moon_anim_frame->x,
                                    big_beetle_part7_moon_anim_frame->y,
                                    big_beetle_part7_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part7_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part7_moon_anim_frame =
        model_animation_get_value(&big_beetle_part7_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part7_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part7,
                                    big_beetle_part7_moon_anim_frame->w,
                                    big_beetle_part7_moon_anim_frame->x,
                                    big_beetle_part7_moon_anim_frame->y,
                                    big_beetle_part7_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (big_beetle_part8_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part8_moon_anim_frame =
        model_animation_get_value(&big_beetle_part8_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part8_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part8,
                                    big_beetle_part8_moon_anim_frame->w,
                                    big_beetle_part8_moon_anim_frame->x,
                                    big_beetle_part8_moon_anim_frame->y,
                                    big_beetle_part8_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part8_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part8_moon_anim_frame =
        model_animation_get_value(&big_beetle_part8_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part8_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part8,
                                    big_beetle_part8_moon_anim_frame->w,
                                    big_beetle_part8_moon_anim_frame->x,
                                    big_beetle_part8_moon_anim_frame->y,
                                    big_beetle_part8_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (big_beetle_part9_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part9_moon_anim_frame =
        model_animation_get_value(&big_beetle_part9_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part9_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part9,
                                    big_beetle_part9_moon_anim_frame->w,
                                    big_beetle_part9_moon_anim_frame->x,
                                    big_beetle_part9_moon_anim_frame->y,
                                    big_beetle_part9_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part9_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part9_moon_anim_frame =
        model_animation_get_value(&big_beetle_part9_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part9_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part9,
                                    big_beetle_part9_moon_anim_frame->w,
                                    big_beetle_part9_moon_anim_frame->x,
                                    big_beetle_part9_moon_anim_frame->y,
                                    big_beetle_part9_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (big_beetle_part10_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part10_moon_anim_frame =
        model_animation_get_value(&big_beetle_part10_moon_anim_translate,
                                  subscene_frame);
    if (big_beetle_part10_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part10,
                                    big_beetle_part10_moon_anim_frame->w,
                                    big_beetle_part10_moon_anim_frame->x,
                                    big_beetle_part10_moon_anim_frame->y,
                                    big_beetle_part10_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part10_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part10_moon_anim_frame =
        model_animation_get_value(&big_beetle_part10_moon_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part10_moon_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx,
                                    subscene_assets->big_beetle_part10,
                                    big_beetle_part10_moon_anim_frame->w,
                                    big_beetle_part10_moon_anim_frame->x,
                                    big_beetle_part10_moon_anim_frame->y,
                                    big_beetle_part10_moon_anim_frame->z,
                                    BIG_BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (beetle1_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *beetle1_moon_anim_translate_frame =
        model_animation_get_value(&beetle1_moon_anim_translate, subscene_frame);
    if (beetle1_moon_anim_translate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx, subscene_assets->beetle1,
                                    beetle1_moon_anim_translate_frame->w,
                                    beetle1_moon_anim_translate_frame->x,
                                    beetle1_moon_anim_translate_frame->y,
                                    beetle1_moon_anim_translate_frame->z,
                                    BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (beetle1_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *beetle1_moon_anim_rotate_frame =
        model_animation_get_value(&beetle1_moon_anim_rotate, subscene_frame);
    if (beetle1_moon_anim_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->beetle1,
          beetle1_moon_anim_rotate_frame->w, beetle1_moon_anim_rotate_frame->x,
          beetle1_moon_anim_rotate_frame->y, beetle1_moon_anim_rotate_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (beetle2_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *beetle2_moon_anim_translate_frame =
        model_animation_get_value(&beetle2_moon_anim_translate, subscene_frame);
    if (beetle2_moon_anim_translate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx, subscene_assets->beetle2,
                                    beetle2_moon_anim_translate_frame->w,
                                    beetle2_moon_anim_translate_frame->x,
                                    beetle2_moon_anim_translate_frame->y,
                                    beetle2_moon_anim_translate_frame->z,
                                    BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (beetle2_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *beetle2_moon_anim_rotate_frame =
        model_animation_get_value(&beetle2_moon_anim_rotate, subscene_frame);
    if (beetle2_moon_anim_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->beetle2,
          beetle2_moon_anim_rotate_frame->w, beetle2_moon_anim_rotate_frame->x,
          beetle2_moon_anim_rotate_frame->y, beetle2_moon_anim_rotate_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  if (beetle3_moon_anim_translate.values_count > 0u) {
    const ModelAnimationValue *beetle3_moon_anim_translate_frame =
        model_animation_get_value(&beetle3_moon_anim_translate, subscene_frame);
    if (beetle3_moon_anim_translate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(engine_ctx, subscene_assets->beetle3,
                                    beetle3_moon_anim_translate_frame->w,
                                    beetle3_moon_anim_translate_frame->x,
                                    beetle3_moon_anim_translate_frame->y,
                                    beetle3_moon_anim_translate_frame->z,
                                    BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }

  if (beetle3_moon_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *beetle3_moon_anim_rotate_frame =
        model_animation_get_value(&beetle3_moon_anim_rotate, subscene_frame);
    if (beetle3_moon_anim_rotate_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->beetle3,
          beetle3_moon_anim_rotate_frame->w, beetle3_moon_anim_rotate_frame->x,
          beetle3_moon_anim_rotate_frame->y, beetle3_moon_anim_rotate_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  // camera
  if (camera_moon_anim_pos.values_count > 0u) {
    const ModelAnimationValue *camera_pos_frame =
        model_animation_get_value(&camera_moon_anim_pos, subscene_frame);
    if (camera_pos_frame != NULL) {
      e3d_Camera_SetPos(engine_ctx, subscene_assets->camera,
                        camera_pos_frame->x, camera_pos_frame->y,
                        camera_pos_frame->z);
    }
  }
  if (camera_moon_anim_target.values_count > 0u) {
    const ModelAnimationValue *camera_target_frame =
        model_animation_get_value(&camera_moon_anim_target, subscene_frame);
    if (camera_target_frame != NULL) {
      e3d_Camera_SetTargetPos(engine_ctx, subscene_assets->camera,
                              camera_target_frame->x, camera_target_frame->y,
                              camera_target_frame->z);
    }
  }
  e3d_Camera_UpdateCamera(engine_ctx, subscene_assets->camera);
  e3d_Renderer_CleanScene(engine_ctx);
  // e3d_Renderer_AddModelToScene(engine_ctx, assets->earth);
  e3d_Renderer_AddModelToScene(engine_ctx, assets->moon);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->ant);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->beetle1);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->beetle2);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->beetle3);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part1);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part2);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part3);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part4);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part5);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part6);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part7);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part8);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part9);
  e3d_Renderer_AddModelToScene(engine_ctx, subscene_assets->big_beetle_part10);
  for (uint32_t i = 0u; i < sizeof(starField) / sizeof(starField[0]); i++) {
    e3d_Renderer_AddPointToScene(engine_ctx, &starField[i]);
  }
  e3d_Renderer_RenderScene(engine_ctx);
  end_scene_frame(scene_ctx);
  demo_end_frame();
}

void scene_12_subscene4_run_scene(DemoContext *demo_ctx,
                                  DemoSceneContext *scene_ctx,
                                  SpaceSceneAssets *assets) {
  if (demo_ctx == NULL || demo_ctx->engine_ctx == NULL || scene_ctx == NULL ||
      assets == NULL) {
    return;
  }

  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  Scene12Subscene4Assets subscene_assets = {0};
  if (!load_assets_subscene4(engine_ctx, assets, &subscene_assets)) {
    unload_assets_subscene4(engine_ctx, &subscene_assets);
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

  unload_assets_subscene4(engine_ctx, &subscene_assets);
}
