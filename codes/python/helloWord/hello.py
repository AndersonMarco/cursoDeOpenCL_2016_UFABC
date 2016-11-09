#!/usr/bin/env python
# -*- coding: utf-8 -*-

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


path=getPath()
fileWithclCode= file(path+"add.cl","r")
strWithclCode= fileWithclCode.read()
fileWithclCode.close()


vectorsLength=1024*1024*2
a_np = np.empty(vectorsLength).astype(np.float32)
b_np = np.empty(vectorsLength).astype(np.float32)
for i in range(vectorsLength):
    a_np[i]= math.log(i+1)
    b_np[i]=math.sin(i)

    
idPlatform=0
idDevice=0
ctx = cl.Context(devices=[cl.get_platforms()[idPlatform].get_devices()[idDevice]])
queue = cl.CommandQueue(ctx)

mf = cl.mem_flags
a_g = cl.Buffer(ctx, mf.READ_ONLY , size=4*vectorsLength)
b_g = cl.Buffer(ctx, mf.READ_ONLY , size=4*vectorsLength)
cl.enqueue_copy(queue, a_g, a_np, is_blocking=False)
cl.enqueue_copy(queue, b_g, b_np, is_blocking=False)
queue.finish()


prg = cl.Program(ctx,strWithclCode).build()

res_g = cl.Buffer(ctx, mf.WRITE_ONLY, 4*vectorsLength)
prg.add(queue,vectorsLength, None, a_g, b_g, res_g)

res_np = np.empty_like(a_np)
cl.enqueue_copy(queue, res_np, res_g, is_blocking=True)
a_g.release()
b_g.release()
res_g.release()
# Check on CPU with Numpy:
#print(res_np - (a_np + b_np))
print(np.linalg.norm(res_np - (a_np + b_np)))
