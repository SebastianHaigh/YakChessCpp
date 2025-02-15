FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
      software-properties-common \
      wget \
      gnupg \
      lsb-release \
      build-essential \
      curl \
      gdb \
      git

RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
      apt-get update && \
      apt-get install -y gcc-12 g++-12

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 12 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 12

RUN bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"

RUN wget https://github.com/Kitware/CMake/releases/download/v3.26.3/cmake-3.26.3-linux-x86_64.sh && \
    chmod +x cmake-3.26.3-linux-x86_64.sh && \
    ./cmake-3.26.3-linux-x86_64.sh --skip-license --prefix=/usr/local

RUN git clone https://github.com/catchorg/Catch2.git && \
    cd Catch2 && \
    git checkout v3.0.1 && \
    cmake -Bbuild -H. -DBUILD_TESTING=OFF && \
    cmake --build build/ --target install

RUN rm -rf /var/lib/apt/lists/* \
    rm -rf Catch2 \
    apt-get clean

ENV CC=/usr/bin/gcc
ENV CXX=/usr/bin/g++-12

RUN ln -s /usr/bin/g++-12 /usr/local/bin/c++
RUN ln -s /usr/local/bin/cmake /usr/bin/cmake

