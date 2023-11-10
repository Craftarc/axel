# Build command (push): docker buildx build --platform linux/arm64,linux/amd64 -f dockerfile/dependencies.Dockerfile -t craftarc/axel:dependencies --push .
# Build command (local): docker build -f dockerfile/dependencies.Dockerfile -t craftarc/axel:dependencies . <- use this for local changes to the image

FROM ubuntu:22.04

# Environment variables
ENV CX gcc
ENV CXX g++
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

## OpenSSL is installed via vcpkg and looks for certs in vcpkg's install directory, where there is nothing.
## This is where the certificates are, so we guide SSL_CTX_set_default_verify_paths() to search here.
ENV SSL_CERT_DIR /etc/ssl/certs/ca-bundle.crt


# Install tools
RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \ 
    git \
    wget \
    curl \
    zip \
    unzip \
    tar \
    ninja-build \
    make \
    gcc-12 \
    g++-12 \
    pkg-config \
    cmake \
    python3 \
    pip \
    binutils-dev \
    gdb

# Python packages
RUN pip install flask

# Set aliases
WORKDIR /usr/bin

RUN ln -sf gcc-12 gcc && \
    ln -sf g++-12 g++

# Set up gdb config
RUN wget -P  ~ https://git.io/.gdbinit --no-check-certificate

# Install vcpkg
WORKDIR /opt

RUN git clone https://github.com/Microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh
