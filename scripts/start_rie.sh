#!/bin/bash
set -euo pipefail

# Set CMake flags here
cmake_arguments="-DCMAKE_BUILD_TYPE=Debug -G Ninja -DAXEL_TEST=YES"

echo ">> CMake arguments: ${cmake_arguments}"

# Get the container id currently running on port 9000
old_container_id=$(docker ps --format '{{.ID}}:{{.Ports}}' | { grep ":9000" || true; }  | cut -d ":" -f 1)

# If no such container was found
if [ -z "${old_container_id}" ];
then
  # Otherwise proceed
  echo ">> No containers running on port 9000 on host machine"
else
  echo ">> Old container found, with id: ${old_container_id}"
  docker kill "${old_container_id}" > /dev/null  # Kill it
  echo ">> Killed container: ${old_container_id}"
fi


# Start the container and mount the project root and RIE
echo ">> Starting new container..."
container_id=$(docker run -d -i \
-v ~/.aws-lambda-rie:/aws-lambda \
-v ~/dev/axel:/app/axel \
-v ~/.aws:/root/.aws \
-p 9000:8080 \
-e AWS_LAMBDA_RUNTIME_API=localhost:9000 \
dependencies)
echo ">> New container started, with id: ${container_id}"

# Build updated executables
docker exec "${container_id}" \
sh -c "cd /app/axel/build && cmake .. ${cmake_arguments} && ninja"
echo ">> Updated executables built"

# Enter the container and run script inside it
echo ">> Entering the container..."
docker exec -it "${container_id}" /app/axel/scripts/docker/start.sh
