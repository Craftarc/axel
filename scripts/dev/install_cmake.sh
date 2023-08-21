# Build CMake for the platform of the current image
echo "Current platform is: ${TARGETPLATFORM}"
if [ "${TARGETPLATFORM}" = "linux/amd64" ]; then
  wget https://github.com/Kitware/CMake/releases/download/v3.26.5/cmake-3.26.5-linux-x86_64.tar.gz
  tar -xzf cmake-3.26.5-linux-x86_64.tar.gz
  cp -r cmake-3.26.5-linux-x86_64/bin/* /usr/local/bin
  cp -r cmake-3.26.5-linux-x86_64/share/* /usr/local/share
  rm -r cmake*
elif [ "${TARGETPLATFORM}" = "linux/arm64" ]; then
  wget https://github.com/Kitware/CMake/releases/download/v3.26.5/cmake-3.26.5-linux-aarch64.tar.gz
  tar -xzf cmake-3.26.5-linux-aarch64.tar.gz
  cp -r cmake-3.26.5-linux-aarch64/bin/* /usr/local/bin
  cp -r cmake-3.26.5-linux-aarch64/share/* /usr/local/share
  rm -r cmake*
else
  exit 1;
fi