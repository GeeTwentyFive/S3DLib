#include "S3DWriter.h"
#include "S3DLoader.h"

#include <stdio.h>


#define VERTICES_COUNT 4
S3D_Vertex vertices[VERTICES_COUNT] = {
        {
                .position = {0.5, 0.5, 0.0},
                .normal = {1.0, 0.0, 0.0},
                .tex_coord = {1.0, 0.0}
        },
        {
                .position = {0.5, -0.5, 0.0},
                .normal = {0.0, 1.0, 0.0},
                .tex_coord = {1.0, 1.0}
        },
        {
                .position = {-0.5, -0.5, 0.0},
                .normal = {-1.0, 0.0, 0.0},
                .tex_coord = {0.0, 1.0}
        },
        {
                .position = {-0.5, 0.5, 0.0},
                .normal = {0.0, -1.0, 0.0},
                .tex_coord = {0.0, 0.0}
        }
};

#define INDICES_COUNT 6
uint32_t indices[INDICES_COUNT] = {
        0, 1, 3,
        1, 2, 3
};

#define TEXTURE_WIDTH 2
#define TEXTURE_HEIGHT 2
S3D_Color texture_data[TEXTURE_WIDTH * TEXTURE_HEIGHT] = {
        {0xFF, 0x00, 0x00, 0xFF}, {0x00, 0xFF, 0x00, 0xFF},
        {0x00, 0x00, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
};

S3D_Mesh mesh = {
        .vertices = (S3D_Vertex*) &vertices,
        .vertices_count = VERTICES_COUNT,

        .indices = (uint32_t*) &indices,
        .indices_count = INDICES_COUNT,

        .texture_width = TEXTURE_WIDTH,
        .texture_height = TEXTURE_HEIGHT,
        .texture_data = (S3D_Color*) &texture_data
};

int main() {
        S3D_Write_ERROR err = S3D_Write(&mesh, "EXAMPLE.s3d");
        if (err != S3D_WRITE_SUCCESS) {
                printf("S3D writing failed with error code %d\n", err);
                return 1;
        }
        puts("Written EXAMPLE.s3d");

        S3D_Mesh* loaded_mesh_data = S3D_Load("EXAMPLE.s3d");
        if (!loaded_mesh_data) {
                puts("Failed to load EXAMPLE.s3d");
                return 1;
        }
        puts("Loaded EXAMPLE.s3d");

        printf(
                "Vertices count: %d\n"
                "Indices count: %d\n"
                "Texture width: %d\n"
                "Texture height: %d\n"
                "\n",

                loaded_mesh_data->vertices_count,
                loaded_mesh_data->indices_count,
                loaded_mesh_data->texture_width,
                loaded_mesh_data->texture_height
        );

        puts("Vertices:");
        for (uint32_t i = 0; i < loaded_mesh_data->vertices_count; i++) {
                printf(
                        "position X: %f\n"
                        "position Y: %f\n"
                        "position Z: %f\n"
                        "normal X: %f\n"
                        "normal Y: %f\n"
                        "normal Z: %f\n"
                        "texcoord U: %f\n"
                        "texcoord V: %f\n"
                        "\n",

                        loaded_mesh_data->vertices[i].position.x,
                        loaded_mesh_data->vertices[i].position.y,
                        loaded_mesh_data->vertices[i].position.z,

                        loaded_mesh_data->vertices[i].normal.x,
                        loaded_mesh_data->vertices[i].normal.y,
                        loaded_mesh_data->vertices[i].normal.z,

                        loaded_mesh_data->vertices[i].tex_coord.u,
                        loaded_mesh_data->vertices[i].tex_coord.v
                );
        }

        puts("Indices:");
        for (uint32_t i = 0; i < loaded_mesh_data->indices_count; i++) {
                printf("%d\n", loaded_mesh_data->indices[i]);
        }

        puts("Texture data:");
        for (
                uint32_t i = 0;
                i < (loaded_mesh_data->texture_width * loaded_mesh_data->texture_height);
                i++
        ) {
                printf(
                        "R: %d\n"
                        "G: %d\n"
                        "B: %d\n"
                        "A: %d\n"
                        "\n",

                        loaded_mesh_data->texture_data[i].r,
                        loaded_mesh_data->texture_data[i].g,
                        loaded_mesh_data->texture_data[i].b,
                        loaded_mesh_data->texture_data[i].a
                );
        }

        S3D_Free(loaded_mesh_data);

        return 0;
}