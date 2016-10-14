__kernel void add(__global const float  *a, __global const float *b, __global float *c, int vectorsLength){
       int id = get_global_id(0);
       int nproc = get_global_size(0);
       int i;
       for(i=id;i<vectorsLength;i=i+nproc){
           c[i]=a[i]+b[i];
       }
}
