cmake_minimum_required(VERSION 3.20)
project(csf_re CXX)
# Build: cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/clang-cl-x86.cmake && ninja -C build
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")  # static CRT: no runtime DLLs needed on the game box

file(GLOB_RECURSE CSF_SRC CONFIGURE_DEPENDS src/*.cpp)
add_library(csf_hook SHARED hook/main.cpp hook/dinput8.def ${CSF_SRC})
set_target_properties(csf_hook PROPERTIES OUTPUT_NAME dinput8)
target_include_directories(csf_hook PRIVATE hook src)
target_compile_options(csf_hook PRIVATE /W3 /EHsc /Zc:threadSafeInit- -Wno-microsoft-cast -Wno-invalid-offsetof)
target_link_libraries(csf_hook PRIVATE kernel32 user32)
target_link_options(csf_hook PRIVATE /MAP:${CMAKE_BINARY_DIR}/dinput8.map)
