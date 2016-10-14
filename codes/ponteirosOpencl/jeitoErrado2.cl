float addRows(float  *row){
    float ret=0.0f;
    for(int i=0;i<20;i++){
        ret=ret+row[i];
    }
    return ret;
}
__kernel void add(__global  float  *a, __global  float *b){
       int id = get_global_id(0);
       b[id]=0.0f;
       b[id]=addRows(&a[id*10]);
}
