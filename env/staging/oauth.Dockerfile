# Build command: docker build -t craftarc/axel:oauth -f dockerfile/oauth.Dockerfile .

FROM ubuntu:22.04

ARG APP_ROOT
ARG PROJECT_ROOT

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \
    python3 \
    pip

# Python packages
RUN pip install flask pytest requests

# Set aliases
WORKDIR /usr/bin

RUN ln -sf python3 python

WORKDIR ${APP_ROOT}

# Copy server modules in
COPY integration/oauth_test.py oauth_test.py
COPY integration/server.py server.py
COPY integration/util/util.py util/util.py
COPY config/config.json config/config.json
COPY integration/util/config.py util/config.py

CMD ["bash"]
