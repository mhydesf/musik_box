FROM ubuntu:20.04

LABEL maintainer="hyde.mikhail@gmail.com"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y software-properties-common \
    && add-apt-repository ppa:ubuntu-toolchain-r/test \
    && apt-get update

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-13 g++-13 \
    libssl-dev \
    zlib1g-dev \
    git \
    wget \
    gnuplot \
    && rm -rf /var/lib/apt/lists/*

RUN wget https://github.com/Kitware/CMake/releases/download/v3.23.2/cmake-3.23.2.tar.gz \
    && tar -zxvf cmake-3.23.2.tar.gz \
    && cd cmake-3.23.2 \
    && ./bootstrap \
    && bash -c "make -j$(( $(nproc) * 2 ))" \
    && make install

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 100

COPY . /usr/src/musik_box

WORKDIR /usr/src/musik_box

RUN cmake -S . -B build/ \
    && cd build \
    && bash -c "make -j$(( $(nproc) * 2 ))"
