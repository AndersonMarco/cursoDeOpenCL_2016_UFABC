#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#ifndef functionsForPPMImages_hpp
#define functionsForPPMImages_hpp


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t *pixel_data_rgba;
    int height;
    int width;
} imagePPM;
imagePPM load_image(char *srcPath);


#ifdef __cplusplus
}
#endif
   
#endif
