#include "utils/math_utils.h"

#include <math.h>

float signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5*((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
}

// --- Vec2 ---
Vec2 vec2_add(Vec2 a, Vec2 b) { return (Vec2){a.x + b.x, a.y + b.y}; }
Vec2 vec2_sub(Vec2 a, Vec2 b) { return (Vec2){a.x - b.x, a.y - b.y}; }
Vec2 vec2_mul(Vec2 a, float s) { return (Vec2){a.x * s, a.y * s}; }
float vec2_dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
float vec2_length(Vec2 v) { return sqrtf(vec2_dot(v, v)); }
Vec2 vec2_normalize(Vec2 v) {
    float len = vec2_length(v);
    if (len > 1e-6f) return vec2_mul(v, 1.0f / len);
    return (Vec2){0, 0};
}

// --- Vec3 ---
Vec3 vec3_add(Vec3 a, Vec3 b) { return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3 vec3_sub(Vec3 a, Vec3 b) { return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }
Vec3 vec3_mul(Vec3 a, float s) { return (Vec3){a.x * s, a.y * s, a.z * s}; }
float vec3_dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return (Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
float vec3_length(Vec3 v) { return sqrtf(vec3_dot(v, v)); }
Vec3 vec3_normalize(Vec3 v) {
    float len = vec3_length(v);
    if (len > 1e-6f) return vec3_mul(v, 1.0f / len);
    return (Vec3){0, 0, 0};
}

// --- Vec4 ---
Vec4 vec4_add(Vec4 a, Vec4 b) { return (Vec4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w}; }
Vec4 vec4_sub(Vec4 a, Vec4 b) { return (Vec4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w}; }
Vec4 vec4_mul(Vec4 a, float s) { return (Vec4){a.x * s, a.y * s, a.z * s, a.w * s}; }
float vec4_dot(Vec4 a, Vec4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
float vec4_length(Vec4 v) { return sqrtf(vec4_dot(v, v)); }
Vec4 vec4_normalize(Vec4 v) {
    float len = vec4_length(v);
    if (len > 1e-6f) return vec4_mul(v, 1.0f / len);
    return (Vec4){0, 0, 0, 0};
}

// --- Mat2 ---
Mat2 mat2_identity(void) {
    Mat2 res = {{{0}}};
    res.m[0][0] = 1.0f; res.m[1][1] = 1.0f;
    return res;
}

Mat2 mat2_mul(Mat2 a, Mat2 b) {
    Mat2 res = {{{0}}};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                res.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return res;
}
Vec2 mat2_vec2_mul(Mat2 m, Vec2 v) {
    return (Vec2){
        m.m[0][0] * v.x + m.m[0][1] * v.y,
        m.m[1][0] * v.x + m.m[1][1] * v.y
    };
}

Mat2 mat2_inverse(Mat2 m) {
    float det = m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0];
    if (fabsf(det) < 1e-6f) return (Mat2){{{0}}};
    float inv_det = 1.0f / det;
    Mat2 res;
    res.m[0][0] =  m.m[1][1] * inv_det;
    res.m[0][1] = -m.m[0][1] * inv_det;
    res.m[1][0] = -m.m[1][0] * inv_det;
    res.m[1][1] =  m.m[0][0] * inv_det;
    return res;
}

// --- Mat3 ---
Mat3 mat3_identity(void) {
    Mat3 res = {{{0}}};
    res.m[0][0] = 1.0f; res.m[1][1] = 1.0f; res.m[2][2] = 1.0f;
    return res;
}

Mat3 mat3_mul(Mat3 a, Mat3 b) {
    Mat3 res = {{{0}}};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                res.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return res;
}
Vec3 mat3_vec3_mul(Mat3 m, Vec3 v) {
    return (Vec3){
        m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z,
        m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z,
        m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z
    };
}

Mat3 mat3_inverse(Mat3 m) {
    float det = m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) -
                m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) +
                m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);

    if (fabsf(det) < 1e-6f) return (Mat3){{{0}}};
    float inv_det = 1.0f / det;

    Mat3 res;
    res.m[0][0] = (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) * inv_det;
    res.m[0][1] = (m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2]) * inv_det;
    res.m[0][2] = (m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1]) * inv_det;
    res.m[1][0] = (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) * inv_det;
    res.m[1][1] = (m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0]) * inv_det;
    res.m[1][2] = (m.m[1][0] * m.m[0][2] - m.m[0][0] * m.m[1][2]) * inv_det;
    res.m[2][0] = (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]) * inv_det;
    res.m[2][1] = (m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1]) * inv_det;
    res.m[2][2] = (m.m[0][0] * m.m[1][1] - m.m[1][0] * m.m[0][1]) * inv_det;
    return res;
}

// --- Mat4 ---
Mat4 mat4_identity(void) {
    Mat4 res = {{{0}}};
    res.m[0][0] = 1.0f; res.m[1][1] = 1.0f;
    res.m[2][2] = 1.0f; res.m[3][3] = 1.0f;
    return res;
}

Mat4 mat4_mul(Mat4 a, Mat4 b) {
    Mat4 res = {{{0}}};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                res.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return res;
}
Vec4 mat4_vec4_mul(Mat4 m, Vec4 v) {
    return (Vec4){
        m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
        m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
        m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
        m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w
    };
}

Mat4 mat4_transpose(Mat4 m) {
    Mat4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.m[i][j] = m.m[j][i];
        }
    }
    return res;
}

Mat4 mat4_inverse(Mat4 m) {
    float n11 = m.m[0][0], n12 = m.m[0][1], n13 = m.m[0][2], n14 = m.m[0][3];
    float n21 = m.m[1][0], n22 = m.m[1][1], n23 = m.m[1][2], n24 = m.m[1][3];
    float n31 = m.m[2][0], n32 = m.m[2][1], n33 = m.m[2][2], n34 = m.m[2][3];
    float n41 = m.m[3][0], n42 = m.m[3][1], n43 = m.m[3][2], n44 = m.m[3][3];

    float t1 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t2 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t3 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t4 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t1 + n21 * t2 + n31 * t3 + n41 * t4;

    if (fabsf(det) < 1e-6f) return (Mat4){{{0}}};

    float inv_det = 1.0f / det;

    Mat4 res;
    res.m[0][0] = t1 * inv_det;
    res.m[0][1] = t2 * inv_det;
    res.m[0][2] = t3 * inv_det;
    res.m[0][3] = t4 * inv_det;

    res.m[1][0] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * inv_det;
    res.m[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * inv_det;
    res.m[1][2] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * inv_det;
    res.m[1][3] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * inv_det;

    res.m[2][0] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * inv_det;
    res.m[2][1] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * inv_det;
    res.m[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * inv_det;
    res.m[2][3] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * inv_det;

    res.m[3][0] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * inv_det;
    res.m[3][1] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * inv_det;
    res.m[3][2] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * inv_det;
    res.m[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * inv_det;

    return res;
}

Mat4 mat4_translate(float x, float y, float z) {
    Mat4 res = mat4_identity();
    res.m[0][3] = x;
    res.m[1][3] = y;
    res.m[2][3] = z;
    return res;
}
Mat4 mat4_scale(float x, float y, float z) {
    Mat4 res = {{{0}}};
    res.m[0][0] = x;
    res.m[1][1] = y;
    res.m[2][2] = z;
    res.m[3][3] = 1.0f;
    return res;
}

Mat4 mat4_rotate_x(float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    Mat4 res = mat4_identity();
    res.m[1][1] = c;  res.m[1][2] = -s;
    res.m[2][1] = s;  res.m[2][2] = c;
    return res;
}
Mat4 mat4_rotate_y(float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    Mat4 res = mat4_identity();
    res.m[0][0] = c;  res.m[0][2] = s;
    res.m[2][0] = -s; res.m[2][2] = c;
    return res;
}
Mat4 mat4_rotate_z(float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    Mat4 res = mat4_identity();
    res.m[0][0] = c;  res.m[0][1] = -s;
    res.m[1][0] = s;  res.m[1][1] = c;
    return res;
}
