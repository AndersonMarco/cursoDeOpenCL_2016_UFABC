#include "commonFunctions.h"
          
char *concatStrings(const char *str1, const char *str2){
    int i;
    char *ret=0x0;
    int lengthStr1=strlen(str1);
    int lengthStr2=strlen(str2);
    ret=(char *)malloc(sizeof(char)*(lengthStr1+lengthStr2+1));

    if(ret==0x0) return ret;    
    for(i=0;i<lengthStr1;i++)
        ret[i]=str1[i];
    
    for(i=0;i<lengthStr2;i++)
        ret[i+lengthStr1]=str2[i];
    
    ret[lengthStr1+lengthStr2]='\0';
    return ret;
}
