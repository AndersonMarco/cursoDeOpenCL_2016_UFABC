from __future__ import absolute_import, print_function
import numpy as np
import pyopencl as cl
import os
import math

def getPath():
    path= os.path.realpath(__file__)
    lenPath=len(path)
    for i in range(lenPath):    
        if(path[lenPath-i-1]==os.sep):
            path=path[0:(lenPath-i)]
            break
    return path


float2Struct = np.dtype([("x", np.float32), ("y", np.float32)])


path=getPath()
fileWithclCode= file(path+"complexSquared.cl","r")
strWithclCode= fileWithclCode.read()
fileWithclCode.close()


vectorsLength=256
a_np = np.empty(vectorsLength, float2Struct)
for i in range(vectorsLength):
    a_np[i]['x']= math.log(i+1)
    a_np[i]['y']=math.sin(i)

    
idPlatform=0
idDevice=0
ctx = cl.Context(devices=[cl.get_platforms()[idPlatform].get_devices()[idDevice]])
queue = cl.CommandQueue(ctx)


mf = cl.mem_flags
a_g = cl.Buffer(ctx, mf.READ_ONLY , size=8*vectorsLength)
res_g = cl.Buffer(ctx, mf.WRITE_ONLY, 8*vectorsLength)
cl.enqueue_copy(queue, a_g, a_np)


prg = cl.Program(ctx,strWithclCode).build()


prg.squareKernel(queue, (vectorsLength,1),None, a_g, res_g)


res_np = np.empty_like(a_np)
cl.enqueue_copy(queue, res_np, res_g)


check_np= np.empty_like(a_np)
for i in range(vectorsLength):
    ax=a_np[i]['x']
    ay=a_np[i]['y']
    check_np[i]['x']=(ax*ax)-(ay*ay);
    check_np[i]['y']=2*ax*ay;


for i in range(256):
    print(str(check_np[i]['x'])+" "+str(check_np[i]['y'])+"i"+"|"+str(res_np[i]['x'])+" "+str(res_np[i]['y']))
2
