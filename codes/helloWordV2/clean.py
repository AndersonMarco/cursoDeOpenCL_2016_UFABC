#!/usr/bin/python
import shutil
import os
def remove_folder(folder):
    # check if folder exists
    if os.path.exists(folder):
         # remove if exists
         shutil.rmtree(folder)
def remove_file(fileArg):
    if os.path.exists(fileArg):
        os.remove(fileArg)

def delete(fileOrFolder):
    if(os.path.exists(fileOrFolder)):
        if(os.path.isfile(fileOrFolder)):
            remove_file(fileOrFolder)
        else:
            remove_folder(fileOrFolder)


listForRemove=[]

path= os.path.realpath(__file__)
lenPath=len(path)
for i in range(lenPath):    
    if(path[lenPath-i-1]==os.sep):
        path=path[0:(lenPath-i)]
        break

projectName=path.split(os.sep)[-2]
projectName=projectName[0].upper()+projectName[1:len(projectName)]

listForRemove.append(projectName+".sln")
listForRemove.append("CMakeFiles")
listForRemove.append("ALL_BUILD.vcxproj")
listForRemove.append("ALL_BUILD.vcxproj.user")
listForRemove.append(projectName+".vcxproj")
listForRemove.append(projectName+".vcxproj.user")
listForRemove.append(projectName+".vcxproj.filters")
listForRemove.append("ALL_BUILD.vcxproj.filters")
listForRemove.append("Makefile")
listForRemove.append("CMakeCache.txt")
listForRemove.append("Debug")
listForRemove.append("Release")
listForRemove.append(projectName+".dir")
listForRemove.append(projectName+".xcodeproj")
listForRemove.append(projectName+".build")
listForRemove.append("RelWithDebInfo")
listForRemove.append("Win32")
listForRemove.append("CMakeCache.txt")
listForRemove.append(projectName+".VC.db")
listForRemove.append(".vs")
listForRemove.append("Build")
listForRemove.append("build")
listForRemove.append("CMakeScripts")
listForRemove.append("cmake_install.cmake")
for fileOrPath in listForRemove:
    delete(path+fileOrPath)


