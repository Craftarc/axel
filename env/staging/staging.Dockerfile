# Build command: docker build -t craftarc/axel:staging -f dockerfile/staging.Dockerfile .
# docker buildx build --platform linux/amd64,linux/arm64 -t craftarc/axel:staging -f dockerfile/staging.Dockerfile . --push

FROM ubuntu:22.04

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \
    nginx-core \
    net-tools \
    vim

# Server configuration
COPY nginx.conf /etc/nginx/

EXPOSE 80



