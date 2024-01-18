# Absolute path to project root
dir=$(realpath "$(dirname "${BASH_SOURCE[0]}")/../..")
echo "${dir}"

# Compile debug binary for main then start it
"${dir}"/scripts/compile/dev.sh
"${dir}"/build/main

