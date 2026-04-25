#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// --- Scalar Utilities ---

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#define clamp(v, lo, hi) (max(lo, min(v, hi)))
#define lerp(a, b, t) ((a) + (t) * ((b) - (a)))

float signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy);

// --- Vector Definitions ---

typedef struct { float x, y; } Vec2;
typedef struct { float x, y, z; } Vec3;
typedef struct { float x, y, z, w; } Vec4;

// --- Vec2 Operations ---

Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_mul(Vec2 a, float s);
float vec2_dot(Vec2 a, Vec2 b);
float vec2_length(Vec2 v);
Vec2 vec2_normalize(Vec2 v);

// --- Vec3 Operations ---

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 a, float s);
float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);
float vec3_length(Vec3 v);
Vec3 vec3_normalize(Vec3 v);

// --- Vec4 Operations ---

Vec4 vec4_add(Vec4 a, Vec4 b);
Vec4 vec4_sub(Vec4 a, Vec4 b);
Vec4 vec4_mul(Vec4 a, float s);
float vec4_dot(Vec4 a, Vec4 b);
float vec4_length(Vec4 v);
Vec4 vec4_normalize(Vec4 v);

// --- Matrix Definitions (Row-Major) ---

typedef struct { float m[2][2]; } Mat2;
typedef struct { float m[3][3]; } Mat3;
typedef struct { float m[4][4]; } Mat4;

// --- Mat2 Operations ---
Mat2 mat2_identity(void);
Mat2 mat2_mul(Mat2 a, Mat2 b);
Vec2 mat2_vec2_mul(Mat2 m, Vec2 v);
Mat2 mat2_inverse(Mat2 m);

// --- Mat3 Operations ---
Mat3 mat3_identity(void);
Mat3 mat3_mul(Mat3 a, Mat3 b);
Vec3 mat3_vec3_mul(Mat3 m, Vec3 v);
Mat3 mat3_inverse(Mat3 m);

// --- Mat4 Operations ---

Mat4 mat4_identity(void);
Mat4 mat4_mul(Mat4 a, Mat4 b);
Vec4 mat4_vec4_mul(Mat4 m, Vec4 v);
Mat4 mat4_inverse(Mat4 m);
Mat4 mat4_transpose(Mat4 m);

// --- Transformation Matrices ---

Mat4 mat4_translate(float x, float y, float z);
Mat4 mat4_scale(float x, float y, float z);
Mat4 mat4_rotate_x(float angle);
Mat4 mat4_rotate_y(float angle);
Mat4 mat4_rotate_z(float angle);

#endif