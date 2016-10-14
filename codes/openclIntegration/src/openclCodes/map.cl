#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void map(__write_only image2d_t bmp, __global float* pointToGLVertex){
   int id = get_global_id(0);
   int nProc = get_global_size(0);

   int2 coords = (int2)(id/128,id%128);
   //Attention to RGBA order
   
   float4 val = (float4)(((float)(id%128))/128.0f, ((float)id/128.0)/(128.0f), 0.0f, 0.6);
   write_imagef(bmp, coords, val);
       
   if(id==0){
       pointToGLVertex[3*4+1]=1.6f;
   }
       
}
