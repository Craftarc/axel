# Build command
# docker build -t axel-backend:1.0 . --no-cache

# Use Amazon Linux 2 as base image/
FROM public.ecr.aws/lambda/provided:al2

# Environment variables
ENV CX gcc
ENV CXX g++

WORKDIR /usr/local

# Update base image and install tools
RUN yum update -y && \
    yum groupinstall -y "Development tools" && \
    yum install -y libcurl-devel \
    cmake3 \
    git \
    which \
    python-devel \
    openssl-devel \
    wget && \
    pip3 install awscli

## INSTALL LIBRARIES
# Install C++ lambda runtime
RUN git clone https://github.com/awslabs/aws-lambda-cpp.git && \
    cd aws-lambda-cpp && \
    mkdir build && \
    cd build && \
    cmake3 .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make && \
    make install && \
    rm -r /usr/local/aws-lambda-cpp

# Install Boost
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.bz2 && \
    tar --bzip2 -xf ./boost_1_82_0.tar.bz2 && \
    rm boost_1_82_0.tar.bz2 && \
    cd boost_1_82_0 && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    ./b2 --with-test install && \
    cd .. && \
    rm -r boost_1_82_0

# Install Botan
RUN wget https://botan.randombit.net/releases/Botan-2.19.3.tar.xz && \
    tar Jxf Botan-2.19.3.tar.xz && \
    rm Botan-2.19.3.tar.xz && \
    cd Botan-2.19.3 && \
    ./configure.py && \
    make && \
    make install && \
    cd .. && \
    rm -r Botan-2.19.3

# Install GoogleTest
RUN git clone https://github.com/google/googletest.git -b v1.13.0 && \
    cd googletest && \
    mkdir build && \
    cd build && \
    cmake3 .. && \
    make && \
    make install && \
    cd .. && \
    rm -r googletest

# Install C++ AWS SDK
RUN git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp && \
    cd aws-sdk-cpp && \
    mkdir build && \
    cd build && \
    cmake3 .. && \
    make && \
    make install && \
    cd .. && \
    rm -r aws-sdk-cpp


## END INSTALL LIBRARIES

# Directory for main app
WORKDIR /app/axel

# Copy only production files
COPY CMakeLists.txt ./
COPY handler.cpp ./
COPY config ./config
COPY src ./src

## Make demo
RUN mkdir build && \
    cd build && \
    cmake3 .. -DCMAKE_BUILD_TYPE=Release && \
    make && \
    make aws-lambda-package-handler
