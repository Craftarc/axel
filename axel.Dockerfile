# Build command:
# docker build -t axel -f axel.Dockerfile .

FROM dependencies:latest

# Directory for main app
WORKDIR /app/axel

# Copy only production files
COPY CMakeLists.txt .
COPY main.cpp .
COPY sandbox.cpp .
COPY config ./config
COPY src ./src

## Make main.zip, suitable for direct upload to AWS Lambda
RUN mkdir build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja && \
    ninja && \
    ninja aws-lambda-package-main && \
    chmod 744 main # Execute permissions

