cd /opt
wget https://github.com/jrprice/Oclgrind/releases/download/v16.10/Oclgrind-16.10-Linux.rpm
aptitude install alien amd-libopencl1 amd-opencl-dev

alien Oclgrind-16.10-Linux.rpm
dpkg -i oclgrind_16.10-2_amd64.deb



