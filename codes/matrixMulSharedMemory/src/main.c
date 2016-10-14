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

#define checkMallocMain(val)                                            \
    if(val==0x0){                                                       \
        createMsgError(MALLOC_ERROR,"malloc ",__LINE__,__func__,__FILE__); \
        freeCode();                                                     \
        return 0;                                                       \
    }

#define checkCLMain(val)                                                \
    if(val!=CL_SUCCESS){                                                \
        createMsgErrorOpenCL((int)val,__LINE__,__func__,__FILE__);      \
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
    int i,j,k;

    int nRows=1200;
    int nColumns=1200;
    int vectorsLength=nRows*nColumns;
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
    float *cTest_h=0x0;


    int vectorsSize=sizeof(float)*vectorsLength;


    a_h=malloc(vectorsSize); checkMallocMain(a_h);
    b_h=malloc(vectorsSize); checkMallocMain(b_h);
    c_h=malloc(vectorsSize); checkMallocMain(c_h);
    cTest_h=malloc(vectorsSize); checkMallocMain(cTest_h);
    for(i=0;i<vectorsLength;i++){
        //a_h[i]=sqrtf((float)i+1);
        //b_h[i]=logf((float)i+1);
        a_h[i]=((double)(rand()%100000))/10000.0;
        b_h[i]=((double)(rand()%100000))/10000.0;
    }
    
    
    length = wai_getExecutablePath(NULL, 0, &dirname_length); 
	resourcePath = (char *)malloc(sizeof(char)*length); checkMallocMain(resourcePath);
    wai_getExecutablePath(resourcePath, length, &dirname_length);
	resourcePath[dirname_length] = '\0';

    
#ifdef _WIN32
    kernelPath=concatStrings(resourcePath,"\\openclCodes\\matmul.cl"); checkMallocMain(kernelPath);
    
#else
    kernelPath=concatStrings(resourcePath,"/openclCodes/matmul.cl"); checkMallocMain(kernelPath);
#endif

    
    catchCode=loadOpenclKernel(&kernelSrc,kernelPath); checkGenericMain(catchCode); checkMallocMain(kernelSrc);


    cl_platform_id platform;
    cl_device_id device;
    displayInfo();
    getPlatformAndDevice(0, 1, &platform, &device);
    kernelData= new__KernelData(kernelSrc,"mult",platform,device,&catchCode);
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

    catchCode = clSetKernelArg(kernel, 3, 64 * sizeof(float), NULL);checkCLMain(catchCode);
    catchCode = clSetKernelArg(kernel, 4, sizeof(int), (void *)&nColumns);checkCLMain(catchCode);
    catchCode = clSetKernelArg(kernel, 5, sizeof(int), (void *)&nRows);checkCLMain(catchCode);
    catchCode = clSetKernelArg(kernel, 6, sizeof(int), (void *)&nColumns);checkCLMain(catchCode);
    catchCode = clSetKernelArg(kernel, 7, sizeof(int), (void *)&nRows);checkCLMain(catchCode);
    size_t global_item_size = 4096; // Process the entire lists
    size_t local_item_size = 64;
    catchCode = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);checkCLMain(catchCode);
     catchCode=clEnqueueReadBuffer(command_queue,c_g,CL_TRUE,0,vectorsSize,c_h,0,NULL,NULL);checkCLMain(catchCode);

     for(i=0;i<nRows;i++){
           for(j=0;j<nColumns;j++){
               float sum=0.0f;
               for(k=(nRows-1);k>=0;k--){
                   sum=sum+(a_h[i*nColumns+k]*b_h[k*nColumns+j]);
               }
               cTest_h[i*nColumns+j]=sum;

           }
       }

     for (i=0;i<vectorsLength;i++){
	 if(c_h[i]!=cTest_h[i])
	     printf("%f || %f  ||%f  || %f\n",a_h[i],b_h[i], c_h[i],cTest_h[i]);
     }
     freeCode();

}
