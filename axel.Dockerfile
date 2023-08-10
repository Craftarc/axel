# Build command:
# docker build -t axel-backend -f axel.Dockerfile .

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
    cmake .. -DCMAKE_BUILD_TYPE=Release -DTEST=YES -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake && \
    make && \
    make aws-lambda-package-main && \
    chmod 744 main # Execute permissions

