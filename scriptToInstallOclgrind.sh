git clone git://git.savannah.gnu.org/readline.git
cd readline
./configure
make
make install

git clone https://github.com/jrprice/Oclgrind.git
cd Oclgrind
mkdir build
echo "deb http://ftp.de.debian.org/debian jessie-backports main " >>/etc/apt/sources.list
aptitude update
aptitude install llvm-3.8 llvm-3.8-runtime  llvm-3.8-dev  clang-3.8  libllvm3.8 libclang-3.8-dev libclang-common-3.8-dev libclang1-3.8 zlib1g-dev git
