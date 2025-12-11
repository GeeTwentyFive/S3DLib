#ifndef _S3D_LOADER_H
#define _S3D_LOADER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef _S3D_TYPES
#define _S3D_TYPES

typedef struct {
        float x;
        float y;
        float z;
} S3D_Vec3;

typedef struct {
        S3D_Vec3 position;
        S3D_Vec3 normal;
        S3D_Vec3 color_RGB;
} S3D_Vertex;

typedef struct {
        S3D_Vertex* vertices;
        uint32_t vertices_count;

        uint32_t* indices;
        uint32_t indices_count;
} S3D_Mesh;

#endif // _S3D_TYPES


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


        // Vertex parsing
        for (uint32_t i = 0; i < mesh_data->vertices_count; i++) {
                mesh_data->vertices[i] = (S3D_Vertex) {
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
                        .color_RGB = {
                                *((float*)(file_data_offset + sizeof(float)*6)),
                                *((float*)(file_data_offset + sizeof(float)*7)),
                                *((float*)(file_data_offset + sizeof(float)*8))
                        }
                };
                file_data_offset += 9*sizeof(float);
        }


        // Index parsing
        for (uint32_t i = 0; i < mesh_data->indices_count; i++) {
                mesh_data->indices[i] = *((uint32_t*)(file_data_offset));
                file_data_offset += sizeof(uint32_t);
        }


        free(file_data);

        return mesh_data;
}

void S3D_Free(S3D_Mesh* mesh_data) {
        free(mesh_data->vertices);
        free(mesh_data->indices);
        free(mesh_data);
}

#endif // _S3D_LOADER_H