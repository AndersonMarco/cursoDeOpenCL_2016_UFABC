__kernel void mult(__global const float  *a, __global const float *b, __global float *c, __local float* sum,int nColumnsA, int nRowsA,int nColumnsB, int nRowsB){
       int id = get_global_id(0);
       int nproc = get_global_size(0);
       
       int grupoId=get_group_id(0); 
       int localId=get_local_id(0);
       int tamGrupo=get_num_groups(0);
       int tamLocal=get_local_size(0);           
       int i,j,k;

       for(i=0;i<nRowsA;i++){
           for(j=grupoId;j<nColumnsB;j=j+tamGrupo){               
               sum[localId]=0.0f;
               
               for(k=localId;k<nRowsB;k=k+tamLocal){
                   sum[localId]=sum[localId]+(a[i*nColumnsA+k]*b[k*nColumnsB+j]);
               }
               barrier(CLK_GLOBAL_MEM_FENCE |CLK_LOCAL_MEM_FENCE);
               if(localId==0){
                   float sumLocal=0.0f;
                   for(k=0;k<tamLocal;k++){
                       sumLocal=sumLocal+sum[k];
                   }
                   c[i*nColumnsB+j]=sumLocal;
               }
               barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);               
           }
       }
}
