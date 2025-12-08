#ifndef _S3D_LOADER_H
#define _S3D_LOADER_H

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


S3D_Mesh* S3D_Load(const char* path) {
        // Reading file data
        FILE* file = fopen(path, "rb");
        if (!file) return NULL;

        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* file_data = (char*)malloc(file_size);
        if (!file_data) {
                fclose(file);
                return NULL;
        }
        if (fread(file_data, 1, file_size, file) != file_size) {
                fclose(file);
                free(file_data);
                return NULL;
        }

        fclose(file);


        char* file_data_offset = file_data;


        // Header + memory allocation
        S3D_Mesh* mesh_data = (S3D_Mesh*) malloc(sizeof(S3D_Mesh));
        if (!mesh_data) {
                free(file_data);
                return NULL;
        }

        mesh_data->vertices_count = *((uint32_t*)(file_data_offset));
        file_data_offset += sizeof(uint32_t);
        mesh_data->vertices = (S3D_Vertex*) malloc(
                sizeof(S3D_Vertex) * mesh_data->vertices_count
        );
        if (!mesh_data->vertices) {
                free(file_data);
                free(mesh_data);
                return NULL;
        }

        mesh_data->indices_count = *((uint32_t*)(file_data_offset));
        file_data_offset += sizeof(uint32_t);
        mesh_data->indices = (uint32_t*) malloc(
                sizeof(uint32_t) * mesh_data->indices_count
        );
        if (!mesh_data->indices) {
                free(file_data);
                free(mesh_data);
                free(mesh_data->vertices);
                return NULL;
        }

        mesh_data->texture_width = *((uint32_t*)(file_data_offset));
        file_data_offset += sizeof(uint32_t);
        mesh_data->texture_height = *((uint32_t*)(file_data_offset));
        file_data_offset += sizeof(uint32_t);
        mesh_data->texture_data = (S3D_Color*) malloc(
                sizeof(S3D_Color) * (
                        mesh_data->texture_width * mesh_data->texture_height
                )
        );
        if (!mesh_data->texture_data) {
                free(file_data);
                free(mesh_data);
                free(mesh_data->vertices);
                free(mesh_data->indices);
                return NULL;
        }


        // Vertex parsing
        for (uint32_t i = 0; i < mesh_data->vertices_count; i++) {
                mesh_data->vertices[i] = S3D_Vertex {
                        .position = {
                                *((float*)(file_data_offset + sizeof(float)*0)),
                                *((float*)(file_data_offset + sizeof(float)*1)),
                                *((float*)(file_data_offset + sizeof(float)*2))
                        },
                        .normal = {
                                *((float*)(file_data_offset + sizeof(float)*3)),
                                *((float*)(file_data_offset + sizeof(float)*4)),
                                *((float*)(file_data_offset + sizeof(float)*5))
                        },
                        .tex_coord = {
                                *((float*)(file_data_offset + sizeof(float)*6)),
                                *((float*)(file_data_offset + sizeof(float)*7))
                        }
                };
                file_data_offset += 8*sizeof(float);
        }


        // Index parsing
        for (uint32_t i = 0; i < mesh_data->indices_count; i++) {
                mesh_data->indices[i] = *((uint32_t*)(file_data_offset));
                file_data_offset += sizeof(uint32_t);
        }


        // Texture parsing
        for (
                uint32_t i = 0;
                i < (mesh_data->texture_width * mesh_data->texture_height);
                i++
        ) {
                mesh_data->texture_data[i] = S3D_Color {
                        .r = *((uint8_t*)(file_data_offset + sizeof(uint8_t)*0)),
                        .g = *((uint8_t*)(file_data_offset + sizeof(uint8_t)*1)),
                        .b = *((uint8_t*)(file_data_offset + sizeof(uint8_t)*2)),
                        .a = *((uint8_t*)(file_data_offset + sizeof(uint8_t)*3))
                };
                file_data_offset += 4*sizeof(uint8_t);
        }


        free(file_data);

        return mesh_data;
}

void S3D_Free(S3D_Mesh* mesh_data) {
        free(mesh_data->vertices);
        free(mesh_data->indices);
        free(mesh_data->texture_data);
        free(mesh_data);
}

#endif // _S3D_LOADER_H