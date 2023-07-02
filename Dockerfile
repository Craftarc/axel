# Build command
# docker build -t axel-backend:1.0 . --no-cache

# Use Amazon Linux 2 as base image/
FROM public.ecr.aws/lambda/provided:al2

# Environment variables
ENV CX gcc
ENV CXX g++

# Update base image
RUN yum update -y && \
    yum groupinstall -y "Development tools" && \
    yum install -y gcc10-c++ \
    libcurl-devel \
    cmake3 \
    git && \
    pip3 install awscli

WORKDIR /app

# Install C++ lambda runtime
RUN git clone https://github.com/awslabs/aws-lambda-cpp.git && \
    cd aws-lambda-cpp && \
    mkdir build && \
    cd build && \
    cmake3 .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/app/lambda-install && \
    make && make install


# Copy project directory
WORKDIR /app/axel

COPY main.cpp .
COPY CMakeLists.txt .

RUN mkdir build && \
    cd build && \
    cmake3 .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/app/lambda-install && \
    make && \
    make && make aws-lambda-package-demo

