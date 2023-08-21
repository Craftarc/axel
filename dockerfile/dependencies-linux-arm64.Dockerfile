# Build command: docker build -t craftarc/axel:dependencies -f dependencies-linux-arm64.Dockerfile .

# ARM64
FROM --platform=linux/arm64 amazonlinux:2

# Environment variables
ENV CX gcc
ENV CXX g++
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

# Install tools
RUN yum -y install git \
    wget \
    curl \
    zip \
    unzip \
    tar \
    ninja-build \
    make \
    gcc10 \
    gcc10-c++ \
    pkgconfig \
    perl-IPC-Cmd \
    autoconf \
    autoconf-archive \
    automake \
    python3


# Set aliases
WORKDIR /usr/bin

RUN ln -s gcc10-gcc gcc && \
    ln -s gcc10-g++ g++ && \
    ln -s gcc10-ar ar && \
    ln -s ninja-build ninja && \
    ln -s gcc10-ranlib ranlib

# Install CMake
WORKDIR /usr/local

RUN wget https://github.com/Kitware/CMake/releases/download/v3.27.3/cmake-3.27.3-linux-aarch64.tar.gz && \
    tar -xzf cmake-3.27.3-linux-aarch64.tar.gz && \
    cp -r cmake-3.27.3-linux-aarch64/bin/* /usr/local/bin && \
    cp -r cmake-3.27.3-linux-aarch64/share/* /usr/local/share && \
    rm -r cmake*

# Install vcpkg
WORKDIR /opt

RUN git clone https://github.com/Microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh

WORKDIR /axel

