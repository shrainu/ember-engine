#pragma once

#include "common.h"


// Type

typedef struct EMBER_Camera {
    mat4 proj;
    mat4 view;
} EMBER_Camera;


// Short API typedef

#ifdef EMBER_SHORT_API

typedef EMBER_Camera * Camera;

#endif


// Default projection

void EMBER_InitDefaultProjection();

void EMBER_SetDefaultProjection(mat4 proj);

const vec4* EMBER_GetDefaultProjection();


// Constructor and destructor

EMBER_Camera* EMBER_CameraNew(float left, float right, float up, float down);

void EMBER_CameraFree(EMBER_Camera* camera);


// Bound Camera

const EMBER_Camera* EMBER_GetBoundCamera();


// Binding

void EMBER_CameraBegin(const EMBER_Camera* camera);

void EMBER_CameraEnd();


// Functionality

void EMBER_CameraSetPosition(EMBER_Camera* camera, vec3 pos);

void EMBER_CameraSetProjection(EMBER_Camera* camera, float left, float right, float up, float down);