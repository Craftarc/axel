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
    python-devel && \
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
COPY ./external/boost_1_82_0.tar.bz2 .
RUN tar --bzip2 -xf ./boost_1_82_0.tar.bz2 && \
    rm boost_1_82_0.tar.bz2 && \
    cd boost_1_82_0 && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    ./b2 --with-test install && \
    cd .. && \
    rm -r boost_1_82_0
## END INSTALL LIBRARIES

# Directory for main app
WORKDIR /app/axel

# Copy only production files
COPY src ./src
COPY test ./test
COPY CMakeLists.txt handler.cpp ./

## Make demo
#RUN mkdir build && \
#    cd build && \
#    cmake3 .. -DCMAKE_BUILD_TYPE=Debug && \
#    make && \
#    make aws-lambda-package-handler

