#ifndef SCENE_12_SPACE_H
#define SCENE_12_SPACE_H

#include "engineApi.h"

typedef struct DemoSceneContext DemoSceneContext;

typedef struct SpaceSceneAssets {
  e3d_Mesh *bug;
  e3d_Mesh *earth;
  e3d_Mesh *moon;
  e3d_Material *bug_material;
  e3d_Material *earth_material;
  e3d_Material *moon_material;
  e3d_Light *light;
  e3d_Camera *camera;
} SpaceSceneAssets;

DemoSceneContext *scene_12_space_get_context(void);

#endif
