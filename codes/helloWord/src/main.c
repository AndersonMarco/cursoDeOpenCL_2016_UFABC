#include <stdlib.h>

#include "errors.h"
#include "math.h"
#include "whereami/whereami.h"
#include "commonFunctions.h"


#include "KernelData.h"
#define freeCode()                                  \
    if(resourcePath) free(resourcePath);            \
    if(a_h) free(a_h);                              \
    if(b_h) free(b_h);                              \
    if(c_h) free(c_h);                              \
    if(kernelData) release__KernelData(kernelData); \
    if(kernelSrc) free(kernelSrc);                  \
    if(kernelPath) free(kernelPath);                \
    if(a_g) releaseBufferOpenCL(a_g);               \
    if(b_g) releaseBufferOpenCL(b_g);               \
    if(c_g) releaseBufferOpenCL(c_g);               

#define checkMallocMain(val)                                        \
    if(val==0x0){                                                 \
        createMsgError(MALLOC_ERROR,"malloc ",__LINE__,__func__,__FILE__);   \
        freeCode();                                                 \
        return 0;                                                   \
    }

#define checkCLMain(val)                                                \
    if(val!=CL_SUCCESS){                                                 \
        createMsgErrorOpenCL((int)val,__LINE__,__func__,__FILE__);         \
        freeCode();                                                     \
        return 0;                                                       \
    }

#define checkGenericMain(val)                                           \
    if(val!=0){                                                         \
        createMsgError(val,"generic error ",__LINE__,__func__,__FILE__); \
        freeCode();                                                     \
        return 0;                                                       \
    }

int main(void){
    int i;
    int vectorsLength=9000;
    int catchCode;
    
    int length, dirname_length;
    char *resourcePath=0x0;    


    char *kernelPath=0x0;
    char *kernelSrc=0x0;
    KernelData *kernelData;

    cl_mem a_g, b_g, c_g;
    float *a_h=0x0;
    float *b_h=0x0;
    float *c_h=0x0;


    int vectorsSize=sizeof(float)*vectorsLength;


    a_h=malloc(vectorsSize); checkMallocMain(a_h);
    b_h=malloc(vectorsSize); checkMallocMain(b_h);
    c_h=malloc(vectorsSize); checkMallocMain(c_h);
    for(i=0;i<vectorsLength;i++){
        a_h[i]=sqrtf((float)i+1);
        b_h[i]=logf((float)i+1);
    }
    
    
    length = wai_getExecutablePath(NULL, 0, &dirname_length); 
	resourcePath = (char *)malloc(sizeof(char)*length); checkMallocMain(resourcePath);
    wai_getExecutablePath(resourcePath, length, &dirname_length);
	resourcePath[dirname_length] = '\0';

    
#ifdef _WIN32
    kernelPath=concatStrings(resourcePath,"\\openclCodes\\add.cl"); checkMallocMain(kernelPath);
    
#else
    kernelPath=concatStrings(resourcePath,"/openclCodes/add.cl"); checkMallocMain(kernelPath);
#endif

    
    
    catchCode=loadOpenclKernel(&kernelSrc,kernelPath); checkGenericMain(catchCode); checkMallocMain(kernelSrc);


    cl_platform_id platform;
    cl_device_id device;
    displayInfo();
    getPlatformAndDevice(0, 0, &platform, &device);
    kernelData= new__KernelData(kernelSrc,"add",platform,device,&catchCode);
    checkCLMain(catchCode); checkGenericMain(catchCode); checkMallocMain(kernelData);

    
    cl_context context=getContext__KernelData(kernelData);
    cl_command_queue command_queue= getCommand_queue__KernelData(kernelData);
    a_g=clCreateBuffer(context, CL_MEM_READ_ONLY,vectorsSize, NULL, &catchCode);checkCLMain(catchCode);
    b_g=clCreateBuffer(context, CL_MEM_READ_ONLY,vectorsSize, NULL, &catchCode);checkCLMain(catchCode);
    c_g=clCreateBuffer(context, CL_MEM_READ_WRITE,vectorsSize, NULL, &catchCode);checkCLMain(catchCode);


    
    catchCode=clEnqueueWriteBuffer(command_queue, a_g, CL_TRUE, 0,vectorsSize, a_h, 0, NULL, NULL); checkCLMain(catchCode);
    catchCode=clEnqueueWriteBuffer(command_queue, b_g, CL_TRUE, 0,vectorsSize, b_h, 0, NULL, NULL); checkCLMain(catchCode);

    
    cl_kernel kernel=getKernel__KernelData(kernelData);    
    catchCode = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_g);checkCLMain(catchCode);
    catchCode = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_g);checkCLMain(catchCode);
    catchCode = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_g);checkCLMain(catchCode);
    size_t global_item_size = vectorsLength; // Process the entire lists       
    catchCode = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, NULL, 0, NULL, NULL);checkCLMain(catchCode);
     catchCode=clEnqueueReadBuffer(command_queue,c_g,CL_TRUE,0,vectorsSize,c_h,0,NULL,NULL);checkCLMain(catchCode);

     freeCode();

}
