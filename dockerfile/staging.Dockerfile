# Build command: docker build -t craftarc/axel:staging -f staging.Dockerfile .

FROM public.ecr.aws/lambda/provided:al2

RUN yum -y install unzip

# WORKDIR inherits from base image: /var/task
COPY build/main.zip .

# Move bootstrap to correct place since ./lambda-entrypoint.sh (inherited ENTRYPOINT script) expects it there.
# The environment also expects the main executable to be in /var/task (which is our working directory already).
RUN unzip main.zip && \
    mv bootstrap /var/runtime



