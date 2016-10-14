__kernel void add(__global  float  *a, __global  float *b,){
       int id = get_global_id(0);
       b[id]=0.0f;
       __global  float *a1=&a[id*10];
       for(int i=0;i<20;i++){
           b[id]=b[id]+a1[i];
       }

}
