#include <stdlib.h>

#include "errors.h"
#include "math.h"
#include "whereami/whereami.h"
#include "commonFunctions.h"
#include "functionsForPPMImages.h"

#include "shader_utils.h"
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/freeglut.h>
/* GLM */
// #define GLM_MESSAGES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KernelData.h"


int screen_width=800, screen_height=600;


GLuint vbo_cube_vertices, vbo_cube_texcoords;
GLuint ibo_cube_elements;
GLuint program;
GLuint texture_id;
GLint attribute_coord3d, attribute_texcoord;
GLint uniform_mvp, uniform_mytexture;


cl_int onDisplayCatchCode=0;


char* resourcePath = 0x0;


KernelData *kernelData=0x0;
cl_mem pointerToopenGLTexture=0x0;
cl_mem pointToGLVertex=0x0;


int callOpencl();


int init_resources_generics(){
    int length, dirname_length;
    length = wai_getExecutablePath(NULL, 0, &dirname_length); 
	resourcePath = (char *)malloc(sizeof(char)*length); 
    wai_getExecutablePath(resourcePath, length, &dirname_length);
	resourcePath[dirname_length] = '\0';
    return 1;
}


int fill_textures(){
    char *pathToImageTexture=0x0;
#ifdef _WIN32
    pathToImageTexture=concatStrings(resourcePath,"\\images\\HatarakuMaouSama.ppm");
#else
    pathToImageTexture=concatStrings(resourcePath,"/images/HatarakuMaouSama.ppm");
#endif
    imagePPM imageText=load_image(pathToImageTexture);
    GLfloat cube_texcoords[2*4*6] = {
        // front
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
    };
    for (int i = 1; i < 6; i++)
        memcpy(&cube_texcoords[i*4*2], &cube_texcoords[0], 2*4*sizeof(GLfloat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, // target
                 0,  // level, 0 = base, no minimap,
                 GL_RGBA, // internalformat
                 imageText.width,  // width
                 imageText.height,  // height
                 0,  // border, always 0 in OpenGL ES
                 GL_RGBA,  // format
                 GL_UNSIGNED_BYTE, // type
                 imageText.pixel_data_rgba);


    return 0;
}
int fill_vbos(){    
    GLfloat cube_vertices[] = {
        // front
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // top
        -1.0,  1.0,  1.0,
        1.0,  1.0,  1.0,
        1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
        // back
        1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,
        -1.0,  1.0, -1.0,
        1.0,  1.0, -1.0,
        // bottom
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0, -1.0,  1.0,
        -1.0, -1.0,  1.0,
        // left
        -1.0, -1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        // right
        1.0, -1.0,  1.0,
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
        1.0,  1.0,  1.0,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

           
    GLushort cube_elements[] = {
        // front
        0,  1,  2,
        2,  3,  0,
        // top
        4,  5,  6,
        6,  7,  4,
        // back
        8,  9, 10,
        10, 11,  8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20,
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

    
    
    return 0;   
}

int init_resources_opengl()
{

    glGenBuffers(1, &vbo_cube_vertices);
    glGenBuffers(1, &vbo_cube_texcoords);
    glGenBuffers(1, &ibo_cube_elements);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  

    GLint link_ok = GL_FALSE;

    GLuint vs, fs;
    char *pathToFragShader=0x0;
    char *pathToVertexShader=0x0; 
#ifdef _WIN32
    pathToVertexShader=concatStrings(resourcePath,"\\shaders\\cube.v.glsl");
    pathToFragShader=concatStrings(resourcePath,"\\shaders\\cube.f.glsl");
#else
    pathToVertexShader=concatStrings(resourcePath,"/shaders/cube.v.glsl");
    pathToFragShader=concatStrings(resourcePath,"/shaders/cube.f.glsl");
#endif

    if ((vs = create_shader(pathToVertexShader, GL_VERTEX_SHADER))   == 0) return 0;
    if ((fs = create_shader(pathToFragShader, GL_FRAGMENT_SHADER)) == 0) return 0;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        fprintf(stderr, "glLinkProgram:");
        return 0;
    }

    const char* attribute_name;
    attribute_name = "coord3d";
    attribute_coord3d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord3d == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }
    attribute_name = "texcoord";
    attribute_texcoord = glGetAttribLocation(program, attribute_name);
    if (attribute_texcoord == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }
    const char* uniform_name;
    uniform_name = "mvp";
    uniform_mvp = glGetUniformLocation(program, uniform_name);
    if (uniform_mvp == -1) {
        fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
        return 0;
    }
    uniform_name = "mytexture";
    uniform_mytexture = glGetUniformLocation(program, uniform_name);
    if (uniform_mytexture == -1) {
        fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
        return 0;
    }
    glFinish();
    return 1;
}

void onIdle() {
    float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * glm::radians(15.0);  // base 15° per second
    glm::mat4 anim = \
        glm::rotate(glm::mat4(1.0f), angle*3.0f, glm::vec3(1, 0, 0)) *  // X axis
        glm::rotate(glm::mat4(1.0f), angle*2.0f, glm::vec3(0, 1, 0)) *  // Y axis
        glm::rotate(glm::mat4(1.0f), angle*4.0f, glm::vec3(0, 0, 1));   // Z axis

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);

    glm::mat4 mvp = projection * view * model * anim;
    glUseProgram(program);
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glutPostRedisplay();
}

void onDisplay()
{
	onDisplayCatchCode = callOpencl();
	if(onDisplayCatchCode != CL_SUCCESS) {
		exit(0);
	}

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);

    glEnableVertexAttribArray(attribute_coord3d);
    // Describe our vertices array to OpenGL (it can not guess its format automatically)
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glVertexAttribPointer(
                          attribute_coord3d, // attribute
                          3,                 // number of elements per vertex, here (x,y,z)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          0,                 // no extra data between each position
                          0                  // offset of first element
                          );

    glEnableVertexAttribArray(attribute_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
    glVertexAttribPointer(
                          attribute_texcoord, // attribute
                          2,                  // number of elements per vertex, here (x,y)
                          GL_FLOAT,           // the type of each element
                          GL_FALSE,           // take our values as-is
                          0,                  // no extra data between each position
                          0                   // offset of first element
                          );

    /* Push each element in buffer_vertices to the vertex shader */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_texcoord);
    glutSwapBuffers();
    glFinish();
}

void onReshape(int width, int height) {
    screen_width = width;
    screen_height = height;
    glViewport(0, 0, screen_width, screen_height);
}

void free_resources_opengl()
{
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_cube_vertices);
    glDeleteBuffers(1, &vbo_cube_texcoords);
    glDeleteBuffers(1, &ibo_cube_elements);
    glDeleteTextures(1, &texture_id);
}
int init_resources_opencl(){
    int catchCode=0;
    char *kernelPath=0x0;
    char *kernelSrc=0x0;


#ifdef _WIN32
    kernelPath=concatStrings(resourcePath,"\\openclCodes\\map.cl"); 
#else
    kernelPath=concatStrings(resourcePath,"/openclCodes/map.cl");
#endif

    catchCode=loadOpenclKernel(&kernelSrc,kernelPath);
    //checkGenericMain(catchCode); checkMallocMain(kernelSrc);


    cl_platform_id platform;
    cl_device_id device;
    getPlatformAndDevice(0, 0, &platform, &device);
    kernelData= new_5__KernelData(kernelSrc,"map",platform,&catchCode);
    checkCL(catchCode);

    pointerToopenGLTexture= clCreateFromGLTexture(getContext__KernelData(kernelData), CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0,texture_id,&catchCode);
    checkCL(catchCode);
    pointToGLVertex=clCreateFromGLBuffer(getContext__KernelData(kernelData),CL_MEM_READ_WRITE,vbo_cube_vertices,NULL);
    checkCL(catchCode);

	return CL_SUCCESS;

}
void free_ressource_opencl() {
	if(pointerToopenGLTexture!=0x0) releaseBufferOpenCL(pointerToopenGLTexture);
	pointerToopenGLTexture = 0x0;
	if(pointToGLVertex != 0x0) releaseBufferOpenCL(pointToGLVertex);
	pointToGLVertex = 0x0;
	if(kernelData) release__KernelData(kernelData);    
	kernelData = 0x0;
}
int callOpencl(){
    int catchCode;
        
    

    cl_command_queue command_queue=getCommand_queue__KernelData(kernelData);
    cl_kernel kernel=getKernel__KernelData(kernelData);
    cl_context context=getContext__KernelData(kernelData);
    

    catchCode=clEnqueueAcquireGLObjects(command_queue,1,&pointerToopenGLTexture,0,0x0,0x0); checkCL(catchCode);
    
    catchCode = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&pointerToopenGLTexture);
    catchCode = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&pointToGLVertex);
    size_t global_item_size = 16384; // Process the entire lists       
    clEnqueueNDRangeKernel(command_queue, kernel, 1, 0x0, &global_item_size, 0x0, 0, 0x0, 0x0);
    
    clFinish(command_queue);
    return 0;
}
int main(int argc, char *argv[]){
    displayInfo();
    init_resources_generics();
    int catchCode=0;        

    
    glutInit(&argc, argv);
    glutInitContextVersion(2,0);
    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(screen_width, screen_height);
    glutCreateWindow("My Textured Cube");

    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return 1;
    }

    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
        return 1;
    }
  

    if (init_resources_opengl()) {
		
        glutDisplayFunc(onDisplay);
        glutReshapeFunc(onReshape);
        glutIdleFunc(onIdle);
        fill_vbos();
        fill_textures();
		if (init_resources_opencl() != 0x0) {
			free_ressource_opencl();
        }

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glutMainLoop();
    }

    free_resources_opengl();    
	free_ressource_opencl();
    return 0;
}

