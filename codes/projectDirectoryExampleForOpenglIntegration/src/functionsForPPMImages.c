#include "functionsForPPMImages.h"
imagePPM load_image(char *srcPath){
    int i;
    FILE* pFile = fopen ( srcPath , "rb" );
    fseek(pFile, 0L, SEEK_END);
    int lengthBuffer = ftell(pFile);
    fseek(pFile, 0L, SEEK_SET);
    char *buffer= (char *)malloc(lengthBuffer*sizeof(char));
    fread (buffer,1,lengthBuffer,pFile);   
    fclose(pFile);

    int lineIsACommentary=0;
    char valueStr[4];
    int pointToValueStr=0;
    int currentLine=0;
    int valuesThatYouHaveRead=0;
    imagePPM imageRet;
    int pointTo_ImageRet_pixel_data_rgba=0;
    
    for(i=0;i<lengthBuffer;i++){
        if(buffer[i]=='#'){
            lineIsACommentary=1;
        }
        else if(lineIsACommentary==1  && buffer[i]=='\n'){
            lineIsACommentary=0;
        }        
        else if((buffer[i]==' '  || buffer[i]=='\n'  || buffer[i]==9 || buffer[i]==13 || buffer[i]==10) && lineIsACommentary==0){
            if(pointToValueStr>0){
                valueStr[pointToValueStr]='\0';
                valuesThatYouHaveRead++;
                if(valuesThatYouHaveRead>1){
                    int value=atoi(valueStr);                     
                    if(valuesThatYouHaveRead>3){
                                               
                        imageRet.pixel_data_rgba[pointTo_ImageRet_pixel_data_rgba]=(uint8_t)value;

                        if((valuesThatYouHaveRead-4)%3==2 ){
                            pointTo_ImageRet_pixel_data_rgba++;                        
                            imageRet.pixel_data_rgba[pointTo_ImageRet_pixel_data_rgba]=UINT8_C(255);


                        }
                        pointTo_ImageRet_pixel_data_rgba++;
   
                    }
                    else if(valuesThatYouHaveRead==2)
                        imageRet.width=value;
                    else if(valuesThatYouHaveRead==3){
                        imageRet.height=value;
                        imageRet.pixel_data_rgba=(uint8_t *)malloc(sizeof(uint8_t)*imageRet.width*imageRet.height*4);
                    }
                            
                }
                pointToValueStr=0;               
            }
            if(buffer[i]=='\n')
                currentLine++;
        }
        else if(lineIsACommentary==0){
            valueStr[pointToValueStr]=buffer[i];
            pointToValueStr++;
        }
    }
    
    for(i=0;i<pointTo_ImageRet_pixel_data_rgba/4;i++){
        uint8_t aux=imageRet.pixel_data_rgba[4*i+1];
        imageRet.pixel_data_rgba[4*i+1]=imageRet.pixel_data_rgba[4*i];
        imageRet.pixel_data_rgba[4*i]=aux;
    }
    return imageRet;

}
