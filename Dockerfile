# Build command
# docker build -t axel-backend:1.0 . --no-cache

# Use Amazon Linux 2 as base image
FROM public.ecr.aws/lambda/provided:al2

# TODO: Add aliases for gcc10-gcc, gcc10-g++, cmake3

WORKDIR /app

# Update base image
RUN yum update -y && \
    yum install -y git \
    curl \
    ninja \
    make \
    cmake3 \
    zip \
    libcurl-devel \
    gcc10 \
    gcc10-c++


COPY ./CMakeLists.txt .

#RUN mkdir build && \
#    cd build && \
#    cmake3 ..

## Install C++ lambda runtime
#RUN git clone https://github.com/awslabs/aws-lambda-cpp.git && \
#    cd aws-lambda-cpp && \
#    mkdir build && \
#    cd build && \
#    cmake3 .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/lambda-install && \
#    make && \
#    make install

