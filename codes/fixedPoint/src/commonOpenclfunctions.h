#ifndef commonOpenclfunctions_h
#define commonOpenclfunctions_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <OpenCL/cl_gl.h>
#else
#include <CL/cl.h>
#include <CL/cl_gl.h>
#endif
#include "errors.h"

/**
 * @brief Abort function execution if ret!= CL_SUCESS, 
 * in other words if a error happens. 
 *
 * This macro is used for check, if return code of 
 * the functions that use Opencl calls is other 
 * than CL_SUCCESS the macro will abort the
 * execution of the functions that call the functions
 * with Opencl calls. A message  with detail of the 
 * error is printed on standart output if a error happens.
 */
#define checkCL(ret)\
    if(ret!=CL_SUCCESS){\
        createMsgErrorOpenCL(ret,__LINE__,__func__,__FILE__); \
        return ret;\
    }

/**
 * @brief Abort function execution if ret!= CL_SUCESS, 
 * in other words if a error happens. 
 *
 * This macro is used for check, if return code of 
 * the functions that use Opencl calls is other 
 * than CL_SUCCESS the macro will abort the
 * execution of the functions that call the functions
 * with Opencl calls. A message  with detail of the 
 * error is printed on standart output if a error happens.
 */
#define checkCLReturnValue(val,ret)                \
    if(val!=CL_SUCCESS){\
        createMsgErrorOpenCL(val,__LINE__,__func__,__FILE__); \
        return ret;\
    }

/**
 * @brief Abort function execution if ret!= CL_SUCESS, 
 * in other words if a error happens. 
 *
 * This macro is used for check, if return code of 
 * the functions that use Opencl calls is other 
 * than CL_SUCCESS the macro will abort the
 * execution of the functions that call the functions
 * with Opencl calls. A message  with detail of the 
 * error is printed on standart output if a error happens.
 */
#define checkCLReturnVoid(ret)                       \
    if(ret!=CL_SUCCESS){                                               \
        createMsgErrorOpenCL(ret,__LINE__,__func__,__FILE__);          \
        return;                                                        \
    }

/**
 * @brief Print a error on standart output.
 * @param error error code 
 * @param mark generally used for notify the line with error.
 * @param functionName function where the error happens
 * @param file file source where there is the function where the error happens.
*/
void createMsgErrorOpenCL(cl_int error,int mark, const char* functionName, const char* file);

/**
 * @brief load a opencl code at file to a string.
 * The maximum size, in bytes, for opencl code is defined
 * by kernelSourceSize.
 * @param kernel opencl code that is loaded from file_name
 * @param file_name path to opencl code file.
 * @return  0 if the function execute whithout error and different of zero otherwise. Read error.h for a error list.
*/
int loadOpenclKernel(char **kernel, const char *file_name);

/**
 * @brief "free" at a opencl buffer
 * @param buffer buffer to release
 * @return 0 if the function execute whithout error and different of zero otherwise. Read error.h or cl.h for a error list.
 */
int releaseBufferOpenCL(cl_mem buffer);

/**
 * @brief get a platform and device from IDs for platform and device
 * @param idPlatform id to opencl platform
 * @param idDevice id to opencl device
 * @param platform opencl platform returned
 * @param device opencl device returned
 * @return 0 if the function execute whithout error and different of zero otherwise. Read error.h or cl.h for a error list.
 */
void getPlatformAndDevice(int idPlatform, int idDevice, cl_platform_id *platform, cl_device_id *device);

void getPlatformAndDeviceWithOpe(int idPlatform, int idDevice, cl_platform_id *platform, cl_device_id *device);

/**
 *@brief output information about opencl device in the machine.
 * Print on standart output  a list of opencl devices and platforms 
 */
void displayInfo();
#endif
