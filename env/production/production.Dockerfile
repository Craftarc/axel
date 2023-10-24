# docker build -t craftarc/axel:production -f production.Dockerfile .
# docker buildx build --platform linux/amd64,linux/arm64 -t craftarc/axel:production -f dockerfile/production.Dockerfile . --push

FROM ubuntu:22.04


RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \
    nginx-core

# Server configuration
COPY nginx.conf /etc/nginx/nginx.conf

EXPOSE 80 443


