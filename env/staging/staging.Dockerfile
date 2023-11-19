# Build command: docker build -t craftarc/axel:staging -f dockerfile/staging.Dockerfile .
# docker buildx build --platform linux/amd64,linux/arm64 -t craftarc/axel:staging -f dockerfile/staging.Dockerfile . --push

FROM ubuntu:22.04

ARG APP_ROOT

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \
    nginx-core \
    net-tools \
    vim \
    binutils-dev


# Server configuration
COPY env/staging/nginx.conf /etc/nginx/nginx.conf

WORKDIR ${APP_ROOT}

# Binary
COPY build/main ${APP_ROOT}

# Config
COPY config/config.json ${APP_ROOT}/config/

EXPOSE 80
