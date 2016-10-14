#include "KernelData.h"
int compileKernel__kernelData(KernelData *self, const char *srcKernel, const char *functionOpencl){
    cl_int catchCodeCode;
     if(self->kernel){
        catchCodeCode = clReleaseKernel(self->kernel);checkCL(catchCodeCode);
    }
    if(self->program){
        catchCodeCode = clReleaseProgram(self->program);checkCL(catchCodeCode);
    }
    self->program=0x0;
    self->kernel=0x0;
    
    //Create a program from the kernel source===================================================
    self->program = clCreateProgramWithSource(self->context, 1, (const char **)&srcKernel,NULL, &catchCodeCode);
    checkCL(catchCodeCode);
     //End=======================================================================================

    //Build the program==========================================================================
    

    catchCodeCode = clBuildProgram(self->program, 1, &self->device_id, NULL, NULL, NULL);
    size_t logSize=0;
    clGetProgramBuildInfo(self->program, self->device_id,CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
    if(catchCodeCode !=CL_SUCCESS && logSize >0){
        clGetProgramBuildInfo(self->program, self->device_id,CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
    
        char *srcLog=(char *)malloc(sizeof(char)*logSize);
        clGetProgramBuildInfo(self->program, self->device_id, CL_PROGRAM_BUILD_LOG, logSize, srcLog, NULL);
        printf("*********************Error in opencl Kernel*********************\n\n");
        printf("%s \n", srcLog);
        printf("******************************END*******************************\n");
    }
    checkCL(catchCodeCode);
    //End========================================================================================
    
    //Create the OpenCL kernel===================================================================
    self->kernel = clCreateKernel(self->program, functionOpencl, &catchCodeCode);
    checkCL(catchCodeCode);
    //End=========================================================================n===============
    return catchCodeCode;
}

int auxForConstructor__KernelData(KernelData *self,cl_platform_id platforms, cl_device_id device_id){
    self->context=NULL;
    self->device_id=device_id;
    self->command_queue=NULL;
    self->program=NULL;
    self->kernel=NULL;
    cl_int catchCode;
    
    //Create an OpenCL context==================================================================
    self->context= clCreateContext( NULL, 1, &device_id, NULL, NULL, &catchCode);
    checkCL(catchCode);
    //End=======================================================================================

    //Create a command queue====================================================================
    self->command_queue = clCreateCommandQueue(self->context, device_id, 0, &catchCode);    
    checkCL(catchCode);
    //End=======================================================================================
    
    self->kernelCreatedFromAnotherKernel=0;
    return catchCode;
}

cl_device_id getDevice_id__kernelData(KernelData *self){
    return self->device_id;
}

cl_context getContext__KernelData(KernelData *self){
    return self->context;
}

cl_kernel  getKernel__KernelData(KernelData *self){
    return self->kernel;
}
cl_program getProgram__KernelData(KernelData *self){
    return self->program;
}

cl_command_queue getCommand_queue__KernelData(KernelData *self){
    return self->command_queue;
}

KernelData* new_4__KernelData(cl_context context, cl_command_queue command_queue,cl_device_id device_id,char *srcKernel, const char *functionOpencl, cl_int *catchCode){
    KernelData *self=(KernelData *)malloc(sizeof(KernelData)); checkMallocReturnPointer(self,catchCode);
    self->context=context;
    self->command_queue=command_queue;
    self->program=NULL;
    self->kernel=NULL;
    self->device_id=device_id;
    *catchCode=compileKernel__kernelData(self,srcKernel,functionOpencl); checkCLReturnValue(*catchCode,self);
    self->kernelCreatedFromAnotherKernel=1;
    return self;
}

KernelData* new_3__KernelData(cl_context context, cl_command_queue command_queue, cl_device_id device_id, int *catchCode){
    KernelData *self=(KernelData *)malloc(sizeof(KernelData)); checkMallocReturnPointer(self,catchCode);
    self->device_id=device_id;
    self->context=context;
    self->command_queue=command_queue;
    self->program=NULL;
    self->kernel=NULL;
    self->kernelCreatedFromAnotherKernel=1;
    return self;      
}



KernelData* new_2__KernelData(cl_platform_id platforms, cl_device_id device_id, cl_int *catchCode){
    KernelData *self=(KernelData *)malloc(sizeof(KernelData)); checkMallocReturnPointer(self,catchCode);
    *catchCode=auxForConstructor__KernelData(self,platforms,device_id); checkCLReturnValue(*catchCode,self);
    return self;
}

KernelData* new__KernelData(char *srcKernel,const char* functionOpencl, cl_platform_id platforms, cl_device_id device_id, cl_int *catchCode){
    KernelData *self=(KernelData *)malloc(sizeof(KernelData)); checkMallocReturnPointer(self,catchCode);
    size_t source_size=strlen(srcKernel); 
    *catchCode=auxForConstructor__KernelData(self,platforms,device_id); checkCLReturnValue(*catchCode,self);
    *catchCode=compileKernel__kernelData(self,srcKernel,functionOpencl); checkCLReturnValue(*catchCode,self);    
    self->kernelCreatedFromAnotherKernel=0;
    return self;
}

int release__KernelData(KernelData *self){
    cl_int catchCode=CL_SUCCESS;
    if(self->kernel){
        catchCode = clReleaseKernel(self->kernel);
        
    }
    if(self->program){
        catchCode = clReleaseProgram(self->program);
        
    }
    if(self->command_queue && !self->kernelCreatedFromAnotherKernel){
        catchCode = clReleaseCommandQueue(self->command_queue);
    }
    if(self->context && !self->kernelCreatedFromAnotherKernel){
        catchCode = clReleaseContext(self->context);       
    }
    free(self);
    return catchCode;
}
