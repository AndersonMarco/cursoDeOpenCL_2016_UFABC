cd /opt 
apt-get  install build-essential clang-3.8 libclang-3.8-dev llvm-3.8-dev llvm-3.8-runtime libllvm3.8v4 llvm-3.8 ocl-icd-dev ocl-icd-opencl-dev opencl-headers clinfo ocl-icd-libopencl1 hwloc libhwloc-dev libltdl-dev
wget https://cmake.org/files/v3.6/cmake-3.6.2-Linux-x86_64.tar.gz
tar -zxvf cmake-3.6.2-Linux-x86_64.tar.gz
wget http://portablecl.org/downloads/pocl-0.13.tar.gz
tar -zxvf pocl-0.13.tar.gz
cd pocl-0.13
mkdir build
cd build
/opt/cmake-3.5.2-Linux-x86_64/bin/cmake ../.
make
make install




cd /opt
apt-get install libpython2.7-dev
apt-get clean
apt-get install python-setuptools
git clone https://github.com/pyopencl/pyopencl.git
cd  pyopencl
git submodule update --init
./configure
python setup.py install

