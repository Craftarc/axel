# docker build -t craftarc/axel:production -f production.Dockerfile .
# docker buildx build --platform linux/amd64,linux/arm64 -t craftarc/axel:production -f dockerfile/production.Dockerfile . --push

FROM ubuntu:22.04

# ca-certificates: for when the server needs to make its own https requests

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \
    nginx-core \
    binutils-dev \
    ca-certificates

# Server configuration
COPY env/production/nginx.conf /etc/nginx/nginx.conf

WORKDIR /app

# Binary
COPY build/main /app

EXPOSE 80 443


