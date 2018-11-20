Ubuntu trusty setup
===================

Recent version of git
---------------------

sudo add-apt-repository ppa:git-core/ppa
sudo apt-get update
sudo apt-get install git

CMake latest release (3.12.4 as of 11/2018)
-------------------------------------------

wget https://cmake.org/files/v3.12/cmake-3.12.4-Linux-x86_64.sh
chmod +x cmake-3.12.4-Linux-x86_64.sh
sudo ./cmake-3.12.4-Linux-x86_64.sh --skip-license --prefix=/opt --include-subdir
sudo ln -s /opt/cmake-3.12.4-Linux-x86_64 /opt/cmake

add /opt/cmake/bin to the PATH environment variable.

GNU compilers 7
---------------

sudo apt-get update -y
sudo apt-get upgrade -y
sudo apt-get dist-upgrade -y
sudo apt-get install build-essential software-properties-common -y
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -y
sudo apt-get install gcc-7 g++-7

If you want this version to become the default gcc/g++ compiler on the system

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 --slave /usr/bin/g++ g++ /usr/bin/g++-7 && \
sudo update-alternatives --config gcc

Add clang
-----------

Add a file llvm-toolchain-trusty-6.0.list to /etc/apt/sources.list.d/ with the apt sources
for llvm/clang/tools:

deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty main
deb-src http://apt.llvm.org/trusty/ llvm-toolchain-trusty main

# 6.0
deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-6.0 main
deb-src http://apt.llvm.org/trusty/ llvm-toolchain-trusty-6.0 main

# 7
deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-7 main
deb-src http://apt.llvm.org/trusty/ llvm-toolchain-trusty-7 main

# Also add the following for the appropriate libstdc++ if you have not done the g++-7
# above
#deb http://ppa.launchpad.net/ubuntu-toolchain-r/test/ubuntu trusty main

To install just clang, lld and lldb (6.0 release):
sudo apt-get install clang-6.0 lldb-6.0 lld-6.0

To install additional tools
sudo apt-get install clang-tools-6.0 -y
sudo apt-get install clang-format-6.0 -y
sudo apt-get install clang-tidy-6.0 -y

To install all key packages:
# LLVM
sudo apt-get install libllvm-6.0-ocaml-dev libllvm6.0 llvm-6.0 llvm-6.0-dev llvm-6.0-doc llvm-6.0-examples llvm-6.0-runtime
# Clang and co
sudo apt-get install clang-6.0 clang-tools-6.0 clang-6.0-doc libclang-common-6.0-dev libclang-6.0-dev libclang1-6.0 clang-format-6.0 python-clang-6.0
# libfuzzer
sudo apt-get install libfuzzer-6.0-dev
# lldb
sudo apt-get install lldb-6.0
# lld (linker)
sudo apt-get install lld-6.0


Building
--------

for g++

cd build &&\
/opt/cmake/bin/cmake -DCMAKE_C_COMPILER="gcc-7" -DCMAKE_CXX_COMPILER="g++-7" ..

for clang

cd build &&\
/opt/cmake/bin/cmake -DCMAKE_C_COMPILER="clang-6.0" -DCMAKE_CXX_COMPILER="clang++-6.0" ..


