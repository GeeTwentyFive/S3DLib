https://github.com/GeeTwentyFive/Simple3DFileFormat


# Loader usage
```c
S3D_Mesh* mesh_data = S3D_Load("PATH/TO/MODEL.s3d");
if (!mesh_data) {
        puts("ERROR: Failed to load mesh");
        exit(1);
}

// use mesh_data...

S3D_Free(mesh_data);
```


# Writer usage
```c
S3D_Write_ERROR err = S3D_Write(&mesh, "PATH/TO/OUT.s3d");
if (err != S3D_WRITE_SUCCESS) {
        printf("S3D writing failed with error code %d\n", err);
        exit(1);
}
```