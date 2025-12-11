#include "S3DWriter.h"
#include "S3DLoader.h"

#include <stdio.h>


#define VERTICES_COUNT 4
S3D_Vertex vertices[VERTICES_COUNT] = {
        {
                .position = {0.5, 0.5, 0.0},
                .normal = {1.0, 0.0, 0.0},
                .color_RGB = {1.0, 0.0, 0.0}
        },
        {
                .position = {0.5, -0.5, 0.0},
                .normal = {0.0, 1.0, 0.0},
                .color_RGB = {0.0, 1.0, 0.0}
        },
        {
                .position = {-0.5, -0.5, 0.0},
                .normal = {-1.0, 0.0, 0.0},
                .color_RGB = {0.0, 0.0, 1.0}
        },
        {
                .position = {-0.5, 0.5, 0.0},
                .normal = {0.0, -1.0, 0.0},
                .color_RGB = {1.0, 1.0, 1.0}
        }
};

#define INDICES_COUNT 6
uint32_t indices[INDICES_COUNT] = {
        0, 1, 3,
        1, 2, 3
};

S3D_Mesh mesh = {
        .vertices = (S3D_Vertex*) &vertices,
        .vertices_count = VERTICES_COUNT,

        .indices = (uint32_t*) &indices,
        .indices_count = INDICES_COUNT,
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
                "\n"
                "Vertices count: %d\n"
                "Indices count: %d\n"
                "\n",

                loaded_mesh_data->vertices_count,
                loaded_mesh_data->indices_count
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
                        "color R: %f\n"
                        "color G: %f\n"
                        "color B: %f\n"
                        "\n",

                        loaded_mesh_data->vertices[i].position.x,
                        loaded_mesh_data->vertices[i].position.y,
                        loaded_mesh_data->vertices[i].position.z,

                        loaded_mesh_data->vertices[i].normal.x,
                        loaded_mesh_data->vertices[i].normal.y,
                        loaded_mesh_data->vertices[i].normal.z,

                        loaded_mesh_data->vertices[i].color_RGB.x,
                        loaded_mesh_data->vertices[i].color_RGB.y,
                        loaded_mesh_data->vertices[i].color_RGB.z
                );
        }

        puts("Indices:");
        for (uint32_t i = 0; i < loaded_mesh_data->indices_count; i += 3) {
                printf(
                        "%d %d %d\n",

                        loaded_mesh_data->indices[i],
                        loaded_mesh_data->indices[i + 1],
                        loaded_mesh_data->indices[i + 2]
                );
        }

        S3D_Free(loaded_mesh_data);


        return 0;
}