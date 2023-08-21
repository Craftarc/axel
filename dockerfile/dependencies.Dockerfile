# Build command: docker buildx build --platform linux/arm64,linux/amd64 -f dockerfile/dependencies.Dockerfile -t craftarc/axel:dependencies .
# Please run the above command in /axel.

# ARM64
FROM amazonlinux:2

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

COPY scripts/install_cmake.sh .

ARG TARGETPLATFORM
RUN chmod 744 install_cmake.sh && \
    ./install_cmake.sh && \
    rm install_cmake.sh

# Install vcpkg
WORKDIR /opt

RUN git clone https://github.com/Microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh

WORKDIR /axel

