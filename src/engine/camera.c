#include "camera.h"

#include "window.h"


// Statics

static mat4 s_ember_default_proj_;

static const float S_EMBER_DEFAULT_FAR_ = 100.0;
static const float S_EMBER_DEFAULT_NEAR_ = -1.0;


// Bound Camera

static const EMBER_Camera* s_ember_bound_camera_ = NULL;


// Default projection

void EMBER_InitDefaultProjection() {
    vec2s window_size = EMBER_GetWindowSize();
    glm_mat4_identity(s_ember_default_proj_);
    glm_ortho(0, window_size.x, 0, window_size.y, S_EMBER_DEFAULT_NEAR_, S_EMBER_DEFAULT_FAR_, s_ember_default_proj_);
}

void EMBER_SetDefaultProjection(mat4 proj) {
    glm_mat4_copy(proj, s_ember_default_proj_);
}

const vec4* EMBER_GetDefaultProjection() {
    return s_ember_default_proj_;
}


// Constructor and destructor

EMBER_Camera* EMBER_CameraNew(float left, float right, float bottom, float top) {

    // Allocate memory for camera
    EMBER_Camera* camera = (EMBER_Camera*) malloc(sizeof(EMBER_Camera));

    // Initialize the camera projection & view
    glm_ortho(left, right, bottom, top, S_EMBER_DEFAULT_NEAR_, S_EMBER_DEFAULT_FAR_, camera->proj);

    glm_mat4_identity(camera->view);
    glm_translate(camera->view, (vec3) {0, 0, 0});

    // Return the camera
    return camera;
}

void EMBER_CameraFree(EMBER_Camera* camera) {
    if (!camera) {
        return;
    }

    free(camera);
}


// Bound Camera

const EMBER_Camera* EMBER_GetBoundCamera() {
    return s_ember_bound_camera_;
}


// Binding

void EMBER_CameraBegin(const EMBER_Camera* camera) {
    s_ember_bound_camera_ = camera;
}

void EMBER_CameraEnd() {
    s_ember_bound_camera_ = NULL;
}


// Functionality

void EMBER_CameraSetPosition(EMBER_Camera* camera, vec3 pos) {
    glm_translate(camera->view, pos);
}

void EMBER_CameraSetProjection(EMBER_Camera* camera, float left, float right, float bottom, float top) {
    glm_ortho(left, right, bottom, top, S_EMBER_DEFAULT_NEAR_, S_EMBER_DEFAULT_FAR_, camera->proj);
}