add_library(container ./src/json_container.cpp)
target_include_directories(container PUBLIC ./include ${PROJECT_ROOT})