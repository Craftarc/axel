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
    python3 \
    gdb \
    binutils-devel # For stack traces


# Set aliases
WORKDIR /usr/bin

RUN ln -sf gcc10-gcc gcc && \
    ln -sf gcc10-g++ g++ && \
    ln -sf gcc10-ar ar && \
    ln -sf ninja-build ninja && \
    ln -sf gcc10-ranlib ranlib

# Install CMake
WORKDIR /usr/local

COPY scripts/dev/install_cmake.sh .

ARG TARGETPLATFORM
RUN chmod 744 install_cmake.sh && \
    ./install_cmake.sh && \
    rm install_cmake.sh

# Install vcpkg
WORKDIR /opt

RUN git clone https://github.com/Microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh

# Install editor

RUN git clone https://github.com/juayhee/dotfiles.git && \
    source dotfiles/bootstrap.sh
