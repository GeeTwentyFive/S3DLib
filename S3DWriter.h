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


typedef enum {
        S3D_WRITE_SUCCESS = 0,
        S3D_WRITE_FAILED_TO_ALLOCATE_MEMORY,
        S3D_WRITE_FAILED_TO_OPEN_FILE,
        S3D_WRITE_FAILED_TO_WRITE_DATA
} S3D_Write_ERROR;

int S3D_Write(S3D_Mesh* mesh_data, const char* out_path) {
        size_t out_data_size = (
                16 +
                mesh_data->vertices_count * 8*sizeof(float) +
                mesh_data->indices_count * sizeof(uint32_t) +
                mesh_data->texture_width*mesh_data->texture_height * 4*sizeof(uint8_t)
        );
        char* out_buf = (char*) malloc(out_data_size);
        if (!out_buf) return S3D_WRITE_FAILED_TO_ALLOCATE_MEMORY;


        char* out_buf_front = out_buf;
        *((uint32_t*)(out_buf_front)) = mesh_data->vertices_count;
        out_buf_front += sizeof(uint32_t);
        *((uint32_t*)(out_buf_front)) = mesh_data->indices_count;
        out_buf_front += sizeof(uint32_t);
        *((uint32_t*)(out_buf_front)) = mesh_data->texture_width;
        out_buf_front += sizeof(uint32_t);
        *((uint32_t*)(out_buf_front)) = mesh_data->texture_height;
        out_buf_front += sizeof(uint32_t);

        for (uint32_t i = 0; i < mesh_data->vertices_count; i++) {
                *((float*)(out_buf_front)) = mesh_data->vertices[i].position.x;
                out_buf_front += sizeof(float);
                *((float*)(out_buf_front)) = mesh_data->vertices[i].position.y;
                out_buf_front += sizeof(float);
                *((float*)(out_buf_front)) = mesh_data->vertices[i].position.z;
                out_buf_front += sizeof(float);

                *((float*)(out_buf_front)) = mesh_data->vertices[i].normal.x;
                out_buf_front += sizeof(float);
                *((float*)(out_buf_front)) = mesh_data->vertices[i].normal.y;
                out_buf_front += sizeof(float);
                *((float*)(out_buf_front)) = mesh_data->vertices[i].normal.z;
                out_buf_front += sizeof(float);

                *((float*)(out_buf_front)) = mesh_data->vertices[i].tex_coord.u;
                out_buf_front += sizeof(float);
                *((float*)(out_buf_front)) = mesh_data->vertices[i].tex_coord.v;
                out_buf_front += sizeof(float);
        }

        for (uint32_t i = 0; i < mesh_data->indices_count; i++) {
                *((uint32_t*)(out_buf_front)) = mesh_data->indices[i];
                out_buf_front += sizeof(uint32_t);
        }

        for (
                uint32_t i = 0;
                i < (mesh_data->texture_width * mesh_data->texture_height);
                i++
        ) {
                *((uint8_t*)(out_buf_front)) = mesh_data->texture_data[i].r;
                out_buf_front += sizeof(uint8_t);
                *((uint8_t*)(out_buf_front)) = mesh_data->texture_data[i].g;
                out_buf_front += sizeof(uint8_t);
                *((uint8_t*)(out_buf_front)) = mesh_data->texture_data[i].b;
                out_buf_front += sizeof(uint8_t);
                *((uint8_t*)(out_buf_front)) = mesh_data->texture_data[i].a;
                out_buf_front += sizeof(uint8_t);
        }


        FILE* out_file = fopen(out_path, "wb");
        if (!out_file) {
                free(out_buf);
                return S3D_WRITE_FAILED_TO_OPEN_FILE;
        }

        if (fwrite(out_buf, 1, out_data_size, out_file) != out_data_size) {
                free(out_buf);
                fclose(out_file);
                return S3D_WRITE_FAILED_TO_WRITE_DATA;
        }

        fclose(out_file);


        free(out_buf);


        return S3D_WRITE_SUCCESS;
}

#endif // _S3D_WRITER_H