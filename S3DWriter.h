#ifndef _S3D_WRITER_H
#define _S3D_WRITER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
        float x;
        float y;
        float z;
} S3D_Vec3;

typedef struct {
        float u;
        float v;
} S3D_TexCoord;

typedef struct {
        S3D_Vec3 position;
        S3D_Vec3 normal;
        S3D_TexCoord tex_coord;
} S3D_Vertex;

typedef struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
} S3D_Color;

typedef struct {
        S3D_Vertex* vertices;
        uint32_t vertices_count;

        uint32_t* indices;
        uint32_t indices_count;

        uint32_t texture_width;
        uint32_t texture_height;
        S3D_Color* texture_data;
} S3D_Mesh;


int S3D_Write(S3D_Mesh* mesh_data, const char* out_path) {
        // TODO
}

#endif // _S3D_WRITER_H