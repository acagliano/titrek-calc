
#ifndef gfx_engine_h
#define gfx_engine_h

// ENGINE TYPEDEFS
typedef struct {
    long x;
    long y;
    long z;
} rend_point_t;
rend_point_t P_AddVectorToPoint(rend_point_t *point, rend_vector_t *vector);
rend_point_t P_SubVectorFromPoint(rend_point_t *point1, rend_point_t *point2);
rend_vector_t P_SubPointFromPoint(rend_point_t *point1, rend_point_t *point2);
void P_SetPointToPoint(rend_point_t *point, long x, long y, long z);

typedef rend_point_t rend_vector_t;
rend_vector_t V_AddVectorToVector(rend_vector_t *vector1, rend_vector_t *vector2);
rend_vector_t V_SubVectorFromVector(rend_vector_t *vector1, rend_vector_t *vector2);
rend_vector_t V_RotateXY(char angle);
rend_vector_t V_RotateYZ(char angle);
rend_vector_t V_RotateXZ(char angle);
rend_vector_t V_Scale(int s0, int s1, int s2);

typedef struct {
    int minX, maxX;     // drawing window X
    int minY, maxY;     // drawing window Y
    char renderAngle;   // FOV
    int renderdist;     // camera depth
} rend_camera_t;
void CAM_InitTo1stP(rend_camera_t *camera, rend_point_t *point);
void CAM_InitTo3rdP(rend_camera_t *camera, rend_point_t *vector);
void CAM_CullMap(rend_camera_t *camera, MapData_t *map);

#endif
