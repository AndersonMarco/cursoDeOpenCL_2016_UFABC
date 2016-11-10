aptitude update
cd /opt
aptitude install pocl-opencl-icd libpocl1 libpocl1-common pocl-opencl-icd libpocl-dev ocl-icd-opencl-dev  ocl-icd-libopencl1 opencl-headers  git amd-opencl-icd  	


wget https://cmake.org/files/v3.6/cmake-3.6.2-Linux-x86_64.tar.gz
tar -zxvf cmake-3.6.2-Linux-x86_64.tar.gz

git clone  https://github.com/Oblomov/clinfo.git
cd clinfo
make
ln -s /opt/clinfo/clinfo  /usr/bin/





