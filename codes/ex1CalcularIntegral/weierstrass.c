#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//for validate: http://www.wolframalpha.com/widgets/view.jsp?id=8ab70731b1553f17c11a3bbc87e0b605

#define PI 3.14159265358979323846

double weierstrass(double x){
    int n;
    double bt=3;
    double at=2;
    double a=1;
    double b=1;
    double sum=0.0;
    for(n=0;n<100;n++){
        sum=sum+((1.0/a)*cos(b*PI*x));
        a=a*at;
        b=b*bt;               
    }
    return sum;
}
double f(double x){//function for code validate 
    return (sqrt(x)+log(x))/3.0;
}
int main(int argc,char* argv[]){
    
    double ini=atof(argv[1]);
    double end=atof(argv[2]);
    double dt=0.001;
    double x;
    for(x=ini;x<end;x=x+dt){
        double y=weierstrass(x);

        printf("%f %f\n",x,y); 

    }
   
    return 0;
}
