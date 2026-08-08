#include "scene_12_subscene3.h"

#include "../../demo_context.h"
#include "../../demo_scene.h"
#include "../scene_12_animations.inc"
#include "../scene_12_star_field.inc"
#include "engineApi.h"
#include "model_animation.h"
#include "storage/gfx_indices.h"

#define BEETLE_ANIMATION_ROTATION_INDEX 0u
#define BEETLE_ANIMATION_TRANSLATION_INDEX 1u

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
  e3d_Camera *camera;
} Scene12Subscene3Assets;

static bool load_assets_subscene3(e3d_EngineContext *engine_ctx,
                                  SpaceSceneAssets *assets,
                                  Scene12Subscene3Assets *subscene_assets) {
  if (subscene_assets->camera != NULL) {
    return true;
  }
  subscene_assets->big_beetle_material = e3d_Material_CreateTexturedMat(
      engine_ctx, GFX_TEXTURE_BIG_BEETLE, 0.0f, 0.0f, true);
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
      engine_ctx, 5.3710976f, -2.2729864f, 9.7624378f, 1.2879658f, 0.61832f,
      -4.4578447f, 0.0f, 1.0f, 0.0f);
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

static void unload_assets_subscene3(e3d_EngineContext *engine_ctx,
                                    Scene12Subscene3Assets *subscene_assets) {
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
                         Scene12Subscene3Assets *subscene_assets,
                         uint32_t subscene_frame) {
  if (big_beetle_part1_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part1_anim_frame =
        model_animation_get_value(&big_beetle_part1_anim_translate,
                                  subscene_frame);
    if (big_beetle_part1_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part1,
          big_beetle_part1_anim_frame->w, big_beetle_part1_anim_frame->x,
          big_beetle_part1_anim_frame->y, big_beetle_part1_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part1_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part1_anim_frame =
        model_animation_get_value(&big_beetle_part1_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part1_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part1,
          big_beetle_part1_anim_frame->w, big_beetle_part1_anim_frame->x,
          big_beetle_part1_anim_frame->y, big_beetle_part1_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  if (big_beetle_part2_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part2_anim_frame =
        model_animation_get_value(&big_beetle_part2_anim_translate,
                                  subscene_frame);
    if (big_beetle_part2_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part2,
          big_beetle_part2_anim_frame->w, big_beetle_part2_anim_frame->x,
          big_beetle_part2_anim_frame->y, big_beetle_part2_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part2_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part2_anim_frame =
        model_animation_get_value(&big_beetle_part2_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part2_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part2,
          big_beetle_part2_anim_frame->w, big_beetle_part2_anim_frame->x,
          big_beetle_part2_anim_frame->y, big_beetle_part2_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  if (big_beetle_part3_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part3_anim_frame =
        model_animation_get_value(&big_beetle_part3_anim_translate,
                                  subscene_frame);
    if (big_beetle_part3_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part3,
          big_beetle_part3_anim_frame->w, big_beetle_part3_anim_frame->x,
          big_beetle_part3_anim_frame->y, big_beetle_part3_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part3_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part3_anim_frame =
        model_animation_get_value(&big_beetle_part3_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part3_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part3,
          big_beetle_part3_anim_frame->w, big_beetle_part3_anim_frame->x,
          big_beetle_part3_anim_frame->y, big_beetle_part3_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  if (big_beetle_part4_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part4_anim_frame =
        model_animation_get_value(&big_beetle_part4_anim_translate,
                                  subscene_frame);
    if (big_beetle_part4_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part4,
          big_beetle_part4_anim_frame->w, big_beetle_part4_anim_frame->x,
          big_beetle_part4_anim_frame->y, big_beetle_part4_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part4_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part4_anim_frame =
        model_animation_get_value(&big_beetle_part4_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part4_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part4,
          big_beetle_part4_anim_frame->w, big_beetle_part4_anim_frame->x,
          big_beetle_part4_anim_frame->y, big_beetle_part4_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  if (big_beetle_part5_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part5_anim_frame =
        model_animation_get_value(&big_beetle_part5_anim_translate,
                                  subscene_frame);
    if (big_beetle_part5_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part5,
          big_beetle_part5_anim_frame->w, big_beetle_part5_anim_frame->x,
          big_beetle_part5_anim_frame->y, big_beetle_part5_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part5_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part5_anim_frame =
        model_animation_get_value(&big_beetle_part5_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part5_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part5,
          big_beetle_part5_anim_frame->w, big_beetle_part5_anim_frame->x,
          big_beetle_part5_anim_frame->y, big_beetle_part5_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  if (big_beetle_part6_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part6_anim_frame =
        model_animation_get_value(&big_beetle_part6_anim_translate,
                                  subscene_frame);
    if (big_beetle_part6_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part6,
          big_beetle_part6_anim_frame->w, big_beetle_part6_anim_frame->x,
          big_beetle_part6_anim_frame->y, big_beetle_part6_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part6_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part6_anim_frame =
        model_animation_get_value(&big_beetle_part6_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part6_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part6,
          big_beetle_part6_anim_frame->w, big_beetle_part6_anim_frame->x,
          big_beetle_part6_anim_frame->y, big_beetle_part6_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  if (big_beetle_part7_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part7_anim_frame =
        model_animation_get_value(&big_beetle_part7_anim_translate,
                                  subscene_frame);
    if (big_beetle_part7_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part7,
          big_beetle_part7_anim_frame->w, big_beetle_part7_anim_frame->x,
          big_beetle_part7_anim_frame->y, big_beetle_part7_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part7_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part7_anim_frame =
        model_animation_get_value(&big_beetle_part7_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part7_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part7,
          big_beetle_part7_anim_frame->w, big_beetle_part7_anim_frame->x,
          big_beetle_part7_anim_frame->y, big_beetle_part7_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  if (big_beetle_part8_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part8_anim_frame =
        model_animation_get_value(&big_beetle_part8_anim_translate,
                                  subscene_frame);
    if (big_beetle_part8_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part8,
          big_beetle_part8_anim_frame->w, big_beetle_part8_anim_frame->x,
          big_beetle_part8_anim_frame->y, big_beetle_part8_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part8_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part8_anim_frame =
        model_animation_get_value(&big_beetle_part8_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part8_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part8,
          big_beetle_part8_anim_frame->w, big_beetle_part8_anim_frame->x,
          big_beetle_part8_anim_frame->y, big_beetle_part8_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  if (big_beetle_part9_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part9_anim_frame =
        model_animation_get_value(&big_beetle_part9_anim_translate,
                                  subscene_frame);
    if (big_beetle_part9_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part9,
          big_beetle_part9_anim_frame->w, big_beetle_part9_anim_frame->x,
          big_beetle_part9_anim_frame->y, big_beetle_part9_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part9_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part9_anim_frame =
        model_animation_get_value(&big_beetle_part9_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part9_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part9,
          big_beetle_part9_anim_frame->w, big_beetle_part9_anim_frame->x,
          big_beetle_part9_anim_frame->y, big_beetle_part9_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }
  if (big_beetle_part10_anim_translate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part10_anim_frame =
        model_animation_get_value(&big_beetle_part10_anim_translate,
                                  subscene_frame);
    if (big_beetle_part10_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part10,
          big_beetle_part10_anim_frame->w, big_beetle_part10_anim_frame->x,
          big_beetle_part10_anim_frame->y, big_beetle_part10_anim_frame->z,
          BEETLE_ANIMATION_TRANSLATION_INDEX);
    }
  }
  if (big_beetle_part10_anim_rotate.values_count > 0u) {
    const ModelAnimationValue *big_beetle_part10_anim_frame =
        model_animation_get_value(&big_beetle_part10_anim_rotate,
                                  subscene_frame);
    if (big_beetle_part10_anim_frame != NULL) {
      e3d_Mesh_ModifyTransformation(
          engine_ctx, subscene_assets->big_beetle_part10,
          big_beetle_part10_anim_frame->w, big_beetle_part10_anim_frame->x,
          big_beetle_part10_anim_frame->y, big_beetle_part10_anim_frame->z,
          BEETLE_ANIMATION_ROTATION_INDEX);
    }
  }

  e3d_Renderer_CleanScene(engine_ctx);
  e3d_Renderer_AddModelToScene(engine_ctx, assets->earth);
  // e3d_Renderer_AddModelToScene(engine_ctx, assets->moon);
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

void scene_12_subscene3_run_scene(DemoContext *demo_ctx,
                                  DemoSceneContext *scene_ctx,
                                  SpaceSceneAssets *assets) {
  if (demo_ctx == NULL || demo_ctx->engine_ctx == NULL || scene_ctx == NULL ||
      assets == NULL) {
    return;
  }

  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  Scene12Subscene3Assets subscene_assets = {0};
  if (!load_assets_subscene3(engine_ctx, assets, &subscene_assets)) {
    unload_assets_subscene3(engine_ctx, &subscene_assets);
    return;
  }

  if (scene_ctx->scene_frame == 0u) {
    scene_ctx->scene_start_time_ms = demo_platform_time_ms();
  }

  uint32_t subscene_frame = 10u;
  for (; subscene_frame < 100 && scene_should_continue(scene_ctx);
       subscene_frame++) {
    demo_begin_frame();
    render_scene(engine_ctx, scene_ctx, assets, &subscene_assets,
                 subscene_frame);
  }

  unload_assets_subscene3(engine_ctx, &subscene_assets);
}
