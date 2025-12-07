https://github.com/GeeTwentyFive/Simple3DFileFormat


# Usage
```c
S3D_Mesh* mesh_data = S3D_Load("PATH/TO/MODEL.s3d");
if (!mesh_data) {
        puts("ERROR: Failed to load mesh");
        exit(1);
}

// use mesh_data...

S3D_Free(mesh_data);
```