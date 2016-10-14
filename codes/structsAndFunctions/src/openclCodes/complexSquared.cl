float2 square(float2 a){
    float2 at; 
    at.x=(a.x*a.x)-(a.y*a.y);
    at.y=2*a.x*a.y;
    return at;
   
}
__kernel void squareKernel(__global const float2  *a, __global  float2 *b){
       int id = get_global_id(0);
       b[id]=square(a[id]);
}
