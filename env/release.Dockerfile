# docker build -t craftarc/axel:release -f release.Dockerfile .

FROM ubuntu:22.04

ARG APP_ROOT
ARG CONFIG_PATH

# ca-certificates: for when the server needs to make its own https requests

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \
    nginx-core \
    binutils-dev \
    ca-certificates \
    snapd

# Server configuration
# Either staging or production's config
COPY ${CONFIG_PATH} /etc/nginx/nginx.conf

WORKDIR ${APP_ROOT}

# Binary
COPY build/main .

# Config
COPY config/config.json config/

# Start script
COPY scripts/start.sh start.sh

CMD ["./start.sh"]

EXPOSE 80 443


