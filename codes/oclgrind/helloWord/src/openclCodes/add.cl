__kernel void add(__global const float  *a, __global const float *b, __global float *c){
       int id = get_global_id(0);
       c[id]=a[id]+b[id];
     
       if(id==1){
           c[0]=a[0]+b[0];
       }
    
      
}
