cmake_minimum_required(VERSION 3.22)
project(hess-ml C CXX)

# set(CMAKE_CUDA_COMPILER /opt/nvidia/hpc_sdk/Linux_x86_64/2022/compilers/bin/nvcc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

include_directories(
        .
)

add_executable(hess-mmc hess-mmc.cc)

# set_target_properties(mmc PROPERTIES CUDA_SEPARABLE_COMPILATION ON)