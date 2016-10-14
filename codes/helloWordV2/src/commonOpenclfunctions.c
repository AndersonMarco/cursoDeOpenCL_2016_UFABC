#include "commonOpenclfunctions.h"
void createMsgErrorOpenCL(cl_int error,int mark, const char* functionName, const char* file){
    printf("Error  \n type: opencl (read the file cl.h for find error)\n code: %d\n line: %d\n function: %s\n file: %s\n", error, mark,functionName, file); \
}

int loadOpenclKernel(char **kernel, const char *file_name){
    FILE *fp;
    char ch;
    int i=0;  
    fp = fopen(file_name,"r"); checkFileOpen(fp,FILEOPEN_ERROR);
    fseek(fp, 0L, SEEK_END); checkReadWriteAtFile(fp,FILEREAD_ERROR);
    int sizeKernel=ftell(fp); checkReadWriteAtFile(fp,FILEREAD_ERROR);
    fseek(fp, 0L, SEEK_SET); checkReadWriteAtFile(fp,FILEREAD_ERROR);
    *kernel=(char *)malloc(sizeof(char)*(sizeKernel));    
    if(*kernel==0x0){
        fclose(fp);
        checkMalloc(*kernel);
    }
    while( ( ch = fgetc(fp) ) != EOF ){
        (*kernel)[i]=ch;
        i++;        
    }
    checkReadWriteAtFile(fp,FILEREAD_ERROR);
    
    (*kernel)[i]='\0';
    return 0;
}

int releaseBufferOpenCL(cl_mem buffer){
    cl_int ret=CL_SUCCESS;
    if(buffer != NULL) ret=clReleaseMemObject(buffer);
    return ret;
}

void getPlatformAndDevice(int idPlatform, int idDevice, cl_platform_id *platform, cl_device_id *device){
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_uint deviceCount;
    cl_device_id* devices;
        // get all platforms
    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);
    clGetDeviceIDs(platforms[idPlatform], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
    devices = (cl_device_id*) malloc(sizeof(cl_device_id) * deviceCount);
    clGetDeviceIDs(platforms[idPlatform], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);
    *platform=platforms[idPlatform];
    *device=devices[idDevice];
    free(devices);
    free(platforms);
}
void displayInfo(){
    unsigned int i, j;
    char* value;
    size_t valueSize;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_uint deviceCount;
    cl_device_id* devices;
    cl_uint maxComputeUnits;
 
    // get all platforms
    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);

    for (i = 0; i < platformCount; i++) {
        clGetPlatformInfo(platforms[i],CL_PLATFORM_NAME,0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        printf("====================================================\n");
        clGetPlatformInfo(platforms[i],CL_PLATFORM_NAME,valueSize, value, NULL);
        printf("%d. Plataform: %s\n", i, value);


        
        free(value);
        // get all devices
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);



        devices = (cl_device_id*) malloc(sizeof(cl_device_id) * deviceCount);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);

        
        // for each device print critical attributes
        for (j = 0; j < deviceCount; j++) {
 
            // print device name
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, NULL);
            printf("%d. Device: %s\n", j, value);
            free(value);
 
            // print hardware device version
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value, NULL);
            printf(" %d.%d Hardware version: %s\n", j+1, 1, value);
            free(value);
 
            // print software driver version
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value, NULL);
            printf(" %d.%d Software version: %s\n", j+1, 2, value);
            free(value);
 
            // print c version supported by compiler for device
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
            printf(" %d.%d OpenCL C version: %s\n", j, 3, value);
            free(value);
 
            // print parallel compute units
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,
                            sizeof(maxComputeUnits), &maxComputeUnits, NULL);
            printf(" %d.%d Parallel compute units: %d\n", j, 4, maxComputeUnits);
 
        }
        free(devices); 
    }
    free(platforms);
}
