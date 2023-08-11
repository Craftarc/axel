# Build command
# docker build -t dependencies -f dependencies.Dockerfile .

FROM ubuntu

# Environment variables
ENV CX gcc
ENV CXX g++
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

# Update base image and install tools
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y build-essential \
    git \
    vim \
    curl \
    zip \
    unzip \
    tar \
    cmake \
    ninja-build \
    pkg-config \
    python3 \
    gdb

WORKDIR /opt

# Install vcpkg
RUN git clone https://github.com/Microsoft/vcpkg.git && \
    cd vcpkg && \
    ./bootstrap-vcpkg.sh

## Install libraries
WORKDIR /opt/vcpkg

# Boost
RUN ./vcpkg install boost:x64-linux

# Google Test
RUN ./vcpkg install gtest:x64-linux

# Botan
RUN ./vcpkg install botan:x64-linux

# AWS C++ Lambda Runtime
RUN ./vcpkg install aws-lambda-cpp:x64-linux

# AWS C++ SDK
RUN ./vcpkg install "aws-sdk-cpp[dynamodb]:x64-linux" --recurse

# spdlog
RUN ./vcpkg install spdlog:x64-linux

## END INSTALL LIBRARIES

